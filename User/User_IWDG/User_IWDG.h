/******************************************************************************
 * @file User_IWDG.h
 * @brief 独立看门狗（IWDG）喂狗任务与接口
 *****************************************************************************/
#ifndef USER_IWDG_H
#define USER_IWDG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void User_IWDG_Refresh(void);
void User_IWDG_Feed_Task(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* USER_IWDG_H */
