/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file User_Display.c
 * 
 * @par dependencies 
 * - User_Display.h
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
#include "User_Display.h"

#include "osal.h"

#include "lvgl.h"
#include "lvgl/examples/porting/lv_port_disp_template.h"
#include "lvgl/examples/porting/lv_port_indev_template.h"
#include "LVGL_ui/gui_guider.h"

#include "User_ExternflashManage.h"
//test
#include "drv_adapter_display.h"
//******************************** Includes *********************************//

//******************************** Variable **********************************//
lv_ui guider_ui;
extern uint8_t g_au8_lvgldata[LVGL_DATA_MAX_SIZE];
//******************************** Variable **********************************//

//******************************** Function definitions ********************************//
uint16_t cycle_cnt = 200;
void display_refresh_task(void *argument)
{
    
    /*先读数据到原有数据中*/
    Read_LvglData(0x00, 1050);
    //lvgl init 
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    setup_ui(&guider_ui);

	for (;;)
	{
        lv_task_handler();
        osal_task_delay_ms(5);
        if(cycle_cnt > 1000)
        {
            cycle_cnt = 200;
        }
				else
				{
					cycle_cnt += 500;
				}
        drv_adapter_display_SetBackLight(cycle_cnt);
	}
}
//******************************** Function definitions ********************************//