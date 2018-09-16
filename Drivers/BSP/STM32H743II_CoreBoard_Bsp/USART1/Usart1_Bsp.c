/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/USART1/Usart1_Bsp.c
  * @author  CME
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
#include "stdio.h"
/*******************************************APP/BSP*************************************************/
#include "Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup USART1 USART1
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

#if EN_USART1_RX == 1
/**
  * @brief  USART1 Receive Buffer
  *			bit15，	接收完成标志
  *			bit14，	接收到0x0d
  *			bit13~0，接收到的有效字节数目
	* @note		USART1
  */
  
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

/**
  * @brief  USART1 Receive Flag
	* @note		USART1
  */
uint16_t USART_RX_STA=0;       //接收状态标记	

/**
  * @brief  HAL库使用的串口接收缓冲
	* @note		USART1
  */
  
uint8_t aRxBuffer[RXBUFFERSIZE];

#endif
/** @}
*/		
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup USART1_Private_Functions_Prototypes USART1 Private Functions Prototypes
  * @{
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);
void CPU_CACHE_Enable(void);

/** @}
*/	
/*----------------------------USART1 Private Functions Prototypes--------------------------------*/ 
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
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;	//achieve higher speed (up to f_PCLK/8)
	
	if(	HAL_UART_Init(&UartHandle) != HAL_OK)	//HAL_UART_Init()会使能UART1
	{
		/* Initialization Error */
		Error_Handler();
	}
#if EN_USART1_RX == 1
	
	HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	
#endif
}

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

	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();

	/* Select SysClk as source of USART1 clocks */
	RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
	RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
	HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

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

#if EN_USART1_RX == 1

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin 	  = USARTx_RX_PIN;  
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = USARTx_RX_AF;
	
	HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);	
	

	HAL_NVIC_EnableIRQ(USART1_IRQn);							//使能USART1中断通道
	HAL_NVIC_SetPriority(USART1_IRQn,3,2);						//设置USART1中断优先级，抢占优先级3，子优先级2。
																//在WWDG中每11ms在中断喂狗;如果使能窗口看门狗，所以理论上窗口看门狗中断优先级应为最高
																//调用BSP_RunPer10ms()函数，根据SysTick,IWDG每600ms一次喂狗;
																//调用延时函数Bsp_Delay_ms()触发Bsp_Idle()中根据SysTick大约2ms喂狗一次;																		
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

#if EN_USART1_RX == 1

/**
  * @brief 串口1中断服务程序 
  */

void USART1_IRQHandler(void)                	
{ 
	uint32_t timeout=0;
    uint32_t maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS == 1	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UartHandle);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UartHandle)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UartHandle,(uint8_t *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS == 1	 	//使用OS
	OSIntExit();  											 
#endif
} 

/**
  * @brief 串口1中断服务程序调用的处理函数 
  */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)			//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)	//接收未完成
		{
			if(USART_RX_STA&0x4000)		//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)
				{
					USART_RX_STA=0;		//接收错误,重新开始
				}
				else 
				{
					USART_RX_STA |= 0x8000;//接收完成了
					#if USART_DEBUG == 1
					HAL_UART_Transmit(&UartHandle,(uint8_t*)USART_RX_BUF,USART_RX_STA&0x3fff,1000);	//发送接收到的数据,长度为USART_RX_STA & 0x3fff
					while(__HAL_UART_GET_FLAG(&UartHandle,UART_FLAG_TC) != SET);					//等待发送结束
					USART_RX_STA=0;
					#endif
				} 
			}
			else 						 //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)
				{
					USART_RX_STA |= 0x4000;	
				}
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN-1))
					{
						USART_RX_STA=0;	//接收数据错误,重新开始接收	
					}  
				}		 
			}
		}
	}
}

#endif

/**
  * @brief Use MicroLIB to support printf 
  */
#if USE_MicroLIB == 1
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

#endif

/**
  * @brief Do not Use MicroLIB to support printf 
  */
#if USE_MicroLIB == 0
//#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USARTx->ISR&0X40)==0);//循环发送,直到发送完毕   
	USARTx->TDR=(uint8_t)ch;      
	return ch;
}

#endif 

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
