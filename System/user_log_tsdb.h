#ifndef USER_LOG_TSDB_H
#define USER_LOG_TSDB_H

#include <stddef.h>
#include <stdbool.h>

void user_log_tsdb_init(void);
bool user_log_tsdb_is_ready(void);

void user_log_port_lock(void);
void user_log_port_unlock(void);

void user_log_tsdb_try_enqueue(const char *log, size_t size);

#endif
