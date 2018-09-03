/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/LED/Led_Bsp.h
  * @author  CME
  * @version V1.0.0
  * @date  	 15-Junly-2018
  * @brief   The template of header file.
  *					 Please copy this file and rename the file name if needed.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __LED_BSP_H
#define __LED_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h7xx_hal.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup LED_BSP
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup LED_BSP_Private_Define LED_BSP Private Define
  * @{
  */
/**
  * @brief  CoreBoard LED LOW LEVEL DEFINES  
  */
#define LED1_Blue_GPIO_PORT                   GPIOC
#define LED1_Blue_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED1_Blue_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()
#define LED1_Blue_PIN                         GPIO_PIN_1

#define LED1_Green_GPIO_PORT                   GPIOH
#define LED1_Green_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOH_CLK_ENABLE()
#define LED1_Green_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOH_CLK_DISABLE()
#define LED1_Green_PIN                         GPIO_PIN_2

#define LED1_Red_GPIO_PORT                   	GPIOH
#define LED1_Red_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOH_CLK_ENABLE()
#define LED1_Red_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOH_CLK_DISABLE()
#define LED1_Red_PIN                         	GPIO_PIN_3

#define LED2_Blue_GPIO_PORT                   GPIOD
#define LED2_Blue_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED2_Blue_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()
#define LED2_Blue_PIN                         GPIO_PIN_11

#define LED2_Green_GPIO_PORT                   GPIOD
#define LED2_Green_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED2_Green_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()
#define LED2_Green_PIN                         GPIO_PIN_12

#define LED2_Red_GPIO_PORT                   	GPIOD
#define LED2_Red_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOD_CLK_ENABLE()
#define LED2_Red_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOD_CLK_DISABLE()
#define LED2_Red_PIN                         	GPIO_PIN_13

#define LEDn                    	((uint32_t)6)

/** @}
*/
/*----------------------------------------LED_BSP Exported Macro---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/** @defgroup LED_BSP_Exported_Types LED_BSP Exported Types
  * @{
  */
/**
  * @brief  Led bumber enum
  */
typedef enum
{
LED1_Blue 	= 0,
LED1_Green 	= 1,
LED1_Red	= 2,
LED2_Blue 	= 3,
LED2_Green 	= 4,
LED2_Red	= 5,
}Led_TypeDef;
/** @}
*/
/*----------------------------------------LED_BSP Exported Types---------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup LED_BSP_Exported_Functions LED_BSP Exported Functions
  * @{
  */
/** @defgroup LED_BSP_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
/** @}
*/
/*********************LED_BSP Exported Functions Group1***************************/
/** @defgroup LED_BSP_Exported_Functions_Group2 Operation Functions
  * @{
  */
void Bsp_InitLed(Led_TypeDef Led);
void Bsp_DeInitLed(Led_TypeDef Led);
void Bsp_LED_On(Led_TypeDef Led);
void Bsp_LED_Off(Led_TypeDef Led);
void Bsp_LED_Toggled(Led_TypeDef Led);
/** @}
*/
/**********************LED_BSP Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------LED_BSP Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------LED_BSP------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __LED_BSP_H */

/**********************************************END OF FILE******************************************/
