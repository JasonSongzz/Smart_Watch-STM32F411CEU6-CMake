/**
 *****************************************************************************************
 *
 * @file User_Periph_Setup.c
 *
 * @brief  User Periph Init Function Implementation.
 *
 *****************************************************************************************
 * @attention
  #####Copyright (c) 2019 GOODIX
  All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of GOODIX nor the names of its contributors may be used
    to endorse or promote products derived from this software without
    specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************************
 */

/*
 * INCLUDE FILES
 *****************************************************************************************
 */
#include "User_Periph_Setup.h"
#include "drv_adapter_port_led.h"
#include "Debug.h"

/* 常见接法：LED 阴极接 MCU → 低电平亮 */
#ifndef USER_LED_ON_LEVEL
#define USER_LED_ON_LEVEL GPIO_PIN_RESET
#endif
/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */
/**
 * @brief 外设初始化（异步生产者-消费者模式）
 * @note 配置各传感器自动刷新间隔及 LED Port 注册
 * 
 * 架构说明：
 * - 所有传感器使用异步生产者-消费者模式
 * - Handler 任务在后台周期性刷新数据
 * - Application 层读取时非阻塞，立即返回缓存数据
 */
void app_periph_init(void)
{

    /* PC13 500ms 半周期闪烁（整周期约 1s） */
    {
        led_port_config_t led_hb = {
            .port = GPIOC,
            .pin = GPIO_PIN_13,
            .on_level = USER_LED_ON_LEVEL,
            .half_period_ms = 500U,
            .kind = LED_KIND_BLINK,
        };
        if (!drv_adapter_port_led_register(0U, &led_hb)) {
            log_e("LED port register failed");
        }
    }
}
