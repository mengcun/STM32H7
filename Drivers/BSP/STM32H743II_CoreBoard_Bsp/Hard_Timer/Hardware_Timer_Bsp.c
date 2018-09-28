/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/HARD_TIMER/Hardware_Timer_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    21-August-2018
  * @brief   This file provides set of firmware functions to manage:
  *			 ��������Ӳ����ʱ����TIM.
  *			 TIM3 ��TIM4 ��16λ
  *			 TIM2 ��TIM5 ��32λ
  *			 TIM2 - TIM5 ʱ������APB1 = HCLK / 2 = 200MHz
  *			 APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
  *			 APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11
  *
  *			 TIM1��ͨ��2���ڲ����ⲿ����ʱ��Ƶ�ʣ�������PE11��ע�ⲻ����FMCͬʱ����
  *			 TIM2��ͨ��2����PWM���ɣ������PA2 PA3
  *			 TIM3��ͨ��1���ڶ�ʱ����ADCת�������ʹ��DMA����ADC����ֵ��������TIM3��ʱ����������ADC����̫��DMA��Ӧ������
  *			 TIM4��USER_DEBUG�����ڼ��㺯��ִ��ʱ�䣬��ʱ��4Ϊ16λ������65535����ʱ����Ϊ10KHz(0.1ms)���������ʱʱ��Ϊ13.1s
  *			 TIM5�����ζ�ʱ��ʹ��,����Ϊ����2us
  *			 TIM6���ڴ���PA4 DAC_CHANNEL1�������ÿTIM6CLK/(TIM_Period + 1)/(TIM_Prescaler + 1)= TIM6CLK / 256����һ��
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
															/*16-bit programmable prescaler used to divide (also ��on the fly��) the counter clock frequency by any factor between 1 and 65535*/
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
  * @brief 	���� TIM��ʱ�жϵ���ִ�еĻص�����ָ��
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
  * @brief  ͨ�ö�ʱ��TIM1�ĳ�ʼ������һ��16λ��ʱ�������ڼ����ⲿPE11������ʱ��
  * @param 	Period: 0x0001 - 0xFFFF��
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @param 	PWM_Duty: 0 - 100
  * @note 	ͨ����ʽ: 1 / (TIM1CLK / Prescaler) * Period = 1 / Frequence; ����1 / (TIM1CLK / Prescaler)Ϊ��һ������ʱ��,PeriodΪ��0��ʼ������Period,��Period +1����������
  *			�õ�Ƶ��: Frequence = TIM1CLK / Period / Prescaler
  *         TIM1CLK = 200MHz = 200 000 000Hz
  */

void Bsp_InitHardTimer_TIM1(uint16_t Period,uint16_t Prescaler, uint32_t TIM_CHANNEL_X)
{ 
	TIM1_Handler.Instance			=TIM1;            							//��ʱ��2
	TIM1_Handler.Init.Prescaler		=Prescaler;    								//��ʱ����Ƶ
	TIM1_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;						//���ϼ���ģʽ
	TIM1_Handler.Init.Period		=Period - 1;          						//�Զ���װ��ֵ,��0��ʼ��������Period - 1ʱ���¿�ʼ��0����,�ܹ�����ΪPeriod
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
  * @brief  ͨ�ö�ʱ��TIM2�ĳ�ʼ������һ��32λ��ʱ����������PA2_TIM_CHANNEL_3, PA3_TIM_CHANNEL_3�������ض����ں��ض�ռ�ձȵ�PWM
  * @param 	Period: 0x00000001 - 0xFFFFFFFF��
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @param 	PWM_Duty: 0 - 100
  * @note 	ͨ����ʽ: 1 / (TIM2CLK / Prescaler) * Period = 1 / Frequence; ����1 / (TIM2CLK / Prescaler)Ϊ��һ������ʱ��,PeriodΪ��0��ʼ������Period,��Period +1����������
  *			�õ�Ƶ��: Frequence = TIM2CLK / Period / Prescaler
  *         TIM2CLK = 200MHz = 200 000 000Hz
  */

void Bsp_InitHardTimer_TIM2(uint32_t Period,uint16_t Prescaler,uint8_t PWM_Duty, uint32_t TIM_CHANNEL_X)
{ 
	TIM2_Handler.Instance			=TIM2;            							//��ʱ��2
	TIM2_Handler.Init.Prescaler		=Prescaler;    								//��ʱ����Ƶ
	TIM2_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;						//���ϼ���ģʽ
	TIM2_Handler.Init.Period		=Period - 1;          						//�Զ���װ��ֵ,��0��ʼ��������Period - 1ʱ���¿�ʼ��0����,�ܹ�����ΪPeriod
	TIM2_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
	
	HAL_TIM_PWM_Init(&TIM2_Handler);       										//��ʼ��PWM
	
	PWMCConfig.OCMode=TIM_OCMODE_PWM1; 											//ģʽѡ��PWM1
	PWMCConfig.Pulse=(uint32_t)(Period * PWM_Duty * 0.01);  					//���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ�,Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
	PWMCConfig.OCPolarity=TIM_OCPOLARITY_LOW; 									//����Ƚϼ���Ϊ�� 
	
	if(TIM_CHANNEL_X == TIM_CHANNEL_3)
	{		
		HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&PWMCConfig,TIM_CHANNEL_3);		//����TIM2ͨ��3
		Bsp_SetTIM2Compare1(Period, Prescaler, PWM_Duty, TIM_CHANNEL_X);		//�����µ�ռ�ձ�
		HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_3);							//����PWM	
	}
	if(TIM_CHANNEL_X == TIM_CHANNEL_4)
	{
		HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&PWMCConfig,TIM_CHANNEL_4);		//����TIM2ͨ��4
		Bsp_SetTIM2Compare1(Period, Prescaler, PWM_Duty, TIM_CHANNEL_X);		//�����µ�ռ�ձ�
		HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_4);							//����	
	}   

} 
 
/**
  * @brief  ͨ�ö�ʱ��TIM3�ĳ�ʼ������һ��16λ��ʱ��, ����PA4 PA6 ADC DualMode Interleaved�����ڴ���ADCת��
  * @param 	Period: 0x0001 - 0xFFFF��
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @note 	ͨ����ʽ: 1 / (TIM3CLK / Prescaler) * Period = 1 / Frequence; ����1 / (TIM3CLK / Prescaler)Ϊ��һ������ʱ��,PeriodΪ��0��ʼ������Period,��Period +1����������
  *			�õ�Ƶ��: Frequence = TIM3CLK / Period / Prescaler
  *         TIM3CLK = 200MHz = 200 000 000Hz
  */

void Bsp_InitHardTimer_TIM3(uint16_t Period,uint16_t Prescaler)
{	
	TIM3_Handler.Instance 			=TIM3;
	TIM3_Handler.Init.Prescaler		=Prescaler;    								//��ʱ����Ƶ
	TIM3_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;						//���ϼ���ģʽ
	TIM3_Handler.Init.Period		=Period - 1;          						//�Զ���װ��ֵ,��0��ʼ��������Period - 1ʱ���¿�ʼ��0����,�ܹ�����ΪPeriod
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
  * @brief	ͨ�ö�ʱ��TIM4�ĳ�ʼ������һ��16λ��ʱ��, ����USER_DEBUGģ���е�usmart_reset_runtime() usmart_get_runtime
  * @param 	Period: 0x0001 - 0xFFFF��
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @note 	ͨ����ʽ: 1 / (TIM4CLK / Prescaler) * Period = 1 / Frequence; ����1 / (TIM4CLK / Prescaler)Ϊ��һ������ʱ��,PeriodΪ��0��ʼ������Period,��Period +1����������
  *			�õ�Ƶ��: Frequence = TIM4CLK / Period / Prescaler
  *         TIM3CLK = 200MHz = 200 000 000Hz
  * 		ȷ������������Ƶ��Ϊ:10Khz����.����,��ʱ����Ҫ�����Զ���װ�ع���!!
  */   
void Bsp_InitHardTimer_TIM4(uint16_t Period,uint16_t Prescaler)
{ 
    __HAL_RCC_TIM4_CLK_ENABLE();
 
    TIM4_Handler.Instance			=TIM4;                          //ͨ�ö�ʱ��4
    TIM4_Handler.Init.Prescaler		=Prescaler;               		//��Ƶ
    TIM4_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;    		//���ϼ�����
    TIM4_Handler.Init.Period		=Period - 1;                    //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM4_Handler);
	
    HAL_NVIC_SetPriority(TIM4_IRQn,3,3);			//����TIM4�ж����ȼ�����ռ���ȼ�3�������ȼ�3 ע��ȴ������ȼ��ͣ����ʱ13.1s
    HAL_NVIC_EnableIRQ(TIM4_IRQn);      			//����ITM4�ж� 
	
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�ж� 	
  }

/**
  * @brief	��ʱ��TIM5��ʼ������һ��32λ��ʱ�������ڶ�ʱһ��ʱ��
  * @param 	Period: 0x00000001 - 0xFFFFFFFF��
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @param 	PWM_Duty: 0 - 100
  * @note 	ͨ����ʽ: 1 / (TIM5CLK / Prescaler) * Period = 1 / Frequence; ����1 / (TIM5CLK / Prescaler)Ϊ��һ������ʱ��,PeriodΪ��0��ʼ������Period,��Period +1����������
  *			�õ�Ƶ��: Frequence = TIM3CLK / Period / Prescaler
  *         TIM5CLK = 200MHz = 200 000 000Hz
  *			TIM5���ʱʱ�䣺0xFFFFFFFF * Frequence
  */  

void Bsp_InitHardTimer_TIM5(uint32_t Period,uint16_t Prescaler)
{  	
    TIM5_Handler.Instance 			= TIM5;                          		//ͨ�ö�ʱ��TIM5
 	TIM5_Handler.Init.Prescaler		=Prescaler;    				//��ʱ����Ƶ
	TIM5_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;		//���ϼ���ģʽ
	TIM5_Handler.Init.Period		=Period - 1;          		//�Զ���װ��ֵ,��0��ʼ��������Period - 1ʱ���¿�ʼ��0����,�ܹ�����ΪPeriod
	TIM5_Handler.Init.ClockDivision	=TIM_CLOCKDIVISION_DIV1;
	
	HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); 						//ʹ�ܶ�ʱ��5�Ͷ�ʱ��5�����ж�    
}

/**
  * @brief  ��ʱ��TIM6��ʼ������һ��32λ��ʱ���������� PA4 DAC�ϵ����ڴ���
  * @param 	Period: 0x00000001 - 0xFFFFFFFF��
  * @param 	Prescaler: 0x0000 - 0xFFFF
  * @param 	PWM_Duty: 0 - 100
  * @note 	ͨ����ʽ: 1 / (TIM3CLK / Prescaler) * Period = 1 / Frequence; ����1 / (TIM3CLK / Prescaler)Ϊ��һ������ʱ��,PeriodΪ��0��ʼ������Period,��Period +1����������
  *			�õ�PWM���Ƶ��Frequence = TIM3CLK / Period / Prescaler
  *         TIM6CLK = 100MHz = 100 000 000Hz
  */

void Bsp_InitHardTimer_TIM6(uint16_t Period,uint16_t Prescaler)
{
	TIM6_Handler.Instance 			=TIM6;		  				
	TIM6_Handler.Init.Prescaler		=Prescaler;    								//��ʱ����Ƶ
	TIM6_Handler.Init.CounterMode	=TIM_COUNTERMODE_UP;						//���ϼ���ģʽ
	TIM6_Handler.Init.Period		=Period - 1;          						//�Զ���װ��ֵ,��0��ʼ��������Period - 1ʱ���¿�ʼ��0����,�ܹ�����ΪPeriod
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
  * @brief  ��ʱ��HRTIM��ʼ��������DAC˫ͨ���Ĵ�����ÿHRTIMCLK / 512 = HRTIMCLK / (TIM_Period + 1)/(TIM_Prescaler + 1);	HRTIMCLK = 200MHz
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
  * @brief	ͨ�ö�ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
  *			�˺����ᱻHAL_TIM_Base_Init()��������
  */  
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)						//����PWM
	{
		__HAL_RCC_TIM2_CLK_ENABLE();        		//ʹ��TIM2ʱ��
		HAL_NVIC_SetPriority(TIM2_IRQn,3,4);		//����TIM2_PWM �ж����ȼ�,��ռ���ȼ�3,�����ȼ�4, ע��ȴ������ȼ���
		HAL_NVIC_EnableIRQ(TIM2_IRQn);      		//����ITM2�ж�   
	}  

    if(htim->Instance==TIM3)						//����ADC Dual Mode to trigger The ADC
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
	}

    if(htim->Instance==TIM5)						//ͨ�ö�ʱ��5
	{
		__HAL_RCC_TIM5_CLK_ENABLE();        		//ʹ��TIMxʱ��
		HAL_NVIC_SetPriority(TIM5_IRQn,4,1);		//����ͨ�ö�ʱ���ж����ȼ�����ռ���ȼ�4�������ȼ�1, ע��ȴ������ȼ���
		HAL_NVIC_EnableIRQ(TIM5_IRQn);      		//����ITMx�ж�   
	} 

    if(htim->Instance==TIM6)						//����DAC ����
	{
		__HAL_RCC_TIM6_CLK_ENABLE();		 		/* TIM6 Periph clock enable */
	}  	
}

/**
  * @brief 	Low level BSP
  * @param 	htim: The timer handle
  */
//��ʱ���ײ�������ʱ��ʹ�ܣ���������,�˺����ᱻHAL_TIM_PWM_Init()����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM2_CLK_ENABLE();						//ʹ�ܶ�ʱ��2
    __HAL_RCC_GPIOA_CLK_ENABLE();						//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_2 | GPIO_PIN_3;           //PA2 | PA3
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  				//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          			//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     	//����
	GPIO_Initure.Alternate=GPIO_AF1_TIM2;				//PA2, PA3����ΪTIM2
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

/**
  * @brief TIM MSP Initialization ����TIM1���ⲿ����ʱ��Ƶ�ʼ��㣬ע����İ彫PE11�ӵ���FMC�ϣ�����ͬʱʹ��
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1)				//�����ⲿ����ʱ��Ƶ�ʼ���
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
	  
		HAL_NVIC_SetPriority(TIM1_CC_IRQn, 4, 2);	//���ö�ʱ��TIM1�ж����ȼ�����ռ���ȼ�4�������ȼ�2, ע��ȴ������ȼ���
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
	if(htim->Instance==TIM2)				//ͨ�ö�ʱ��2
	{
		__HAL_RCC_TIM2_FORCE_RESET();
		__HAL_RCC_TIM2_RELEASE_RESET();	
		HAL_NVIC_DisableIRQ(TIM2_IRQn);      
	}
    if(htim->Instance==TIM3)				//ͨ�ö�ʱ��3
	{
		__HAL_RCC_TIM3_FORCE_RESET();
		__HAL_RCC_TIM3_RELEASE_RESET();	
	}
    if(htim->Instance==TIM5)				//ͨ�ö�ʱ��5
	{
		__HAL_RCC_TIM5_FORCE_RESET();
		__HAL_RCC_TIM5_RELEASE_RESET();	
		HAL_NVIC_DisableIRQ(TIM5_IRQn);      
	}
  
	if(htim->Instance==TIM6)				//ͨ�ö�ʱ��6
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
  * @brief	��ʱ��1�жϷ����� ����TIM1���ⲿ����ʱ��Ƶ�ʼ��㣬ע����İ彫PE11�ӵ���FMC�ϣ�����ͬʱʹ��
  */ 
void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM1_Handler);
}

#if USMART_ENTIMX_SCAN == 1

/**
  * @brief	��ʱ��4�жϷ����� ����USER_DEBUGE��
  */ 
void TIM4_IRQHandler(void)
{ 		    		  			       
    if(__HAL_TIM_GET_IT_SOURCE(&TIM4_Handler,TIM_IT_UPDATE)==SET)//����ж�
    {
        usmart_dev.scan();	//ִ��usmartɨ��
        __HAL_TIM_SET_COUNTER(&TIM4_Handler,0);;    //��ն�ʱ����CNT
        __HAL_TIM_SET_AUTORELOAD(&TIM4_Handler,100);//�ָ�ԭ��������
    }
    __HAL_TIM_CLEAR_IT(&TIM4_Handler, TIM_IT_UPDATE);//����жϱ�־λ
}

#endif

/**
  * @brief 	��ʱ��5�жϷ�����
  */

void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);
}

/**
  * @brief 	��ʱ��6�жϷ�����, ����DAC���ɲ����õĲ���
  */

void TIM6_DAC_IRQHandler(void)
{
  HAL_DAC_IRQHandler(&DAC_Handler);
}

/**
  * @brief	��ʱ���жϷ��������ã���ͬ�ķ�����������stm32h7xxx_hal_tim.c��
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
  * @brief	��ʱ���жϷ��������ã���ͬ�ķ�����������stm32h7xxx_hal_tim.c��
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
  * @brief  ��ʱ���жϷ��������ã���ͬ�ķ�����������stm32h7xxx_hal_tim.c��
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
  *	@param		_CC : Channel 1��2��3, 4
  *	@param		_uiTimeOut : The timeout period
  *	@param		_pCallBack : callback functions
  */ 
  
// ʹ��TIM5�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�����������ͬʱ����4��ͨ������������.
// ��ʱ��������2us ����Ҫ�ķ��ڵ��ñ�������ִ��ʱ�䣬�����ڲ������˲�����С��
// TIM5 ��32λ��ʱ��.��ʱ��Χ�ܴ�
// _CC : ����ͨ������1��2��3, 4
// _uiTimeOut : ��ʱʱ��,32λ��ʱ��,��� 4294��
// _pCallBack : ��ʱʱ�䵽�󣬱�ִ�еĺ��� 
void Bsp_StarGenerictHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
//���ڲ���ִ�д˺�������ʱΪ1.16us(ͨ���߼������ǲ���IO��ת)	
//	GPIOC->BSRRH=1<<1;		// ��GPIOC_PIN1�ɵ͵�ƽ
//	Bsp_Delay_ms(500);
//	GPIOC->BSRRL=1<<1;		// ��GPIOC_PIN1�ɸߵ�ƽ
	
    uint32_t cnt_now;
    uint32_t cnt_tar;
	
	sOCConfig.OCMode     = TIM_OCMODE_TIMING;
	sOCConfig.OCPolarity = TIM_OCPOLARITY_LOW;
	
	/* ���ڲ������øú����ĺ�ʱ*/
    if (_uiTimeOut < 1)
    {
        ;
    }
    else
    {
        _uiTimeOut -= 1;
    }

    cnt_now = __HAL_TIM_GetCounter(&TIM5_Handler);    	/* ��ȡ��ǰ�ļ�����ֵ */
    cnt_tar = cnt_now + _uiTimeOut;						/* ���㲶��ļ�����ֵ */
    if (_CC == 1)
    {
        s_TIM_CallBack_CC1 = (void (*)(void))_pCallBack;
		
		if(HAL_TIM_OC_ConfigChannel(&TIM5_Handler, &sOCConfig, TIM_CHANNEL_1) != HAL_OK)
		{
		Error_Handler();
		}
        __HAL_TIM_SET_COMPARE(&TIM5_Handler, TIM_CHANNEL_1, cnt_tar);      	/* ���ò���Ƚϼ�����CC1 */
        __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_CC1);						/* ��CC1�жϱ�־λ.*/
		if(HAL_TIM_OC_Start_IT(&TIM5_Handler, TIM_CHANNEL_3) != HAL_OK)		/* ʹ��CC1�ж� */
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
        __HAL_TIM_SET_COMPARE(&TIM5_Handler, TIM_CHANNEL_2, cnt_tar);      	/* ���ò���Ƚϼ�����CC2 */
        __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_CC2);						/* ��CC2�жϱ�־λ.*/
		if(HAL_TIM_OC_Start_IT(&TIM5_Handler, TIM_CHANNEL_2) != HAL_OK)		/* ʹ��CC2�ж� */
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
        __HAL_TIM_SET_COMPARE(&TIM5_Handler, TIM_CHANNEL_3, cnt_tar);      	/* ���ò���Ƚϼ�����CC3 */
        __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_CC3);						/* ��CC3�жϱ�־λ.*/
		if(HAL_TIM_OC_Start_IT(&TIM5_Handler, TIM_CHANNEL_3) != HAL_OK)		/* ʹ��CC3�ж� */
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
        __HAL_TIM_SET_COMPARE(&TIM5_Handler, TIM_CHANNEL_4, cnt_tar);      	/* ���ò���Ƚϼ�����CC4 */
        __HAL_TIM_CLEAR_IT(&TIM5_Handler, TIM_IT_CC4);						/* ��CC4�жϱ�־λ.*/
		if(HAL_TIM_OC_Start_IT(&TIM5_Handler, TIM_CHANNEL_4) != HAL_OK)		/* ʹ��CC4�ж� */
		{
			Error_Handler();
		}
    }
	else
    {
        return;
    }
		
//	GPIOC->BSRRH=1<<1;		// ����GPIOC_PIN1�ɵ͵�ƽ
//	Bsp_Delay_ms(500);
//	GPIOC->BSRRL=1<<1;		// ��GPIOC_PIN1�ɸߵ�ƽ

}

/**
  * @brief	����Ӳ����ʱ������ָ��ʱ����TimeOut�ص�����,��Ҫʹ��ʱ���Ը��Ƴ�ȥ�޸ĳ�����
  */ 

void _pFunctionHardTimeout(void)
{
#if HARDWARE_TIM5_DEBUG == 1
	Bsp_Printf("TIM5 Delay TimeOut! User can add functions here when timeout! \r\n");
#endif
}

/**
  * @brief 	����TIM1��ͨ��2����LSIƵ��
  * @return Input Frequence
  */

uint32_t TIM1_ComputeInputFreq(void)
{
	extern uint32_t	uwFrequency;
#if HARDWARE_TIM1_DEBUG == 1
	Bsp_Printf("Initial TIM1_CHANNEL2 to Compute the input Frequence in PE11 ! \r\n");
#endif
#if HARDWARE_TIM1_ENABLE == 1	
	/* ��ʼ��TIM1��ʱ�� */		
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
