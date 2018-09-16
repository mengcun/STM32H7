/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/ADC/Adc_DualMode_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 06-September-2018
  * @brief   The ADC of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __ADC_DUALMODE_BSP_H
#define __ADC_DUALMODE_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h7xx_hal.h"
/*******************************************APP/BSP*************************************************/
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup ADC
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup ADC_Private define ADC Private define
  * @{
  */
  
/**
  * @brief   Size of array containing ADC converted values 
  */
#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)  256) 

/**
  * @brief  The resolution of the ADC12
  */
#define VDD_APPLI                      ((uint32_t) 3300)    /* Value of analog voltage supply Vdda (unit: mV) */
#define RANGE_8BITS                    ((uint32_t)  255)    /* Max digital value with a full range of 8 bits */
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max digital value with a full range of 12 bits */
#define RANGE_16BITS                   ((uint32_t)65535)    /* Max digital value with a full range of 16 bits */

/**
  * @brief  Computation of ADC master conversion result from ADC dual mode conversion result (ADC master and ADC slave
  *         results concatenated on data register of ADC master).
  * @param  DATA: ADC dual mode conversion result
  */
#define COMPUTATION_DUALMODEINTERLEAVED_ADCMASTER_RESULT(DATA)	((DATA) & 0x0000FFFF)

/**
  * @brief  Computation of ADC slave conversion result from ADC dual mode conversion result (ADC master and ADC slave
  *         results concatenated on data register of ADC master).
  * @param  DATA: ADC dual mode conversion result
  */
#define COMPUTATION_DUALMODEINTERLEAVED_ADCSLAVE_RESULT(DATA)	((DATA) >> 16)

/** @}
*/
/*----------------------------------------ADC Private define---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Private_c DAC_LFSR_NOISE Private variables
  * @{
  */
/**
  * @brief The Handler for ADC1
  */  
extern ADC_HandleTypeDef    	ADC_Handler_Master;
  
/**
  * @brief The Handler for ADC1
  */  
extern ADC_HandleTypeDef    	ADC_Handler_Slave;
  
/**
  * @brief The ADC1 Converted value array
  */ 
extern ALIGN_32BYTES(__IO uint32_t   	aADCDualConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE]);   	/* ADC dual mode interleaved conversion results (ADC master and ADC slave results concatenated on data register 32 bits of ADC master). */
extern ALIGN_32BYTES(__IO uint16_t   	aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE]);       	/* Dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
extern ALIGN_32BYTES(__IO uint16_t   	aADCyConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE]);       	/* Dispatch dual conversion values into arrays corresponding to each ADC conversion values. */
extern uint8_t         					ubADCConversionComplete;                        			/* Set into ADC conversion complete callback */

/** @}
*/		
/*----------------------------ADC Private variables--------------------------------*/ 
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */
/** @defgroup ADC_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
void Bsp_Init_DualModeADC(ADC_TypeDef *ADCx, uint32_t Channelx, ADC_TypeDef *ADCy, uint32_t Channely);

/** @}
*/
/*********************ADC Exported Functions Group1***************************/
/** @defgroup ADC_Exported_Functions_Group2 Operation Functions
  * @{
  */
  
/** @}
*/
/**********************ADC Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------ADC Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------ADC------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif	/*ADC_DualMode_Interleaved*/

/**********************************************END OF FILE******************************************/
