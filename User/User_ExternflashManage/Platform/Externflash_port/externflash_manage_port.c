/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file drv_adapter_port_externflashoment.c
 * 
 * @par dependencies 
 * - drv_adapter_port_externflashoment.h
 * 
 * @author Jack | R&D Dept. | EternalChip 立芯嵌入式
 * 
 * @brief Provide the HAL APIs of AHT21 and corresponding opetions.
 * 
 * Processing flow:
 * 
 * 1.The aht21_inst function will instantiate the bsp_aht21_driver_t object and
 * with the needed funtion interface. 
 * 
 * 2.Then the users could all the IOs from instances of bsp_aht21_driver_t.
 * 
 * @version V1.0 2023-12-03
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/

//******************************** Includes *********************************//
#include "externflash_manage_port.h"
#include "drv_adapter_port_flash.h"
#include "drv_adapter_flash.h"

//******************************** Includes *********************************//
//******************************** Defines **********************************//

//******************************** Defines **********************************//

//******************************** Variables ********************************//

//******************************** Variables ********************************//

//******************************** Functions ********************************//

void externflash_init(void) {

    drv_adapter_externflash_init();
    return;
}

void externflash_deinit(void) {

    drv_adapter_externflash_deinit();

    return;
}

void externflash_read(uint32_t addr, uint32_t size, uint8_t *p_buf) {

    drv_adapter_externflash_read(addr, size, p_buf);

    return;
}

void externflash_write(uint32_t addr, uint32_t size, uint8_t *p_buf) {

    drv_adapter_externflash_write(addr, size, p_buf);

    return;
}

void externflash_write_noerase(void) {

    drv_adapter_externflash_write_noerase();

    return;
}

void externflash_erasechip(void) {

    drv_adapter_externflash_erasechip();

    return;
}

void externflash_erasesector(void) {

    drv_adapter_externflash_erasesector();

    return;
}

void externflash_wakeup(void) {

    drv_adapter_externflash_wakeup();

    return;
}

void externflash_Test(void) {

    drv_adapter_externflash_test();
    return;
}

void externflash_sleep(void) {

    drv_adapter_externflash_sleep();

    return;
}
