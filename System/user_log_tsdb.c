/*
 * Mirror EasyLogger output to FlashDB TSDB on partition log_tsdb (0x300000, 3 MiB).
 * elog_port_output: RTT + non-blocking queue; LogFlash task calls fdb_tsl_append.
 */
#include <string.h>

#include "user_log_tsdb.h"
#include "osal.h"
#include "osal_error.h"
#include "sfud.h"

#include <flashdb.h>
#undef log_e
#undef log_i
#undef log_d

#define LOG_TAG "log_flash"
#include "elog.h"

/* Must be < physical sector size (4096 on W25Q) */
#define USER_LOG_LINE_CAP 384U
#define USER_LOG_QUEUE_DEPTH 24U

static struct fdb_tsdb s_tsdb;
static osal_queue_handle_t s_q;
static osal_mutex_handle_t s_port_mutex;
static osal_mutex_handle_t s_fdb_mutex;
static volatile uint8_t s_ready;

static fdb_time_t log_ts_get(void)
{
    return (fdb_time_t)osal_task_get_tick_count();
}

static void fdb_lock_cb(fdb_db_t db)
{
    (void)db;
    if (s_fdb_mutex != NULL) {
        (void)osal_mutex_take(s_fdb_mutex, OSAL_MAX_DELAY);
    }
}

static void fdb_unlock_cb(fdb_db_t db)
{
    (void)db;
    if (s_fdb_mutex != NULL) {
        (void)osal_mutex_give(s_fdb_mutex);
    }
}

static void user_log_flush_task(void *arg)
{
    (void)arg;
    char line[USER_LOG_LINE_CAP];

    for (;;) {
        if (osal_queue_receive(s_q, (void *)line, OSAL_MAX_DELAY) != OSAL_SUCCESS) {
            continue;
        }
        if (!user_log_tsdb_is_ready()) {
            continue;
        }
        size_t len = 0U;
        while (len < USER_LOG_LINE_CAP && line[len] != '\0') {
            len++;
        }
        if (len == 0U) {
            continue;
        }
        struct fdb_blob blob_st;
        fdb_blob_t blob = fdb_blob_make(&blob_st, line, len);
        if (fdb_tsl_append(&s_tsdb, blob) != FDB_NO_ERR) {
            /* avoid elog recursion: optional SEGGER only */
        }
    }
}

void user_log_port_lock(void)
{
    if (s_port_mutex != NULL) {
        (void)osal_mutex_take(s_port_mutex, OSAL_MAX_DELAY);
    }
}

void user_log_port_unlock(void)
{
    if (s_port_mutex != NULL) {
        (void)osal_mutex_give(s_port_mutex);
    }
}

void user_log_tsdb_try_enqueue(const char *log, size_t size)
{
    if (!s_ready || s_q == NULL || log == NULL || size == 0U) {
        return;
    }
    char line[USER_LOG_LINE_CAP];
    size_t n = size;
    if (n >= USER_LOG_LINE_CAP) {
        n = USER_LOG_LINE_CAP - 1U;
    }
    memcpy(line, log, n);
    line[n] = '\0';
    (void)osal_queue_send(s_q, line, 0);
}

bool user_log_tsdb_is_ready(void)
{
    return s_ready != 0U;
}

void user_log_tsdb_init(void)
{
    if (sfud_get_device(0) == NULL || !sfud_get_device(0)->init_ok) {
        log_e("log_tsdb: SFUD not ready");
        return;
    }

    memset(&s_tsdb, 0, sizeof(s_tsdb));

    if (osal_mutex_create(&s_port_mutex) != OSAL_SUCCESS) {
        log_e("log_tsdb: port mutex failed");
        return;
    }
    if (osal_mutex_create(&s_fdb_mutex) != OSAL_SUCCESS) {
        log_e("log_tsdb: fdb mutex failed");
        return;
    }
    if (osal_queue_create(USER_LOG_QUEUE_DEPTH, USER_LOG_LINE_CAP, &s_q) != OSAL_SUCCESS) {
        log_e("log_tsdb: queue failed");
        return;
    }

    fdb_tsdb_control(&s_tsdb, FDB_TSDB_CTRL_SET_LOCK, fdb_lock_cb);
    fdb_tsdb_control(&s_tsdb, FDB_TSDB_CTRL_SET_UNLOCK, fdb_unlock_cb);

    fdb_err_t err = fdb_tsdb_init(&s_tsdb, "elog", "log_tsdb", log_ts_get, USER_LOG_LINE_CAP - 1U, NULL);
    if (err != FDB_NO_ERR) {
        log_e("log_tsdb: fdb_tsdb_init err %d", (int)err);
        return;
    }

    s_ready = 1U;

    static osal_task_handle_t flush_handle;
    if (osal_task_create("LogFlash", user_log_flush_task, 512U * 4U, 10U, &flush_handle, NULL) != OSAL_SUCCESS) {
        log_e("log_tsdb: flush task create failed");
        s_ready = 0U;
        return;
    }

    log_i("log_tsdb: TSDB on log_tsdb 0x300000 len 0x300000, line cap %u", (unsigned)USER_LOG_LINE_CAP);
}
