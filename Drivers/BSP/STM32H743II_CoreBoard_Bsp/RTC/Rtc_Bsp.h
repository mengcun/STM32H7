/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/RTC/RTC_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    28-August-2018
  * @brief   The template of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __RTC_BSP_H
#define __RTC_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
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
/** @addtogroup RTC_Calendar
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/** @defgroup RTC_Calendar_Exported_Macro RTC_Calendar Exported Macro
  * @{
  */
/**
  * @brief  RTC_Calendar
  */
extern RTC_HandleTypeDef 	RTC_Handler;  		//RTC¾ä±ú
extern RTC_TimeTypeDef 		RTC_TimeStruct;		//RTC Time Struct
extern RTC_DateTypeDef 		RTC_DateStruct;		//RTC Date Struct
extern RTC_AlarmTypeDef		RTC_AlarmStruct;	//RTC Alarm Struct
extern RTC_TamperTypeDef  	RTC_TamperStruct;	//RTC Tamper Struct

/**
  * @brief  Buffers used for displaying Time and Date
  */
extern uint8_t aShowTime[40];
extern uint8_t aShowDate[40];
extern uint8_t aShowWeek[10];
extern uint8_t ShowTimeStamp[40];
extern uint8_t aShowDateStamp[40];
/** @}
*/
/*----------------------------------------RTC_Calendar Exported Macro---------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup RTC_Calendar_Exported_Functions RTC_Calendar Exported Functions
  * @{
  */
/** @defgroup RTC_Calendar_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
uint8_t Bsp_RTC_Init(void);

/** @}
*/
/*********************RTC_Calendar Exported Functions Group1***************************/
/** @defgroup RTC_Calendar_Exported_Functions_Group2 Operation Functions
  * @{
  */
HAL_StatusTypeDef Bsp_RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t subsec,uint8_t ampm);
HAL_StatusTypeDef Bsp_RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week);

void Bsp_RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);
void Bsp_RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt);
void Bsp_RTC_Set_TimeStamp(void);
void Bsp_RTC_Set_Tamper(void);
void Bsp_RTC_Set_BackupRAM(void);

void Bsp_RTC_GetTime(void);
void Bsp_RTC_GetDate(void);
void Bsp_RTC_GetAlarm(void);
void GetInfo_Calendar(void);

void Bsp_WriteToRTC_BKP_DR(uint32_t FirstRTCBackupData);
uint32_t Bsp_CheckRTC_BKP_DR(uint32_t FirstRTCBackupData);
uint32_t Bsp_IsBackupRegReset(void);

uint32_t Bsp_WriteToSRAM(void);

/** @}
*/
/**********************RTC_Calendar Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------RTC_Calendar Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------RTC_Calendar------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __RTC_BSP_H */

/**********************************************END OF FILE******************************************/
