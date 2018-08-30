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
#include "stm32h743ii_Coreboard_Bsp.h"
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
  * @brief	��ʱ��3���
  */
TIM_HandleTypeDef TIM3_Handler;
/**
  * @brief	��ʱ��3ͨ��1���
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
  * @brief TIM3 PWM���ֳ�ʼ�� 
  * @param 	Period:�Զ���װֵ��
  *			Prescaler:ʱ��Ԥ��Ƶ��
  */

void Bsp_TIM3_PWM_Init(uint16_t Period,uint16_t Prescaler)
{ 
    TIM3_Handler.Instance=TIM3;            										//��ʱ��3
    TIM3_Handler.Init.Prescaler=Prescaler - 1;       								//��ʱ����Ƶ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;							//���ϼ���ģʽ
    TIM3_Handler.Init.Period=Period;          									//�Զ���װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       										//��ʼ��PWM
    
    TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1; 									//ģʽѡ��PWM1
    TIM3_CH1Handler.Pulse=(Period - 1) / 2;            									//���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�
																				//Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; 								//����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);	//����TIM3ͨ��1
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);								//����PWMͨ��4
}

/**
  * @brief 	��ʱ���ײ�������ʱ��ʹ�ܣ���������,�˺����ᱻHAL_TIM_PWM_Init()����
  * @param 	htim:��ʱ�����
  */

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();						//ʹ�ܶ�ʱ��3
    __HAL_RCC_GPIOB_CLK_ENABLE();						//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_4;           				//PB3 PIN4
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  				//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          			//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     	//����
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;				//PB3����ΪTIM3_CH1
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
  * @brief 	����TIMͨ��4��ռ�ձ�
  * 		compare:�Ƚ�ֵ
  */

void Bsp_SetTIM3Compare1(uint32_t Compare)
{
	TIM3->CCR1=Compare; 
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
