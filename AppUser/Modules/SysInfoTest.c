/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/Modules/SysInfoTest.c
  * @author  MCD Application Team
  * @version SW:V1.0.0 HW:V1.0
  * @date    15-August-2018
  * @brief   为了获取FreeRTOS的任务信息，需要创建一个定时器，这个定时器的时间基准精度要高于
	*              系统时钟节拍。这样得到的任务信息才准确。
	*              本文件提供的函数仅用于测试目的，切不可将其用于实际项目，原因有两点：
	*               1. FreeRTOS的系统内核没有对总的计数时间做溢出保护。
	*               2. 定时器中断是50us进入一次，比较影响系统性能。
	*              --------------------------------------------------------------------------------------
	*              本文件使用的是32位变量来保存50us一次的计数值，最大支持计数时间：
	*              2^32 * 50us / 3600s = 59.6分钟。使用中测试的任务运行计数和任务占用率超过了59.6分钟将不准确。
	*
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The Template canbe used as follows:
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
/** @defgroup SystemInfoTest SystemInfoTest
  * @brief SystemInfoTest
  * @{
  */

/* Private Defines -------------------------------------------------------------------*/
/** @defgroup SystemInfoTest_Private_Defines SystemInfoTest Private Defines
  * @{
  */
/**
  * @brief  SystemInfoTest Private definition
  */
/* 定时器频率，50us一次中断 */
#define  timerINTERRUPT_FREQUENCY	20000

/* 中断优先级 */
#define  timerHIGHEST_PRIORITY		2

/* 被系统调用 */
volatile uint32_t ulHighFrequencyTimerTicks = 0UL;	

/** @}
*/
/*----------------------------------------APP_MAIN Private Define----------------------------------*/
	
/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup Template_Private_Functions Template Private Functions
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
/*--------------------------------------Template Private Functions---------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup Template_Exported_Functions Template Exported Functions
  * @{
  */
/** @defgroup Template_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize XXXX
  @endverbatim
  * @{
  */
/**
  * @brief Initialize the XXXX according to the specified
  * @param param: brief.
  * @retval status
  */
//CODING HERE Function
/** @}
*/
/*********************Template Exported Functions Group1**************************/

/** @defgroup Template_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the XXXX
  @endverbatim
  * @{
  */
/**
  * @brief 
  * @param param: brief.
  * @retval status
  */
//CODING HERE Function
/** @}
*/
/****************************XXXX Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------Template Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------Template-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
