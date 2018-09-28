/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/HARD_TIMER/Hardware_Timer_Bsp.c
  * @author  CME
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
  *			 TIM2的通道2用于PWM生成，输出在PA2 PA3
  *			 TIM3的通道1用于定时触发ADC转换，如果使用DMA传输ADC采样值，必须用TIM3定时触发，否则ADC触发太快DMA反应不过来
  *			 TIM4早USER_DEBUG中用于计算函数执行时间，定时器4为16位最大计数65535，计时周期为10KHz(0.1ms)，所以最长计时时间为13.1s
  *			 TIM5做单次定时器使用,精度为正负2us
  *			 TIM6用于触发PA4 DAC_CHANNEL1的输出，每TIM6CLK/(TIM_Period + 1)/(TIM_Prescaler + 1)= TIM6CLK / 256更新一次
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
/** @defgroup Hardware_Timer Hardware_Timer
  * @brief Hardware_Timer Hardware_Timer
  * @{
  */
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup Hardware_Timer_Private define Hardware_Timer Private define
  * @{
  */
#define TIMER_FREQUENCY				((uint32_t) 500)   	/* Timer frequency (unit: Hz). With a timer 32 bits and time base freq min 1Hz */
#define TIMER_FREQUENCY_RANGE_MIN	((uint32_t)   1)    	/* Timer minimum frequency (unit: Hz), used to calculate frequency range. */
#define TIMER_PRESCALER_MAX_VALUE	(0xFFFF-1)       		/* Timer prescaler maximum value (0xFFFF for timer both 32 bits and 16 bits) */
															/*16-bit programmable prescaler used to divide (also “on the fly”) the counter clock frequency by any factor between 1 and 65535*/
/** @}
*/
/*----------------------------------------Hardware_Timer Private define----------------------------*/  
  
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup Hardware_Timer_Private_Variables Hardware_Timer Private Variables
  * @{
  */

/**
  * @brief		TIM1 Private Handle
  */
TIM_HandleTypeDef 			TIM1_Handler;     

/**
  * @brief		TIM2 Private Handle
  */
TIM_HandleTypeDef 			TIM2_Handler;     

/**
  * @brief		TIM3 Private Handle
  */
TIM_HandleTypeDef 			TIM3_Handler;

/**
  * @brief		TIM4 Private Handle
  */
TIM_HandleTypeDef 			TIM4_Handler;    

/**
  * @brief		TIM5 Private Handle
  */
TIM_HandleTypeDef 			TIM5_Handler;  

/**
  * @brief 		The TIM6 Handler
  */ 
TIM_HandleTypeDef  			TIM6_Handler;

/**
  * @brief 		The HRTIM Handler
  */ 
HRTIM_HandleTypeDef 		HRTIM_Handler;

/**
  * @brief		Timer Output Compare Configuration for TIM1
  */
TIM_OC_InitTypeDef 			sOCConfig;

/**
  * @brief		Timer ADC master Configuration for TIM2
  */
TIM_MasterConfigTypeDef 	Master_Timer_Config;

/**
  * @brief		TIM2_CHANNEX used for PWM
  */
TIM_OC_InitTypeDef 			PWMCConfig; 

/**
  * @brief		Timer Input Capture Configuration for TIM5
  */
TIM_IC_InitTypeDef     		sICConfig;

/**
  * @brief 		Timer TRGO Configuration for TIM6
  */ 
TIM_MasterConfigTypeDef 	sMasterConfig;

/**
  * @brief 		The HRTIM TIME_BASE configuration
  */ 
HRTIM_TimeBaseCfgTypeDef 	HRTime_Base_Config_Master;
	
/**
  * @brief 		The HRTIM TIMER configuration
  */ 
HRTIM_TimerCfgTypeDef 		HRTimer_Config_Master;

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
  * @brief  通用定时器TIM1的初始化，是一个16位定时器，用于计算外部PE11的输入时钟
  * @param 	Period: 0x0001 - 0xFFFF。
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @param 	PWM_Duty: 0 - 100
  * @note 	通过公式: 1 / (TIM1CLK / Prescaler) * Period = 1 / Frequence; 其中1 / (TIM1CLK / Prescaler)为计一个数的时间,Period为从0开始计数到Period,共Period +1个计数个数
  *			得到频率: Frequence = TIM1CLK / Period / Prescaler
  *         TIM1CLK = 200MHz = 200 000 000Hz
  */

void Bsp_InitHardTimer_TIM1(uint16_t Period,uint16_t Prescaler, uint32_t TIM_CHANNEL_X)
{ 
	TIM1_Handler.Instance			=TIM1;            							//定时器2
	TIM1_Handler.Init.Prescaler		=Prescaler;    								//定时器分频
	TIM1_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;						//向上计数模式
	TIM1_Handler.Init.Period		=Period - 1;          						//自动重装载值,从0开始当计数到Period - 1时重新开始从0计数,总共计数为Period
	TIM1_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
	
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
	if(HAL_TIM_IC_ConfigChannel(&TIM1_Handler, &sICConfig, TIM_CHANNEL_X) != HAL_OK)
	{
	/* Configuration Error */
	Error_Handler();
	}

	/*##-3- Start the Input Capture in interrupt mode ##########################*/
	if(HAL_TIM_IC_Start_IT(&TIM1_Handler, TIM_CHANNEL_X) != HAL_OK)
	{
	/* Starting Error */
	Error_Handler();
	}
 } 

 /**
  * @brief  通用定时器TIM2的初始化，是一个32位定时器，用于在PA2_TIM_CHANNEL_3, PA3_TIM_CHANNEL_3上生成特定周期和特定占空比的PWM
  * @param 	Period: 0x00000001 - 0xFFFFFFFF。
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @param 	PWM_Duty: 0 - 100
  * @note 	通过公式: 1 / (TIM2CLK / Prescaler) * Period = 1 / Frequence; 其中1 / (TIM2CLK / Prescaler)为计一个数的时间,Period为从0开始计数到Period,共Period +1个计数个数
  *			得到频率: Frequence = TIM2CLK / Period / Prescaler
  *         TIM2CLK = 200MHz = 200 000 000Hz
  */

void Bsp_InitHardTimer_TIM2(uint32_t Period,uint16_t Prescaler,uint8_t PWM_Duty, uint32_t TIM_CHANNEL_X)
{ 
	TIM2_Handler.Instance			=TIM2;            							//定时器2
	TIM2_Handler.Init.Prescaler		=Prescaler;    								//定时器分频
	TIM2_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;						//向上计数模式
	TIM2_Handler.Init.Period		=Period - 1;          						//自动重装载值,从0开始当计数到Period - 1时重新开始从0计数,总共计数为Period
	TIM2_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
	
	HAL_TIM_PWM_Init(&TIM2_Handler);       										//初始化PWM
	
	PWMCConfig.OCMode=TIM_OCMODE_PWM1; 											//模式选择PWM1
	PWMCConfig.Pulse=(uint32_t)(Period * PWM_Duty * 0.01);  					//设置比较值,此值用来确定占空比,默认比较值为自动重装载值的一半,即占空比为50%
	PWMCConfig.OCPolarity=TIM_OCPOLARITY_LOW; 									//输出比较极性为低 
	
	if(TIM_CHANNEL_X == TIM_CHANNEL_3)
	{		
		HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&PWMCConfig,TIM_CHANNEL_3);		//配置TIM2通道3
		Bsp_SetTIM2Compare1(Period, Prescaler, PWM_Duty, TIM_CHANNEL_X);		//设置新的占空比
		HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_3);							//开启PWM	
	}
	if(TIM_CHANNEL_X == TIM_CHANNEL_4)
	{
		HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&PWMCConfig,TIM_CHANNEL_4);		//配置TIM2通道4
		Bsp_SetTIM2Compare1(Period, Prescaler, PWM_Duty, TIM_CHANNEL_X);		//设置新的占空比
		HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_4);							//开启	
	}   

} 
 
/**
  * @brief  通用定时器TIM3的初始化，是一个16位定时器, 用于PA4 PA6 ADC DualMode Interleaved中周期触发ADC转换
  * @param 	Period: 0x0001 - 0xFFFF。
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @note 	通过公式: 1 / (TIM3CLK / Prescaler) * Period = 1 / Frequence; 其中1 / (TIM3CLK / Prescaler)为计一个数的时间,Period为从0开始计数到Period,共Period +1个计数个数
  *			得到频率: Frequence = TIM3CLK / Period / Prescaler
  *         TIM3CLK = 200MHz = 200 000 000Hz
  */

void Bsp_InitHardTimer_TIM3(uint16_t Period,uint16_t Prescaler)
{	
	TIM3_Handler.Instance 			=TIM3;
	TIM3_Handler.Init.Prescaler		=Prescaler;    								//定时器分频
	TIM3_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;						//向上计数模式
	TIM3_Handler.Init.Period		=Period - 1;          						//自动重装载值,从0开始当计数到Period - 1时重新开始从0计数,总共计数为Period
	TIM3_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
	TIM3_Handler.Init.RepetitionCounter = 0x0;

	if (HAL_TIM_Base_Init(&TIM3_Handler) != HAL_OK)
	{
		Error_Handler();
	}
	Master_Timer_Config.MasterOutputTrigger  = TIM_TRGO_UPDATE;	  		/* Timer TRGO selection */
	Master_Timer_Config.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	Master_Timer_Config.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&TIM3_Handler, &Master_Timer_Config) != HAL_OK)
	{
		Error_Handler();
	}
}
 
/**
  * @brief	通用定时器TIM4的初始化，是一个16位定时器, 用于USER_DEBUG模块中的usmart_reset_runtime() usmart_get_runtime
  * @param 	Period: 0x0001 - 0xFFFF。
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @note 	通过公式: 1 / (TIM4CLK / Prescaler) * Period = 1 / Frequence; 其中1 / (TIM4CLK / Prescaler)为计一个数的时间,Period为从0开始计数到Period,共Period +1个计数个数
  *			得到频率: Frequence = TIM4CLK / Period / Prescaler
  *         TIM3CLK = 200MHz = 200 000 000Hz
  * 		确保计数器计数频率为:10Khz即可.另外,定时器不要开启自动重装载功能!!
  */   
void Bsp_InitHardTimer_TIM4(uint16_t Period,uint16_t Prescaler)
{ 
    __HAL_RCC_TIM4_CLK_ENABLE();
 
    TIM4_Handler.Instance			=TIM4;                          //通用定时器4
    TIM4_Handler.Init.Prescaler		=Prescaler;               		//分频
    TIM4_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;    		//向上计数器
    TIM4_Handler.Init.Period		=Period - 1;                    //自动装载值
    TIM4_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM4_Handler);
	
    HAL_NVIC_SetPriority(TIM4_IRQn,3,3);			//设置TIM4中断优先级，抢占优先级3，子优先级3 注意比串口优先级低，最长计时13.1s
    HAL_NVIC_EnableIRQ(TIM4_IRQn);      			//开启ITM4中断 
	
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4和定时器4中断 	
  }

/**
  * @brief	定时器TIM5初始化，是一个32位定时器，用于定时一定时间
  * @param 	Period: 0x00000001 - 0xFFFFFFFF。
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @param 	PWM_Duty: 0 - 100
  * @note 	通过公式: 1 / (TIM5CLK / Prescaler) * Period = 1 / Frequence; 其中1 / (TIM5CLK / Prescaler)为计一个数的时间,Period为从0开始计数到Period,共Period +1个计数个数
  *			得到频率: Frequence = TIM3CLK / Period / Prescaler
  *         TIM5CLK = 200MHz = 200 000 000Hz
  *			TIM5最大定时时间：0xFFFFFFFF * Frequence
  */  

void Bsp_InitHardTimer_TIM5(uint32_t Period,uint16_t Prescaler)
{  	
    TIM5_Handler.Instance 			= TIM5;                          		//通用定时器TIM5
 	TIM5_Handler.Init.Prescaler		=Prescaler;    				//定时器分频
	TIM5_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;		//向上计数模式
	TIM5_Handler.Init.Period		=Period - 1;          		//自动重装载值,从0开始当计数到Period - 1时重新开始从0计数,总共计数为Period
	TIM5_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
	
	HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); 						//使能定时器5和定时器5更新中断    
}

/**
  * @brief  定时器TIM6初始化，是一个32位定时器，用于在 PA4 DAC上的周期触发
  * @param 	Period: 0x00000001 - 0xFFFFFFFF。
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @param 	PWM_Duty: 0 - 100
  * @note 	通过公式: 1 / (TIM3CLK / Prescaler) * Period = 1 / Frequence; 其中1 / (TIM3CLK / Prescaler)为计一个数的时间,Period为从0开始计数到Period,共Period +1个计数个数
  *			得到PWM输出频率Frequence = TIM3CLK / Period / Prescaler
  *         TIM6CLK = 100MHz = 100 000 000Hz
  */

void Bsp_InitHardTimer_TIM6(uint16_t Period,uint16_t Prescaler)
{
	TIM6_Handler.Instance 			=TIM6;		  				
	TIM6_Handler.Init.Prescaler		=Prescaler;    								//定时器分频
	TIM6_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;						//向上计数模式
	TIM6_Handler.Init.Period		=Period - 1;          						//自动重装载值,从0开始当计数到Period - 1时重新开始从0计数,总共计数为Period
	TIM6_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
	TIM6_Handler.Init.RepetitionCounter = 0;
	
	if (HAL_TIM_Base_Init(&TIM6_Handler) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;	
	
	if (HAL_TIMEx_MasterConfigSynchronization(&TIM6_Handler, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}	
	
	HAL_TIM_Base_Start(&TIM6_Handler);	
}

/**
  * @brief  定时器HRTIM初始化，用在DAC双通道的触发，每HRTIMCLK / 512 = HRTIMCLK / (TIM_Period + 1)/(TIM_Prescaler + 1);	HRTIMCLK = 200MHz
  *			HRTIM Configuration
  */
void Bsp_InitHardTimer_HRTIM(void)
{  
	/* HRTIM Initialization */
	HRTIM_Handler.Instance = (HRTIM_TypeDef *)NULL;
	
	HRTIM_Handler.hdmaMaster = (DMA_HandleTypeDef *)NULL;    
	HRTIM_Handler.hdmaTimerA = (DMA_HandleTypeDef *)NULL;     
	HRTIM_Handler.hdmaTimerB = (DMA_HandleTypeDef *)NULL;  
	HRTIM_Handler.hdmaTimerC = (DMA_HandleTypeDef *)NULL;  
	HRTIM_Handler.hdmaTimerD = (DMA_HandleTypeDef *)NULL;  
	HRTIM_Handler.hdmaTimerE = (DMA_HandleTypeDef *)NULL;
		
	HRTIM_Handler.Instance = HRTIM1;
	HRTIM_Handler.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
	HAL_HRTIM_Init(&HRTIM_Handler);

	/* Time base master configuration */
	HRTime_Base_Config_Master.Mode = HRTIM_MODE_CONTINUOUS;
	HRTime_Base_Config_Master.Period = 0x200;		
	HRTime_Base_Config_Master.PrescalerRatio = 0;
	HRTime_Base_Config_Master.RepetitionCounter = 0;
	
	HAL_HRTIM_TimeBaseConfig(&HRTIM_Handler,HRTIM_TIMERINDEX_MASTER,&HRTime_Base_Config_Master);
													 
	/* Master Time configuration */
	HRTimer_Config_Master.InterruptRequests = HRTIM_MASTER_IT_MUPD;
	HRTimer_Config_Master.DMARequests = HRTIM_TIM_DMA_NONE;
	HRTimer_Config_Master.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
	HRTimer_Config_Master.StartOnSync = HRTIM_SYNCSTART_DISABLED;
	HRTimer_Config_Master.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
	HRTimer_Config_Master.DACSynchro = HRTIM_DACSYNC_DACTRIGOUT_1;
	HRTimer_Config_Master.PreloadEnable = HRTIM_PRELOAD_ENABLED;
	HRTimer_Config_Master.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
	HRTimer_Config_Master.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	HRTimer_Config_Master.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
	HRTimer_Config_Master.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
	HRTimer_Config_Master.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
	HRTimer_Config_Master.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
	HRTimer_Config_Master.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	HRTimer_Config_Master.DelayedProtectionMode = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
	HRTimer_Config_Master.UpdateTrigger= HRTIM_TIMUPDATETRIGGER_NONE;
	HRTimer_Config_Master.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
	HRTimer_Config_Master.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
	
	HAL_HRTIM_WaveformTimerConfig(&HRTIM_Handler,HRTIM_TIMERINDEX_MASTER,&HRTimer_Config_Master);
	/*Enable HRTIM peripheral counter*/
	HAL_HRTIM_WaveformCounterStart(&HRTIM_Handler, HRTIM_TIMERID_MASTER);			

}

/**
  * @brief	通用定时器底册驱动，开启时钟，设置中断优先级
  *			此函数会被HAL_TIM_Base_Init()函数调用
  */  
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)						//用于PWM
	{
		__HAL_RCC_TIM2_CLK_ENABLE();        		//使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,3,4);		//设置TIM2_PWM 中断优先级,抢占优先级3,子优先级4, 注意比串口优先级低
		HAL_NVIC_EnableIRQ(TIM2_IRQn);      		//开启ITM2中断   
	}  

    if(htim->Instance==TIM3)						//用于ADC Dual Mode to trigger The ADC
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
	}

    if(htim->Instance==TIM5)						//通用定时器5
	{
		__HAL_RCC_TIM5_CLK_ENABLE();        		//使能TIMx时钟
		HAL_NVIC_SetPriority(TIM5_IRQn,4,1);		//设置通用定时器中断优先级，抢占优先级4，子优先级1, 注意比串口优先级低
		HAL_NVIC_EnableIRQ(TIM5_IRQn);      		//开启ITMx中断   
	} 

    if(htim->Instance==TIM6)						//用于DAC 触发
	{
		__HAL_RCC_TIM6_CLK_ENABLE();		 		/* TIM6 Periph clock enable */
	}  	
}

/**
  * @brief 	Low level BSP
  * @param 	htim: The timer handle
  */
//定时器底层驱动，时钟使能，引脚配置,此函数会被HAL_TIM_PWM_Init()调用
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM2_CLK_ENABLE();						//使能定时器2
    __HAL_RCC_GPIOA_CLK_ENABLE();						//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_2 | GPIO_PIN_3;           //PA2 | PA3
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  				//复用推完输出
    GPIO_Initure.Pull=GPIO_PULLUP;          			//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     	//高速
	GPIO_Initure.Alternate=GPIO_AF1_TIM2;				//PA2, PA3复用为TIM2
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

/**
  * @brief TIM MSP Initialization 用于TIM1的外部输入时钟频率计算，注意核心板将PE11接到了FMC上，不能同时使用
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1)				//用于外部输入时钟频率计算
	{
		GPIO_InitTypeDef   GPIO_InitStruct;
		__HAL_RCC_TIM1_CLK_ENABLE();
	 	__HAL_RCC_GPIOE_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	  
		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 4, 2);	//设置定时器TIM1中断优先级，抢占优先级4，子优先级2, 注意比串口优先级低
		HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
	} 
}

/**
  * @brief HRTIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hhrtim: HRTIM handle pointer
  */
void HAL_HRTIM_MspInit(HRTIM_HandleTypeDef *hhrtim)
{
  /* TIM6 Periph clock enable */
  __HAL_RCC_HRTIM1_CLK_ENABLE();
}

/**
  * @brief TIM MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param htim: TIM handle pointer
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)				//通用定时器2
	{
		__HAL_RCC_TIM2_FORCE_RESET();
		__HAL_RCC_TIM2_RELEASE_RESET();	
		HAL_NVIC_DisableIRQ(TIM2_IRQn);      
	}
    if(htim->Instance==TIM3)				//通用定时器3
	{
		__HAL_RCC_TIM3_FORCE_RESET();
		__HAL_RCC_TIM3_RELEASE_RESET();	
	}
    if(htim->Instance==TIM5)				//通用定时器5
	{
		__HAL_RCC_TIM5_FORCE_RESET();
		__HAL_RCC_TIM5_RELEASE_RESET();	
		HAL_NVIC_DisableIRQ(TIM5_IRQn);      
	}
  
	if(htim->Instance==TIM6)				//通用定时器6
	{
		__HAL_RCC_TIM6_FORCE_RESET();
		__HAL_RCC_TIM6_RELEASE_RESET();	
#if SINWAVE_GEN_FOR_TEST_ENABLE == 1
		HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
#endif	/*SINWAVE_GEN_FOR_TEST_ENABLE*/
	}
}

/**
  * @brief HRTIM MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hhrtim: HRTIM handle pointer
  */
void HAL_HRTIM_MspDeInit(HRTIM_HandleTypeDef *hhrtim)
{

  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_HRTIM1_CLK_DISABLE();

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
  * @brief 	定时器6中断服务函数, 用于DAC生成测试用的波形
  */

void TIM6_DAC_IRQHandler(void)
{
  HAL_DAC_IRQHandler(&DAC_Handler);
}

/**
  * @brief	定时器中断服务函数调用，不同的服务函数调用在stm32h7xxx_hal_tim.c中
  */ 

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM5_Handler))
    {
#if HARDWARE_TIM5_DEBUG == 1
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
#if HARDWARE_TIM5_DEBUG == 1
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
#if HARDWARE_TIM1_DEBUG == 1
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
#if HARDWARE_TIM5_DEBUG == 1
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
#if HARDWARE_TIM1_DEBUG == 1
	Bsp_Printf("Initial TIM1_CHANNEL2 to Compute the input Frequence in PE11 ! \r\n");
#endif
#if HARDWARE_TIM1_ENABLE == 1	
	/* 初始化TIM1定时器 */		
	Bsp_InitHardTimer_TIM1(0xFFFF, 0, TIM_CHANNEL_2);
#endif 	/*HARDWARE_TIM1_ENABLE*/
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
