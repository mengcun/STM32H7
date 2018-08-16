/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/stm32h743ii_Coreboard_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    15-July-2018
  * @brief   This file contains definitions for:

  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __STM32H743II_COREBOARD_BSP_H
#define __STM32H743II_COREBOARD_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stm32h7xx_hal.h"
/*******************************************APP/BSP*************************************************/
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup CoreBoard_BSP
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup STM32H743II_CoreBoard_Private_Defines STM32H743II CoreBoard Private Defines
  * @{
  */
/**
  * @brief  USART1 Private definition
  */
#define EN_USARTx_RX	//如果使能串口接收，需要定义 #define EN_USARTx_RX
#define NoMicorLIN		//使用printf函数时不用MicroLIN
#define NO_USE_HAL_UART_RxCpltCallback	//接收中断中不使用HAL库中的函数
/**
  * @brief STM32H743II_CoreBoard BSP Driver version number V1.0.0
  */	
#define __STM32H743II_COREBOARD_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32H743II_COREBOARD_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32H743II_COREBOARD_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32H743II_COREBOARD_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32H743II_COREBOARD_BSP_VERSION  ((__STM32H743II_COREBOARD_BSP_VERSION_MAIN << 24)\
                                             |(__STM32H743II_COREBOARD_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32H743II_COREBOARD_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32H743II_COREBOARD_BSP_VERSION_RC))
/**
  * @brief STM32H743II_CoreBoard BSP USE FREE_RTOS
  */																						 
#define  USE_FreeRTOS      1

#if USE_FreeRTOS == 1
	#include "FreeRTOS.h"
	#include "task.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* 开关全局中断的宏 */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
#endif
																						 
/**
  * @brief  Configure the MPU as "Write through", to guarantee the write access coherence
  */
#define Write_Through() (SCB->CACR|=1<<2) //强制D-Cache透写,如不开启,实际使用中可能遇到各种问题
/**
  * @brief  Define the TRUE as 1; FALSE as 0
  */
#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/** @}
*/
/*----------------------------------------APP_MAIN Private Define----------------------------------*/

/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Exported_Functions CoreBoard_BSP Exported Functions
  * @{
  */
/** @defgroup CoreBoard_BSP_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
void Coreboard_BSP_Init(void);
/** @}
*/
/*********************CoreBoard_BSP_Exported_Functions_Group1***************************/
/** @defgroup CoreBoard_BSP_Exported_Functions_Group2 Operation Functions
  * @{
  */
uint8_t Get_DCahceSta(void);
uint8_t Get_ICahceSta(void);
uint32_t BSP_GetVersion(void);
void Error_Handler(void);
void assert_failed(uint8_t* file, uint32_t line);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

/** @}
*/
/**********************CoreBoard_BSP_Exported_Functions_Group2**************************/
/** @}
*/
/*--------------------------------CoreBoard_BSP_Exported_Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------CoreBoard_BSP------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __STM32H743II_COREBOARD_BSP_H */

/**********************************************END OF FILE******************************************/
