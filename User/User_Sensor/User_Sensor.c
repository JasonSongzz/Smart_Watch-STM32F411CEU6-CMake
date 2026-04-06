/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file User_Init.c
 * 
 * @par dependencies 
 * - User_Init.h
 * 
 * @author Simon | R&D Dept. | EternalChip 立芯嵌入式
 * 
 * @brief For application layer task initialization
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2025-2-24
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/
//******************************** Includes *********************************//
#include "User_Sensor.h"
#include "osal.h"
#include "Debug.h"
#include "temp_hui_port.h"
//******************************** Includes *********************************//

//******************************** Variable **********************************//

//******************************** Variable **********************************//

//******************************** Function definitions ********************************//
/**
 * @brief User initialization task thread
 * @param[in] :None
 * @param[Out] :None
 * @return None
 * 
 * */
void sensor_polling_task(void *argument)
{
  /* USER CODE BEGIN userTaskFunction */
  float temp,humi;
  /* Infinite loop */
  for(;;)
  {
    log_d("Sensor Read");
    osal_task_delay_ms(2000);
    temphumi_read_temp_and_humi(&temp,&humi);
  }
  /* USER CODE END userTaskFunction */
}
//******************************** Function definitions ********************************//
