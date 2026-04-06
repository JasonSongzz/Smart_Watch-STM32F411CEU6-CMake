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
 #include "EcuVersion.h"
 //******************************** Includes *********************************//

//******************************** Variable **********************************//
const uint8_t         CST_ECUInternalSoftwareVersion[ECU_INTERNAL_SOFTWARE_VERSION_SIZE] = {'A','1','0','0'};
const uint8_t         CST_ECUInternalSubSoftwareVersion[ECU_INTERNAL_SUB_SOFTWARE_VERSION_SIZE] = {'0','0','0','1'};
//******************************** Variable **********************************//