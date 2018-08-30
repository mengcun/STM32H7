/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/Hard_Timer/Hardware_Timer_Bsp.h
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
/** @defgroup Hardware_Timer_Private_Define Hardware_Timer Private Define
  * @{
  */
/**
  * @brief		TIMx取值TIM2 - TIM5;
  *			 	TIM2和TIM5 是32位定时器。定时范围很大
  *			 	TIM3和TIM4 是16位定时器,TIM3用于PWM_CHANNEL1生成，在PB3引脚上输出
  */
#define TIMx							TIM5
#define TIMx_Handler					TIM5_Handler
#define	TIMx_IRQn						TIM5_IRQn
#define	__HAL_RCC_TIMx_CLK_ENABLE()		__HAL_RCC_TIM5_CLK_ENABLE()            //使能TIMx时钟
/**
  * @brief		TIMx的自动重装载值大小，分频到了1us;
  *				对于16位定时器，TIM3 和TIM4 最大0xFFFF / 1000 = 65.5ms; 
  *				对于32位定时器，TIM2 和TIM5 最大0xFFFFFFFF / 1000000 = 4294s
  */	
#define usPeriod	0x000F4240	// 1s = 1 * 1000000 us

/** @}
*/
/**********************Hardware_Timer Private Define**************************/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
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
void Bsp_InitGeneralHardTimer(void);
void Bsp_InitHardTimer_TIM1(void);
void Bsp_StarGenerictHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack);
void _pFunctionHardTimeout(void);
uint32_t GetFrequency(void);
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
