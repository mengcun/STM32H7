/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/Soft_Timer/Software_Timer_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 21-August-2018
  * @brief   The Software_Timer of header file.
  *					 
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __SOFTWARE_TIMER_BSP_H
#define __SOFTWARE_TIMER_BSP_H

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
/** @addtogroup Software_Timer
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup Software_Timer_Private_Defines Software Timer Private Defines
  * @{
  */
/**
  * @brief  ���ɸ������ʱ��ȫ�ֱ���
  */
//ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
#define TMR_COUNT	4		 //�����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 3) 

/** @}
*/
/*----------------------------------------Software_Timer Private Define----------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/** @defgroup Software_Timer_Exported_types Software Timer Exported types
  * @{
  */
/**
  * @brief	�����ʱ������ģʽ
  */
typedef enum
{
	TMR_ONCE_MODE = 0,		/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
}TMR_MODE_E;
/**
  * @brief  ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ����������
  */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint32_t Count;	/* ������ */
	volatile uint32_t PreLoad;	/* ������Ԥװֵ */
}SOFT_TMR;

/** @}
*/
/*----------------------------------------Software_Timer Exported types----------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup Software_Timer_Exported_Functions Software_Timer Exported Functions
  * @{
  */
/** @defgroup Software_Timer_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
void Bsp_InitSoftTimer(uint16_t SYSCLK);

/** @}
*/
/*********************Software_Timer Exported Functions Group1***************************/
/** @defgroup Software_Timer_Exported_Functions_Group2 Operation Functions
  * @{
  */
void Bsp_Delay_us(uint32_t nus);
void Bsp_Delay_ms(uint32_t nms);

void Bsp_StartTimer(uint8_t _id, uint32_t _period);
void Bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void Bsp_StopTimer(uint8_t _id);
uint8_t Bsp_CheckTimer(uint8_t _id);
int32_t Bsp_GetRunTime(void);

/** @}
*/
/**********************Software_Timer Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------Software_Timer Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------Software_Timer------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __SOFTWARE_TIMER_BSP_H */

/**********************************************END OF FILE******************************************/
