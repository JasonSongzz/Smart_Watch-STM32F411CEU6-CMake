/******************************************************************************
 * @file drv_adapter_led.c
 *****************************************************************************/
#include "drv_adapter_led.h"

static led_drv_t s_led_drv[LED_DEV_MAX];

bool drv_adapter_led_reg(uint32_t index, const led_drv_t *dev)
{
    const led_drv_t *src = dev;

    if (index >= LED_DEV_MAX || src == NULL) {
        return false;
    }
    s_led_drv[index].idx = src->idx;
    s_led_drv[index].user_data = src->user_data;
    s_led_drv[index].led_drv_init = src->led_drv_init;
    s_led_drv[index].led_drv_set_constant = src->led_drv_set_constant;
    s_led_drv[index].led_drv_process = src->led_drv_process;
    return true;
}

void drv_adapter_led_init(uint32_t dev_index)
{
    if (dev_index >= LED_DEV_MAX) {
        return;
    }
    led_drv_t *dev = &s_led_drv[dev_index];
    if (dev->led_drv_init != NULL) {
        dev->led_drv_init(dev);
    }
}

void drv_adapter_led_set_constant(uint32_t dev_index, int on)
{
    if (dev_index >= LED_DEV_MAX) {
        return;
    }
    led_drv_t *dev = &s_led_drv[dev_index];
    if (dev->led_drv_set_constant != NULL) {
        dev->led_drv_set_constant(dev, on);
    }
}

void drv_adapter_led_process(uint32_t dev_index)
{
    if (dev_index >= LED_DEV_MAX) {
        return;
    }
    led_drv_t *dev = &s_led_drv[dev_index];
    if (dev->led_drv_process != NULL) {
        dev->led_drv_process(dev);
    }
}
