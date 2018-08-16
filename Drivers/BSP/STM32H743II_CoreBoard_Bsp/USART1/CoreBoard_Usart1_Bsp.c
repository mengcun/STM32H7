/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/USART1/CoreBoard_Usart1_Bsp.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    16-July-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The USART1 canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h743ii_Coreboard_Bsp.h"
#include "CoreBoard_Usart1_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup USART1 USART1
  * @brief USART1 used as printf()
  * @{
  */
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup USART1_Private_Variables USART1 Private Variables
  * @{
  */
/**
  * @brief  USART1 Private Handle
	* @note		USART1
  */
UART_HandleTypeDef UartHandle; //UART句柄
/**
  * @brief  USART1 Private Variables
	* @note		USART1
  */
#ifdef EN_USARTx_RX 				//使能串口1接收
uint16_t USART_RX_STA=0;       	//接收状态标记	
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
uint8_t USART_RX_BUF[USART_REC_LEN];     	//接收缓冲,最大USART_REC_LEN个字节.
																					//串口1中断服务程序中注意,读取USARTx->SR能避免莫名其妙的错误   	
																					//接收状态
																					//bit15，	接收完成标志
																					//bit14，	接收到0x0d
																					//bit13~0，	接收到的有效字节数目
#endif /*EN_USARTx_RX*/
/** @}
*/		
/*----------------------------USART1 Private Variables--------------------------------*/ 
/* Private Types ------------------------------------------------------------------------------*/
/** @defgroup USART1_Private_Variables USART1 Private Types
  * @{
  */
/**
  * @brief  Printf() 标准库需要的支持函数
  */
#ifdef NoMicorLIN
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 
#endif /*NoMicorLIN*/
/** @}
*/		
/*----------------------------USART1 Private Types--------------------------------*/
/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup USART1_Private_Functions USART1 Private Functions
  * @{
  */
#ifdef MicroLIB
	
/**
  * @brief  Retargets the C library printf function to the USART.
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
#endif /*MicroLIB*/

#ifdef NoMicorLIN
//#pragma import(__use_no_semihosting)             
/**
  * @brief  定义_sys_exit()以避免使用半主机模式    
  */
void _sys_exit(int x) 
{ 
	x = x; 
} 
/**
  * @brief  重定义fputc函数    
  */ 
int fputc(int ch, FILE *f)
{ 	
	while((USARTx->ISR&0X40)==0);//循环发送,直到发送完毕   
	USARTx->TDR=(uint8_t)ch;      
	return ch;
}
#endif /*NoMicorLIN*/

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

//  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Select SysClk as source of USART1 clocks */
//  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
//  RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
//  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
	
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin 			= USARTx_RX_PIN;  
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  //GPIO_InitStruct.Alternate = USARTx_RX_AF;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
#ifdef EN_USARTx_RX
  
  /*##-3- Configure the NVIC for UART ########################################*/   
  /* NVIC for USARTx */
	HAL_NVIC_SetPriority(USARTx_IRQn,0,1);			//抢占优先级0，子优先级1
	HAL_NVIC_EnableIRQ(USARTx_IRQn);				//使能USART1中断通道
#endif		
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
#ifdef EN_USARTx_RX
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);
#endif		
}
/** @}
*/
/*--------------------------------------USART1 Private Functions---------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup USART1_Exported_Functions USART1 Exported Functions
  * @{
  */
/** @defgroup USART1_Exported_Functions_Group1 Initialization and de-initialization functions
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
  * @brief Initialize the USART1 according to the specified
  */
void Bsp_InitUsart1(uint32_t bound)
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) : 
                      BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = None parity
      - BaudRate    = baud
      - Hardware flow control disabled (RTS and CTS signals) */

  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = bound;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  //UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;	//achieve higher speed (up to f_PCLK/8)
	 //HAL_UART_Init()会使能UART1
	if(	HAL_UART_Init(&UartHandle) != HAL_OK)
	{
	  /* Initialization Error */
    Error_Handler();
	}
#ifdef EN_USARTx_RX   //如果使能了串口接收
  /*##-2- Put UART peripheral in reception process ###########################*/
  /* Any data received will be stored "aRxBuffer" buffer : the number max of
     data received is 10 */
  if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }
	//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
#endif
}
/** @}
*/
/*********************USART1 Exported Functions Group1**************************/

/** @defgroup USART1_Exported_Functions_Group2 Operation Functions
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
#ifdef EN_USARTx_RX   //如果使能了接收
#ifdef USE_HAL_UART_RxCpltCallback
/**
  * @brief HAL库中断处理公用函数,在接收到 RXBUFFERSIZE 个字节后会进入该代码
  * @param huart: 串口中断句柄.
  * @retval status
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USARTx)//如果是串口x
	{
		Bsp_LED_On(LED2_Blue);
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else 
				{
					USART_RX_STA|=0x8000;	//接收完成了 
					Bsp_LED_Off(LED2_Blue);
				}
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}
	else
	{Error_Handler();}
}

#endif /*USE_HAL_UART_RxCpltCallback*/

#endif /*EN_USARTx_RX*/

/** @}
*/
/****************************XXXX Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------USART1 Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------USART1-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
