/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/CRC/Crc_Bsp.c
  * @author  CME
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
	The CRC canbe used as follows:
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
  
/** @defgroup CRC CRC
  * @brief CRC
  * @{
  */

/* Private Exported constants -------------------------------------------------------------------*/
/** @defgroup CRC_Private_Exported constants CRC Private Exported constants
  * @{
  */
/**
  * @brief		CRC Private Handle
  */
CRC_HandleTypeDef   CRC_Handler;
 
/**
  * @brief		Storing the Default computed CRC value
  */
__IO uint32_t DefaultCRCValue = 0;
 
/**
  * @brief		Storing the Accumulate computed CRC value
  */
__IO uint32_t AccumulateCRCValue = 0;

/**
  * @brief		Expected CRC Value
  */
uint32_t uwExpectedCRCValue = 0x379E9F06;

/**
  * @brief		Data need to be compute CRC
  */
static const uint32_t aDataBuffer[DATA_BUFFER_SIZE] =
{
  0x00001021, 0x20423063, 0x408450a5, 0x60c670e7, 0x9129a14a, 0xb16bc18c,
  0xd1ade1ce, 0xf1ef1231, 0x32732252, 0x52b54294, 0x72f762d6, 0x93398318,
  0xa35ad3bd, 0xc39cf3ff, 0xe3de2462, 0x34430420, 0x64e674c7, 0x44a45485,
  0xa56ab54b, 0x85289509, 0xf5cfc5ac, 0xd58d3653, 0x26721611, 0x063076d7,
  0x569546b4, 0xb75ba77a, 0x97198738, 0xf7dfe7fe, 0xc7bc48c4, 0x58e56886,
  0x78a70840, 0x18612802, 0xc9ccd9ed, 0xe98ef9af, 0x89489969, 0xa90ab92b,
  0x4ad47ab7, 0x6a961a71, 0x0a503a33, 0x2a12dbfd, 0xfbbfeb9e, 0x9b798b58,
  0xbb3bab1a, 0x6ca67c87, 0x5cc52c22, 0x3c030c60, 0x1c41edae, 0xfd8fcdec,
  0xad2abd0b, 0x8d689d49, 0x7e976eb6, 0x5ed54ef4, 0x2e321e51, 0x0e70ff9f,
  0xefbedfdd, 0xcffcbf1b, 0x9f598f78, 0x918881a9, 0xb1caa1eb, 0xd10cc12d,
  0xe16f1080, 0x00a130c2, 0x20e35004, 0x40257046, 0x83b99398, 0xa3fbb3da,
  0xc33dd31c, 0xe37ff35e, 0x129022f3, 0x32d24235, 0x52146277, 0x7256b5ea,
  0x95a88589, 0xf56ee54f, 0xd52cc50d, 0x34e224c3, 0x04817466, 0x64475424,
  0x4405a7db, 0xb7fa8799, 0xe75ff77e, 0xc71dd73c, 0x26d336f2, 0x069116b0,
  0x76764615, 0x5634d94c, 0xc96df90e, 0xe92f99c8, 0xb98aa9ab, 0x58444865,
  0x78066827, 0x18c008e1, 0x28a3cb7d, 0xdb5ceb3f, 0xfb1e8bf9, 0x9bd8abbb,
  0x4a755a54, 0x6a377a16, 0x0af11ad0, 0x2ab33a92, 0xed0fdd6c, 0xcd4dbdaa,
  0xad8b9de8, 0x8dc97c26, 0x5c644c45, 0x3ca22c83, 0x1ce00cc1, 0xef1fff3e,
  0xdf7caf9b, 0xbfba8fd9, 0x9ff86e17, 0x7e364e55, 0x2e933eb2, 0x0ed11ef0
};

/** @}
*/		
/*----------------------------CRC Private Exported constants--------------------------------*/ 
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup CRC_Exported_Functions CRC Exported Functions
  * @{
  */
/** @defgroup CRC_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize CRC
  @endverbatim
  * @{
  */
/**
  * @brief Initialize the CRC as default polynomial 0x4C11DB7, default init value
  */
void Bsp_InitDefautCRC(void)
{
	CRC_Handler.Instance = CRC;
	CRC_Handler.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;				/* The c is used */
	CRC_Handler.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;				/* The default init value is used */
	CRC_Handler.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;			/* The input data are not inverted */
	CRC_Handler.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;		/* The output data are not inverted */
	CRC_Handler.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;				/* The input data are 32-bit long words */
	if (HAL_CRC_Init(&CRC_Handler) != HAL_OK)
	{
		Error_Handler();		/* Initialization Error */
	}
}

/**
* @brief Initialize the CRC as the polynomial is set manually to 0x9B that is X^8 + X^7 + X^4 + X^3 + X + 1, default init value
  */
void Bsp_InitUserDefineCRC(void)
{
	CRC_Handler.Instance = CRC;
	CRC_Handler.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;				/* The default polynomial is not used. 
																						It is required to defined it in CrcHandle.Init.GeneratingPolynomial */
	CRC_Handler.Init.GeneratingPolynomial    = CRC_POLYNOMIAL_USER_DEFINE;				/* Set the value of the polynomial */
	CRC_Handler.Init.CRCLength               = CRC_POLYLENGTH_8B;						/* The user-defined generating polynomial generates a 8-bit long CRC */
	CRC_Handler.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;				/* The default init value is used */
	CRC_Handler.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;			/* The input data are not inverted */
	CRC_Handler.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;		/* The output data are not inverted */
	CRC_Handler.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;				/* The input data are 32-bit long words */
	if (HAL_CRC_Init(&CRC_Handler) != HAL_OK)
	{
		Error_Handler();		/* Initialization Error */
	}
}

/**
  * @brief CRC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripherals clock enable
  * @param hcrc: CRC handle pointer
  */
void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
  /* CRC Peripheral clock enable */
  __HAL_RCC_CRC_CLK_ENABLE();
}

/**
  * @brief CRC MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripherals clock
  * @param hcrc: CRC handle pointer
  */
void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc)
{
  /* Enable CRC reset state */
  __HAL_RCC_CRC_FORCE_RESET();

  /* Release CRC from reset state */
  __HAL_RCC_CRC_RELEASE_RESET();
}

/** @}
*/
/*********************CRC Exported Functions Group1**************************/

/** @defgroup CRC_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the CRC
  @endverbatim
  * @{
  */
/**
  * @brief 	Computed The CRC and stored to DefaultCRCValue variable
  *			The CRC calculator is re-initialized with the default polynomial 0x4C11DB7
  *			4 AHB clock cycles for 32-bit
  *			2 AHB clock cycles for 16-bit
  *			1 AHB clock cycles for 8-bit
  */
void Bsp_ComputeCRCDefault(void)
{
	DefaultCRCValue = HAL_CRC_Calculate(&CRC_Handler, (uint32_t *)aDataBuffer, DATA_BUFFER_SIZE);
#if CRC_DEBUG == 1
	Bsp_Printf("Computed The CRC by re-initialized with the default polynomial 0x4C11DB7! CRC = %X \r\n", DefaultCRCValue);
#endif
}

/**
  * @brief 	Computed The CRC and stored to AccumulateCRCValue variable
  *			The CRC calculator is not re-initialized, instead the previously computed CRC is used as initial value.  
  */
void Bsp_ComputeCRCAccumulate(void)
{
	AccumulateCRCValue = HAL_CRC_Accumulate(&CRC_Handler, (uint32_t *)&aDataBuffer, DATA_BUFFER_SIZE);
#if CRC_DEBUG == 1
	Bsp_Printf("Computed The CRC by the previously computed CRC! CRC = %X \r\n", AccumulateCRCValue);
#endif
}

/** @}
*/
/****************************CRC Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------CRC Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------CRC-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
