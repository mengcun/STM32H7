/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/ADC/Adc_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 06-September-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The ADC canbe used as follows:
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
/** @defgroup ADC ADC
  * @brief ADC
	* @note	ADC
  * @{
  */

/* Private variables -------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Private_c DAC_LFSR_NOISE Private variables
  * @{
  */
/**
  * @brief The Handler for ADC1
  */  
ADC_HandleTypeDef    	ADC1_Handler;

/**
  * @brief The Handler for ADC1
  */  
ADC_HandleTypeDef    	ADC3_Handler;

/**
  * @brief The Handler for ADC
  */  
ADC_ChannelConfTypeDef	sConfig;

/**
  * @brief The Handler for DMA
  */  		
DMA_HandleTypeDef       DMA_Handler;

/**
  * @brief Variable containing ADC conversions data 
  */ 
__IO uint32_t ADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];

/**
  * @brief Variable about the result of Conversion.
  */
uint8_t ADC_Conversion_Complete;

/**
  * @brief Variable about the status of nalogWatchdog.
  */
uint8_t ADC_AnalogWatchdogStatus;

/**
  * @brief  Variable containing ADC converted value
  */
uint32_t 	CPU_Internal_Temperature;	/*The actual Temperature output value(.C)*/
uint32_t	VREFINT;				/*The actual VREFINT output value(mV)*/
uint32_t	VBAT;					/*The actual VBAT output value(mV)*/
	
/** @}
*/		
/*----------------------------ADC Private variables--------------------------------*/ 

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup ADC_Private_Functions ADC Private Functions
  * @{
  */
/**
  * @brief 
  * @param param: brief.
  * @retval status
  */
//CODING HERE Private Functions
/** @}
*/
/*--------------------------------------ADC Private Functions---------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */
/** @defgroup ADC_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize ADC
  @endverbatim
  * @{
  */
/**
  * @brief 	Initialize the ADC3 according to the specified  
  * @brief 	ADCCLK = HCLK(AHB) / 4 = SYSCLK / 8 = 400 / 8 = 50M
  * @brief 	ADC采样率 = ADC主频 / （采样时间 + 转换时间） 50MHz / (Sampling Time + Conversion Time) = 50 / (8.5 + 16 cycles) = 2.04Mbps.
  * @brief  ADC采样频率： sConfig.SamplingTime + ADC3_Handler.Init.Resolution.
  * @brief 	Get the ADC converted value By polling mode.
  * @param 	Channelx : ADC_CHANNEL_0~ADC_CHANNEL_16
  * @note	ADC3, ADC_CHANNEL_17 -- VBAT; ADC_CHANNEL_18 -- CPU_Internal_Temperature; ADC_CHANNEL_19 -- VREFINT; 
  */
void Bsp_Init_ADC3_SINGLE_CHANNEL(uint32_t Channelx)
{  
	ADC3_Handler.Instance					   = ADC3;
	if (HAL_ADC_DeInit(&ADC3_Handler) != HAL_OK)
	{
		Error_Handler();
	}
	ADC3_Handler.Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;      /* Synchronous clock mode, input ADC clock divided by 4*/
	ADC3_Handler.Init.Resolution               = ADC_RESOLUTION_16B;            /* 16-bit resolution for converted data */
	ADC3_Handler.Init.ScanConvMode             = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	ADC3_Handler.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
	ADC3_Handler.Init.LowPowerAutoWait         = DISABLE;                       /* Auto-delayed conversion feature disabled */
	ADC3_Handler.Init.ContinuousConvMode       = DISABLE;                       /* Continuous mode enabled (automatic conversion restart after each conversion) */
	ADC3_Handler.Init.NbrOfConversion          = 1;                             /* Parameter discarded because sequencer is disabled */
	ADC3_Handler.Init.DiscontinuousConvMode    = DISABLE;                       /* Parameter discarded because sequencer is disabled */
	ADC3_Handler.Init.NbrOfDiscConversion      = 0;                             /* Parameter discarded because sequencer is disabled */
	ADC3_Handler.Init.ExternalTrigConv         = ADC_SOFTWARE_START;       		/* Software start to trig the 1st conversion manually, without external event */
	ADC3_Handler.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
	ADC3_Handler.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;  		/* 规则通道的数据仅仅保存在DR寄存器里面*/
	ADC3_Handler.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
	ADC3_Handler.Init.OversamplingMode         = DISABLE;                       /* No oversampling */
	ADC3_Handler.Init.BoostMode                = ENABLE;                        /* Enable Boost mode as ADC clock frequency is bigger than 20 MHz */
   
	if (HAL_ADC_Init(&ADC3_Handler) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_ADCEx_Calibration_Start(&ADC3_Handler, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}	
	sConfig.Channel					=Channelx;                          //通道
	sConfig.Rank					=ADC_REGULAR_RANK_1;                //1个序列
	sConfig.SamplingTime			=ADC_SAMPLETIME_810CYCLES_5;     	//采样时间       
	sConfig.SingleDiff				=ADC_SINGLE_ENDED;  				//单边采集          		
	sConfig.OffsetNumber			=ADC_OFFSET_NONE;             	
	sConfig.Offset=0;   	
	sConfig.OffsetRightShift       	= DISABLE;                    		/* No Right Offset Shift */
	sConfig.OffsetSignedSaturation 	= DISABLE;                    		/* No Signed Saturation */
	HAL_ADC_ConfigChannel(&ADC3_Handler,&sConfig);        				//通道配置	
}

/**
  * @brief 	Initialize the ADC1 according to the specified  
  * @brief 	ADCCLK = HCLK(AHB) / 4 = SYSCLK / 8 = 400 / 8 = 50M
  * @brief 	ADC采样率 = ADC主频 / （采样时间 + 转换时间） 50MHz / (Sampling Time + Conversion Time) = 50 / (8.5 + 16 cycles) = 2.04Mbps.
  * @brief  ADC采样频率： sConfig.SamplingTime + ADC1_Handler.Init.Resolution.
  * @brief 	Get the ADC converted value through DMA
  * @param 	Channelx : ADC_CHANNEL_0~ADC_CHANNEL_16
  * @note	ADC1, ADC_CHANNEL_17 -- VBAT; ADC_CHANNEL_18 -- CPU_Internal_Temperature; ADC_CHANNEL_19 -- VREFINT; 
  */
void Bsp_Init_ADC12_MULTI_CHANNEL(void)
{  
	ADC1_Handler.Instance					   = ADC1;
	
	if (HAL_ADC_DeInit(&ADC1_Handler) != HAL_OK)
	{
		Error_Handler();
	}
	ADC1_Handler.Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;      	/* Synchronous clock mode, input ADC clock divided by 4*/
	ADC1_Handler.Init.Resolution               = ADC_RESOLUTION_16B;            	/* 16-bit resolution for converted data */
	ADC1_Handler.Init.ScanConvMode             = ENABLE;                       		/* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	ADC1_Handler.Init.EOCSelection             = ADC_EOC_SEQ_CONV;           		/* EOC flag picked-up to indicate conversion end */
	ADC1_Handler.Init.LowPowerAutoWait         = DISABLE;                       	/* Auto-delayed conversion feature disabled */
	ADC1_Handler.Init.ContinuousConvMode       = ENABLE;                        	/* Continuous mode enabled (automatic conversion restart after each conversion) */
	ADC1_Handler.Init.NbrOfConversion          = 2;                             	/* Specify the number of ranks that will be converted within the regular group sequencer */
	ADC1_Handler.Init.DiscontinuousConvMode    = DISABLE;                       	/* Parameter discarded because sequencer is enabled */
	ADC1_Handler.Init.NbrOfDiscConversion      = 0;                             	/* Parameter discarded because sequencer is enabled */
	ADC1_Handler.Init.ExternalTrigConv         = ADC_SOFTWARE_START;       			/* Software start to trig the 1st conversion manually, without external event */
	ADC1_Handler.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC1_Handler.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;	/* 规则通道的数据使用DMA循环保存*/
	ADC1_Handler.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;      	/* DR register is overwritten with the last conversion result in case of overrun */
	ADC1_Handler.Init.OversamplingMode         = DISABLE;                       	/* No oversampling */
	ADC1_Handler.Init.BoostMode                = ENABLE;                        	/* Enable Boost mode as ADC clock frequency is bigger than 20 MHz */
  
	if (HAL_ADC_Init(&ADC1_Handler) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_ADCEx_Calibration_Start(&ADC1_Handler, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}	
		
	sConfig.Channel					=ADC_CHANNEL_4;            			//PA4引脚上的 ADC_CHANNEL_4,用做ADC1
	sConfig.Rank					=ADC_REGULAR_RANK_1;                //1个序列
	sConfig.SamplingTime			=ADC_SAMPLETIME_810CYCLES_5;     	//采样时间       
	sConfig.SingleDiff				=ADC_SINGLE_ENDED;  				//单边采集          		
	sConfig.OffsetNumber			=ADC_OFFSET_NONE;             	
	sConfig.Offset=0;   	
	sConfig.OffsetRightShift       	= DISABLE;                    		/* No Right Offset Shift */
	sConfig.OffsetSignedSaturation 	= DISABLE;                    		/* No Signed Saturation */
	HAL_ADC_ConfigChannel(&ADC1_Handler,&sConfig);        				//通道配置	

	
	sConfig.Channel					=ADC_CHANNEL_6;            			//PA6引脚上的 ADC_CHANNEL_6,用做ADC2
	sConfig.Rank					=ADC_REGULAR_RANK_2;                //1个序列
	sConfig.SamplingTime			=ADC_SAMPLETIME_64CYCLES_5;     		//采样时间       
	sConfig.SingleDiff				=ADC_SINGLE_ENDED;  				//单边采集          		
	sConfig.OffsetNumber			=ADC_OFFSET_NONE;             	
	sConfig.Offset=0;   	
	sConfig.OffsetRightShift       	= DISABLE;                    		/* No Right Offset Shift */
	sConfig.OffsetSignedSaturation 	= DISABLE;                    		/* No Signed Saturation */
	HAL_ADC_ConfigChannel(&ADC1_Handler,&sConfig);        				//通道配置	

	
	if (HAL_ADC_Start_DMA(&ADC1_Handler, (uint32_t *)ADCxConvertedData, ADC_CONVERTED_DATA_BUFFER_SIZE) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  * @param hadc: ADC handle pointer
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *ADCx_Handler)
{
	if(ADCx_Handler->Instance == ADC3)
	{
		__HAL_RCC_ADC3_CLK_ENABLE();    
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);
	}
	
	if(ADCx_Handler->Instance == ADC1)
	{	
		GPIO_InitTypeDef   			GPIO_InitStruct;
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_DAC12_CLK_ENABLE();
		__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_CLKP);		
		__HAL_RCC_DMA1_CLK_ENABLE();
		
		GPIO_InitStruct.Pin 	= GPIO_PIN_4 | GPIO_PIN_6;	
		GPIO_InitStruct.Mode 	= GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull 	= GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		DMA_Handler.Instance                 = DMA1_Stream1;
		DMA_Handler.Init.Request             = DMA_REQUEST_ADC2;
		DMA_Handler.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		DMA_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
		DMA_Handler.Init.MemInc              = DMA_MINC_ENABLE;
		DMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		DMA_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
		DMA_Handler.Init.Mode                = DMA_CIRCULAR;
		DMA_Handler.Init.Priority            = DMA_PRIORITY_HIGH;
		
		HAL_DMA_DeInit(&DMA_Handler);
		HAL_DMA_Init(&DMA_Handler);

		__HAL_LINKDMA(ADCx_Handler, DMA_Handle, DMA_Handler);
   
		HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 2, 4);					//设置DMA_Stream1用与ADC中断优先级，抢占优先级2，子优先级4。ADC > DMA > DAC
		HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);	
	}
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  * @param hadc: ADC handle pointer
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *ADCx_Handler)
{
	if(ADCx_Handler->Instance == ADC3)
	{
		__HAL_RCC_ADC3_FORCE_RESET();
		__HAL_RCC_ADC3_RELEASE_RESET();
		__HAL_RCC_ADC3_CLK_DISABLE();
	}
	if(ADCx_Handler->Instance == ADC1)
	{
		__HAL_RCC_ADC12_FORCE_RESET();
		__HAL_RCC_ADC12_RELEASE_RESET();
		__HAL_RCC_ADC12_CLK_DISABLE();	
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_6);
	}
}

/** @}
*/
/*********************ADC Exported Functions Group1**************************/

/** @defgroup ADC_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the ADC
  @endverbatim
  * @{
  */

/**
  * @brief Get the certain times ADC converted value and then get the average
  * @param ADCx : ADC1 or ADC2 or ADC3
  * @param Channelx : ADC_CHANNEL_0~ADC_CHANNEL_16
  * @param times : How many times
  * @retval The ADC converted average value
  */

uint32_t Bsp_Get_Adc_Average(ADC_HandleTypeDef ADCx_Handler,uint8_t times)
{
	uint32_t temp_val = 0;
	uint32_t  		t;
	uint32_t  Average = 0;

	for(t=0;t<times;t++)
	{
		HAL_ADC_Start(&ADCx_Handler);                               	//开启ADC
		HAL_ADC_PollForConversion(&ADCx_Handler,10);                	//轮询转换
		temp_val += (uint32_t)HAL_ADC_GetValue(&ADCx_Handler);
		Bsp_Delay_ms(5);
	}
	Average = temp_val / times;
	Bsp_Printf("Get the ADC Average on ADC3 is %d \r\n",Average);
	
	return Average;
}

/**
  * @brief Get the Internal tempature ,The temperature sensor is internally connected to ADC3 Vinp[18]
  * @retval The temputare value in degress
  * @note Temperature('C) = (110-30) / (TS_CAL2-TS_CAL1) * (TS_DATA - TS_CAL1) + 30
  * @note TS_CAL1 is the temperature sensor calibration value acquired at 30°C
  * @note TS_CAL2 is the temperature sensor calibration value acquired at 110°C
  * @note TS_DATA is the actual temperature sensor output value converted by ADC
  */

uint32_t Bsp_Get_CPU_Intern_Temperature(void)
{
    uint32_t TS_CAL1;
    uint32_t TS_CAL2;
	uint32_t TS_DATA;
  
    TS_CAL1 = *(__IO uint16_t*)(0X1FF1E820);	/*reference to STM32H7_DataSheet_Page170*/
    TS_CAL2 = *(__IO uint16_t*)(0X1FF1E840);
    
	
	TS_DATA = Bsp_Get_Adc_Average(ADC3_Handler,10);	//读取内部温度传感器通道,10次取平均
	
    CPU_Internal_Temperature=(110.0-30.0)/(TS_CAL2-TS_CAL1)*(TS_DATA-TS_CAL1)+30;
	Bsp_Printf("The Temperature in CPU is %d degress! \r\n",CPU_Internal_Temperature);
	return CPU_Internal_Temperature;
}

/**
  * @brief Retreive the system internal voltage reference,The internal voltage reference is internally connected to the input channel ADC3 VINP[19].
  * @retval The Reference voltage after retreive in mV
  * @note VREF+ = 3.3 V x VREFINT_CAL / VREFINT_DATA
  * @note VREFINT_CAL is the VREFINT calibration value
  * @note VREFINT_DATA is the actual VREFINT output value converted by ADC
  */

uint32_t Bsp_Get_CPU_Intern_VerfInt(void)
{
	uint32_t	VREFINT_CAL;	/*VREFINT_CAL is the VREFINT calibration value*/
    uint32_t	VREFINT_DATA;	/*VREFINT_DATA is the actual VREFINT output value converted by ADC*/
	
    VREFINT_CAL = *(__IO uint16_t*)(0X1FF1E860);	/*reference to STM32H7_DataSheet_Page104*/

	VREFINT_DATA = Bsp_Get_Adc_Average(ADC3_Handler, 10);
	VREFINT = VREFINT_CAL * 3300 / VREFINT_DATA;  /* Get the actually VREFINT (mV)*/
	Bsp_Printf("The CPU_Intern_VerfInt is %d mV! \r\n",VREFINT);
	return VREFINT;
}

/**
  * @brief Retreive the system Battary voltage,The internal voltage reference is internally connected to the input channel ADC3 VINP[17].
  * @retval The Battary voltage after retreive in mV
  * @note VBAT+ = 4 * 3.3 V x VBAT_DATA / 0XFFFF
  * @note VBAT_DATA is the actual VREFINT output value converted by ADC
  */

uint32_t Bsp_Get_Board_VBAT(void)
{
    uint32_t	VBAT_DATA;	/*VREFINT_DATA is the actual VBAT output value converted by ADC*/

	VBAT_DATA = Bsp_Get_Adc_Average(ADC3_Handler, 10);
	VBAT = 4 * VBAT_DATA * VREFINT / 0XFFFF;  /* Get the actually VBAT (mV)*/
	Bsp_Printf("The Board Vbat is %d mV! \r\n",VBAT);
	return VBAT;
}

/**
* @brief  This function handles DMA interrupt request. used for ADC
*/
void DMA1_Stream1_IRQHandler(void)
{
#if ADC12_MULTI_CHANNEL_CONVERT == 1
	HAL_DMA_IRQHandler(ADC1_Handler.DMA_Handle);
	Bsp_Printf("DMA_Stream1 sendout the  converted data of ADC1! \r\n");
#endif	/*ADC12_MULTI_CHANNEL_CONVERT*/
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  ADC1_Handler : ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @note   When ADC_TRIGGER_FROM_TIMER is disabled, conversions are software-triggered
  *         and are too fast for DMA post-processing. Therefore, to reduce the computational 
  *         load, the output buffer filled up by the DMA is post-processed only when 
  *         ADC_TRIGGER_FROM_TIMER is enabled.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *ADCx_Handler)
{
	if(ADCx_Handler->Instance == ADC1)
	{
		Bsp_Printf("The data get from ADC1 on PA4 is %d! \r\n",ADCxConvertedData[0]);
		Bsp_Printf("The data get from ADC2 on PA6 is %d! \r\n",ADCxConvertedData[1]);
	}
	ADC_Conversion_Complete = TRUE;	
}

/**
  * @brief  Analog watchdog callback in non blocking mode. 
  * @param  hadc: ADC handle
  */
  void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* ADCx_Handler)
{
	if(ADCx_Handler->Instance == ADC1)
	{
		Bsp_Printf("The ADC3 Level Out Of Window! \r\n");
	}	
	
	if(ADCx_Handler->Instance == ADC2)
	{
		Bsp_Printf("The ADC2 Level Out Of Window! \r\n");
	}	
	
	if(ADCx_Handler->Instance == ADC1)
	{
		Bsp_Printf("The ADC1 Level Out Of Window! \r\n");
	}	
				
	ADC_AnalogWatchdogStatus = SET;
}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @note   When ADC_TRIGGER_FROM_TIMER is disabled, conversions are software-triggered
  *         and are too fast for DMA post-processing. Overrun issues are observed and to
  *         avoid ending up in the infinite loop of Error_Handler(), no call to this
  *         latter is done in case of HAL_ADC_ERROR_OVR error.
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *ADCx_Handler)
{
    Error_Handler();
}

/** @}
*/
/****************************ADC Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------ADC Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------ADC-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
