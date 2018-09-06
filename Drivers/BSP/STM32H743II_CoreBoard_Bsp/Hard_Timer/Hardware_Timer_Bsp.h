/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/HARD_TIMER/Hardware_Timer_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 21-August-2018
  * @brief   The Hardware_Timer of header file.
  *					 
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __HARDWARE_TIMER_BSP_H
#define __HARDWARE_TIMER_BSP_H

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
/** @addtogroup Hardware_Timer
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup Hardware_Timer_Private define Hardware_Timer Private define
  * @{
  */
/**
  * @brief  The define for Hardware_Timer DEBUG
  */
#define HARDWARE_TIMER_DEBUG		0
/** @}
*/
/*---------------------------------Hardware_Timer Private define-----------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/** @defgroup Hardware_Timer_Private_Exported constants Hardware_Timer Exported constants
  * @{
  */
/**
  * @brief		TIM4 Private Handle
  */
extern TIM_HandleTypeDef 		TIM4_Handler;      //定时器4句柄 
/**
  * @brief		usmart_dev
  */
extern struct _m_usmart_dev usmart_dev;				//在usmart_config.c里面定义

/** @}
*/
/*----------------------------------------Hardware_Timer_Exported constants---------------------------------------*/

/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup Hardware_Timer_Exported_Functions Hardware_Timer Exported Functions
  * @{
  */
/** @defgroup Hardware_Timer_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
/** @}
*/
/*********************Hardware_Timer Exported Functions Group1***************************/
/** @defgroup Hardware_Timer_Exported_Functions_Group2 Operation Functions
  * @{
  */
void Bsp_InitHardTimer_TIM1(void);
void Bsp_InitHardTimer_TIM3(uint16_t Frequence,uint16_t Prescaler,uint32_t PWM_Duty);
void Bsp_InitHardTimer_TIM4(uint16_t Period,uint16_t Prescaler);
void Bsp_InitHardTimer_TIM5(void);
void Bsp_InitHardTimer_TIM6(void);
void Bsp_InitHardTimer_HRTIM(void);

void Bsp_StarGenerictHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack);
void _pFunctionHardTimeout(void);
uint32_t TIM1_ComputeInputFreq(void);
/** @}
*/
/**********************Hardware_Timer Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------Hardware_Timer Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------Hardware_Timer------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __HARDWARE_TIMER_BSP_H */

/**********************************************END OF FILE******************************************/
