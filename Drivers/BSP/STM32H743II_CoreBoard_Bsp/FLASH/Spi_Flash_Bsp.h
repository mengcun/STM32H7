/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/FLASH/Spi_Flash_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 14-September-2018
  * @brief   The SPI_FLASH of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __SPI_FLASH_BSP_H
#define __SPI_FLASH_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h7xx_hal.h"
/*******************************************APP/BSP*************************************************/
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup SPI_FLASH
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH SPI_FLASH Private define
  * @{
  */
  
/**
  * @brief W25X系列/Q系列芯片列表
  */  
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

/**
  * @brief W25X系列指令表
  */  
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9
#define W25X_SetReadParam		0xC0 
#define W25X_EnterQPIMode       0x38
#define W25X_ExitQPIMode        0xFF
/** @}
*/		
/*--------------------------------------SPI_FLASH Private define--------------------------------*/

/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH SPI_FLASH Private variables
  * @{
  */
/**
  * @brief The Handler for QSPI
  */  
extern QSPI_HandleTypeDef 		QSPI_Handler;
/**
  * @brief The Type of W25QXX
  */ 
  
extern uint16_t W25QXX_TYPE;		   
/**
  * @brief The Total Amount of W25QXX
  */ 
extern uint32_t TotalSize;	

/**
  * @brief The Section Amount of W25QXX
  */ 
extern uint16_t PageSize;

/** @}
*/		
/*--------------------------------------SPI_FLASH Private variables--------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH_Exported_Functions SPI_FLASH Exported Functions
  * @{
  */
/** @defgroup SPI_FLASH_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
uint8_t Bsp_QSPI_Init(void);
uint8_t Bsp_W25QXX_Init(void);
/** @}
*/
/*********************SPI_FLASH Exported Functions Group1***************************/
/** @defgroup SPI_FLASH_Exported_Functions_Group2 Operation Functions
  * @{
  */
uint8_t Bsp_QSPI_Receive(uint8_t* buf,uint32_t datalen);
uint8_t Bsp_QSPI_Transmit(uint8_t* buf,uint32_t datalen);
uint16_t Bsp_W25QXX_ReadID(void);  	    					//读取FLASH ID
uint8_t Bsp_W25QXX_ReadSR(uint8_t regno);       			//读取状态寄存器
uint8_t Bsp_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen);	//判断是否需要擦除

void Bsp_QSPI_Send_CMD(uint32_t instruction,uint32_t address,uint32_t dummyCycles,uint32_t instructionMode,uint32_t addressMode,uint32_t addressSize,uint32_t dataMode);
void Bsp_Read_FLASH_Info(void);
void Bsp_W25QXX_QSPI_Enable(void);							//使能QSPI模式
void Bsp_W25QXX_QSPI_Disable(void);							//关闭QSPI模式
void Bsp_W25QXX_4ByteAddr_Enable(void);     				//使能4字节地址模式
void Bsp_W25QXX_Write_SR(uint8_t regno,uint8_t sr);  		//写状态寄存器
void Bsp_W25QXX_Write_Enable(void);  						//写使能 
void Bsp_W25QXX_Write_Disable(void);						//写保护
void Bsp_W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);	//写flash,不校验
void Bsp_W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   			//读取flash
void Bsp_W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);			//写入flash
void Bsp_W25QXX_Erase_Chip(void);    	  					//整片擦除
void Bsp_W25QXX_Erase_Sector(uint32_t Dst_Addr);			//扇区擦除
void Bsp_W25QXX_Wait_Busy(void);           					//等待空闲

void Bsp_ReadTest(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
void Bsp_ReadSpeedTest(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
void Bsp_Write_With_Check_Test(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void Bsp_Write_Without_Check_Test(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void Bsp_Write_All_Page_Test(uint8_t _data, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void Bsp_Erase_ChipTest(void);
void Bsp_Erase_Page_Test(uint32_t Dst_Addr);

/** @}
*/
/**********************SPI_FLASH Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------SPI_FLASH Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------SPI_FLASH------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_BSP_H */

/**********************************************END OF FILE******************************************/
