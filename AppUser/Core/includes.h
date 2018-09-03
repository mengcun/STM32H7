/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/core/includes.h
  * @author  CME
  * @version V1.0.0
  * @date  	 14-Junly-2018
  * @brief   The header of in common use.
  *					 Add the common use headers and function definitions
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __INCLUDES_H
#define __INCLUDES_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*******************************************APP/BSP*************************************************/
#include "Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup APP_MAIN
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup APP_MAIN_Private_Define APP_MAIN Private Define
  * @{
  */
  
#define SYSTEM_SUPPORT_OS  0
#define SYSTEM_DEBUG  0

/** @}
*/
/*----------------------------------------APP_MAIN Exported Define---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup APP_MAIN_Exported_Functions APP_MAIN Exported Functions
  * @{
  */
/** @defgroup APP_MAIN_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
//CODE HERE
/** @}
*/
/*********************APP_MAIN Exported Functions Group1***************************/
/** @defgroup APP_MAIN_Exported_Functions_Group2 Operation Functions
  * @{
  */
//CODE HERE
/** @}
*/
/**********************APP_MAIN Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------APP_MAIN Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------APP_MAIN------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __TEMPLATE_H */

/**********************************************END OF FILE******************************************/
