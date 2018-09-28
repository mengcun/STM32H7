/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/WDG/Wdg_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    26-August-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The WatchDog canbe used as follows:
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
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup WatchDog WatchDog
  * @brief WatchDog
  * @{
  */

/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup WatchDog_Private_Variables WatchDog Private Variables
  * @{
  */
/**
  * @brief	WatchDog Private Handle
  */
IWDG_HandleTypeDef 	IWDG_Handler; 
/**
  * @brief	Window WatchDog Private Handle
  */
WWDG_HandleTypeDef   WWDG_Handler;

/** @}
*/		
/*----------------------------WatchDog Private Variables--------------------------------*/
/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup WDG_Private_Functions WatchDog Private Functions
  * @{
  */
/**
  * @brief  Timeout calculation function.
  */
static uint32_t TimeoutCalculation(uint32_t timevalue);
/** @}
*/
/*--------------------------------------WatchDog Private Functions---------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup WDG_Exported_Functions WatchDog Exported Functions
  * @{
  */
/** @defgroup WDG_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize WatchDog
  @endverbatim
  * @{
  */
/**
  * @brief 	Initialize the WatchDog according to the specified
  * @param 	prescaler: 0 - 7.
  * @param	reloard: 0 - 0XFFF.
  * @note	Tout=((4*2^prescaler)*reloard)/32 (ms). (4*2^prescaler)
  */
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!),������Ƶ���ӣ�����HAL����ֱ�Ӷ�����ǻ����ķ�Ƶ����
//rlr:�Զ���װ��ֵ,0~0XFFF.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms). (4*2^prer)
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
void Bsp_IWDG_Init(uint8_t prescaler,uint16_t reloard)
{
	/*Check if the system has resumed from IWDG reset*/
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST) != RESET)
	{
		/* IWDG1RST flag set: Turn LED1_Green on */
		Bsp_LED_On(LED1_Green);
		/* Insert 4s delay */
		Bsp_Delay_ms(4000);
		/* Clear reset flags in any case */
		__HAL_RCC_CLEAR_RESET_FLAGS();
		/* Prior to clear IWDG1RST flag: Turn LED1_Green off */
		Bsp_LED_Off(LED1_Green);
	}
	/*Init & Start IWDG peripheral*/
    IWDG_Handler.Instance		=	IWDG1;
    IWDG_Handler.Init.Prescaler	=	prescaler;    				//����IWDG��Ƶϵ��
    IWDG_Handler.Init.Reload	=	reloard;        			//��װ��
    IWDG_Handler.Init.Window	=	IWDG_WINDOW_DISABLE;		//�رմ��ڹ���
    if(HAL_IWDG_Init(&IWDG_Handler)  != HAL_OK)					//��ʼ�������ÿ��Ź�
	{
		/* Initialization Error */
		Error_Handler();
	}
}

/**
  * @brief 	Initialize the Window WatchDog according to the specified
  * @param 	timer_counter: T[6:0], Counter value
  * @param	Window_counter: W[6:0], Window value
  * @param	prescaler: Frequency division(WDGTB), 3-bit efficiency 
  * @return WWDG_timeout(ms)
 */
  /*		WWDG_timeout(ms) = ((4096 * 2^WDGTB * timer_counter) / (pclk1 / 1000));	
  * 		WDGTB	T[5:0] = 0X00	T[5:0] = 0X3F
  *			0 		40.96us			2.62ms
  *			1 		81.92us 		5.24ms
  *			2 		163.84us 		10.48ms
  *			3 		372.68us 		20.96ms
  */
uint32_t Bsp_WWDG_Init(uint8_t timer_counter,uint8_t Window_counter,uint32_t prescaler)
{
	/*Check if the system has resumed from WWDG reset*/
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDG1RST) != RESET)
	{
		/* WWDG1RST flag set: Turn LED1 on */
		Bsp_LED_On(LED1_Blue);
		/* Insert 4s delay */
		Bsp_Delay_ms(4000);
		/* Clear reset flags in any case */
		__HAL_RCC_CLEAR_RESET_FLAGS();
		/* Prior to clear WWDG1RST flag: Turn LED1 off */
		Bsp_LED_Off(LED1_Blue);
	}
	/* Enable system wide reset */
	HAL_RCCEx_WWDGxSysResetConfig(RCC_WWDG1);
    WWDG_Handler.Instance		=	WWDG1;
    WWDG_Handler.Init.Prescaler	=	prescaler;  	//���÷�Ƶϵ��	��Ƶϵ����WDGTB��,3λ��Ч
    WWDG_Handler.Init.Window	=	Window_counter; //���ô���ֵ	W[6:0],�ϴ���ֵ 95 cycles,ȷ������0X40,���򴰿ڲ�����.
    WWDG_Handler.Init.Counter	=	timer_counter;  //���ü�����ֵ	T[6:0],������ֵ  127 cycles
    WWDG_Handler.Init.EWIMode	=	WWDG_EWI_ENABLE;//ʹ�ܴ��ڿ��Ź���ǰ�����ж�
	if (HAL_WWDG_Init(&WWDG_Handler) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
	/*WWDG_timeout:�����ڴ�ʱ�����ι����������Ϊʹ�õ����ж�ι��������ֻ�ǰ�ʱ��δ�ӡ����,��ԼΪ11ms*/
	return TimeoutCalculation((WWDG_Handler.Init.Counter-WWDG_Handler.Init.Window) + 1) + 1;
}

/**
  * @brief 	WWDG MSP Initialization 
  * @param 	hwwdg: WWDG handle pointer
  */
//�˺����ᱻHAL_WWDG_Init()����,WWDG�ײ�������ʱ�����ã��ж�����
//��IWDG��ͬ��WWDGû��ר��32KHzʱ�ӣ�ʹ�õ���PCLK = 100MHz��ʱ��
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
    __HAL_RCC_WWDG1_CLK_ENABLE();           							//ʹ�ܴ��ڿ��Ź�ʱ��
    HAL_NVIC_SetPriority(WWDG_IRQn,0,0);    							//���ô��ڿ��Ź��ж����ȼ�����ռ���ȼ�0�������ȼ�0�����ڿ��Ź���Լÿ11msι��һ�Σ����ȼ���ߣ�����ռ�����жϡ�
    HAL_NVIC_EnableIRQ(WWDG_IRQn);          							//ʹ�ܴ��ڿ��Ź��ж�
}

/**
  * @brief  Timeout calculation function.
  *         This function calculates any timeout related to 
  *         WWDG with given prescaler and system clock.
  * @param  timevalue: period in term of WWDG counter cycle.(timer_counter - Window_counter)
  * @retval None
  */
static uint32_t TimeoutCalculation(uint32_t timevalue)
{
	uint32_t timeoutvalue = 0;
	uint32_t pclk1 = 0;
	uint32_t wdgtb = 0;
	/* Get PCLK1 value */
	pclk1 = HAL_RCC_GetPCLK1Freq();
	/* get prescaler */
	switch(WWDG_Handler.Init.Prescaler)
	{
		case WWDG_PRESCALER_1:   wdgtb = 1;   break;
		case WWDG_PRESCALER_2:   wdgtb = 2;   break;
		case WWDG_PRESCALER_4:   wdgtb = 4;   break;
		case WWDG_PRESCALER_8:   wdgtb = 8;   break;
		case WWDG_PRESCALER_16:  wdgtb = 16;  break;
		case WWDG_PRESCALER_32:  wdgtb = 32;  break;
		case WWDG_PRESCALER_64:  wdgtb = 64;  break;
		case WWDG_PRESCALER_128: wdgtb = 128; break;
		default: Error_Handler(); break;
	}
	/* calculate timeout (ms)*/
	timeoutvalue = ((4096 * wdgtb * timevalue) / (pclk1 / 1000));	
	return timeoutvalue;
}

/** @}
*/
/*********************WatchDog Exported Functions Group1**************************/

/** @defgroup WDG_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the WatchDog
  @endverbatim
  * @{
  */
/**
  * @brief 	IWDG���Ź�ι������
  */
void Bsp_IWDG_Feed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); //��װ��
}

/**
  * @brief 	WWDG���ڿ��Ź��жϷ�����

  */
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&WWDG_Handler);
}

/**
  * @brief 	�жϷ�����������̣��˺����ᱻHAL_WWDG_IRQHandler()����
  */

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
#if WWDG_ENABLE == 1
	HAL_WWDG_Refresh(&WWDG_Handler);//���´��ڿ��Ź�ֵ
#endif
#if WWDG_DEBUG == 1
	Bsp_Printf("WWDG feed in HAL_WWDG_EarlyWakeupCallback() every 11 ms!\r\n");
#endif
}

/** @}
*/
/****************************WatchDog Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------WatchDog Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------WatchDog-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
