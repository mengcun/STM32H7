/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/Soft_Timer/Hardware_Timer_Bsp.c
  * @author  MCD Application Team
  * @version SW:V1.0.0 HW:V1.0
  * @date    21-August-2018
  * @brief   This file provides set of firmware functions to manage:
  *			 定义用于硬件定时器的TIM,可以使用通用定时器TIM2 - TIM5,每个定时器有4个通道.
  *			 TIM3 和TIM4 是16位
  *			 TIM2 和TIM5 是32位
  *			 TIM2 - TIM5 时钟来自APB1 = HCLK / 2 = 200MHz
  *			 APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
  *			 APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
  *
  * 		 TIM1: The TIM1CLK frequency is set to 2*PCK2 = HCLK = SystemCoreClock/2 (Hz), the Prescaler is 0,
  *			 so the TIM1 counter clock is SystemCoreClock/2 (Hz).
  *			 SystemCoreClock is set to 400 MHz.
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The Hardware_Timer canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/*******************************************APP/BSP*************************************************/
#include "stm32h743ii_Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup Hardware_Timer Hardware_Timer
  * @brief 	使用TIM2-5做单次定时器使用, 定时时间到后执行回调函数。可以同时启动4个定时器，互不干扰。
  *        	定时精度正负10us （主要耗费在调用本函数的执行时间，函数内部进行了补偿减小误差）
  * @{
  */
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup Hardware_Timer_Private_Variables Hardware_Timer Private Variables
  * @{
  */
/**
  * @brief		Generic TIMx Private Handle
  */
TIM_HandleTypeDef 		TIMx_Handler;      //通用定时器句柄 
/**
  * @brief		TIM1 Private Handle
  */
TIM_HandleTypeDef 		TIM1_Handler;      //定时器1句柄 

/**
  * @brief		Timer Output Compare Configuration Structure declaration
  */
TIM_OC_InitTypeDef 		sOCConfig;

/**
  * @brief		Timer Input Capture Configuration Structure declaration
  */
TIM_IC_InitTypeDef     	sICConfig;

/**
* @brief		uwFrequency: External signal Frequency Value computation by TIM1, uwFrequency = TIM1 counter clock / uwDiffCapture (Hz)
*				uwDiffCapture: The difference between two consecutive TIM1 captures.
  */
uint32_t	uwFrequency = 0;
uint32_t	uwDiffCapture = 0;

/* Captured Values */
uint32_t	uwIC2Value1 = 0;
uint32_t	uwIC2Value2 = 0;
/* Capture index */
uint16_t	uhCaptureIndex = 0;

/** @}
*/		
/*----------------------------Hardware_Timer Private Variables--------------------------------*/ 
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup Software_Timer_Private_Functions_Prototypes Software_Timer Private Functions Prototypes
  * @{
  */
/**
  * @brief 	保存 TIM定时中断到后执行的回调函数指针
  */ 
static void (*s_TIM_CallBack_CC1)(void);
static void (*s_TIM_CallBack_CC2)(void);
static void (*s_TIM_CallBack_CC3)(void);
static void (*s_TIM_CallBack_CC4)(void);
/** @}
*/		
/*----------------------------Hardware_Timer Private Functions Prototypes--------------------------------*/ 
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup Hardware_Timer_Private_Functions_Prototypes Hardware_Timer Private Functions Prototypes
  * @{
  */
/** @}
*/		
/*----------------------------Hardware_Timer Private Functions Prototypes--------------------------------*/ /** @defgroup Hardware_Timer_Exported_Functions Hardware_Timer Exported Functions
  * @{
  */
/** @defgroup Hardware_Timer_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize XXXX
  @endverbatim
  * @{
  */
/**
  * @brief	通用定时器5中断初始化,定时器5在APB1上，APB1的定时器时钟为200MHz
  *			定时器溢出时间计算方法:Tout=((usPeriod)*(usPrescaler))/uiTIMxCLK us.	uiTIMxCLK = 定时器工作频率,单位:Mhz
  *			这里使用的是定时器5!(定时器2挂在APB1上，时钟为HCLK/2)
  */  
 void Bsp_InitGeneralHardTimer(void)
{  	
    TIMx_Handler.Instance = TIMx;                          		//通用定时器TIMx
    TIMx_Handler.Init.Prescaler = 200 - 1;                    	//uiTIMxCLK / 1000000; /* 分频到周期 1us */ /*减去1 为了更加准确*/
    TIMx_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;    		//向上计数器
    TIMx_Handler.Init.Period = usPeriod - 1;                    //自动装载值	/*减去1 为了更加准确*/
    TIMx_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;	//时钟分频因子
    
	HAL_TIM_Base_Init(&TIMx_Handler);
    
    HAL_TIM_Base_Start_IT(&TIMx_Handler); //使能定时器5和定时器5更新中断：TIM_IT_UPDATE    
}

/**
  * @brief	高级定时器TIM1的初始化，是一个16位自动重装定时器
  *			TIM1 counter clock is SystemCoreClock/2 (Hz) =  200MHz
  *			TIM1 configuration: Input Capture mode
  *			The external signal is connected to TIM1 CH2 pin (PE.11), 注意核心板将PE11接到了FMC上，不能同时使用。  
  *			The Rising edge is used as active edge,
  *			The TIM1 CCR2 is used to compute the frequency value 
  *			The minimum frequency value to measure is TIM1 counter clock / CCR MAX = 200 MHz / 65535
  *			Due to TIM1_CC_IRQHandler processing time (around 5ns), the minimum frequency value to measure is around 4kHz.
  */  
 void Bsp_InitHardTimer_TIM1(void)
 {
    /*##-1- Configure the TIM peripheral #######################################*/
	/* Initialize TIMx peripheral as follows:
	   + Period = 0xFFFF
	   + Prescaler = 0
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	TIM1_Handler.Instance = TIM1;
	TIM1_Handler.Init.Period            = 0xFFFF;
	TIM1_Handler.Init.Prescaler         = 0;
	TIM1_Handler.Init.ClockDivision     = 0;
	TIM1_Handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TIM1_Handler.Init.RepetitionCounter = 0;
	if(HAL_TIM_IC_Init(&TIM1_Handler) != HAL_OK)
	{
	/* Initialization Error */
	Error_Handler();
	}
	/*##-2- Configure the Input Capture channel ################################*/ 
	/* Configure the Input Capture of channel 2 */
	sICConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
	sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
	sICConfig.ICFilter    = 0;   
	if(HAL_TIM_IC_ConfigChannel(&TIM1_Handler, &sICConfig, TIM_CHANNEL_2) != HAL_OK)
	{
	/* Configuration Error */
	Error_Handler();
	}

	/*##-3- Start the Input Capture in interrupt mode ##########################*/
	if(HAL_TIM_IC_Start_IT(&TIM1_Handler, TIM_CHANNEL_2) != HAL_OK)
	{
	/* Starting Error */
	Error_Handler();
	}
 }

/**
  * @brief	通用定时器底册驱动，开启时钟，设置中断优先级
  *			此函数会被HAL_TIM_Base_Init()函数调用
  */  
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIMx)					//通用定时器
	{
		__HAL_RCC_TIMx_CLK_ENABLE();            //使能TIMx时钟
		HAL_NVIC_SetPriority(TIMx_IRQn,4,1);    //设置通用定时器中断优先级，抢占优先级4，子优先级1, 注意比串口优先级低
		HAL_NVIC_EnableIRQ(TIMx_IRQn);          //开启ITMx中断   
	} 
	
    if(htim->Instance==TIM3)					//用于PWM
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,3,4);    //设置TIM3_PWM 中断优先级,抢占优先级3,子优先级4, 注意比串口优先级低
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}  	
}
/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
 
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM1_CLK_ENABLE();
  
  /* Enable GPIO channels Clock */
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /* Configure  (TIMx_Channel) in Alternate function, push-pull and high speed */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  
  /*##-2- Configure the NVIC for TIMx #########################################*/

  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 4, 2);	//设置定时器TIM1中断优先级，抢占优先级4，子优先级2, 注意比串口优先级低

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
}

/**
  * @brief	定时器中断服务函数
  * 		其内部会对相应的中断标志位进行详细判断,确定来源后,会自动清掉该中断标志位同时调用不类型的回函数.
  *			所以我们控制逻辑只用编写在中断回调函数,并且不需要清标志位.
  */ 
void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM1_Handler);
}

/**
  * @brief 	定时器3中断服务函数
  */

void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

/**
  * @brief 	定时器5中断服务函数
  */

void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIMx_Handler);
}
/**
  * @brief	定时器中断服务函数调用，不同的服务函数调用在stm32h7xxx_hal_tim.c中
  */ 

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIMx_Handler))
    {
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			s_TIM_CallBack_CC1();			
		}
		else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			s_TIM_CallBack_CC2();
		}
		else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			s_TIM_CallBack_CC3();
		}
		else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			s_TIM_CallBack_CC4();
		}	
	}		
}
/**
  * @brief	定时器中断服务函数调用，不同的服务函数调用在stm32h7xxx_hal_tim.c中
  */ 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIMx_Handler))
    {
		BSP_Printf("TIMx Period Elapsed! \r\n");
    }
	if(htim==(&TIM3_Handler))			   //用于PWM
    {
        Bsp_LED_Toggled(LED2_Blue);        //LED0反转
    }
}
/**
  * @brief  使用TIM1计算外部输入时钟频率
  * @param  htim : hadc handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if(uhCaptureIndex == 0)
		{
			/* Get the 1st Input Capture value */
			uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			uhCaptureIndex = 1;
		}
		else if(uhCaptureIndex == 1)
		{
			/* Get the 2nd Input Capture value */
			uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); 

			/* Capture computation */
			if (uwIC2Value2 > uwIC2Value1)
			{
				uwDiffCapture = (uwIC2Value2 - uwIC2Value1); 
			}
			else if (uwIC2Value2 < uwIC2Value1)
			{
				/* 0xFFFF is max TIM1_CCRx value */
				uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2) + 1;
			}
			else
			{
				/* If capture values are equal, we have reached the limit of frequency
				   measures */
				Error_Handler();
			}
			/* Frequency computation: for this example TIMx (TIM1) is clocked by
			 2*APB2Clk as APB2CLKDivider are set to RCC_APB2_DIV2 */      
			uwFrequency = 2*HAL_RCC_GetPCLK2Freq() / uwDiffCapture;
			uhCaptureIndex = 0;
		}
	}
}

/** @}
*/
/*********************Hardware_Timer Exported Functions Group1**************************/

/** @defgroup Hardware_Timer_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the XXXX
  @endverbatim
  * @{
  */
/**
  * @brief		使用TIM2-5做单次定时器使用, 定时时间到后执行回调函数。可以同时启动4个定时器，互不干扰.
  *             定时精度正负2us （主要耗费在调用本函数的执行时间，函数内部进行了补偿减小误差）
  *			 	TIM2和TIM5 是32位定时器.定时范围很大
  *			 	TIM3和TIM4 是16位定时器.
  *	@param		_CC : 捕获通道几，1，2，3, 4
  *             _uiTimeOut : 超时时间, 单位 1us.对于16位定时器,最大 65.5ms;对于32位定时器,最大 4294秒
  *             _pCallBack : 定时时间到后，被执行的函数
  */ 
void Bsp_StarGenerictHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
//用于测量执行此函数所用时为1.16us(通过逻辑分析仪测量IO翻转)	
//	GPIOC->BSRRH=1<<1;		// 置GPIOC_PIN1成低电平
//	Bsp_Delay_ms(500);
//	GPIOC->BSRRL=1<<1;		// 置GPIOC_PIN1成高电平
	
    uint32_t cnt_now;
    uint32_t cnt_tar;
	
	sOCConfig.OCMode     = TIM_OCMODE_TIMING;
	sOCConfig.OCPolarity = TIM_OCPOLARITY_LOW;
	
	/* 用于补偿调用该函数的耗时*/
    if (_uiTimeOut < 1)
    {
        ;
    }
    else
    {
        _uiTimeOut -= 1;
    }

    cnt_now = __HAL_TIM_GetCounter(&TIMx_Handler);    	/* 读取当前的计数器值 */
    cnt_tar = cnt_now + _uiTimeOut;						/* 计算捕获的计数器值 */
    if (_CC == 1)
    {
        s_TIM_CallBack_CC1 = (void (*)(void))_pCallBack;
		
		if(HAL_TIM_OC_ConfigChannel(&TIMx_Handler, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIMx_Handler, TIM_CHANNEL_1, cnt_tar);      	/* 设置捕获比较计数器CC1 */
        __HAL_TIM_CLEAR_IT(&TIMx_Handler, TIM_IT_CC1);						/* 清CC1中断标志位.*/
		if(HAL_TIM_OC_Start_IT(&TIMx_Handler, TIM_CHANNEL_3) != HAL_OK)		/* 使能CC1中断 */
		{
			Error_Handler();
		}
    }
    else if (_CC == 2)
    {
		s_TIM_CallBack_CC2 = (void (*)(void))_pCallBack;

		if(HAL_TIM_OC_ConfigChannel(&TIMx_Handler, &sOCConfig, TIM_CHANNEL_2) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIMx_Handler, TIM_CHANNEL_2, cnt_tar);      	/* 设置捕获比较计数器CC2 */
        __HAL_TIM_CLEAR_IT(&TIMx_Handler, TIM_IT_CC2);						/* 清CC2中断标志位.*/
		if(HAL_TIM_OC_Start_IT(&TIMx_Handler, TIM_CHANNEL_2) != HAL_OK)		/* 使能CC2中断 */
		{
			Error_Handler();
		}
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack_CC3 = (void (*)(void))_pCallBack;

 		if(HAL_TIM_OC_ConfigChannel(&TIMx_Handler, &sOCConfig, TIM_CHANNEL_3) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIMx_Handler, TIM_CHANNEL_3, cnt_tar);      	/* 设置捕获比较计数器CC3 */
        __HAL_TIM_CLEAR_IT(&TIMx_Handler, TIM_IT_CC3);						/* 清CC3中断标志位.*/
		if(HAL_TIM_OC_Start_IT(&TIMx_Handler, TIM_CHANNEL_3) != HAL_OK)		/* 使能CC3中断 */
		{
			Error_Handler();
		}
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack_CC4 = (void (*)(void))_pCallBack;

		if(HAL_TIM_OC_ConfigChannel(&TIMx_Handler, &sOCConfig, TIM_CHANNEL_4) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIMx_Handler, TIM_CHANNEL_4, cnt_tar);      	/* 设置捕获比较计数器CC4 */
        __HAL_TIM_CLEAR_IT(&TIMx_Handler, TIM_IT_CC4);						/* 清CC4中断标志位.*/
		if(HAL_TIM_OC_Start_IT(&TIMx_Handler, TIM_CHANNEL_4) != HAL_OK)		/* 使能CC4中断 */
		{
			Error_Handler();
		}
    }
	else
    {
        return;
    }
		
//	GPIOC->BSRRH=1<<1;		// 置置GPIOC_PIN1成低电平
//	Bsp_Delay_ms(500);
//	GPIOC->BSRRL=1<<1;		// 置GPIOC_PIN1成高电平

}

/**
  * @brief	启用硬件定时器到达指定时间后的TimeOut回调函数,需要使用时可以复制出去修改程序名
  */ 
void _pFunctionHardTimeout(void)
{
	Bsp_LED_Toggled(LED2_Red);
}

/**
  * @brief 	配置TIM1的通道4测试LSI频率
  * @return LSI Frequency	 实际测试LSI = 34000左右
  */
uint32_t GetFrequency(void)
{
	extern uint32_t	uwFrequency;
	
	/* 初始化TIM1定时器 */		
	Bsp_InitHardTimer_TIM1();
	
	return uwFrequency;
}

/** @}
*/
/****************************XXXX Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------Hardware_Timer Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------Hardware_Timer-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
