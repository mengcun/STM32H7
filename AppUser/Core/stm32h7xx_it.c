/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/core/stm32h7xx_it.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    13-July-2018
  * @brief   Main Interrupt Service Routines.
  ******************************************************************************
  *	����ֻ��Ҫ�����Ҫ���жϺ������ɡ�һ���жϺ������ǹ̶��ģ������޸��������ļ��еĺ�����
  *	�����ļ��ǻ�������ļ�������ÿ���жϵķ���������Щ����ʹ����WEAK �ؼ��֣���ʾ�����壬�����
  *	��������c�ļ����ض����˸÷��������������ͬ��������ô�����ļ����жϺ������Զ���Ч����Ҳ��
  *	�����ض���ĸ��
  *
  *	Ϊ�˼�ǿģ�黯�����ǽ��齫�жϷ�������ɢ����Ӧ������ģ���ļ��С�����systick�жϷ������
  *	���� bsp_timer.c �ļ��С�
  *
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "stm32h7xx_it.h"
#include "Led_Bsp.h"
#include "Usart1_Bsp.h"
#include "Led_Bsp.h"
/** @addtogroup STM32H743II_CoreBoard
  * @{
  */

/** @addtogroup APP_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Private_Define CoreBoard_BSP Private Define
  * @{
  */
#define ERR_INFO "\r\nEnter HardFault_Handler, System Halt.\r\n"
/** @}
*/	
/*----------------------------CoreBoard_BSP Private Functions Prototypes--------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  */
void HardFault_Handler(void)
{
 #if 1
  const char *pError = ERR_INFO;
  uint8_t i;

  for (i = 0; i < sizeof(ERR_INFO); i++)
  {
     USARTx->TDR = pError[i];
     /* �ȴ����ͽ��� */
     while ((USARTx->ISR&0X40)==0);
  }
#endif
 /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception. MemManage�������ж�,������ж��Ժ�,���޷��ָ���������!!
  */
void MemManage_Handler(void)
{
	Bsp_LED_Off(LED1_Green);
	Bsp_LED_On(LED1_Red);	
	Bsp_Printf("Mem Access Error!!\r\n"); 	//���������Ϣ
	Bsp_Delay_ms(1000);	
	Bsp_Printf("Soft Reseting...\r\n");		//��ʾ�������
	Bsp_Delay_ms(1000);	
	NVIC_SystemReset();					//��λ
}

/**
  * @brief  This function handles Bus Fault exception.
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  */
/*
void SVC_Handler(void)
{
}
*/
/**
  * @brief  This function handles Debug Monitor exception.
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  */

void PendSV_Handler(void)
{
}

/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/

/**
  * @}
  */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
