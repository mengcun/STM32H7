/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/RNG/Rng_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 01-September-2018
  * @brief   The template of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __RNG_BSP_H
#define __RNG_BSP_H

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
/** @addtogroup RNG
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup RNG_Private define RNG Private define
  * @{
  */
/**
  * @brief  The define for Interrupt RNG enable
  */
#define RNG_IT_ENABLE		0
/**
  * @brief  The define for RNG DEBUG
  */
#define RNG_DEBUG		0
/**
  * @brief  The size of databuffer that store the Random Number
  */
#define RANDOM_SIZE		8
/** @}
*/
/*----------------------------------------RNG Private define---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/** @defgroup RNG_Exported_Macro RNG Exported constants
  * @{
  */
/**
  * @brief		RNG Private Handle
  */
extern RNG_HandleTypeDef Rng_Handler;

/**
  * @brief		Used for storing 8 Random 32bit Numbers in IT mode
  */
extern __IO uint32_t IT_Random32bit;

/**
  * @brief		Used for storing 8 Random 32bit Numbers in POLL mode
  */
extern __IO uint32_t POLL_Random32bit;

/**
  * @brief		Data need to be compute RNG
  */
/** @}
*/
/*----------------------------------------RNG Exported constants---------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup RNG_Exported_Functions RNG Exported Functions
  * @{
  */
/** @defgroup RNG_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
void Bsp_InitRNG(void);
void RNG_Get_RandomNum(void);
void RNG_Get_RandomRange(uint32_t min,uint32_t max);

/** @}
*/
/*********************RNG Exported Functions Group1***************************/
/** @defgroup RNG_Exported_Functions_Group2 Operation Functions
  * @{
  */
/** @}
*/
/**********************RNG Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------RNG Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------RNG------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __RNG_BSP_H */

/**********************************************END OF FILE******************************************/
