/*
 * FAL flash device on SFUD (device index 0). Call after sfud_init().
 */
#include <fal.h>
#include <sfud.h>
#include <stdint.h>
#include <string.h>

static sfud_flash *s_sfud;

static int fal_w25q_init(void);
static int fal_w25q_read(long offset, uint8_t *buf, size_t size);
static int fal_w25q_write(long offset, const uint8_t *buf, size_t size);
static int fal_w25q_erase(long offset, size_t size);

struct fal_flash_dev g_fal_w25q64 = {.name = FAL_W25Q64_DEV_NAME,
    .addr = 0,
    .len = 8UL * 1024UL * 1024UL,
    .blk_size = 4096,
    .ops = {fal_w25q_init, fal_w25q_read, fal_w25q_write, fal_w25q_erase},
    .write_gran = 1};

static int fal_w25q_init(void)
{
    s_sfud = sfud_get_device(0);
    if (s_sfud == NULL || !s_sfud->init_ok) {
        return -1;
    }
    g_fal_w25q64.blk_size = s_sfud->chip.erase_gran;
    g_fal_w25q64.len = s_sfud->chip.capacity;
    return 0;
}

static int fal_w25q_read(long offset, uint8_t *buf, size_t size)
{
    if (s_sfud == NULL || !s_sfud->init_ok || buf == NULL) {
        return -1;
    }
    if (offset < 0 || (uint64_t)offset + size > g_fal_w25q64.len) {
        return -1;
    }
    if (sfud_read(s_sfud, (uint32_t)offset, size, buf) != SFUD_SUCCESS) {
        return -1;
    }
    return (int)size;
}

static int fal_w25q_write(long offset, const uint8_t *buf, size_t size)
{
    if (s_sfud == NULL || !s_sfud->init_ok || buf == NULL) {
        return -1;
    }
    if (offset < 0 || (uint64_t)offset + size > g_fal_w25q64.len) {
        return -1;
    }
    if (sfud_write(s_sfud, (uint32_t)offset, size, buf) != SFUD_SUCCESS) {
        return -1;
    }
    return (int)size;
}

static int fal_w25q_erase(long offset, size_t size)
{
    if (s_sfud == NULL || !s_sfud->init_ok) {
        return -1;
    }
    if (offset < 0 || (uint64_t)offset + size > g_fal_w25q64.len) {
        return -1;
    }
    if (sfud_erase(s_sfud, (uint32_t)offset, size) != SFUD_SUCCESS) {
        return -1;
    }
    return (int)size;
}
