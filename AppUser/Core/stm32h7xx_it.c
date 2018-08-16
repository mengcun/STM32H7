/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/core/stm32h7xx_it.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    13-July-2018
  * @brief   Main Interrupt Service Routines.
  ******************************************************************************
  *
  *
  * 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "stm32h7xx_it.h"
#include "CoreBoard_Usart1_Bsp.h"
#include "Coreboard_Led_Bsp.h"

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */

/** @addtogroup APP_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/
#ifdef EN_USARTx_RX
/**
  * @brief  //����x���жϷ������.
  * @param  None
  * @retval None
  */
void USARTx_IRQHandler(void)                	
{ 
#ifdef SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif /*SYSTEM_SUPPORT_OS*/

		
#ifdef NO_USE_HAL_UART_RxCpltCallback
	uint8_t Res;

	if((__HAL_UART_GET_FLAG(&UartHandle,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Bsp_LED_On(LED2_Blue);
    HAL_UART_Receive(&UartHandle,&Res,1,1000); 

		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA = 0;//���մ���,���¿�ʼ
				else
				{					
					USART_RX_STA|=0x8000;	//��������� 
					Bsp_LED_Off(LED2_Blue);
				}
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF] = Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		} 
	}	
#endif /*NO_USE_HAL_UART_RxCpltCallback*/
	
	HAL_UART_IRQHandler(&UartHandle);	//����HAL���жϴ����ú���

#ifdef USE_HAL_UART_RxCpltCallback
	uint32_t timeout=0;
  uint32_t maxDelay=0x1FFFF;	
	timeout=0;
  /*  Wait for the end of the transfer */
  while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
	{
		timeout++; //��ʱ����
    if(timeout>maxDelay)	
   /* Transfer error in reception process */
    {Error_Handler();}		
	}
	timeout=0;
  /* Put UART peripheral in reception process */
	while(HAL_UART_Receive_IT(&UartHandle,(uint8_t *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
  {
		timeout++; //��ʱ����
    if(timeout>maxDelay)	
   /* Transfer error in reception process */
    {Error_Handler();}
  }
#endif /*USE_HAL_UART_RxCpltCallback*/
	
#ifdef SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif /*SYSTEM_SUPPORT_OS*/
} 
#endif /*EN_USARTx_RX*/
/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  MemManage�������ж�,������ж��Ժ�,���޷��ָ���������!!
  */
void MemManage_Handler(void)
{
	Bsp_LED_Off(LED1_Green);
	Bsp_LED_On(LED1_Red);	
	printf("Mem Access Error!!\r\n"); 	//���������Ϣ
	delay_ms(1000);	
	printf("Soft Reseting...\r\n");		//��ʾ�������
	delay_ms(1000);	
	NVIC_SystemReset();					//��λ
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
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
  * @param  None
  * @retval None
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
  * @param  None
  * @retval None
  */
/*
void SVC_Handler(void)
{
}
*/
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*
void PendSV_Handler(void)
{
}
*/
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*
void SysTick_Handler(void)
{
  HAL_IncTick();
}
*/
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
