/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/
 
  #include <stdio.h>

 /* Ensure stdint is only used by the compiler, and not the assembler. */
 //��Բ�ͬ�ı��������ò�ͬ��stdint.h�ļ�
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
 #include <stdint.h>
 extern volatile uint32_t ulHighFrequencyTimerTicks;
 extern void StartIdleMonitor(void);
 extern void EndIdleMonitor(void);
 extern uint32_t SystemCoreClock;
#endif

///* CPU Usage */
//#define traceTASK_SWITCHED_IN()   								StartIdleMonitor()
//#define traceTASK_SWITCHED_OUT()  								EndIdleMonitor()

///* USE CCM RAM */
//#define configAPPLICATION_ALLOCATED_HEAP  				1

/***************************************************************************************************************/
/*                                        FreeRTOS������������ѡ��                                              */
/***************************************************************************************************************/
#define configUSE_PREEMPTION											1	 	//1ʹ����ռʽ�ںˣ�0ʹ��Э��
#define configUSE_TIME_SLICING										1		//1ʹ��ʱ��Ƭ����(Ĭ��ʽʹ�ܵ�)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION		1 	//1�������ⷽ����ѡ����һ��Ҫ���е�����,�Ż����ȼ��б���Ҫִ�е�������ȼ�������㷨,һ����Ӳ������ǰ����ָ������ʹ�õ�,MCUû����ЩӲ��ָ��Ļ��˺�Ӧ������Ϊ0��
#define configUSE_TICKLESS_IDLE										0   //1���õ͹���ticklessģʽ
#define configUSE_QUEUE_SETS											1		//1���ö���
#define configCPU_CLOCK_HZ												(SystemCoreClock)						//CPUƵ��
#define configTICK_RATE_HZ												((TickType_t) 1000)					//ʱ�ӽ���Ƶ�ʣ���������Ϊ1000�����ھ���1ms
#define configMAX_PRIORITIES											(8)													//��ʹ�õ�������ȼ��� 0 -7 ���˸�
#define configMINIMAL_STACK_SIZE									((unsigned short )512)			//��������ʹ�õĶ�ջ��С
#define configMAX_TASK_NAME_LEN										(16)												//���������ַ�������
#define configUSE_16_BIT_TICKS										0		//ϵͳ���ļ����������������ͣ�1��ʾΪ16λ�޷������Σ�0��ʾΪ32λ�޷�������
#define configIDLE_SHOULD_YIELD										1		//Ϊ1ʱ�����������CPUʹ��Ȩ������ͬ���ȼ����û�����
#define configUSE_TASK_NOTIFICATIONS            	1   //Ϊ1ʱ��������֪ͨ���ܣ�Ĭ�Ͽ���
#define configUSE_MUTEXES													1		//Ϊ1ʱʹ�û����ź���
#define configQUEUE_REGISTRY_SIZE									8		//��Ϊ0ʱ��ʾ���ö��м�¼�������ֵ�ǿ��Լ�¼�Ķ��к��ź��������Ŀ��
#define configCHECK_FOR_STACK_OVERFLOW						0		//����0ʱ���ö�ջ�����⹦�ܣ����ʹ�ô˹����û������ṩһ��ջ������Ӻ��������ʹ�õ�,��ֵ����Ϊ1����2����Ϊ������ջ�����ⷽ����
#define configUSE_RECURSIVE_MUTEXES								1		//Ϊ1ʱʹ�õݹ黥���ź���
#define configUSE_MALLOC_FAILED_HOOK							0		//1ʹ���ڴ�����ʧ�ܹ��Ӻ���
#define configUSE_APPLICATION_TASK_TAG						0
#define configUSE_COUNTING_SEMAPHORES							1		//Ϊ1ʱʹ�ü����ź���

/***************************************************************************************************************/
/*                                FreeRTOS������ʱ�������״̬�ռ��йص�����ѡ��                                 */
/***************************************************************************************************************/
/* Run time and task stats gathering related definitions. */
//#define configGENERATE_RUN_TIME_STATS             0		 //Ϊ1ʱ��������ʱ��ͳ�ƹ���
#define configUSE_TRACE_FACILITY									1		 //ʹ�ܴ����ý���Ӷ���Ľṹ���Ա�ͺ������Դ���Э�����ӻ��͸��٣���ʹ��IAR�е�FreeRTOS���ʱҪʹ��������ã������޷���ʾ����ջ��ʹ�����
//#define configUSE_STATS_FORMATTING_FUNCTIONS      1		 //���configUSE_TRACE_FACILITYͬʱΪ1ʱ���������3������:prvWriteNameToBuffer(),vTaskList(),vTaskGetRunTimeStats()                                                                      
//#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()  (ulHighFrequencyTimerTicks = 0ul)
//#define portGET_RUN_TIME_COUNTER_VALUE()          ulHighFrequencyTimerTicks

/***************************************************************************************************************/
/*                                FreeRTOS�빳�Ӻ����йص�����ѡ��                                              */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK												0   //1��ʹ�ÿ��й��ӣ�0����ʹ��
#define configUSE_TICK_HOOK												0   //1��ʹ��ʱ��Ƭ���ӣ�0����ʹ��

/***************************************************************************************************************/
/*                                FreeRTOS���ڴ������й�����ѡ��                                                */
/***************************************************************************************************************/
#define configSUPPORT_DYNAMIC_ALLOCATION        	1             									//֧�ֶ�̬�ڴ�����
#define configTOTAL_HEAP_SIZE											((size_t)(64*1024))     				//ϵͳ�����ܵĶѴ�С

/***************************************************************************************************************/
/*                                FreeRTOS��Э���йص�����ѡ��                                                  */
/***************************************************************************************************************/
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 										0																//Ϊ1ʱ����Э�̣�����Э���Ժ��������ļ�croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES 					(2)													    //Э�̵���Ч���ȼ���Ŀ

/***************************************************************************************************************/
/*                                FreeRTOS��ѡ��������ѡ��                                                      */
/***************************************************************************************************************/
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet		 							1
#define INCLUDE_uxTaskPriorityGet		 							1
#define INCLUDE_vTaskDelete				 								1
#define INCLUDE_vTaskCleanUpResources	 						1
#define INCLUDE_vTaskSuspend			 								1
#define INCLUDE_vTaskDelayUntil			 							1
#define INCLUDE_vTaskDelay				 								1
#define INCLUDE_xEventGroupSetBitFromISR 					1
#define INCLUDE_xTimerPendFunctionCall   					0

/***************************************************************************************************************/
/*                                FreeRTOS�������ʱ���йص�����ѡ��                                             */
/***************************************************************************************************************/
/* Software timer definitions. */
//#define configUSE_TIMERS			 										0														  //Ϊ1ʱ���������ʱ��
//#define configTIMER_TASK_PRIORITY    							(configMAX_PRIORITIES-1)		  //�����ʱ�����ȼ�
//#define configTIMER_QUEUE_LENGTH     							20														//�����ʱ�����г���
//#define configTIMER_TASK_STACK_DEPTH 							(configMINIMAL_STACK_SIZE * 2)//�����ʱ�������ջ��С

/***************************************************************************************************************/
/*                                FreeRTOS���ж��йص�����ѡ��                                                   */
/***************************************************************************************************************/
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY		0x0f													//�ж�������ȼ�

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	0x01											//ϵͳ�ɹ��������ж����ȼ�

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
/* Normal assert() semantics without relying on the provision of an assert.h header file. */
//#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }		
#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
#define configASSERT( x )  if( ( x ) == 0 ) vAssertCalled(__FILE__,__LINE__)

/***************************************************************************************************************/
/*                                FreeRTOS���жϷ������йص�����ѡ��                                           */
/***************************************************************************************************************/	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler	
	
#endif /* FREERTOS_CONFIG_H */

