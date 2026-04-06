/******************************************************************************
 * @file drv_adapter_port_led.h
 * @brief LED Port 层：GPIO + 静态/闪烁策略，通过 drv_adapter_led 向应用注册。
 *****************************************************************************/
#ifndef DRV_ADAPTER_PORT_LED_H
#define DRV_ADAPTER_PORT_LED_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "drv_adapter_led.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LED_KIND_STATIC = 0,
    LED_KIND_BLINK,
} led_kind_t;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState on_level;
    uint32_t half_period_ms; /* 闪烁半周期 ms；STATIC 时可为 0 */
    led_kind_t kind;
} led_port_config_t;

/**
 * @brief 注册一路 LED（索引 0..LED_DEV_MAX-1）
 * @return true 成功；false 参数非法或该 index 已注册
 */
bool drv_adapter_port_led_register(uint32_t index, const led_port_config_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* DRV_ADAPTER_PORT_LED_H */
