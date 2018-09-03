/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/core/stm32h7xx_it.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    13-July-2018
  * @brief   Main Interrupt Service Routines.
  ******************************************************************************
  *	我们只需要添加需要的中断函数即可。一般中断函数名是固定的，除非修改了启动文件中的函数名
  *	启动文件是汇编语言文件，定了每个中断的服务函数，这些函数使用了WEAK 关键字，表示弱定义，因此如
  *	果我们在c文件中重定义了该服务函数（必须和它同名），那么启动文件的中断函数将自动无效。这也就
  *	函数重定义的概念。
  *
  *	为了加强模块化，我们建议将中断服务程序分散到对应的驱动模块文件中。比如systick中断服务程序
  *	放在 bsp_timer.c 文件中。
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
     /* 等待发送结束 */
     while ((USARTx->ISR&0X40)==0);
  }
#endif
 /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception. MemManage错误处理中断,进入此中断以后,将无法恢复程序运行!!
  */
void MemManage_Handler(void)
{
	Bsp_LED_Off(LED1_Green);
	Bsp_LED_On(LED1_Red);	
	Bsp_Printf("Mem Access Error!!\r\n"); 	//输出错误信息
	Bsp_Delay_ms(1000);	
	Bsp_Printf("Soft Reseting...\r\n");		//提示软件重启
	Bsp_Delay_ms(1000);	
	NVIC_SystemReset();					//软复位
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
