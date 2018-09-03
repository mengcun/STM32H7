/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/SDRAM/Sdram_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 20-Junly-2018
  * @brief   The template of header file.
  *					 Please copy this file and rename the file name if needed.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __SDRAM_BSP_H
#define __SDRAM_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h7xx_hal.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup SDRAM_BSP
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup SDRAM_Private_Define SDRAM Private Define
  * @{
  */
/**
  * @brief  SDRAM Private Define
	* @note		SDRAM Address
  */
#define SDRM_TEST  0

#define Bank5_SDRAM_ADDR    ((uint32_t)(0XC0000000)) //SDRAM开始地址
//SDRAM配置参数
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

/** @}
*/
/*----------------------------------------SDRAM Private Define---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/** @defgroup SDRAM_Exported_Macro SDRAM Exported Macro
  * @{
  */
/**
  * @brief  SDRAM Macro definition
	* @note		SDRAM extern macro
  */
extern SDRAM_HandleTypeDef SDRAM_Handler;//SDRAM句柄
/** @}
*/
/*----------------------------------------SDRAM Exported Macro---------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup SDRAM_Exported_Functions SDRAM Exported Functions
  * @{
  */
/** @defgroup SDRAM_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
void Bsp_SDRAM_Init(void);
void Bsp_SDRAM_MPU_Config(void);
/** @}
*/
/*********************SDRAM Exported Functions Group1***************************/
/** @defgroup SDRAM_Exported_Functions_Group2 Operation Functions
  * @{
  */

uint8_t Bsp_SDRAM_Send_Cmd(uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval);
void Bsp_FMC_SDRAM_WriteBuffer(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t n);
void Bsp_FMC_SDRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n);
void Bsp_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram);
void Bsp_Fsmc_Sdram_Test(void);
/** @}
*/
/**********************SDRAM Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------SDRAM Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------SDRAM_BSP------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __SDRAM_BSP_H */

/**********************************************END OF FILE******************************************/
