/******************************************************************************
 * Copyright (C) 2026, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file User_Init.h
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
#ifndef __USER_INIT_H__  //Avoid repeated including same files later
#define __USER_INIT_H__

//******************************** Includes *********************************//
#include <stdint.h>
#include "osal.h"
//******************************** Includes *********************************//

//******************************** Defines **********************************//
//******************************** Defines **********************************//

//******************************** Variable **********************************//
extern osal_task_handle_t userTaskInitHandle;
extern osal_task_handle_t userLedTaskHandle;
extern osal_task_handle_t watchdogTaskHandle;
//******************************** Variable **********************************//

//******************************** Declaring ********************************//

int8_t UserAppTask_Init(void);
//******************************** Declaring ********************************//

#endif /* __USER_INIT_H__ */
