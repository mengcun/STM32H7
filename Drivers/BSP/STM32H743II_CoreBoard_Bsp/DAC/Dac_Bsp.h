/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/DAC/Dac_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 14-Junly-2018
  * @brief   The template of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __DAC_BSP_H
#define __DAC_BSP_H

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
/** @addtogroup DAC_LFSR_NOISE
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Private define DAC_LFSR_NOISE Private define
  * @{
  */
/**
  * @brief The DAC handler
  */  
extern DAC_HandleTypeDef    	DAC_Handler;
/**
  * @brief Used for DAC WAVE TEST
  */ 
extern uint8_t 					iTestDAC;

/** @}
*/
/*-----------------------------------DAC_LFSR_NOISE Private define---------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Exported_variables DAC_LFSR_NOISE Exported variables
  * @{
  */
/**
  * @brief Unmask DAC channel LFSR for noise wave generation
  */
extern uint32_t DAC_LFSRUnmaskTbl[12];
/**
  * @brief Unmask DAC channel LFSR for triangle wave generation
  */
extern uint32_t DAC_TriangleAmpTbl[12];
/** @}
*/
/*----------------------------------------DAC_LFSR_NOISE Exported variables---------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Exported_Functions DAC_LFSR_NOISE Exported Functions
  * @{
  */
/** @defgroup DAC_LFSR_NOISE_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */

void Bsp_InitNoiseDAC(uint32_t DAC_LFSRUNMASK_BITS);  
void Bsp_InitTriangleDAC(uint32_t DAC_TRIANGLEAMPLITUDE);
void Bsp_InitEscalatorDAC(void);
void Bsp_InitSinWaveDAC(void);
void Bsp_InitDualWaveDAC(uint32_t DAC_TRIANGLEAMPLITUDE, uint32_t DAC_LFSRUNMASK_BITS);

/** @}
*/
/*********************DAC_LFSR_NOISE Exported Functions Group1***************************/
/** @defgroup DAC_LFSR_NOISE_Exported_Functions_Group2 Operation Functions
  * @{
  */
  
void DAC_Ch1_NoiseConfig(uint32_t DAC_LFSRUNMASK_BITS);
void DAC_Ch1_TriangleConfig(uint32_t DAC_TRIANGLEAMPLITUDE);
void DAC_Ch1_EscalatorConfig(void);
void DAC_Ch1_SinWaveConfig(void);
void Bsp_Ch12_DualWaveConfig(uint32_t DAC_TRIANGLEAMPLITUDE, uint32_t DAC_LFSRUNMASK_BITS);

/** @}
*/
/**********************DAC_LFSR_NOISE Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------DAC_LFSR_NOISE Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------DAC_LFSR_NOISE------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __DAC_BSP_H */

/**********************************************END OF FILE******************************************/
