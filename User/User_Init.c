/******************************************************************************
 * Copyright (C) 2026, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file User_Init.c
 * 
 * @par dependencies 
 * - User_Init.h
 * 
 * @author Jason Song
 * 
 * @brief For application layer task initialization
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2026-4-3
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/
//******************************** Includes *********************************//
#include <stdint.h>
#include "User_Init.h"
#include "osal.h"
#include "User_LED.h"
#include "User_IWDG.h"
#include "Debug.h"
#include "sfud.h"
#include "user_log_tsdb.h"
//******************************** Includes *********************************//

//******************************** Variable **********************************//
osal_task_handle_t userTaskInitHandle;
osal_task_handle_t userLedTaskHandle;
osal_task_handle_t watchdogTaskHandle;    /* 看门狗任务句柄 */
//******************************** Variable **********************************//

/**
 * @brief User initialization task thread
 * @param[in] :None
 * @param[Out] :None
 * @return None
 * 
 * */
static void userTaskInitFunction(void *argument)
{
  /* USER CODE BEGIN userTaskFunction */
  {
    if (sfud_init() == SFUD_SUCCESS) {
      log_i("SFUD: W25Q64 (SPI1/PA4 CS) initialized");
      user_log_tsdb_init();
    } else {
      log_e("SFUD: initialization failed");
    }

    User_LED_Init();
    osal_task_create("LedTask", User_Led_Task, 256,
        14, (osal_task_handle_t)(uintptr_t)&userLedTaskHandle, NULL);

    osal_task_create("WatchDogTask", User_IWDG_Feed_Task, 128,
        17, (osal_task_handle_t)(uintptr_t)&watchdogTaskHandle, NULL);

  }
  
  osal_task_delete(NULL);
  /* USER CODE END userTaskFunction */
}

/**
 * @brief OS creation initialization thread task
 * @param[in] :None
 * @param[Out] :None
 * @return None
 * 
 * */
int8_t UserAppTask_Init(void)
{
  int8_t ret = (int8_t)0;
  /* Flash 配置初始化等需要一定栈深（原 512B 易在 UserConfig_Init 附近溢出） */
  osal_task_create("userTask", userTaskInitFunction, 512 * 4,
      16, (osal_task_handle_t)(uintptr_t)&userTaskInitHandle, NULL);
  return ret;
}

//******************************** Function definitions ********************************//
