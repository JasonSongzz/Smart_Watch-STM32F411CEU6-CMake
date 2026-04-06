/******************************************************************************
 * @file User_LED.h
 * @brief LED 应用任务（注册/默认电平见 User_Periph_Setup，初始化见 User_Init）
 *****************************************************************************/
#ifndef USER_LED_H
#define USER_LED_H

void User_LED_Init(void);

void User_Led_Task(void *argument);

#endif /* USER_LED_H */
