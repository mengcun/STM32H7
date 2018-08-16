/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/core/main.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    13-July-2018
  * @brief   This CoreBoard is used for study.
  *					 -
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
#include "includes.h"

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup APP_MAIN APP_MAIN
  * @brief This is the main() function here.
  * @{
  */

/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup APP_MAIN_Private_Functions_Prototypes APP_MAIN Private Functions Prototypes
  * @{
  */
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);
/** @}
*/		
/*----------------------------APP_MAIN Private Functions Prototypes--------------------------------*/ 

/* Private variables-------------------------------------------------------------------*/
/** @defgroup APP_MAIN_Private_variables APP_MAIN Private variables
  * @{
  */
static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

/** @}
*/		
/*----------------------------APP_MAIN Private variables--------------------------------*/ 

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup APP_MAIN_Private_Functions APP_MAIN Private Functions
  * @{
  */
/**
  * @brief  Main program
  */

int main(void)
{	
	/* -1- 在启动调度前，为了防止初始化STM32外设时有中断服务程序执行，这里禁止全局中断(除了NMI和HardFault)。
	  这样做的好处是：
	  1. 防止执行的中断服务程序中有FreeRTOS的API函数。
	  2. 保证系统正常启动，不受别的中断影响。
	  3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
	  在移植文件port.c中的函数prvStartFirstTask中会重新开启全局中断。通过指令cpsie i开启，__set_PRIMASK(1)和cpsie i是等效的。*/
	DISABLE_INT();
	/* -2- Coreboard Init:CPU L1 Cache Enable; STM32H7xx HAL library initialization; System Clock 400MHz */
	Coreboard_BSP_Init();	
	
	/* -3- 创建任务 */
	AppTaskCreate();
	
  /* -4- 启动调度，开始执行任务 */
  vTaskStartScheduler();

	/* -5- 如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
	  heap空间不足造成创建失败，此要加大FreeRTOSConfig.h文件中定义的heap大小：
	  #define configTOTAL_HEAP_SIZE	      ( ( size_t ) ( 30 * 1024 ) ) */
	while(1)
	{
		Bsp_LED_Off(LED1_Green);
		Bsp_LED_Toggled(LED1_Red);
		vTaskDelay(1000);
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTaskTaskUserIF
*	功能说明: 接口消息处理，这里用作LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters)
{
    while(1)
    {
		Bsp_LED_Toggled(LED1_Blue);
		vTaskDelay(100);
		}
}

/*
*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
static void vTaskLED(void *pvParameters)
{
    while(1)
    {
		Bsp_LED_Toggled(LED2_Blue);
        vTaskDelay(200);
    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 信息处理，这里是用作LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
    while(1)
    {
		Bsp_LED_Toggled(LED2_Green);
        vTaskDelay(300);
    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作LED闪烁
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
    while(1)
    {
		/* LED闪烁 */
		Bsp_LED_Toggled(LED2_Red);
        vTaskDelay(400);
    }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskTaskUserIF,   	/* 任务函数  */
                 "vTaskUserIF",     	/* 任务名    */
                 512,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 5,                 	/* 任务优先级*/
                 &xHandleTaskUserIF );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskLED,    		/* 任务函数  */
                 "vTaskLED",  		/* 任务名    */
                 512,         		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 6,           		/* 任务优先级*/
                 &xHandleTaskLED ); /* 任务句柄  */
	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 512,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 7,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 512,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 10,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}
/** @}
*/
/*--------------------------------------APP_MAIN Private Functions---------------------------------*/

/** @}
*/
/*----------------------------------------------APP_MAIN-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
