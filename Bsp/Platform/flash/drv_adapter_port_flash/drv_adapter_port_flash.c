/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 * 
 * All Rights Reserved.
 * 
 * @file drv_adapter_port_temphumioment.c
 * 
 * @par dependencies 
 * - drv_adapter_port_temphumioment.h
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
#include "main.h"
#include "osal.h"
#include "drv_adapter_port_flash.h"
#include "drv_adapter_flash.h"
#include "ec_bsp_flash_handler.h"
#include "Debug.h"
#include "spi.h"
#include "stdarg.h"
//******************************** Includes *********************************//

//******************************** Defines **********************************//
//******************************** Defines **********************************//

//******************************** Variables ********************************//
osal_task_handle_t flash_handler_task;
//******************************** Variables ********************************//

//******************************** Functions ********************************//
/***********************************************************************************
 *                 Static Declarations For temphumionment
 ***********************************************************************************/
static void externflash_drv_init(externflash_drv_t *dev);
static void externflash_drv_deinit(externflash_drv_t *dev);
static void externflash_drv_read(externflash_drv_t *dev, uint32_t addr, uint32_t size, uint8_t *p_buf);
static void externflash_drv_write(externflash_drv_t *dev, uint32_t addr, uint32_t size, uint8_t *p_buf);
static void externflash_drv_write_noerase(externflash_drv_t *dev);
static void externflash_drv_erasechip(externflash_drv_t *dev);
static void externflash_drv_erasesector(externflash_drv_t *dev);
static void externflash_drv_wakeup(externflash_drv_t *dev);
static void externflash_drv_sleep(externflash_drv_t *dev);
static void externflash_drv_test(externflash_drv_t *dev);
/***********************************************************************************
 *                 Public Implements
 ***********************************************************************************/
void drv_adapter_externflash_register(void)
{
    externflash_drv_t _externflash_drv = {
        .idx = 0,
        .dev_id = 0,
        .externflash_drv_init = externflash_drv_init,
        .externflash_drv_deinit = externflash_drv_deinit,
        .externflash_drv_read = externflash_drv_read,
        .externflash_drv_write = externflash_drv_write,
        .externflash_drv_write_noerase = externflash_drv_write_noerase,
        .externflash_drv_erasechip = externflash_drv_erasechip,
        .externflash_drv_erasesector = externflash_drv_erasesector,
        .externflash_drv_wakeup = externflash_drv_wakeup,
        .externflash_drv_sleep = externflash_drv_sleep,
        .externflash_drv_test = externflash_drv_test
    };

    drv_adapter_externflash_reg(0, &_externflash_drv);
}

/***********************************************************************************
 *                 Static Implements for Display
 ***********************************************************************************/
void debug_log_i(const char *format,...)
{
  char buf_str[200 + 1];
  va_list args;
  va_start(args, format);
  vsnprintf(buf_str, sizeof(buf_str), format, args);
  va_end(args);
  elog_i("W25Q64:","%s", buf_str);
}

void debug_log_e(const char *format,...)
{
  char buf_str[200 + 1];
  va_list args;
  va_start(args, format);
  vsnprintf(buf_str, sizeof(buf_str), format, args);
  va_end(args);
  elog_e("W25Q64:","%s", buf_str);
}

void debug_log_w(const char *format,...)
{
  char buf_str[200 + 1];
  va_list args;
  va_start(args, format);
  vsnprintf(buf_str, sizeof(buf_str), format, args);
  va_end(args);
  elog_w("W25Q64:","%s", buf_str);
}
uint8_t spi_cs_set(void)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
  return 0;
}

uint8_t spi_cs_deset(void)
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
  return 0;
}

uint8_t spi_write_byte(uint8_t data)
{
  uint8_t ret;
  ret = HAL_SPI_Transmit(&hspi1, (uint8_t *)&data, 1, HAL_MAX_DELAY);
  return ret;
}

uint8_t spi_read_byte(void)
{
  uint8_t data;
  HAL_SPI_Receive(&hspi1, (uint8_t *)&data, 1, HAL_MAX_DELAY);
  return data;
}
extern osal_sema_handle_t userExtFlashSemaHandle;
void read_finish_callback(void * p_buff)
{
  /*数据采集完成，释放信号量*/
  osal_sema_give(userExtFlashSemaHandle);
}

void write_finish_callback(void * p_buff)
{
  __NOP;
}

static void externflash_drv_init(externflash_drv_t *dev)
{
    //外部接口实现挂载
    static spi_w25q64_interface_t spi_w25q64_instance =
    {
        .pf_spi_init = MX_SPI1_Init,
        .pf_spi_deinit = NULL,
        .pf_spi_cs_set = spi_cs_set,
        .pf_spi_cs_deset = spi_cs_deset,
        .pf_spi_write_byte = spi_write_byte,
        .pf_spi_read_byte = spi_read_byte,
        .pf_critical_enter = osal_enter_critical,
        .pf_critical_exit  = osal_exit_critical,
    };
  
    static timebase_interface_t timebase_instance =
    {
        .pf_tick_count_get = HAL_GetTick,
    };
    static os_delay_interface_t os_delay_instance =
    {
        .pf_os_delay_ms = osal_task_delay_ms,
    };
  
    static flash_handler_os_interface_t flash_handler_os_instance =
    {
        .pf_os_delay         = osal_task_delay_ms,
        .pf_os_queue_create  = osal_queue_create,
        .pf_os_queue_receive = osal_queue_receive,
        .pf_os_queue_send    = osal_queue_send
    };
  
    static debug_flash_handler_interface_t debug_flash_handler_instance =
    {
      .pf_log_i = debug_log_i,
      .pf_log_w = debug_log_w,
      .pf_log_e = debug_log_e,
    };
  
    static flash_handler_all_input_arg_t flash_handler_all_input_arg =
    {
        .p_spi_w25q64_instance  = &spi_w25q64_instance,
        .p_timebase_instance    = &timebase_instance,
        .p_debug_flash_instance = &debug_flash_handler_instance,
        .p_os_instance          = &flash_handler_os_instance,
        .p_os_delay_instance    = &os_delay_instance,
    };

    //创建flash handler线程
    osal_task_create("ExtFlashDrv", flash_handler_thread, 512,
        26, flash_handler_task,&flash_handler_all_input_arg);
}

static void externflash_drv_deinit(externflash_drv_t *dev)
{
    //销毁线程
}


static void externflash_drv_read(externflash_drv_t *dev, uint32_t addr, uint32_t size, uint8_t *p_buf)
{
    flash_handler_event_t read_even ={
        .addr = addr,
        .size = size,
        .p_data = p_buf,
        .type = FLASH_HANDLER_EVENT_READ,
        .pf_callback = read_finish_callback,
    };
    flash_handler_event_put(&read_even);
}

static void externflash_drv_write(externflash_drv_t *dev, uint32_t addr, uint32_t size, uint8_t *p_buf)
{
    flash_handler_event_t write_even ={
        .addr = addr,
        .size = size,
        .p_data = p_buf,
        .type = FLASH_HANDLER_EVENT_WRITE,
        .pf_callback = write_finish_callback,
    };
    flash_handler_event_put(&write_even);
}

static void externflash_drv_write_noerase(externflash_drv_t *dev)
{

}

static void externflash_drv_erasechip(externflash_drv_t *dev)
{

}

static void externflash_drv_erasesector(externflash_drv_t *dev)
{

}

static void externflash_drv_wakeup(externflash_drv_t *dev)
{

}

static void externflash_drv_sleep(externflash_drv_t *dev)
{

}

static void externflash_drv_test(externflash_drv_t *dev)
{

}



//******************************** Functions ********************************//
