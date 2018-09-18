/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/ADC/Adc_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 06-September-2018
  * @brief   The ADC of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __ADC_BSP_H
#define __ADC_BSP_H

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
  * @brief  Size of array aADCxConvertedData[] 
  */
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  32)

/**
  * @brief  The resolution of the ADC12 = 2 ^ Resolution (in the Bsp_Init_ADC())
  */
#define RANGE_8BITS                    ((uint32_t)  255)    /* Max digital value with a full range of 8 bits */
#define RANGE_12BITS                   ((uint32_t) 4095)    /* Max digital value with a full range of 12 bits */
#define RANGE_16BITS                   ((uint32_t)65535)    /* Max digital value with a full range of 16 bits */

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
extern ADC_HandleTypeDef    	ADC1_Handler;
  
/**
  * @brief The ADC1 Converted value array
  */ 
extern __IO uint32_t ADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];

/**
  * @brief Variable about the result of Conversion.
  */
extern uint8_t ADC_Conversion_Complete;


/**
  * @brief Variable about the status of nalogWatchdog.
  */
extern uint8_t ADC_AnalogWatchdogStatus;

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
void Bsp_Init_ADC3_SINGLE_CHANNEL(uint32_t Channelx);
void Bsp_Init_ADC12_MULTI_CHANNEL(void);

/** @}
*/
/*********************ADC Exported Functions Group1***************************/
/** @defgroup ADC_Exported_Functions_Group2 Operation Functions
  * @{
  */
  
uint32_t Bsp_Get_Adc_Average(ADC_HandleTypeDef ADCx_Handler,uint8_t times);
uint32_t Bsp_Get_CPU_Intern_Temperature(void);
uint32_t Bsp_Get_CPU_Intern_VerfInt(void);
uint32_t Bsp_Get_Board_VBAT(void);

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

#endif /* __ADC_BSP_H */

/**********************************************END OF FILE******************************************/
