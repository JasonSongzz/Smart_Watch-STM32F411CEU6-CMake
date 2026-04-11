/******************************************************************************
 * @file User_IWDG.c
 * @brief IWDG 专用喂狗任务（与业务任务解耦）
 *****************************************************************************/
#include <stdint.h>
#include "User_IWDG.h"
#include "iwdg.h"
#include "osal.h"
#include "Debug.h"

#ifndef USER_IWDG_FEED_MS
#define USER_IWDG_FEED_MS (1000U)
#endif

void User_IWDG_Refresh(void)
{
  (void)HAL_IWDG_Refresh(&hiwdg);
}

void User_IWDG_Feed_Task(void *argument)
{
  (void)argument;
  for (;;)
  {
    User_IWDG_Refresh();
    osal_task_delay_ms(USER_IWDG_FEED_MS);
  }
}
