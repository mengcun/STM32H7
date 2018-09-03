/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/HARD_TIMER/Hardware_Timer_Bsp.c
  * @author  MCD Application Team
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
  *			 TIM3��ͨ��1����2KHz PWM���ɣ������PB4���ţ���ʱ��3Ϊ16λ������65535��PWM��СƵ��Ϊ1/65535 KHz
  *			 TIM4��USER_DEBUG�����ڼ��㺯��ִ��ʱ�䣬��ʱ��4Ϊ16λ������65535����ʱ����Ϊ10KHz(0.1ms)���������ʱʱ��Ϊ13.1s
  *			 TIM5�����ζ�ʱ��ʹ��,����Ϊ����2us
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
TIM_HandleTypeDef 		TIM1_Handler;      //��ʱ��1��� 

/**
  * @brief		TIM4 Private Handle
  */
TIM_HandleTypeDef 		TIM4_Handler;      //��ʱ��4��� 

/**
  * @brief		Generic TIMx Private Handle
  */
TIM_HandleTypeDef 		TIM5_Handler;      //��ʱ��5��� 
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
  * @brief	�߼���ʱ��TIM1�ĳ�ʼ������һ��16λ�Զ���װ��ʱ��,���ڼ����ⲿ����ʱ�ӵ�Ƶ��
  *			TIM1 counter clock is SystemCoreClock/2 (Hz) =  200MHz
  *			TIM1 configuration: Input Capture mode
  *			The external signal is connected to TIM1 CH2 pin (PE.11), ע����İ彫PE11�ӵ���FMC�ϣ�����ͬʱʹ�á�  
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
  * @brief	ͨ�ö�ʱ��4�жϳ�ʼ��,ȷ������������Ƶ��Ϊ:10Khz����.����,��ʱ����Ҫ�����Զ���װ�ع���!!
  *			����USER_DEBUGģ���е�usmart_reset_runtime() usmart_get_runtime 
  */   

void Bsp_InitHardTimer_TIM4(uint16_t Period,uint16_t Prescaler)
{ 
    __HAL_RCC_TIM4_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM4_IRQn,3,3);	//����TIM4�ж����ȼ�����ռ���ȼ�3�������ȼ�3 ע��ȴ������ȼ��ͣ����ʱ13.1s
    HAL_NVIC_EnableIRQ(TIM4_IRQn);      	//����ITM4�ж�    
    
    TIM4_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��4
    TIM4_Handler.Init.Prescaler=Prescaler;                     //��Ƶ
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM4_Handler.Init.Period=Period;                        //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM4_Handler);
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�ж� 					 
}
 
#endif

/**
  * @brief	ͨ�ö�ʱ��5�жϳ�ʼ��,��ʱ��5��APB1�ϣ�APB1�Ķ�ʱ��ʱ��Ϊ200MHz
  *			��ʱ�����ʱ����㷽��:Tout=((usPeriod)*(usPrescaler))/uiTIMxCLK us.	uiTIMxCLK = ��ʱ������Ƶ��,��λ:Mhz
  *			����ʹ�õ��Ƕ�ʱ��5!(��ʱ��2����APB1�ϣ�ʱ��ΪHCLK/2)
  *			TIM5 ���0xFFFFFFFF / 1000000 = 4294s
  */  
 void Bsp_InitHardTimer_TIM5(void)
{  	
    TIM5_Handler.Instance = TIM5;                          		//ͨ�ö�ʱ��TIM5
    TIM5_Handler.Init.Prescaler = 200 - 1;                    	//uiTIMxCLK / 1000000; /* ��Ƶ������ 1us */ /*��ȥ1 Ϊ�˸���׼ȷ*/
    TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;    		//���ϼ�����
    TIM5_Handler.Init.Period = 0x000F4240 - 1;                  //�Զ�װ��ֵ	/*��ȥ1 Ϊ�˸���׼ȷ*/  1s = 1 * 1000000 us
    TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;	//ʱ�ӷ�Ƶ����
    
	HAL_TIM_Base_Init(&TIM5_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM5_Handler); 						//ʹ�ܶ�ʱ��5�Ͷ�ʱ��5�����ж�    
}

/**
  * @brief	ͨ�ö�ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
  *			�˺����ᱻHAL_TIM_Base_Init()��������
  */  
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM5)				//ͨ�ö�ʱ��
	{
		__HAL_RCC_TIM5_CLK_ENABLE();        //ʹ��TIMxʱ��
		HAL_NVIC_SetPriority(TIM5_IRQn,4,1);//����ͨ�ö�ʱ���ж����ȼ�����ռ���ȼ�4�������ȼ�1, ע��ȴ������ȼ���
		HAL_NVIC_EnableIRQ(TIM5_IRQn);      //����ITMx�ж�   
	} 
	
    if(htim->Instance==TIM3)					//����PWM
	{
		__HAL_RCC_TIM3_CLK_ENABLE();        //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,3,4);//����TIM3_PWM �ж����ȼ�,��ռ���ȼ�3,�����ȼ�4, ע��ȴ������ȼ���
		HAL_NVIC_EnableIRQ(TIM3_IRQn);      //����ITM3�ж�   
	}  	
}
/**
  * @brief TIM MSP Initialization ����TIM1���ⲿ����ʱ��Ƶ�ʼ��㣬ע����İ彫PE11�ӵ���FMC�ϣ�����ͬʱʹ��
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

  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 4, 2);	//���ö�ʱ��TIM1�ж����ȼ�����ռ���ȼ�4�������ȼ�2, ע��ȴ������ȼ���

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
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
  * @brief	��ʱ���жϷ��������ã���ͬ�ķ�����������stm32h7xxx_hal_tim.c��
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
  * @brief	��ʱ���жϷ��������ã���ͬ�ķ�����������stm32h7xxx_hal_tim.c��
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
  * @brief  ��ʱ���жϷ��������ã���ͬ�ķ�����������stm32h7xxx_hal_tim.c��
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
#if SYSTEM_DEBUG == 1
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
#if SYSTEM_DEBUG == 1
	Bsp_Printf("Initial TIM1_CHANNEL2 to Compute the input Frequence in PE11 ! \r\n");
#endif	
	/* ��ʼ��TIM1��ʱ�� */		
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
