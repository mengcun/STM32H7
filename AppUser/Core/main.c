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
/** @}
*/		
/*----------------------------APP_MAIN Private Functions Prototypes--------------------------------*/ 

/* Private variables-------------------------------------------------------------------*/
/** @defgroup APP_MAIN_Private_variables APP_MAIN Private variables
  * @{
  */
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
	/* -1- Coreboard Init:CPU L1 Cache Enable; STM32H7xx HAL library initialization; System Clock 400MHz */
	Coreboard_BSP_Init();	
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
