#ifndef __ST7789_LINK
#define __ST7789_LINK

#include "main.h"
#include "stm32f411xe.h"

#include "bsp_st7789_driver.h"

#include "FreeRTOS.h"
#include "task.h"

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_tx;

static inline uint8_t spi_transmit_data( uint8_t *pData, uint32_t dataLength)
{
	HAL_SPI_Transmit(&hspi2, pData, dataLength, 0xFF);
	return 0;
}

static inline uint8_t spi_transmit_data_dma( uint8_t *pData, uint32_t dataLength)
{
	HAL_SPI_Transmit_DMA(&hspi2, pData, dataLength);
	while (hspi2.hdmatx->State != HAL_DMA_STATE_READY);
	
	return 0;
}

//    uint8_t (*pf_write_reset_pin)(uint8_t pinState);
//    uint8_t (*pf_write_cs_pin)(uint8_t pinState); 
//    uint8_t (*pf_write_dc_pin)(uint8_t pinState); 
static inline uint8_t gpio_write_reset_pin(uint8_t pinState)
{
	if(0 == pinState)
	{
		HAL_GPIO_WritePin(SPI_RESET_GPIO_Port, SPI_RESET_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(SPI_RESET_GPIO_Port, SPI_RESET_Pin, GPIO_PIN_SET);
	}
	return pinState;
}

static inline uint8_t gpio_write_cs_pin(uint8_t pinState)
{
	if(0 == pinState)
	{
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
	}
	return pinState;
}

static inline uint8_t gpio_write_dc_pin(uint8_t pinState)
{
	if(0 == pinState)
	{
		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	}
	return pinState;
}


/*****************************************************************************/






#endif /* __ST7789_LINK */
