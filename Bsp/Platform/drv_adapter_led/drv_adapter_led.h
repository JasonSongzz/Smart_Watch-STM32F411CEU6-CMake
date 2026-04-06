/******************************************************************************
 * @file drv_adapter_led.h
 * @brief LED 抽象适配层（wrapper）：与具体 GPIO/闪烁策略解耦，由 Port 注册实例。
 *****************************************************************************/
#ifndef DRV_ADAPTER_LED_H
#define DRV_ADAPTER_LED_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct led_drv_t {
    uint32_t idx;
    void *user_data;
    void (*led_drv_init)(struct led_drv_t *dev);
    void (*led_drv_set_constant)(struct led_drv_t *dev, int on);
    void (*led_drv_process)(struct led_drv_t *dev);
} led_drv_t;

#ifndef LED_DEV_MAX
#define LED_DEV_MAX  (4U)
#endif

bool drv_adapter_led_reg(uint32_t index, const led_drv_t *dev);

void drv_adapter_led_init(uint32_t dev_index);
void drv_adapter_led_set_constant(uint32_t dev_index, int on);
void drv_adapter_led_process(uint32_t dev_index);

#ifdef __cplusplus
}
#endif

#endif /* DRV_ADAPTER_LED_H */
