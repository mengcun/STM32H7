/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/SD_MMC_C_H/SD_MMC.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 14-Junly-2018
  * @brief   The template of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __SD_BSP_H
#define __SD_BSP_H

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
/** @addtogroup SD_MMC
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup SD_MMC_Private_define SD_MMC Private define
  * @{
  */
/**
  * @brief  SD_MMC Macro definition
  */
#define SD_TIMEOUT      	((uint32_t)100000000)       //超时时间
#define SD_TRANSFER_OK     	((uint8_t)0x00)
#define SD_TRANSFER_BUSY   	((uint8_t)0x01)
/** @}
*/
/*----------------------------------------SD_MMC Private define---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup SD_MMC_Exported_variables SD_MMC Exported variables
  * @{
  */
extern SD_HandleTypeDef        SDCARD_Handler;      	//SD卡句柄
extern HAL_SD_CardInfoTypeDef  SDCardInfo;          	//SD卡信息结构体

/** @}
*/
/*----------------------------------------SD_MMC Exported variables---------------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup SD_MMC_Exported_Functions SD_MMC Exported Functions
  * @{
  */
/** @defgroup SD_MMC_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
uint8_t Bsp_SD_Init(void);

/** @}
*/
/*********************SD_MMC Exported Functions Group1***************************/
/** @defgroup SD_MMC_Exported_Functions_Group2 Operation Functions
  * @{
  */
uint8_t Bsp_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo);
uint8_t Bsp_SD_GetCardState(void);
uint8_t Bsp_SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt);
uint8_t Bsp_SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt);
void Bsp_Show_SD_Info(void);
void Bsp_SD_ReadTest(uint32_t secaddr,uint32_t seccnt);
void Bsp_SD_WriteTest(uint32_t secaddr,uint32_t seccnt);


/** @}
*/
/**********************SD_MMC Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------SD_MMC Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------SD_MMC------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __SD_BSP_H */

/**********************************************END OF FILE******************************************/
