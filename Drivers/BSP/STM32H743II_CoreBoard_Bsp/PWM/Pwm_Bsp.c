/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/PWM/Pwm_Bsp.c
  * @author  MCD Application Team
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
/** @defgroup PWM_Private_Functions_Prototypes PWM Private Functions Prototypes
  * @{
  */
//CODING HERE
/** @}
*/		
/*----------------------------PWM Private Functions Prototypes--------------------------------*/ 
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup WatchDog_Private_Variables WatchDog Private Variables
  * @{
  */
/**
  * @brief	定时器3句柄
  */
TIM_HandleTypeDef TIM3_Handler;
/**
  * @brief	定时器3通道1句柄
  */
TIM_OC_InitTypeDef TIM3_CH1Handler; 

/** @}
*/		
/*----------------------------WatchDog Private Variables--------------------------------*/

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

/**
  * @brief TIM3 PWM Init 
  * @param 	Frequence: The PWM Frequence(KHz)。
  * @param 	Prescaler: CLK divide
  * @param 	PWM_Duty: duty
  */

void Bsp_TIM3_PWM_Init(uint16_t Frequence,uint16_t Prescaler,uint32_t PWM_Duty)
{ 
    TIM3_Handler.Instance=TIM3;            										//定时器3
    TIM3_Handler.Init.Prescaler=Prescaler - 1;       							//定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;							//向上计数模式
    TIM3_Handler.Init.Period=1000 / Frequence;          						//自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       										//初始化PWM
    
    TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1; 									//模式选择PWM1
    TIM3_CH1Handler.Pulse=(TIM3_Handler.Init.Period - 1) * 0.5;            		//设置比较值,此值用来确定占空比,默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; 								//输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);	//配置TIM3通道1
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);								//开启PWM通道1
	
	Bsp_SetTIM3Compare1(Frequence,PWM_Duty);									//设置新的占空比

}

/**
  * @brief 	Low level BSP
  * @param 	htim: The timer handle
  */
//定时器底层驱动，时钟使能，引脚配置,此函数会被HAL_TIM_PWM_Init()调用
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();						//使能定时器3
    __HAL_RCC_GPIOB_CLK_ENABLE();						//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_4;           				//PB4 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  				//复用推完输出
    GPIO_Initure.Pull=GPIO_PULLUP;          			//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     	//高速
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;				//PB3复用为TIM3_CH1
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

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
  * @brief 	Set TIMER CHANNEL 1
  * @param 	PWM_Duty: Duty
  */

void Bsp_SetTIM3Compare1(uint16_t Frequence,uint8_t PWM_Duty)
{
	TIM3->ARR=1000 / Frequence;          							 //自动重装载值,周期
    TIM3->CCR1=(TIM3_Handler.Init.Period - 1) * PWM_Duty * 0.01;     //设置比较值,此值用来确定占空比，
//#if SYSTEM_DEBUG == 1		
	Bsp_Printf("The PWM in PB4 is generated as %d Khz, %d%% \r\n",Frequence,PWM_Duty);
//#endif
}
//设置TIM通道4的占空比
//compare:比较值
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
