/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/ADC/Adc_DualMode_Bsp.c
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
  * @brief The Handler for AdcHandle_master
  */  
ADC_HandleTypeDef    	ADC_Handler_Master;

/**
  * @brief The Handler for AdcHandle_slave
  */  
ADC_HandleTypeDef    	ADC_Handler_Slave;

/**
  * @brief   Variable containing ADC conversions results
  */ 
ALIGN_32BYTES(__IO uint32_t   	aADCDualConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE]);   	/* ADC dual mode interleaved conversion results (ADC master and ADC slave results concatenated on data register 32 bits of ADC master). */
ALIGN_32BYTES(__IO uint16_t   	aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE]);       	/* Dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
ALIGN_32BYTES(__IO uint16_t   	aADCyConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE]);       	/* Dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
uint8_t         			  	ubADCConversionComplete = FALSE;                        	/* Set into ADC conversion complete callback */

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
  * @brief 	Initialize the ADC1 ADC2 according to the specified. ADCCLK = PCLK2 / 4 = HCLK / 8 = 400 / 8 = 50M
			ADC²ÉÑùÆµÂÊ£º sConfig.SamplingTime + Conversion Time(ADCx_Handler.Init.Resolution.) = 8.5 + 16 cycles = 24.5cyc ; Conversion Time = 50MHz / 24.5cyc = 2.04Mbps.
			ADC sampling rate can be increased by modifying resolution and sampling time, but this will decrease the conversion accuracy.
			Both ADC are using the same DMA (DMA of ADC master). DMA is used as well by DAC in case of WAVEFORM_VOLTAGE_GENERATION_FOR_TEST enabled. 
			ADC conversions results are transferred automatically by DMA, into variable array "aADCDualConvertedValues".
			ADC master and ADC slave results are concatenated in a common 32-bit data register: 
			ADC master results in the 16 LSB [15:0] (with ADC resolution 16 bits, bits effectively used are [15:0])
			ADC slave results in the 16 MSB [31:16] (with ADC resolution 16 bits, bits effectively used are [31:16])
			DMA are configured to operate continuously, in circular mode. Data is transferred by words every ADC1+ADC2 conversions.
			When DMA transfer half-buffer and complete buffer length are reached, callbacks HAL_ADC_ConvCpltCallback() and HAL_ADC_ConvCpltCallback() are called.
			Dual conversion values are extracted from the common register and stored into 2 arrays corresponding to each ADC conversion values (aADCxConvertedValues, aADCyConvertedValues).
 
			ADC1 and ADC2 are configured to convert the same channel: ADC_CHANNEL_18 on pin PA.04.
			If compilation switch "WAVEFORM_VOLTAGE_GENERATION_FOR_TEST" is defined, the voltage input on ADC channel comes 
			from the DAC channel. This means that, if they are different, ADC pin and DAC pin must be connected by a wire. 
 
  * @param 	*ADCx : as AdcHandle_master
  * @param 	Channelx : ADC_CHANNEL_0~ADC_CHANNEL_16
  * @param 	*ADCy : AdcHandle_slave
  * @param 	Channelx : ADC_CHANNEL_0~ADC_CHANNEL_16
  */
void Bsp_Init_DualModeADC(ADC_TypeDef *ADCx, uint32_t Channelx, ADC_TypeDef *ADCy, uint32_t Channely)
{  
	ADC_ChannelConfTypeDef		sConfig;
	ADC_MultiModeTypeDef     	MultiModeInit;

	ADC_Handler_Master.Instance=ADCx;	
	if (HAL_ADC_DeInit(&ADC_Handler_Master) != HAL_OK)
	{
		Error_Handler();
	}
	ADC_Handler_Master.Init.ClockPrescaler           = ADC_CLOCK_SYNC_PCLK_DIV4;      /* Synchronous clock mode, input ADC clock divided by 4*/
	ADC_Handler_Master.Init.Resolution               = ADC_RESOLUTION_16B;            /* 16-bit resolution for converted data */
	ADC_Handler_Master.Init.ScanConvMode             = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	ADC_Handler_Master.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
	ADC_Handler_Master.Init.LowPowerAutoWait         = DISABLE;                       /* Auto-delayed conversion feature disabled */
#if ADC_DUAL_TRIGGER_FROM_TIMER == 1
	ADC_Handler_Master.Init.ContinuousConvMode       = DISABLE;                         /* Continuous mode disabled to have only 1 conversion at each conversion trig */
#elseif ADC_DUAL_TRIGGER_FROM_TIMER == 0
	ADC_Handler_Master.Init.ContinuousConvMode       = ENABLE;                          /* Continuous mode to have maximum conversion speed (no delay between conversions) */
#endif	/*ADC_DUAL_TRIGGER_FROM_TIMER*/
	ADC_Handler_Master.Init.NbrOfConversion          = 1;                             /* Parameter discarded because sequencer is disabled */
	ADC_Handler_Master.Init.DiscontinuousConvMode    = DISABLE;                       /* Parameter discarded because sequencer is disabled */
	ADC_Handler_Master.Init.NbrOfDiscConversion      = 1;                             /* Parameter discarded because sequencer is disabled */
#if ADC_DUAL_TRIGGER_FROM_TIMER == 1
	ADC_Handler_Master.Init.ExternalTrigConv         = ADC_EXTERNALTRIG_T3_TRGO;        /* Timer 3 external event triggering the conversion */
	ADC_Handler_Master.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_RISING;
#elseif ADC_DUAL_TRIGGER_FROM_TIMER == 0

	ADC_Handler_Master.Init.ExternalTrigConv         = ADC_SOFTWARE_START;              /* Software start to trigger the 1st conversion manually, without external event */
	ADC_Handler_Master.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;   /* Parameter discarded because trigger of conversion by software start (no external event) */
#endif	/*ADC_DUAL_TRIGGER_FROM_TIMER*/
	ADC_Handler_Master.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR; /* ADC DMA circular requested */
	ADC_Handler_Master.Init.Overrun                  = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
	ADC_Handler_Master.Init.OversamplingMode         = DISABLE;                       /* No oversampling */
	ADC_Handler_Master.Init.BoostMode                = ENABLE;                        /* Enable Boost mode as ADC clock frequency is bigger than 20 MHz */
	if (HAL_ADC_Init(&ADC_Handler_Master) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Handler_Slave.Instance = ADCy;
	if (HAL_ADC_DeInit(&ADC_Handler_Slave) != HAL_OK)
	{
		Error_Handler();
	}
	ADC_Handler_Slave.Init = ADC_Handler_Master.Init;									/* Same configuration as ADC master, with continuous mode and external      */
	ADC_Handler_Slave.Init.ContinuousConvMode    = DISABLE;							/* trigger disabled since ADC master is triggering the ADC slave            */
	ADC_Handler_Slave.Init.ExternalTrigConv      = ADC_SOFTWARE_START;				/* conversions                                                              */

	if (HAL_ADC_Init(&ADC_Handler_Slave) != HAL_OK)
	{
		Error_Handler();
	}	
	
	/* Configuration of channel on ADC (master) regular group on sequencer rank 1 */
	/* Note: Considering IT occurring after each number of                      */
	/*       "ADCCONVERTEDVALUES_BUFFER_SIZE" ADC conversions (IT by DMA end    */
	/*       of transfer), select sampling time and ADC clock with sufficient   */
	/*       duration to not create an overhead situation in IRQHandler.        */
	sConfig.Channel      = Channelx;                /* Sampled channel number */
	sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;    /* Minimum sampling time */
	sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
	sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */ 
	sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */

	if (HAL_ADC_ConfigChannel(&ADC_Handler_Master, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/* Configuration of channel on ADC (slave) regular group on sequencer rank 1 */
	/* Same channel as ADCx for dual mode interleaved: both ADC are converting  */
	/* the same channel.                                                        */
	sConfig.Channel = Channely;

	if (HAL_ADC_ConfigChannel(&ADC_Handler_Slave, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
  
	if (HAL_ADCEx_Calibration_Start(&ADC_Handler_Master, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_ADCEx_Calibration_Start(&ADC_Handler_Slave, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}

	/* Configuration of multimode */
	/* Multimode parameters settings and set ADCy (slave) under control of      */
	/* ADCx (master).                                                           */
	MultiModeInit.Mode = ADC_DUALMODE_INTERL;
	MultiModeInit.DualModeData = ADC_DUALMODEDATAFORMAT_32_10_BITS;  			/* ADC and DMA configured in resolution 32 bits to match with both ADC master and slave resolution */
	MultiModeInit.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;

	if (HAL_ADCEx_MultiModeConfigChannel(&ADC_Handler_Master, &MultiModeInit) != HAL_OK)
	{
		Error_Handler();
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
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @note   When ADC_TRIGGER_FROM_TIMER is disabled, conversions are software-triggered
  *         and are too fast for DMA post-processing. Therefore, to reduce the computational 
  *         load, the output buffer filled up by the DMA is post-processed only when 
  *         ADC_TRIGGER_FROM_TIMER is enabled.
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
#if ADC_Single_DMA1_Enable == 1
	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */ 
	SCB_InvalidateDCache_by_Addr((uint32_t *) &aADCDualConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE/2], 4*ADCCONVERTEDVALUES_BUFFER_SIZE/2);
#endif
#if ADC_DUAL_TRIGGER_FROM_TIMER == 1
	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */ 
	SCB_InvalidateDCache_by_Addr((uint32_t *) &aADCDualConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE/2], 4*ADCCONVERTEDVALUES_BUFFER_SIZE/2);
	uint32_t tmp_index = 0;
	for (tmp_index = (ADCCONVERTEDVALUES_BUFFER_SIZE/2); tmp_index < ADCCONVERTEDVALUES_BUFFER_SIZE; tmp_index++)		/* Dispatch dual conversion values into 2 arrays */
	{																													/* corresponding to each ADC conversion values. */
		aADCxConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(aADCDualConvertedValues[tmp_index]);
		aADCyConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(aADCDualConvertedValues[tmp_index]);
	}
#endif /* ADC_DUAL_TRIGGER_FROM_TIMER */
	ubADCConversionComplete = TRUE;		/* Set variable to report DMA transfer status to main program */
}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @note   When ADC_TRIGGER_FROM_TIMER is disabled, conversions are software-triggered
  *         and are too fast for DMA post-processing. Therefore, to reduce the computational 
  *         load, the output buffer filled up by the DMA is post-processed only when 
  *         ADC_TRIGGER_FROM_TIMER is enabled.
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
#if ADC_Single_DMA1_Enable == 1
	/* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes */ 
	SCB_InvalidateDCache_by_Addr((uint32_t *) &aADCDualConvertedValues[0], 4*ADCCONVERTEDVALUES_BUFFER_SIZE/2);
#endif
#if ADC_DUAL_TRIGGER_FROM_TIMER == 1
	/* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes */ 
	SCB_InvalidateDCache_by_Addr((uint32_t *) &aADCDualConvertedValues[0], 4*ADCCONVERTEDVALUES_BUFFER_SIZE/2);
	uint32_t tmp_index = 0;
	for (tmp_index = 0; tmp_index < (ADCCONVERTEDVALUES_BUFFER_SIZE/2); tmp_index++)	/* Dispatch dual conversion values into 2 arrays */
	{																													/* corresponding to each ADC conversion values. */
		aADCxConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(aADCDualConvertedValues[tmp_index]);
		aADCyConvertedValues[tmp_index] = (uint16_t) COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(aADCDualConvertedValues[tmp_index]);
	}
#endif /* ADC_DUAL_TRIGGER_FROM_TIMER */
	ubADCConversionComplete = FALSE;		/* Reset variable to report DMA transfer status to main program */
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
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
#if ADC_DUAL_TRIGGER_FROM_TIMER == 0
	if (HAL_IS_BIT_CLR(hadc->ErrorCode, HAL_ADC_ERROR_OVR)) 	/* In case of ADC error, call main error handler */
	{
#endif /* ADC_DUAL_TRIGGER_FROM_TIMER */
	Error_Handler();
#if ADC_DUAL_TRIGGER_FROM_TIMER == 0
	}
#endif /* ADC_DUAL_TRIGGER_FROM_TIMER */
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
