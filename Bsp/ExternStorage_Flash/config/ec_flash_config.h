/******************************************************************************
 * Copyright (C) 2024 mizhijitou
 * 
 * All Rights Reserved.
 * 
 * @file ec_flash_config.h
 * 
 * 
 * @author mizhijitou
 * 
 * @brief Provide the flash config.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V1.0 2024-12-10
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/
#ifndef __EC_FLASH_CONFIG_H__
#define __EC_FLASH_CONFIG_H__

/***********************************Defines **********************************/
/*-------DEBUG Config ----------*/
#define FLASH_DEBUG /*All the Switch before use debug port need enable it*/

#ifdef  FLASH_DEBUG

#define W25Q64_DEBUG            /*Driver Debug*/
#define FLASH_HANDLER_DEBUG     /*Handler Debug*/

#endif

/*-------OS Config --------------*/
#define OS_SUPPORT

/*-------Hardware Config --------*/
#define USE_HARDWARE_SPI

/***********************************Defines **********************************/
#endif
