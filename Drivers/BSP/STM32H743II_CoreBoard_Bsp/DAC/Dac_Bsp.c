/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/DAC/Dac_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    14-July-2018
  * @brief   This file provides set of firmware functions to manage:
  *			 More about the  LFSR please see: https://blog.csdn.net/ACdreamers/article/details/44656743
  * 		 TIM6用于触发PA4 DAC_CHANNEL1的输出，每TIM6CLK/(TIM_Period + 1)/(TIM_Prescaler + 1)= TIM6CLK / 256更新一次
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The DAC_LFSR_NOISE canbe used as follows:
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
/** @defgroup DAC_LFSR_NOISE DAC_LFSR_NOISE
  * @brief DAC_LFSR_NOISE
	* @note	DAC_LFSR_NOISE
  * @{
  */

/* Private variables -------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Private_c DAC_LFSR_NOISE Private variables
  * @{
  */
  
/**
  * @brief The DAC handler
  */  
DAC_HandleTypeDef    	DAC_Handler;
/**
  * @brief The DAC Channel Configurtion
  */  
static DAC_ChannelConfTypeDef 	sConfig;
/**
  * @brief Used for DAC WAVE TEST
  */ 
uint8_t iTestDAC = 0;

/**
  * @brief Unmask DAC channel LFSR for noise wave generation
  */
uint32_t DAC_LFSRUnmaskTbl[12] = 
{
    DAC_LFSRUNMASK_BIT0,           
    DAC_LFSRUNMASK_BITS1_0 ,           
    DAC_LFSRUNMASK_BITS2_0 ,           
    DAC_LFSRUNMASK_BITS3_0 ,           
    DAC_LFSRUNMASK_BITS4_0 ,          
    DAC_LFSRUNMASK_BITS5_0 ,           
    DAC_LFSRUNMASK_BITS6_0 ,            
    DAC_LFSRUNMASK_BITS7_0 ,          
    DAC_LFSRUNMASK_BITS8_0 ,           
    DAC_LFSRUNMASK_BITS9_0 ,          
    DAC_LFSRUNMASK_BITS10_0 ,        
    DAC_LFSRUNMASK_BITS11_0 
};

/**
  * @brief Unmask DAC channel LFSR for triangle wave generation
  */
uint32_t DAC_TriangleAmpTbl[12] = 
{
    DAC_TRIANGLEAMPLITUDE_1,           
    DAC_TRIANGLEAMPLITUDE_3,           
    DAC_TRIANGLEAMPLITUDE_7,           
    DAC_TRIANGLEAMPLITUDE_15,           
    DAC_TRIANGLEAMPLITUDE_31,          
    DAC_TRIANGLEAMPLITUDE_63,           
    DAC_TRIANGLEAMPLITUDE_127,            
    DAC_TRIANGLEAMPLITUDE_255,          
    DAC_TRIANGLEAMPLITUDE_511,           
    DAC_TRIANGLEAMPLITUDE_1023,          
    DAC_TRIANGLEAMPLITUDE_2047,        
    DAC_TRIANGLEAMPLITUDE_4095 
};

/**
  * @brief The value for the Escalator
  */
const uint8_t aEscalator12bit[32] = {
									7, 15, 23, 31, 39, 47, 55, 63, 71, 79, 
									87, 95, 103, 111, 119, 127, 135, 143, 151, 
									159, 167, 175, 183, 191, 199, 207, 215, 223, 231, 239, 247, 255};

/**
  * @brief The value for the SIN
  */
const uint16_t Sine12bit[32] = {
								2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
								3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 
								599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

/** @}
*/		
/*----------------------------DAC_LFSR_NOISE Private variables--------------------------------*/ 
/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Exported_Functions DAC_LFSR_NOISE Exported Functions
  * @{
  */
/** @defgroup DAC_LFSR_NOISE_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize DAC_LFSR_NOISE
  @endverbatim
  * @{
  */
/**
  * @brief Initialize the DAC_LFSR_NOISE according to the specified
  * @param DAC_LFSRUNMASK_BITS: DAC channel LFSR for noise wave generation
  *        This parameter can be one of the following values: 
  *        @arg DAC_LFSRUNMASK_BIT0: Unmask DAC channel LFSR bit0 for noise wave generation
  *        @arg DAC_LFSRUNMASK_BITS1_0: Unmask DAC channel LFSR bit[1:0] for noise wave generation  
  *        @arg DAC_LFSRUNMASK_BITS2_0: Unmask DAC channel LFSR bit[2:0] for noise wave generation
  *        @arg DAC_LFSRUNMASK_BITS3_0: Unmask DAC channel LFSR bit[3:0] for noise wave generation 
  *        @arg DAC_LFSRUNMASK_BITS4_0: Unmask DAC channel LFSR bit[4:0] for noise wave generation 
  *        @arg DAC_LFSRUNMASK_BITS5_0: Unmask DAC channel LFSR bit[5:0] for noise wave generation 
  *        @arg DAC_LFSRUNMASK_BITS6_0: Unmask DAC channel LFSR bit[6:0] for noise wave generation 
  *        @arg DAC_LFSRUNMASK_BITS7_0: Unmask DAC channel LFSR bit[7:0] for noise wave generation 
  *        @arg DAC_LFSRUNMASK_BITS8_0: Unmask DAC channel LFSR bit[8:0] for noise wave generation 
  *        @arg DAC_LFSRUNMASK_BITS9_0: Unmask DAC channel LFSR bit[9:0] for noise wave generation 
  *        @arg DAC_LFSRUNMASK_BITS10_0: Unmask DAC channel LFSR bit[10:0] for noise wave generation 
  *        @arg DAC_LFSRUNMASK_BITS11_0: Unmask DAC channel LFSR bit[11:0] for noise wave generation 
  */

void Bsp_InitNoiseDAC(uint32_t DAC_LFSRUNMASK_BITS)
{
	Bsp_DAC_Ch1_NoiseConfig(DAC_LFSRUNMASK_BITS);
}

/**
  * @brief Initialize the TRIANGLE according to the specified
  * @param DAC_LFSRUNMASK_BITS: DAC channel LFSR for noise wave generation
  *        This parameter can be one of the following values:
  *        @arg DAC_TRIANGLEAMPLITUDE_1: Select max triangle amplitude of 1
  *        @arg DAC_TRIANGLEAMPLITUDE_3: Select max triangle amplitude of 3
  *        @arg DAC_TRIANGLEAMPLITUDE_7: Select max triangle amplitude of 7
  *        @arg DAC_TRIANGLEAMPLITUDE_15: Select max triangle amplitude of 15
  *        @arg DAC_TRIANGLEAMPLITUDE_31: Select max triangle amplitude of 31
  *        @arg DAC_TRIANGLEAMPLITUDE_63: Select max triangle amplitude of 63
  *        @arg DAC_TRIANGLEAMPLITUDE_127: Select max triangle amplitude of 127
  *        @arg DAC_TRIANGLEAMPLITUDE_255: Select max triangle amplitude of 255
  *        @arg DAC_TRIANGLEAMPLITUDE_511: Select max triangle amplitude of 511
  *        @arg DAC_TRIANGLEAMPLITUDE_1023: Select max triangle amplitude of 1023
  *        @arg DAC_TRIANGLEAMPLITUDE_2047: Select max triangle amplitude of 2047
  *        @arg DAC_TRIANGLEAMPLITUDE_4095: Select max triangle amplitude of 4095                               
  */
void Bsp_InitTriangleDAC(uint32_t DAC_TRIANGLEAMPLITUDE)
{
	Bsp_DAC_Ch1_TriangleConfig(DAC_TRIANGLEAMPLITUDE);
}

/**
  * @brief Initialize the ESCALATOR according to the specified                             
  */
void Bsp_InitEscalatorDAC(void)
{
	Bsp_DAC_Ch1_EscalatorConfig();
}

/**
  * @brief Initialize the SIN wave according to the specified                             
  */
void Bsp_InitSinWaveDAC(void)
{
	Bsp_DAC_Ch1_SinWaveConfig();
}

/**
  * @brief Initialize the SIN wave according to the specified                             
  */
void Bsp_InitDualWaveDAC(uint32_t DAC_TRIANGLEAMPLITUDE, uint32_t DAC_LFSRUNMASK_BITS)
{
	Bsp_InitHardTimer_HRTIM();
	Bsp_Ch12_DualWaveConfig(DAC_TRIANGLEAMPLITUDE, DAC_LFSRUNMASK_BITS);
}

/**
  * @brief DAC MSP Initialization,The DMA is used for generate the Sin wave and the Escalator
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdac: DAC handle pointer
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef *DACx_Handler)
{

	DMA_HandleTypeDef  		DMA_DAC1_Handler;
	GPIO_InitTypeDef   		GPIO_InitStruct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();		  	/* Enable GPIO clock */
	__HAL_RCC_DAC12_CLK_ENABLE();		 	/* DAC Periph clock enable */
	
	GPIO_InitStruct.Pin 	= GPIO_PIN_4 | GPIO_PIN_5;		/* DAC Channel1 GPIO pin configuration */
	GPIO_InitStruct.Mode 	= GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
#if SINWAVE_GEN_FOR_TEST_ENABLE == 1	
	__HAL_RCC_DMA1_CLK_ENABLE();		  	/* DMA1 clock enable */
	DMA_DAC1_Handler.Instance 					= DMA1_Stream5;
	DMA_DAC1_Handler.Init.Request  				= DMA_REQUEST_DAC1;
	DMA_DAC1_Handler.Init.Direction			 	= DMA_MEMORY_TO_PERIPH;
	DMA_DAC1_Handler.Init.PeriphInc 			= DMA_PINC_DISABLE;
	DMA_DAC1_Handler.Init.MemInc 				= DMA_MINC_ENABLE;
	DMA_DAC1_Handler.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_HALFWORD;/* Transfer to DAC by byte to match with DAC configuration: DAC resolution 32 bits */
	DMA_DAC1_Handler.Init.MemDataAlignment 		= DMA_MDATAALIGN_HALFWORD;/* Transfer to DAC by byte to match with DAC configuration: DAC resolution 32 bits */
	DMA_DAC1_Handler.Init.Mode 					= DMA_CIRCULAR;
	DMA_DAC1_Handler.Init.Priority 				= DMA_PRIORITY_HIGH;

	HAL_DMA_Init(&DMA_DAC1_Handler);

	__HAL_LINKDMA(DACx_Handler, DMA_Handle1, DMA_DAC1_Handler);			/* Associate the initialized DMA handle to the DAC handle */

	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 2, 4);						//设置DMA_Stream5用与DAC中断优先级，抢占优先级2，子优先级4。ADC > DMA > DAC
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 3, 0);							//设置TIM6用于DAC中断优先级，抢占优先级3，子优先级0，ADC > DMA > DAC
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
#endif	/*SINWAVE_GEN_FOR_TEST_ENABLE*/

}

/**
  * @brief  DeInitializes the DAC MSP.
  * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *DACx_Handler)
{
	__HAL_RCC_DAC12_FORCE_RESET();		  /* Enable DAC reset state */
	__HAL_RCC_DAC12_RELEASE_RESET();	  /* Release DAC from reset state */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);	

#if SINWAVE_GEN_FOR_TEST_ENABLE == 1	
	HAL_DMA_DeInit(DACx_Handler->DMA_Handle1);
	HAL_DMA_DeInit(DACx_Handler->DMA_Handle2);
	
    HAL_NVIC_DisableIRQ(DMA1_Stream5_IRQn);
#endif	/*SINWAVE_GEN_FOR_TEST_ENABLE*/
}

/** @}
*/
/*********************DAC_LFSR_NOISE Exported Functions Group1**************************/

/** @defgroup DAC_LFSR_NOISE_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the DAC_LFSR_NOISE
  @endverbatim
  * @{
  */
/**
  * @brief  DAC Channel1 Noise Configuration
  */
void Bsp_DAC_Ch1_NoiseConfig(uint32_t DAC_LFSRUNMASK_BITS)
{
	DAC_Handler.Instance = DAC1;
	
	if (HAL_DAC_DeInit(&DAC_Handler) != HAL_OK)	  				/*DeInit the DAC peripheral*/
	{
		Error_Handler();
	}  
	if (HAL_DAC_Init(&DAC_Handler) != HAL_OK)		  			/*Configure the DAC peripheral*/
	{
		Error_Handler();
	}
	
	sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;		  			/*##-2- Configure DAC channel1*/
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	
	if (HAL_DAC_ConfigChannel(&DAC_Handler, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_DACEx_NoiseWaveGenerate(&DAC_Handler, DAC_CHANNEL_1, DAC_LFSRUNMASK_BITS) != HAL_OK)
	{
		Error_Handler();										/* Triangle wave generation Error */
	}
	
	if (HAL_DAC_Start(&DAC_Handler, DAC_CHANNEL_1) != HAL_OK)	/*Enable DAC Channel1*/
	{
		Error_Handler();
	}	
	
	if (HAL_DAC_SetValue(&DAC_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0) != HAL_OK)		/*Set DAC Channel1 DHR register */
	{
		Error_Handler();
	}

}

/**
  * @brief  DAC Channel1 Triangle Configuration
  */
void Bsp_DAC_Ch1_TriangleConfig(uint32_t DAC_TRIANGLEAMPLITUDE)
{
	DAC_Handler.Instance = DAC1; 
	
	if (HAL_DAC_Init(&DAC_Handler) != HAL_OK)		  			/*Configure the DAC peripheral*/
	{
		Error_Handler();
	}
	
	sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;		  			/*##-2- Configure DAC channel1*/
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	
	if (HAL_DAC_ConfigChannel(&DAC_Handler, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_DACEx_TriangleWaveGenerate(&DAC_Handler, DAC_CHANNEL_1, DAC_TRIANGLEAMPLITUDE) != HAL_OK)
	{
		Error_Handler();										/* Triangle wave generation Error */
	}	

	if (HAL_DAC_Start(&DAC_Handler, DAC_CHANNEL_1) != HAL_OK)	/*Enable DAC Channel1*/
	{
		Error_Handler();
	}
	
	if (HAL_DAC_SetValue(&DAC_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x100) != HAL_OK)		/*Set DAC Channel1 DHR register */
	{
		Error_Handler();
	}
}

/**
  * @brief  DAC Channel1 Escalator Configuration
  */
void Bsp_DAC_Ch1_EscalatorConfig(void)
{
	DAC_Handler.Instance = DAC1;
	
	if (HAL_DAC_DeInit(&DAC_Handler) != HAL_OK)	  				/*DeInit the DAC peripheral*/
	{
		Error_Handler();
	}  
	if (HAL_DAC_Init(&DAC_Handler) != HAL_OK)		  			/*Configure the DAC peripheral*/
	{
		Error_Handler();
	}
	sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;		  			/*##-2- Configure DAC channel1*/
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	
	if (HAL_DAC_ConfigChannel(&DAC_Handler, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_DAC_Start_DMA(&DAC_Handler, DAC_CHANNEL_1, (uint32_t *)aEscalator12bit, 6, DAC_ALIGN_8B_R) != HAL_OK)
	{
		Error_Handler();		    /* Start DMA Error */
	}
}

/**
  * @brief  DAC Channel1 SIN wave Configuration
  */
void Bsp_DAC_Ch1_SinWaveConfig(void)
{
	DAC_Handler.Instance = DAC1;
	
	if (HAL_DAC_DeInit(&DAC_Handler) != HAL_OK)	  				/*DeInit the DAC peripheral*/
	{
		Error_Handler();
	}  
	if (HAL_DAC_Init(&DAC_Handler) != HAL_OK)		  			/*Configure the DAC peripheral*/
	{
		Error_Handler();
	}
	sConfig.DAC_Trigger		 = DAC_TRIGGER_T6_TRGO;		  			/*##-2- Configure DAC channel1*/
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
	sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
	
	if (HAL_DAC_ConfigChannel(&DAC_Handler, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_DAC_Start_DMA(&DAC_Handler, DAC_CHANNEL_1, (uint32_t *)Sine12bit, 32, DAC_ALIGN_12B_R) != HAL_OK)
	{
		Error_Handler();		    /* Start DMA Error */
	}
}

/**
  * @brief  For this example, generate a waveform voltage on a spare DAC
  *         channel, so user has just to connect a wire between DAC channel 
  *         (pin PA.04) and ADC channel (pin PA.04) to run this example.
  *         (this prevents the user from resorting to an external signal generator)
  *         This function configures the DAC and generates a constant voltage of Vdda/2.
  *         To modify the voltage level, use function "WaveformVoltageGenerationForTest_Update"
  */
void Bsp_WaveformVoltageGenerationForTest_Config(uint32_t Resulotion)
{

	DAC_Handler.Instance = DAC1;
	if (HAL_DAC_Init(&DAC_Handler) != HAL_OK)
	{
		Error_Handler();
	}

	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

	if (HAL_DAC_ConfigChannel(&DAC_Handler, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
  
	if (HAL_DAC_SetValue(&DAC_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Resulotion) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_DAC_Start(&DAC_Handler, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief  DAC Channel12 Dual wave Configuration
  */
void Bsp_Ch12_DualWaveConfig(uint32_t DAC_TRIANGLEAMPLITUDE, uint32_t DAC_LFSRUNMASK_BITS)
{
	DAC_Handler.Instance = DAC1;
	
	if (HAL_DAC_DeInit(&DAC_Handler) != HAL_OK)	  				/*DeInit the DAC peripheral*/
	{
		Error_Handler();
	}  
	if (HAL_DAC_Init(&DAC_Handler) != HAL_OK)		  			/*Configure the DAC peripheral*/
	{
		Error_Handler();
	}
	sConfig.DAC_Trigger = DAC_TRIGGER_HR1_TRGO1;		  		/*Configure DAC channel1*/
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	
	if (HAL_DAC_ConfigChannel(&DAC_Handler, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_DAC_ConfigChannel(&DAC_Handler, &sConfig, DAC_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_DAC_Start(&DAC_Handler, DAC_CHANNEL_1) != HAL_OK)	/*Enable DAC Channel1*/
	{
		Error_Handler();
	}
	if (HAL_DAC_Start(&DAC_Handler, DAC_CHANNEL_2) != HAL_OK)	/*Enable DAC Channel2*/
	{
		Error_Handler();
	}
	if (HAL_DACEx_DualSetValue(&DAC_Handler, DAC_ALIGN_12B_R, 0xFF, 0xFF) != HAL_OK)
	{
		Error_Handler();										
	}
    if (HAL_DACEx_TriangleWaveGenerate(&DAC_Handler,DAC_CHANNEL_1,DAC_TRIANGLEAMPLITUDE) != HAL_OK)
    {
          Error_Handler();
    }
	if (HAL_DACEx_NoiseWaveGenerate(&DAC_Handler, DAC_CHANNEL_2, DAC_LFSRUNMASK_BITS) != HAL_OK)
	{
		Error_Handler();									
	}	
}


/**
* @brief  This function handles DMA interrupt request. used for Escalator wave
*/
void DMA1_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(DAC_Handler.DMA_Handle1);
#if SINWAVE_GEN_FOR_TEST_DEBUG == 1
	Bsp_Printf("DMA_Stream5 sendout the data of wave! \r\n");
#endif	/*SINWAVE_GEN_FOR_TEST_DEBUG*/
}

/** @}
*/
/****************************DAC_LFSR_NOISE Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------DAC_LFSR_NOISE Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------DAC_LFSR_NOISE-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
