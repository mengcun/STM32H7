/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/Modules/SysInfoTest.c
  * @author  MCD Application Team
  * @version SW:V1.0.0 HW:V1.0
  * @date    15-August-2018
  * @brief   Ϊ�˻�ȡFreeRTOS��������Ϣ����Ҫ����һ����ʱ���������ʱ����ʱ���׼����Ҫ����
	*              ϵͳʱ�ӽ��ġ������õ���������Ϣ��׼ȷ��
	*              ���ļ��ṩ�ĺ��������ڲ���Ŀ�ģ��в��ɽ�������ʵ����Ŀ��ԭ�������㣺
	*               1. FreeRTOS��ϵͳ�ں�û�ж��ܵļ���ʱ�������������
	*               2. ��ʱ���ж���50us����һ�Σ��Ƚ�Ӱ��ϵͳ���ܡ�
	*              --------------------------------------------------------------------------------------
	*              ���ļ�ʹ�õ���32λ����������50usһ�εļ���ֵ�����֧�ּ���ʱ�䣺
	*              2^32 * 50us / 3600s = 59.6���ӡ�ʹ���в��Ե��������м���������ռ���ʳ�����59.6���ӽ���׼ȷ��
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
/***************************************Include StdLib**********************************************/
/*******************************************APP/BSP*************************************************/
#include "stm32h743ii_Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup SystemInfoTest SystemInfoTest
  * @brief SystemInfoTest
  * @{
  */

/* Private Defines -------------------------------------------------------------------*/
/** @defgroup SystemInfoTest_Private_Defines SystemInfoTest Private Defines
  * @{
  */
/**
  * @brief  SystemInfoTest Private definition
  */
/* ��ʱ��Ƶ�ʣ�50usһ���ж� */
#define  timerINTERRUPT_FREQUENCY	20000

/* �ж����ȼ� */
#define  timerHIGHEST_PRIORITY		2

/* ��ϵͳ���� */
volatile uint32_t ulHighFrequencyTimerTicks = 0UL;	

/** @}
*/
/*----------------------------------------APP_MAIN Private Define----------------------------------*/
	
/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup Template_Private_Functions Template Private Functions
  * @{
  */
/**
  * @brief 
  * @param param: brief.
  * @retval status
  */
//CODING HERE Private Functions
/** @}
*/
/*--------------------------------------Template Private Functions---------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup Template_Exported_Functions Template Exported Functions
  * @{
  */
/** @defgroup Template_Exported_Functions_Group1 Initialization and de-initialization functions
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
  * @brief Initialize the XXXX according to the specified
  * @param param: brief.
  * @retval status
  */
//CODING HERE Function
/** @}
*/
/*********************Template Exported Functions Group1**************************/

/** @defgroup Template_Exported_Functions_Group2 Operation Functions
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
/**
  * @brief 
  * @param param: brief.
  * @retval status
  */
//CODING HERE Function
/** @}
*/
/****************************XXXX Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------Template Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------Template-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
