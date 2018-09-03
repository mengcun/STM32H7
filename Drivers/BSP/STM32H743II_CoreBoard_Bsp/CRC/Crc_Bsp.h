/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/CRC/Crc_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 01-September-2018
  * @brief   The template of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __CRC_BSP_H
#define __CRC_BSP_H

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
/** @addtogroup CRC
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup CRC_Private define CRC Private define
  * @{
  */
/**
  * @brief  The size of databuffer that store the data need to be compute CRC
  */
#define DATA_BUFFER_SIZE		114
/**
  * @brief  The user defined polynomial
  */
#define CRC_POLYNOMIAL_USER_DEFINE		0x9B		/* X^8 + X^7 + X^4 + X^3 + X + 1 */
#define CRC_POLYNOMIAL_TRAIN_COMM  		0x65  		/* X^7 + X^6 + X^5 + X^2 + 1, used in Train Communication Network, IEC 60870-5[17] */

/** @}
*/
/*----------------------------------------CRC Private define---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/** @defgroup CRC_Exported_Macro CRC Exported constants
  * @{
  */
/**
  * @brief		CRC Private Handle
  */
extern CRC_HandleTypeDef   CRC_Handler;
 
/**
  * @brief		Storing the computed CRC value
  */
extern __IO uint32_t uwCRCValue;

/**
  * @brief		Expected CRC Value
  */
extern uint32_t uwExpectedCRCValue;

/**
  * @brief		Data need to be compute CRC
  */
/** @}
*/
/*----------------------------------------CRC Exported constants---------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup CRC_Exported_Functions CRC Exported Functions
  * @{
  */
/** @defgroup CRC_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
void Bsp_InitDefautCRC(void);
void Bsp_InitUserDefineCRC(void);
/** @}
*/
/*********************CRC Exported Functions Group1***************************/
/** @defgroup CRC_Exported_Functions_Group2 Operation Functions
  * @{
  */
void Bsp_ComputeCRCDefault(void);
void Bsp_ComputeCRCAccumulate(void);
/** @}
*/
/**********************CRC Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------CRC Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------CRC------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __CRC_BSP_H */

/**********************************************END OF FILE******************************************/
