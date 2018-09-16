/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/PWM/Pwm_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    28-August-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The PWM canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "Coreboard_Bsp.h"
/*******************************************APP/BSP*************************************************/
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup PWM PWM
  * @brief PWM
	* @note	PWM
  * @{
  */

/* Private functions Prototypes -------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup PWM_Exported_Functions PWM Exported Functions
  * @{
  */
/** @defgroup PWM_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize PWM
  @endverbatim
  * @{
  */
/** @}
*/
/*********************PWM Exported Functions Group1**************************/

/** @defgroup PWM_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the PWM
  @endverbatim
  * @{
  */

/**
  * @brief 	Set PWM_Duty TIM3 Compare1 For PWM. PWM输出频率Frequence = TIM3CLK /  (Prescaler * Period)
  * @param 	PWM_Duty: 0 -100
  */

void Bsp_SetTIM2Compare1(uint32_t Period, uint16_t Prescaler, uint8_t PWM_Duty, uint32_t TIM_CHANNEL_X)
{
	if(TIM_CHANNEL_X == TIM_CHANNEL_3)
	{
		TIM2->ARR  = Period - 1;          					//自动重装载值,从0开始当计数到Period - 1时重新开始从0计数,总共计数为Period
		TIM2->PSC  = Prescaler - 1;          				//分频系数
		TIM2->CCR3 = (uint32_t)(Period * PWM_Duty * 0.01);  //设置比较值,此值用来确定占空比，	
#if HARDWARE_TIM2PWM_DEBUG == 1		
		Bsp_Printf("The PWM in PA2, TIM_CHANNEL_3 is generated as %d Hz, %d%% \r\n",(uint32_t)(200000000 / (Period * Prescaler)), PWM_Duty);	
#endif
	}
	if(TIM_CHANNEL_X == TIM_CHANNEL_4)
	{
		TIM2->ARR  = Period - 1;          					//自动重装载值,从0开始当计数到Period - 1时重新开始从0计数,总共计数为Period
		TIM2->PSC  = Prescaler - 1;          				//分频系数
		TIM2->CCR4 = (uint32_t)(Period * PWM_Duty * 0.01);  //设置比较值,此值用来确定占空比，
#if HARDWARE_TIM2PWM_DEBUG == 1		
		Bsp_Printf("The PWM in PA3, TIM_CHANNEL_4 is generated as %d Hz, %d%% \r\n",(uint32_t)(200000000 / (Period * Prescaler)), PWM_Duty);	
#endif
	}
}

/** @}
*/
/****************************PWM Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------PWM Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------PWM-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
