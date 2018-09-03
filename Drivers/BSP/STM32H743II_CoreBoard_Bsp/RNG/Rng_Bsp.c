/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/RNG/Rng_Bsp.c
  * @author  MCD Application Team
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 01-September-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The RNG canbe used as follows:
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
/** @defgroup RNG RNG
  * @brief RNG
	* @note	RNG
  * @{
  */

/* Private Exported constants -------------------------------------------------------------------*/
/** @defgroup RNG_Private_Exported constants RNG Private Exported constants
  * @{
  */
/**
  * @brief		RNG Private Handle
  */
RNG_HandleTypeDef Rng_Handler;
 
/**
  * @brief		Used for storing Random 32bit Numbers generated in IT mode
  */
__IO uint32_t IT_Random32bit = 0;


/**
  * @brief		Used for storing Random 32bit Numbers generated in Polling mode
  */
__IO uint32_t POLL_Random32bit = 0;

/** @}
*/		
/*----------------------------RNG Private Exported constants--------------------------------*/ 
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup RNG_Exported_Functions RNG Exported Functions
  * @{
  */
/** @defgroup RNG_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize RNG
  @endverbatim
  * @{
  */
/**
  * @brief Initialize the RNG
  * @return 0 = RNG Initial OK; 1 = Initial error
  */
void Bsp_InitRNG(void)
{
	
	Rng_Handler.Instance = RNG;

	if (HAL_RNG_DeInit(&Rng_Handler) != HAL_OK)		/* DeInitialize the RNG peripheral */
	{
		Error_Handler();	/* DeInitialization Error */
	}    

	if (HAL_RNG_Init(&Rng_Handler) != HAL_OK)		/* Initialize the RNG peripheral */
	{
		Error_Handler();	/* Initialization Error */
	}
#if RNG_IT_ENABLE == 1	
	if (HAL_RNG_GenerateRandomNumber_IT(&Rng_Handler)!= HAL_OK)	
	{
		Error_Handler();	/* Initialization Error */	
	}
	
#else	
	uint16_t Retry=0;
    while((__HAL_RNG_GET_FLAG(&Rng_Handler,RNG_FLAG_DRDY)==RESET) && (Retry<10000))//等待RNG准备就绪
    {
        Retry++;
        Bsp_Delay_us(10);
    }
    if(Retry>=10000) 
	{
		Error_Handler();	/* Initialization Error */	
	}
#endif

}

/**
  * @brief RNG MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hrng: RNG handle pointer
  */
void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{  

	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	/*Select PLL output as RNG clock source */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
	PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;	/*选择PLL， 时钟为400MHz*/
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);


	/* RNG Peripheral clock enable */
	__HAL_RCC_RNG_CLK_ENABLE();
	
#if RNG_IT_ENABLE == 1	
	HAL_NVIC_SetPriority(RNG_IRQn,3,1);    						//设置随机数生成器中断优先级,抢占优先级3,子优先级1,中断优先级比串口优先级高,但是在中断处理完成后会关闭中断.
																//只有在需要生成随机数时候才会开启中断,所以合理规划程序会避免中断嵌套造成的系统不稳定
	HAL_NVIC_EnableIRQ(RNG_IRQn);          						//使能随机数生成器中断
#endif

}

/**
  * @brief RNG MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrng: RNG handle pointer
  */
void HAL_RNG_MspDeInit(RNG_HandleTypeDef *hrng)
{
	/* Enable RNG reset state */
	__HAL_RCC_RNG_FORCE_RESET();

	/* Release RNG from reset state */
	__HAL_RCC_RNG_RELEASE_RESET();
} 

/** @}
*/
/*********************RNG Exported Functions Group1**************************/

/** @defgroup RNG_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the RNG
  @endverbatim
  * @{
  */

/**
  * @brief 	得到RNG随机数
  */
  
void RNG_Get_RandomNum(void)
{	
	uint32_t random32bit; 
	HAL_RNG_GenerateRandomNumber(&Rng_Handler,&random32bit);
	POLL_Random32bit = random32bit;
#if SYSTEM_DEBUG == 1
	Bsp_Printf("The Random32bit is generated in Polling Mode, RNG= %X! \r\n",POLL_Random32bit);
#endif	
}

/**
  * @brief 	生成[min,max]范围的随机数
  */
  
void RNG_Get_RandomRange(uint32_t min,uint32_t max)
{ 	
    uint32_t random32bit;
	HAL_RNG_GenerateRandomNumber(&Rng_Handler,&random32bit);
	POLL_Random32bit = random32bit % (max-min+1) + min;
#if SYSTEM_DEBUG == 1
	Bsp_Printf("The Random32bit is generated in Polling Mode between [%d,%d], RNG = %X! \r\n",min,max,POLL_Random32bit);
#endif
}

#if RNG_IT_ENABLE == 1	

/**
  * @brief 	RNG随机数生成器中断服务函数
  */
void RNG_IRQHandler(void)
{
	HAL_RNG_IRQHandler(&Rng_Handler);
}

/**
  * @brief 	中断服务函数处理过程，此函数会被HAL_RNG_IRQHandler()调用
  */

void HAL_RNG_ReadyDataCallback(RNG_HandleTypeDef *hrng, uint32_t random32bit)
{ 
	__HAL_RNG_DISABLE_IT(hrng);		/*Disable the RNG IT*/
	IT_Random32bit = random32bit;
#if SYSTEM_DEBUG == 1
	Bsp_Printf("The Random32bit is generated with Interrupt, RNG = %X! \r\n",IT_Random32bit);
#endif	
}
/**
  * @brief 	中断服务函数处理过程，此函数会被HAL_RNG_IRQHandler()调用
  */

void HAL_RNG_ErrorCallback(RNG_HandleTypeDef *hrng)
{
	__HAL_RNG_DISABLE_IT(hrng);			/*Disable the RNG IT*/
#if SYSTEM_DEBUG == 1
		Bsp_Printf("RNG Error Occured! \r\n");
#endif		

}

#endif /*RNG_IT_ENABLE == 1	*/
/** @}
*/
/****************************RNG Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------RNG Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------RNG-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
