/******************************************************************************
 * @file User_LED.c
 * @brief 应用层 LED 任务：仅周期调度（注册见 User_Periph_Setup，初始化见 User_Init）。
 *****************************************************************************/
#include "User_LED.h"

#include "drv_adapter_led.h"
#include "osal.h"
#include "Debug.h"

void User_LED_Init(void)
{
    drv_adapter_led_init(0U);
    log_i("LED: PC13 1s blink (500ms/500ms) have already been initialized");
}

void User_Led_Task(void *argument)
{
    (void)argument;

    for (;;) {
        drv_adapter_led_process(0U); /* 与 app_periph_init 中 drv_adapter_port_led_register(0, ...) 一致 */
        osal_task_delay_ms(10U);
    }
}
