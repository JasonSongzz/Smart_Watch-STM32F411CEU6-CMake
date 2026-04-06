/******************************************************************************
 * Copyright (C) 2026, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file Flash.c
 * 
 * @par dependencies 
 * - Flash.h
 * 
 * @author Jason Song
 * 
 * @brief Functions related to reading and writing in the chip's flash area.
 *        Implemented using HAL library.
 * 
 * Processing flow:
 * 
 * call directly.
 * 
 * @version V2.0 2024-XX-XX (Updated to HAL Library)
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "flash.h"
#include "stm32f4xx_hal.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t STMFLASH_GetFlashSector(uint32_t addr);

/* Private functions ---------------------------------------------------------*/
/* extern variables ---------------------------------------------------------*/

/**
 * @brief  This function is used to erase the flash area.
 * @param  addr: Start address to be erased
 * @param  size: Size of area to be erased (in bytes)
 * @retval 0 : Success
 *         1 : Fail
 */
uint8_t Flash_erase(uint32_t addr, uint32_t size)
{
    //将App代码的所有涉及到的扇区全部擦除

    /*判断size在几个扇区内*/
    uint32_t flash_start_sector = 0;
    uint32_t flash_end_sector = 0;
    flash_start_sector = STMFLASH_GetFlashSector(addr);
    flash_end_sector = STMFLASH_GetFlashSector(addr + size - 1); // 减1以正确计算结束扇区

    // 获取Flash解锁状态
    if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) {
        return 1; // Flash忙，无法操作
    }

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;
    
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; // 对于F4系列，通常使用范围3
    EraseInitStruct.Sector = flash_start_sector;
    EraseInitStruct.NbSectors = (flash_end_sector - flash_start_sector) + 1;

    // 解锁Flash
    if(HAL_FLASH_Unlock() != HAL_OK) {
        return 1;
    }

    // 执行擦除操作
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        HAL_FLASH_Lock(); // 锁定Flash
        return 1;
    }

    // 锁定Flash
    HAL_FLASH_Lock();

    return 0;
}

/**
 * @brief  Get flash sector number by address
 * @param  addr: Address to get sector for
 * @retval Sector number
 */
static uint32_t STMFLASH_GetFlashSector(uint32_t addr)
{
    if(addr < ADDR_FLASH_SECTOR_1) return FLASH_SECTOR_0;
    else if(addr < ADDR_FLASH_SECTOR_2) return FLASH_SECTOR_1;
    else if(addr < ADDR_FLASH_SECTOR_3) return FLASH_SECTOR_2;
    else if(addr < ADDR_FLASH_SECTOR_4) return FLASH_SECTOR_3;
    else if(addr < ADDR_FLASH_SECTOR_5) return FLASH_SECTOR_4;
    else if(addr < ADDR_FLASH_SECTOR_6) return FLASH_SECTOR_5;
    else if(addr < ADDR_FLASH_SECTOR_7) return FLASH_SECTOR_6;
    else if(addr < ADDR_FLASH_SECTOR_8) return FLASH_SECTOR_7;
    else if(addr < ADDR_FLASH_SECTOR_9) return FLASH_SECTOR_8;
    else if(addr < ADDR_FLASH_SECTOR_10) return FLASH_SECTOR_9;
    else if(addr < ADDR_FLASH_SECTOR_11) return FLASH_SECTOR_10;
    return FLASH_SECTOR_11; // 默认返回最后一个扇区
}

/**
 * @brief  Unlock Flash
 */
void Flash_Unlock(void) {
    HAL_FLASH_Unlock();
}

/**
 * @brief  Lock Flash
 */
void Flash_Lock(void) {
    HAL_FLASH_Lock();
}

/**
 * @brief  Erase application sector
 * @param  FLASH_Sector: Sector number to erase
 * @retval HAL_StatusTypeDef
 */
HAL_StatusTypeDef EraseAppSector(uint32_t FLASH_Sector)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;
    
    // 设置擦除参数
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FLASH_Sector;
    EraseInitStruct.NbSectors = 1; // 只擦除一个扇区

    // 解锁Flash
    if(HAL_FLASH_Unlock() != HAL_OK) {
        return HAL_ERROR;
    }

    // 执行擦除操作
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);

    // 锁定Flash
    HAL_FLASH_Lock();

    return status;
}

/**
 * @brief  Write a word to flash
 * @param  address: Address to write to
 * @param  data: Data to write
 * @retval HAL_StatusTypeDef
 */
HAL_StatusTypeDef Flash_Write(uint32_t address, uint32_t data) {
    // 解锁Flash
    if(HAL_FLASH_Unlock() != HAL_OK) {
        return HAL_ERROR;
    }
    
    // 清除所有错误标志
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                          FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    
    // 编程一个字（32位）
    HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);
    
    // 检查编程是否成功
    if (status != HAL_OK) {
        // 数据写入失败
        // 在这里添加错误处理代码
        HAL_FLASH_Lock();
        return status;
    }
    
    // 验证写入的数据
    if (*(uint32_t*)address != data) {
        HAL_FLASH_Lock();
        return HAL_ERROR; // 写入验证失败
    }
    
    // 锁定Flash
    HAL_FLASH_Lock();
    
    return HAL_OK;
}

/**
 * @brief  Read a word from flash
 * @param  address: Address to read from
 * @retval Data at the specified address
 */
uint32_t Flash_Read(uint32_t address) {
    return *(uint32_t*)address;
}

/**
 * @brief  Public wrapper to get flash sector number by address
 */
uint32_t Flash_GetSector(uint32_t addr)
{
    return STMFLASH_GetFlashSector(addr);
}

/**
 * @brief  Get flash sector size in bytes
 */
uint32_t Flash_GetSectorSize(uint32_t sector)
{
    switch (sector) {
        case FLASH_SECTOR_0:
        case FLASH_SECTOR_1:
        case FLASH_SECTOR_2:
        case FLASH_SECTOR_3:
            return 16U * 1024U;
        case FLASH_SECTOR_4:
            return 64U * 1024U;
        case FLASH_SECTOR_5:
        case FLASH_SECTOR_6:
        case FLASH_SECTOR_7:
        case FLASH_SECTOR_8:
        case FLASH_SECTOR_9:
        case FLASH_SECTOR_10:
        case FLASH_SECTOR_11:
            return 128U * 1024U;
        default:
            return 0U;
    }
}

/**
 * @brief  Read bytes from internal flash (memory-mapped)
 */
void Flash_ReadBuffer(uint32_t address, uint8_t* buf, uint32_t len)
{
    memcpy(buf, (const void*)address, len);
}

/**
 * @brief  Write bytes to internal flash (word programming)
 * @note   address and len must be 4-byte aligned. Destination must be erased.
 */
HAL_StatusTypeDef Flash_WriteBuffer(uint32_t address, const uint8_t* buf, uint32_t len)
{
    if ((address & 3U) || (len & 3U)) {
        return HAL_ERROR;
    }

    if (HAL_FLASH_Unlock() != HAL_OK) {
        return HAL_ERROR;
    }

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    HAL_StatusTypeDef status = HAL_OK;
    const uint32_t* p = (const uint32_t*)buf;
    uint32_t words = len / 4U;

    for (uint32_t i = 0; i < words; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i * 4U, p[i]);
        if (status != HAL_OK) {
            break;
        }
    }

    HAL_FLASH_Lock();
    return status;
}
