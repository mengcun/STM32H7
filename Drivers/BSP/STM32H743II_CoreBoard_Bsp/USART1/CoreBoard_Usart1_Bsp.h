/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/USART1/USART1.h
  * @author  CME
  * @version V1.0.0
  * @date  	 14-Junly-2018
  * @brief   The template of header file.
  *					 Please copy this file and rename the file name if needed.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __COREBOARD_USART1_BSP_H
#define __COREBOARD_USART1_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdio.h"	
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
/** @addtogroup USART1
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup USART1_Private_Define USART1 Private Define
  * @{
  */

#ifdef NoMicroLIB
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif /*MicroLIB*/

#ifdef EN_USARTx_RX 				//使能串口1接收
#define USART_REC_LEN  200  //定义最大接收字节数 200
#define RXBUFFERSIZE   1 		//缓存大小
#endif /*EN_USARTx_RX*/

/* Definition for USARTx clock resources */
#define USARTx         									 USART1		//使用USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF     								 GPIO_AF7_USART1

/* Definition for USARTx's NVIC IRQ and IRQ Handlers */
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

/** @}
*/
/*----------------------------------------USART1 Exported Define---------------------------------------*/

/* Exported Variables ---------------------------------------------------------------------------------*/
/** @defgroup USART1_Private_Variables USART1 Exported Variables
  * @{
  */
/**
  * @brief  USART1 Exported Handle
	* @note		USART1
  */
extern UART_HandleTypeDef UartHandle; //UARTx句柄

#ifdef EN_USARTx_RX 									//使能串口1接收
extern uint16_t USART_RX_STA;       	//接收状态标记	
extern uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
extern uint8_t USART_RX_BUF[USART_REC_LEN];     	
//接收缓冲,最大USART_REC_LEN个字节.
//串口1中断服务程序中注意,读取USARTx->SR能避免莫名其妙的错误   	
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
#endif /*EN_USARTx_RX*/
/** @}
*/		
/*----------------------------USART1 Exported Variables --------------------------------------------*/ 

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup USART1_Exported_Functions USART1 Exported Functions
  * @{
  */
/** @defgroup USART1_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
/** @}
*/
/*********************USART1 Exported Functions Group1***************************/
/** @defgroup USART1_Exported_Functions_Group2 Operation Functions
  * @{
  */
#ifdef EN_USARTx_RX   //如果使能了接收
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#endif
void Bsp_InitUsart1(uint32_t bound);
/** @}
*/
/**********************USART1 Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------USART1 Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------USART1------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __COREBOARD_USART1_BSP_H */

/**********************************************END OF FILE******************************************/
