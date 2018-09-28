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
//prer:分频数:0~7(只有低3位有效!),决定分频因子，但在HAL库中直接定义的是换算后的分频因子
//rlr:自动重装载值,0~0XFFF.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms). (4*2^prer)
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
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
    IWDG_Handler.Init.Prescaler	=	prescaler;    				//设置IWDG分频系数
    IWDG_Handler.Init.Reload	=	reloard;        			//重装载
    IWDG_Handler.Init.Window	=	IWDG_WINDOW_DISABLE;		//关闭窗口功能
    if(HAL_IWDG_Init(&IWDG_Handler)  != HAL_OK)					//初始化并启用看门狗
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
    WWDG_Handler.Init.Prescaler	=	prescaler;  	//设置分频系数	分频系数（WDGTB）,3位有效
    WWDG_Handler.Init.Window	=	Window_counter; //设置窗口值	W[6:0],上窗口值 95 cycles,确保大于0X40,否则窗口不存在.
    WWDG_Handler.Init.Counter	=	timer_counter;  //设置计数器值	T[6:0],计数器值  127 cycles
    WWDG_Handler.Init.EWIMode	=	WWDG_EWI_ENABLE;//使能窗口看门狗提前唤醒中断
	if (HAL_WWDG_Init(&WWDG_Handler) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
	/*WWDG_timeout:必须在此时间段内喂狗，但是因为使用的是中断喂狗，这里只是把时间段打印出来,大约为11ms*/
	return TimeoutCalculation((WWDG_Handler.Init.Counter-WWDG_Handler.Init.Window) + 1) + 1;
}

/**
  * @brief 	WWDG MSP Initialization 
  * @param 	hwwdg: WWDG handle pointer
  */
//此函数会被HAL_WWDG_Init()调用,WWDG底层驱动，时钟配置，中断配置
//与IWDG不同，WWDG没有专用32KHz时钟，使用的是PCLK = 100MHz的时钟
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
    __HAL_RCC_WWDG1_CLK_ENABLE();           							//使能窗口看门狗时钟
    HAL_NVIC_SetPriority(WWDG_IRQn,0,0);    							//设置窗口看门狗中断优先级，抢占优先级0，子优先级0，窗口看门狗大约每11ms喂狗一次，优先级最高，可抢占串口中断。
    HAL_NVIC_EnableIRQ(WWDG_IRQn);          							//使能窗口看门狗中断
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
  * @brief 	IWDG看门狗喂狗函数
  */
void Bsp_IWDG_Feed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); //重装载
}

/**
  * @brief 	WWDG窗口看门狗中断服务函数

  */
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&WWDG_Handler);
}

/**
  * @brief 	中断服务函数处理过程，此函数会被HAL_WWDG_IRQHandler()调用
  */

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
#if WWDG_ENABLE == 1
	HAL_WWDG_Refresh(&WWDG_Handler);//更新窗口看门狗值
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
