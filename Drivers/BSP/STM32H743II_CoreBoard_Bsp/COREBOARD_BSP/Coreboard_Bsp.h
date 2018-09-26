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
#ifndef __COREBOARD_BSP_H
#define __COREBOARD_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
#include "includes.h"
/***************************************Include StdLib**********************************************/
#include "stdint.h"
#include "stm32h7xx_hal.h"
#include "stm32h743xx.h"
/*******************************************APP/BSP*************************************************/
#include "Led_Bsp.h"
#include "Mpu_Bsp.h"
#include "Lcd_Bsp.h"
#include "Sdram_Bsp.h" 
#include "Software_Timer_Bsp.h"
#include "Usart1_Bsp.h"
#include "Hardware_Timer_Bsp.h"
#include "Wdg_Bsp.h"
#include "Rtc_Bsp.h"
#include "Pwm_Bsp.h"
#include "Crc_Bsp.h"
#include "Rng_Bsp.h"
#include "Dac_Bsp.h"
#include "Adc_Bsp.h"
#include "Spi_Flash_Bsp.h"
#include "Sd_Bsp.h"
#include "Malloc_Bsp.h"
#include "Bsp_FatFs_BasicFunction.h"	 
#include "Bsp_FatFs_ExtendFunctions.h"
	 
#include "user_dbg.h"
#include "user_dbg_str.h"
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
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup STM32H743II_CoreBoard_Private_Defines STM32H743II CoreBoard Private Defines
  * @{
  */

/**
  * @brief  The define for ADC Single Mode.
  */

#define	ADC12_MULTI_CHANNEL_CONVERT	0
#define	ADC3_SINGLE_CHANNEL_CONVERT	0

/**
  * @brief  The define for DAC_LFSR_NOISE DEBUG
  */
#define DAC_WAVE_DEBUG				0
#define SINWAVE_GEN_FOR_TEST		0

/**
  * @brief  The define for Hardware_Timer DEBUG
  */
#define HARDWARE_TIM1_DEBUG			0
#define HARDWARE_TIM5_DEBUG			0
#define HARDWARE_TIM2PWM_DEBUG		0

/**
  * @brief  The define for IWDG DEBUG
  */
#define IWDG_DEBUG					0

/**
  * @brief  The define for WWDG DEBUG
  */
#define WWDG_DEBUG					0 
 
/**
  * @brief  The define for CRC DEBUG
  */
#define CRC_DEBUG					0

/**
  * @brief  The define for RNG DEBUG
  */
#define RNG_DEBUG					0

/**
  * @brief  The define for RTC DEBUG
  */
#define RTC_GET_TIME				0
#define RTC_ALARMA_DEBUG			0
#define RTC_BKRAM_DEBUG				0
#define RTC_WAKEUP_DEBUG			0
#define RTC_TIMESTAMP_DEBUG			0
#define RTC_TAMPER_DEBUG			0

/**
  * @brief  The define for W25QXX DEBUG
  */
#define W25QXX_DEBUG				1
#define W25QXX_TEST					0

/**
  * @brief  The define for SD_MMC_DEBUG
  */
#define SD_MMC_DEBUG    			1
#define SD_DMA_MODE     			0	//1：DMA模式，0：查询模式

/**
  * @brief  The define for SDRAM DEBUG
  * @brief  This should be opened always
  */
#define SDRAM_DEBUG					1

/**
  * @brief  The define for LCD DEBUG
  * @brief  This should be opened always
  */ 
#define RGBLCD_DEBUG				1

/**
  * @brief  The define for USER DEBUG
  */  
#define USMART_DEBUG				1

/**
  * @brief  The define for MOUNT_SD_CARD
  */  
#define MOUNT_SD_CARD				1

/**
  * @brief  The define for USE_FLASH_AS_VOLUM
  *			This should be always 0 unless we indeed need it.
  */  
#define USE_FLASH_AS_VOLUM			0

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
  * @brief  Configure the MPU as "Write through", to guarantee the write access coherence
  */
#define Write_Through() (SCB->CACR|=1<<2) //强制D-Cache透写,如不开启,实际使用中可能遇到各种问题
/**
  * @brief  开关全局中断的宏
  */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/**
  * @brief  这个宏仅用于调试阶段排错
  */
#define Bsp_Printf		printf
/**
  * @brief  CPU空闲时执行的函数
  */
#define CPU_IDLE()		bsp_Idle()
/**
  * @brief  定义TRUE = 1; FALSE = 0;
  */
#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/** @}
*/
/*----------------------------------------CoreBoard_BSP Private Define----------------------------------*/

/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/** @defgroup STM32H743II_CoreBoard_Private_types STM32H743II CoreBoard Private types
  * @{
  */
/**
  * @brief STM32H743II_CoreBoard Information
  */	
typedef struct
{
	uint8_t BOARD_NAME[40];
	uint8_t CPU_NAME[40];
	uint8_t CPU_ID[40];
	uint8_t BSP_VERSION[40];
	uint8_t LCD_ID[40];
}Infor_CoreBoard;

/** @}
*/
/*----------------------------------------CoreBoard_BSP Private Define----------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Private variables		CoreBoard_BSP Private variables
  * @{
  */
static volatile uint8_t CountEvery10ms;
/** @}
*/
/**********************CoreBoard_BSP Private variables**************************/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Extern variables		CoreBoard_BSP Extern variables
  * @{
  */
extern IWDG_HandleTypeDef IWDG_Handler; 
/** @}
*/
/**********************CoreBoard_BSP Extern variables**************************/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Extern functions -------------------------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Extern_functions		CoreBoard_BSP Extern functions
  * @{
  */
extern void _pFunctionHardTimeout(void);
/** @}
*/
/**********************CoreBoard_BSP Extern functions**************************/
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
uint8_t Bsp_Get_DCahceSta(void);
uint8_t Bsp_Get_ICahceSta(void);
uint32_t BSP_GetVersion(void);
void Error_Handler(void);
void assert_failed(uint8_t* file, uint32_t line);
void BSP_Idle(void);
void BSP_RunPer1ms(void);
void BSP_RunPer10ms(void);
void GetInfo_CoreBoard(void);

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

#endif /* __COREBOARD_BSP_H */

/**********************************************END OF FILE******************************************/
