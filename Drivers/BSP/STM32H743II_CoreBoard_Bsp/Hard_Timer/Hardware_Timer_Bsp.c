/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/HARD_TIMER/Hardware_Timer_Bsp.c
  * @author  MCD Application Team
  * @version SW:V1.0.0 HW:V1.0
  * @date    21-August-2018
  * @brief   This file provides set of firmware functions to manage:
  *			 定义用于硬件定时器的TIM.
  *			 TIM3 和TIM4 是16位
  *			 TIM2 和TIM5 是32位
  *			 TIM2 - TIM5 时钟来自APB1 = HCLK / 2 = 200MHz
  *			 APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
  *			 APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
  *
  *			 TIM1的通道2用于测量外部输入时钟频率，输入在PE11，注意不能与FMC同时复用
  *			 TIM3的通道1用于2KHz PWM生成，输出在PB4引脚，定时器3为16位最大计数65535，PWM最小频率为1/65535 KHz
  *			 TIM4早USER_DEBUG中用于计算函数执行时间，定时器4为16位最大计数65535，计时周期为10KHz(0.1ms)，所以最长计时时间为13.1s
  *			 TIM5做单次定时器使用,精度为正负2us
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
#include "Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup Hardware_Timer Hardware_Timer
  * @brief Hardware_Timer Hardware_Timer
  * @{
  */
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup Hardware_Timer_Private_Variables Hardware_Timer Private Variables
  * @{
  */

/**
  * @brief		TIM1 Private Handle
  */
TIM_HandleTypeDef 		TIM1_Handler;      //定时器1句柄 

/**
  * @brief		TIM4 Private Handle
  */
TIM_HandleTypeDef 		TIM4_Handler;      //定时器4句柄 

/**
  * @brief		Generic TIMx Private Handle
  */
TIM_HandleTypeDef 		TIM5_Handler;      //定时器5句柄 
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
/*----------------------------Hardware_Timer Private Functions Prototypes--------------------------------*/ 
/** @defgroup Hardware_Timer_Exported_Functions Hardware_Timer Exported Functions
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
  * @brief	高级定时器TIM1的初始化，是一个16位自动重装定时器,用于计算外部输入时钟的频率
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

 #if USMART_ENTIMX_SCAN == 1
 
/**
  * @brief	通用定时器4中断初始化,确保计数器计数频率为:10Khz即可.另外,定时器不要开启自动重装载功能!!
  *			用于USER_DEBUG模块中的usmart_reset_runtime() usmart_get_runtime 
  */   

void Bsp_InitHardTimer_TIM4(uint16_t Period,uint16_t Prescaler)
{ 
    __HAL_RCC_TIM4_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM4_IRQn,3,3);	//设置TIM4中断优先级，抢占优先级3，子优先级3 注意比串口优先级低，最长计时13.1s
    HAL_NVIC_EnableIRQ(TIM4_IRQn);      	//开启ITM4中断    
    
    TIM4_Handler.Instance=TIM4;                          //通用定时器4
    TIM4_Handler.Init.Prescaler=Prescaler;                     //分频
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM4_Handler.Init.Period=Period;                        //自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM4_Handler);
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4和定时器4中断 					 
}
 
#endif

/**
  * @brief	通用定时器5中断初始化,定时器5在APB1上，APB1的定时器时钟为200MHz
  *			定时器溢出时间计算方法:Tout=((usPeriod)*(usPrescaler))/uiTIMxCLK us.	uiTIMxCLK = 定时器工作频率,单位:Mhz
  *			这里使用的是定时器5!(定时器2挂在APB1上，时钟为HCLK/2)
  *			TIM5 最大0xFFFFFFFF / 1000000 = 4294s
  */  
 void Bsp_InitHardTimer_TIM5(void)
{  	
    TIM5_Handler.Instance = TIM5;                          		//通用定时器TIM5
    TIM5_Handler.Init.Prescaler = 200 - 1;                    	//uiTIMxCLK / 1000000; /* 分频到周期 1us */ /*减去1 为了更加准确*/
    TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;    		//向上计数器
    TIM5_Handler.Init.Period = 0x000F4240 - 1;                  //自动装载值	/*减去1 为了更加准确*/  1s = 1 * 1000000 us
    TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;	//时钟分频因子
    
	HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); 						//使能定时器5和定时器5更新中断    
}

/**
  * @brief	通用定时器底册驱动，开启时钟，设置中断优先级
  *			此函数会被HAL_TIM_Base_Init()函数调用
  */  
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM5)				//通用定时器
	{
		__HAL_RCC_TIM5_CLK_ENABLE();        //使能TIMx时钟
		HAL_NVIC_SetPriority(TIM5_IRQn,4,1);//设置通用定时器中断优先级，抢占优先级4，子优先级1, 注意比串口优先级低
		HAL_NVIC_EnableIRQ(TIM5_IRQn);      //开启ITMx中断   
	} 
	
    if(htim->Instance==TIM3)					//用于PWM
	{
		__HAL_RCC_TIM3_CLK_ENABLE();        //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,3,4);//设置TIM3_PWM 中断优先级,抢占优先级3,子优先级4, 注意比串口优先级低
		HAL_NVIC_EnableIRQ(TIM3_IRQn);      //开启ITM3中断   
	}  	
}
/**
  * @brief TIM MSP Initialization 用于TIM1的外部输入时钟频率计算，注意核心板将PE11接到了FMC上，不能同时使用
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
 
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIM1 Peripheral clock enable */
  __HAL_RCC_TIM1_CLK_ENABLE();
  
  /* Enable GPIO channels Clock */
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /* Configure  (TIM1_Channel) in Alternate function, push-pull and high speed */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  
  /*##-2- Configure the NVIC for TIM1 #########################################*/

  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 4, 2);	//设置定时器TIM1中断优先级，抢占优先级4，子优先级2, 注意比串口优先级低

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
}

/**
  * @brief	定时器1中断服务函数 用于TIM1的外部输入时钟频率计算，注意核心板将PE11接到了FMC上，不能同时使用
  */ 
void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM1_Handler);
}

#if USMART_ENTIMX_SCAN == 1

/**
  * @brief	定时器4中断服务函数 用于USER_DEBUGE中
  */ 
void TIM4_IRQHandler(void)
{ 		    		  			       
    if(__HAL_TIM_GET_IT_SOURCE(&TIM4_Handler,TIM_IT_UPDATE)==SET)//溢出中断
    {
        usmart_dev.scan();	//执行usmart扫描
        __HAL_TIM_SET_COUNTER(&TIM4_Handler,0);;    //清空定时器的CNT
        __HAL_TIM_SET_AUTORELOAD(&TIM4_Handler,100);//恢复原来的设置
    }
    __HAL_TIM_CLEAR_IT(&TIM4_Handler, TIM_IT_UPDATE);//清除中断标志位
}

#endif

/**
  * @brief 	定时器5中断服务函数
  */

void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);
}
/**
  * @brief	定时器中断服务函数调用，不同的服务函数调用在stm32h7xxx_hal_tim.c中
  */ 

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM5_Handler))
    {
#if SYSTEM_DEBUG == 1
		Bsp_Printf("TIM5 Timeout Occured! \r\n");
#endif
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
    if(htim==(&TIM5_Handler))
    {
#if SYSTEM_DEBUG == 1
		Bsp_Printf("TIM5 Period Elapsed every 1s! \r\n");
#endif
    }
}
/**
  * @brief  定时器中断服务函数调用，不同的服务函数调用在stm32h7xxx_hal_tim.c中
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
#if SYSTEM_DEBUG == 1
		Bsp_Printf("TIM1 External Input Frequence \r\n");
#endif
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
  * @brief		Start TIMER 5 For time out
  *	@param		_CC : Channel 1，2，3, 4
  *	@param		_uiTimeOut : The timeout period
  *	@param		_pCallBack : callback functions
  */ 
  
// 使用TIM5做单次定时器使用, 定时时间到后执行回调函数。可以同时启动4个通道，互不干扰.
// 定时精度正负2us （主要耗费在调用本函数的执行时间，函数内部进行了补偿减小误差）
// TIM5 是32位定时器.定时范围很大
// _CC : 捕获通道几，1，2，3, 4
// _uiTimeOut : 超时时间,32位定时器,最大 4294秒
// _pCallBack : 定时时间到后，被执行的函数 
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

    cnt_now = __HAL_TIM_GetCounter(&TIM5_Handler);    	/* 读取当前的计数器值 */
    cnt_tar = cnt_now + _uiTimeOut;						/* 计算捕获的计数器值 */
    if (_CC == 1)
    {
        s_TIM_CallBack_CC1 = (void (*)(void))_pCallBack;
		
		if(HAL_TIM_OC_ConfigChannel(&TIM5_Handler, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIM5_Handler, TIM_CHANNEL_1, cnt_tar);      	/* 设置捕获比较计数器CC1 */
        __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_CC1);						/* 清CC1中断标志位.*/
		if(HAL_TIM_OC_Start_IT(&TIM5_Handler, TIM_CHANNEL_3) != HAL_OK)		/* 使能CC1中断 */
		{
			Error_Handler();
		}
    }
    else if (_CC == 2)
    {
		s_TIM_CallBack_CC2 = (void (*)(void))_pCallBack;

		if(HAL_TIM_OC_ConfigChannel(&TIM5_Handler, &sOCConfig, TIM_CHANNEL_2) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIM5_Handler, TIM_CHANNEL_2, cnt_tar);      	/* 设置捕获比较计数器CC2 */
        __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_CC2);						/* 清CC2中断标志位.*/
		if(HAL_TIM_OC_Start_IT(&TIM5_Handler, TIM_CHANNEL_2) != HAL_OK)		/* 使能CC2中断 */
		{
			Error_Handler();
		}
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack_CC3 = (void (*)(void))_pCallBack;

 		if(HAL_TIM_OC_ConfigChannel(&TIM5_Handler, &sOCConfig, TIM_CHANNEL_3) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIM5_Handler, TIM_CHANNEL_3, cnt_tar);      	/* 设置捕获比较计数器CC3 */
        __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_CC3);						/* 清CC3中断标志位.*/
		if(HAL_TIM_OC_Start_IT(&TIM5_Handler, TIM_CHANNEL_3) != HAL_OK)		/* 使能CC3中断 */
		{
			Error_Handler();
		}
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack_CC4 = (void (*)(void))_pCallBack;

		if(HAL_TIM_OC_ConfigChannel(&TIM5_Handler, &sOCConfig, TIM_CHANNEL_4) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIM5_Handler, TIM_CHANNEL_4, cnt_tar);      	/* 设置捕获比较计数器CC4 */
        __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_CC4);						/* 清CC4中断标志位.*/
		if(HAL_TIM_OC_Start_IT(&TIM5_Handler, TIM_CHANNEL_4) != HAL_OK)		/* 使能CC4中断 */
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
#if SYSTEM_DEBUG == 1
	Bsp_Printf("TIM5 Delay TimeOut! User can add functions here when timeout! \r\n");
#endif
}

/**
  * @brief 	配置TIM1的通道2测试LSI频率
  * @return Input Frequence
  */

uint32_t TIM1_ComputeInputFreq(void)
{
	extern uint32_t	uwFrequency;
#if SYSTEM_DEBUG == 1
	Bsp_Printf("Initial TIM1_CHANNEL2 to Compute the input Frequence in PE11 ! \r\n");
#endif	
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
