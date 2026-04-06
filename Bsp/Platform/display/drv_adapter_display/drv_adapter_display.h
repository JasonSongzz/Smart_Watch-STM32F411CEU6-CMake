/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file drv_adapter_display.h
 * 
 * @par dependencies 
 * - <Driver_Layer>.h
 * - stdbool.h
 * - stdint.h
 * 
 * @author Simon | R&D Dept. | EternalChip 立芯嵌入式
 * 
 * @brief Provide the HAL APIs of flash handler 
 * and corresponding operations.
 * 
 * Processing flow:
 * 
 * Call directly.
 * 
 * @version V1.0 2023-12-03
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/
 
#ifndef __DRV_ADAPTER_DISPLAY_H__  /* Avoid repeated inclusion */
#define __DRV_ADAPTER_DISPLAY_H__


//******************************** Includes *********************************//
#include "stdint.h"
#include "stdbool.h"
//******************************** Includes *********************************//
//******************************** Defines **********************************//

//******************************** Defines **********************************//


//******************************** Typedefs *********************************//
typedef struct _display_drv_t {

    uint32_t idx;                                                                                           /**< Record the instance index. */
    uint32_t dev_id;                                                                                        /**< Record the device id. */
    void * user_data;                                                                                       /**< Record the user data */
    void (* display_drv_init)(struct _display_drv_t * dev);                                                 /**< function pointer to device init */
    void (* display_drv_deinit)(struct _display_drv_t * dev);                                                 /**< function pointer to device init */
    void (* display_drv_draw_pixel)(struct _display_drv_t * dev,uint16_t x, uint16_t y, uint16_t color);    /**< function pointer to device init */
    void (* display_SetBackLight)(struct _display_drv_t * dev,uint16_t u16_resolution);
}display_drv_t;

//******************************** Typedefs *********************************//


//**************************** Interface Structs ****************************//

//**************************** Interface Structs ****************************//


//******************************** Classes **********************************//

//******************************** Classes **********************************//


//******************************** APIs *************************************//
bool drv_adapter_display_reg(uint32_t index, display_drv_t * dev);

void drv_adapter_display_init(void);

void drv_adapter_display_deinit(void);

void drv_adapter_display_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

void drv_adapter_display_SetBackLight(uint16_t u16_resolution);
//******************************** APIs *************************************//


#endif /* drv_adapter_display */
