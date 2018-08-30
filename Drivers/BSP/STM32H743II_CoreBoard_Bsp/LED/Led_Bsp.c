/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/LED/Led_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    14-July-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The Led_Bsp canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/*******************************************APP/BSP*************************************************/
#include "stm32h743ii_Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/
/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup LED_BSP LED_BSP
  * @brief Initial the LED in Coreboard
	* @note	First release 2018/07/15
  * @{
  */
/* Exported constants -----------------------------------------------------------------------------*/
/** @defgroup LED_BSP_Exported_Constants LED_BSP Exported Constants
  * @{
  */
/**
  * @brief  Led GPIO pin number
  */
const uint32_t GPIO_PIN[LEDn] = {	LED1_Blue_PIN,
									LED1_Green_PIN,
									LED1_Red_PIN,
									LED2_Blue_PIN,
									LED2_Green_PIN,
									LED2_Red_PIN};
/** @}
*/
/*----------------------------------------LED_BSP Exported Constants-----------------------------------*/		
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup LED_BSP_Private_Functions_Prototypes LED_BSP Private Functions Prototypes
  * @{
  */
/** @}
*/		
/*----------------------------LED_BSP Private Functions Prototypes--------------------------------*/ 

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup LED_BSP_Private_Functions LED_BSP Private Functions
  * @{
  */

/** @}
*/
/*--------------------------------------LED_BSP Private Functions---------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup LED_BSP_Exported_Functions LED_BSP Exported Functions
  * @{
  */
/** @defgroup LED_BSP_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize LED_BSP
  @endverbatim
  * @{
  */
/**
  * @brief  Configures LED on GPIO.
  * @param  Led: LED to be configured.
  *          This parameter can be one of the following values:
  *            @arg  LED1_Blue
  *            @arg  LED1_Green
  *            @arg  LED1_Red
  *            @arg  LED2_Blue
  *            @arg  LED2_Green
  *            @arg  LED2_Red
  */
void Bsp_InitLed(Led_TypeDef Led)
{
	GPIO_InitTypeDef	gpio_initstruct;
	GPIO_TypeDef*	gpio_led;
	switch(Led)
	{
		case LED1_Blue:
			gpio_led = LED1_Blue_GPIO_PORT;
			LED1_Blue_GPIO_CLK_ENABLE();
			break;
		case LED1_Green:
			gpio_led = LED1_Green_GPIO_PORT;
			LED1_Green_GPIO_CLK_ENABLE();			
			break;
		case LED1_Red:
			gpio_led = LED1_Red_GPIO_PORT;
			LED1_Red_GPIO_CLK_ENABLE();
		break;
		case LED2_Blue:
			gpio_led = LED2_Blue_GPIO_PORT;
			LED2_Blue_GPIO_CLK_ENABLE();			
		break;
		case LED2_Green:
			gpio_led = LED2_Green_GPIO_PORT;
			LED2_Green_GPIO_CLK_ENABLE();			
		break;
		case LED2_Red:
			gpio_led = LED2_Red_GPIO_PORT;
			LED2_Red_GPIO_CLK_ENABLE();			
		break;
		default:
			break;
	}
    /* Configure the GPIO_LED pin */
    gpio_initstruct.Pin = GPIO_PIN[Led];
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initstruct.Pull = GPIO_PULLUP;
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(gpio_led, &gpio_initstruct);

    /* By default, turn off LED */
    HAL_GPIO_WritePin(gpio_led, GPIO_PIN[Led], GPIO_PIN_SET);	
}
/** @}
*/
/*********************LED_BSP Exported Functions Group1**************************/

/** @defgroup LED_BSP_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the LED_BSP
  @endverbatim
  * @{
  */
/**
  * @brief  DeInit LEDs
  * @param  Led: LED to be configured.
  *          This parameter can be one of the following values:
  *            @arg  LED1_Blue
  *            @arg  LED1_Green
  *            @arg  LED1_Red
  *            @arg  LED2_Blue
  *            @arg  LED2_Green
  *            @arg  LED2_Red
	* @note Led DeInit does not disable the GPIO clock
  */
void Bsp_DeInitLed(Led_TypeDef Led)
{
	GPIO_InitTypeDef	gpio_initstruct;
	GPIO_TypeDef*	gpio_led;
	switch(Led)
	{
		case LED1_Blue:
			gpio_led = LED1_Blue_GPIO_PORT;
			break;
		case LED1_Green:
			gpio_led = LED1_Green_GPIO_PORT;
			break;
		case LED1_Red:
			gpio_led = LED1_Red_GPIO_PORT;
		break;
		case LED2_Blue:
			gpio_led = LED2_Blue_GPIO_PORT;
		break;
		case LED2_Green:
			gpio_led = LED2_Green_GPIO_PORT;
		break;
		case LED2_Red:
			gpio_led = LED2_Red_GPIO_PORT;
		break;
		default:
			break;
	}
    /* Turn off LED */
    HAL_GPIO_WritePin(gpio_led, GPIO_PIN[Led], GPIO_PIN_RESET);	
    /* Configure the GPIO_LED pin */
    gpio_initstruct.Pin = GPIO_PIN[Led];
    HAL_GPIO_DeInit(gpio_led, gpio_initstruct.Pin);	
}
/**
  * @brief  Turns selected LED On.
  * @param  Led: LED to be set on
  *          This parameter can be one of the following values:
  *            @arg  LED1_Blue
  *            @arg  LED1_Green
  *            @arg  LED1_Red
  *            @arg  LED2_Blue
  *            @arg  LED2_Green
  *            @arg  LED2_Red
  */
void Bsp_LED_On(Led_TypeDef Led)
{
	switch(Led)
	{
		case LED1_Blue:
			HAL_GPIO_WritePin(LED1_Blue_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_RESET);
			break;
		case LED1_Green:
			HAL_GPIO_WritePin(LED1_Green_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_RESET);
			break;
		case LED1_Red:
			HAL_GPIO_WritePin(LED1_Red_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_RESET);
			break;
		case LED2_Blue:
			HAL_GPIO_WritePin(LED2_Blue_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_RESET);
			break;
		case LED2_Green:
			HAL_GPIO_WritePin(LED2_Green_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_RESET);
			break;
		case LED2_Red:
			HAL_GPIO_WritePin(LED2_Red_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_RESET);
			break;
		default:
			break;
	}
}
/**
  * @brief  Turns selected LED Off.
  * @param  Led: LED to be set off
  *          This parameter can be one of the following values:
  *            @arg  LED1_Blue
  *            @arg  LED1_Green
  *            @arg  LED1_Red
  *            @arg  LED2_Blue
  *            @arg  LED2_Green
  *            @arg  LED2_Red
  */
void Bsp_LED_Off(Led_TypeDef Led)
{
	switch(Led)
	{
		case LED1_Blue:
			HAL_GPIO_WritePin(LED1_Blue_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_SET);
			break;
		case LED1_Green:
			HAL_GPIO_WritePin(LED1_Green_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_SET);
			break;
		case LED1_Red:
			HAL_GPIO_WritePin(LED1_Red_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_SET);
		break;
		case LED2_Blue:
			HAL_GPIO_WritePin(LED2_Blue_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_SET);
		break;
		case LED2_Green:
			HAL_GPIO_WritePin(LED2_Green_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_SET);
		break;
		case LED2_Red:
			HAL_GPIO_WritePin(LED2_Red_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_SET);
		break;
		default:
			break;
	}
}
/**
  * @brief  Toggles the selected LED.
  * @param  Led: LED to be toggled
  *          This parameter can be one of the following values:
  *            @arg  LED1_Blue
  *            @arg  LED1_Green
  *            @arg  LED1_Red
  *            @arg  LED2_Blue
  *            @arg  LED2_Green
  *            @arg  LED2_Red
  */
void Bsp_LED_Toggled(Led_TypeDef Led)
{
	switch(Led)
	{
		case LED1_Blue:
			HAL_GPIO_TogglePin(LED1_Blue_GPIO_PORT, GPIO_PIN[Led]);
			break;
		case LED1_Green:
			HAL_GPIO_TogglePin(LED1_Green_GPIO_PORT, GPIO_PIN[Led]);
			break;
		case LED1_Red:
			HAL_GPIO_TogglePin(LED1_Red_GPIO_PORT, GPIO_PIN[Led]);
		break;
		case LED2_Blue:
			HAL_GPIO_TogglePin(LED2_Blue_GPIO_PORT, GPIO_PIN[Led]);
		break;
		case LED2_Green:
			HAL_GPIO_TogglePin(LED2_Green_GPIO_PORT, GPIO_PIN[Led]);
		break;
		case LED2_Red:
			HAL_GPIO_TogglePin(LED2_Red_GPIO_PORT, GPIO_PIN[Led]);
		break;
		default:
			break;
	}
}
/** @}
*/
/****************************LED_BSP Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------LED_BSP Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------LED_BSP-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
