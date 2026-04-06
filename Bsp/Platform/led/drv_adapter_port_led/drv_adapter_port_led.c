/******************************************************************************
 * @file drv_adapter_port_led.c
 * @note 闪烁周期与工程中传感器一致：用 HAL_GetTick() 毫秒时间基（同 temp_humi Port 的 timebase_interface）。
 *       传感器 Handler 周期本身是用 osal_task_delay_ms(ms) 阻塞等待，非 osal_task_get_tick_count。
 *****************************************************************************/
#include "drv_adapter_port_led.h"

#include <stddef.h>

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState on_level;
    uint32_t half_period_ms;
} led_port_hw_t;

typedef struct {
    bool inited_slot;
    led_kind_t kind;
    led_port_hw_t hw;
    /** 下次翻转时刻（HAL_GetTick 毫秒，与 bsp 驱动 timebase 一致） */
    uint32_t next_deadline_ms;
} led_port_ctx_t;

static led_port_ctx_t s_ctx[LED_DEV_MAX];

static void gpio_write_level(const led_port_hw_t *hw, int on)
{
    GPIO_PinState s;

    if (on) {
        s = hw->on_level;
    } else {
        s = (hw->on_level == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    }
    HAL_GPIO_WritePin(hw->port, hw->pin, s);
}

static void led_port_rcc_enable(GPIO_TypeDef *port)
{
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
#if defined(GPIOF)
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
#endif
#if defined(GPIOG)
    } else if (port == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE();
#endif
    } else if (port == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    } else {}
}

/** 按实例配置初始化单引脚推挽输出（低电平初始） */
static void led_port_gpio_init_pin(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef g = {0};

    led_port_rcc_enable(port);
    g.Pin = pin;
    g.Mode = GPIO_MODE_OUTPUT_PP;
    g.Pull = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &g);
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

static led_port_ctx_t *ctx_from_dev(led_drv_t *dev)
{
    if (dev == NULL) {
        return NULL;
    }
    return (led_port_ctx_t *)dev->user_data;
}

static void led_static_init(led_drv_t *dev)
{
    led_port_ctx_t *c = ctx_from_dev(dev);

    if (c == NULL) {
        return;
    }
    led_port_gpio_init_pin(c->hw.port, c->hw.pin);
    gpio_write_level(&c->hw, 0);
}

static void led_static_set(led_drv_t *dev, int on)
{
    led_port_ctx_t *c = ctx_from_dev(dev);

    if (c == NULL) {
        return;
    }
    gpio_write_level(&c->hw, on);
}

static void led_static_process(led_drv_t *dev)
{
    (void)dev;
}

static void led_blink_init(led_drv_t *dev)
{
    led_port_ctx_t *c = ctx_from_dev(dev);
    uint32_t period_ms;

    if (c == NULL) {
        return;
    }
    led_port_gpio_init_pin(c->hw.port, c->hw.pin);
    gpio_write_level(&c->hw, 0);
    period_ms = (c->hw.half_period_ms != 0U) ? c->hw.half_period_ms : 1U;
    c->next_deadline_ms = HAL_GetTick() + period_ms;
}

static void led_blink_set(led_drv_t *dev, int on)
{
    led_port_ctx_t *c = ctx_from_dev(dev);

    if (c == NULL) {
        return;
    }
    gpio_write_level(&c->hw, on);
}

static void led_blink_process(led_drv_t *dev)
{
    led_port_ctx_t *c = ctx_from_dev(dev);
    uint32_t now;
    uint32_t period_ms;

    if (c == NULL) {
        return;
    }
    now = HAL_GetTick();
    if ((int32_t)(now - c->next_deadline_ms) < 0) {
        return;
    }
    HAL_GPIO_TogglePin(c->hw.port, c->hw.pin);
    period_ms = (c->hw.half_period_ms != 0U) ? c->hw.half_period_ms : 1U;
    c->next_deadline_ms = now + period_ms;
}

static void port_led_init(led_drv_t *dev)
{
    led_port_ctx_t *c = ctx_from_dev(dev);

    if (c == NULL) {
        return;
    }
    if (c->kind == LED_KIND_BLINK) {
        led_blink_init(dev);
    } else {
        led_static_init(dev);
    }
}

static void port_led_set_constant(led_drv_t *dev, int on)
{
    led_port_ctx_t *c = ctx_from_dev(dev);

    if (c == NULL) {
        return;
    }
    if (c->kind == LED_KIND_BLINK) {
        led_blink_set(dev, on);
    } else {
        led_static_set(dev, on);
    }
}

static void port_led_process(led_drv_t *dev)
{
    led_port_ctx_t *c = ctx_from_dev(dev);

    if (c == NULL) {
        return;
    }
    if (c->kind == LED_KIND_BLINK) {
        led_blink_process(dev);
    } else {
        led_static_process(dev);
    }
}

bool drv_adapter_port_led_register(uint32_t index, const led_port_config_t *cfg)
{
    led_drv_t d;
    led_port_ctx_t *c;

    if (index >= LED_DEV_MAX || cfg == NULL) {
        return false;
    }
    if (cfg->port == NULL || cfg->pin == 0U) {
        return false;
    }
    c = &s_ctx[index];
    if (c->inited_slot) {
        return false;
    }

    c->kind = cfg->kind;
    c->hw.port = cfg->port;
    c->hw.pin = cfg->pin;
    c->hw.on_level = cfg->on_level;
    c->hw.half_period_ms = cfg->half_period_ms;
    c->next_deadline_ms = 0U;
    c->inited_slot = true;

    d.idx = index;
    d.user_data = c;
    d.led_drv_init = port_led_init;
    d.led_drv_set_constant = port_led_set_constant;
    d.led_drv_process = port_led_process;

    if (!drv_adapter_led_reg(index, &d)) {
        c->inited_slot = false;
        return false;
    }
    return true;
}
