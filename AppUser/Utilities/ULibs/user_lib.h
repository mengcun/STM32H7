/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/Utilities/ULibs/User_Lib.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    21-August-2018
  * @brief   The User_Lib of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __USER_LIB_H
#define __USER_LIB_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
#include <stdint.h>
/***************************************Include StdLib**********************************************/
/*******************************************APP/BSP*************************************************/
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup User_Lib
  * @{
  */ 
	 
/** @defgroup User_Lib_Exported_Macro User_Lib Exported Macro
  * @{
  */
/**
  * @brief  User_Lib Macro definition
	* @note		User_Lib
  */
	//CODE HERE
/** @}
*/
/*----------------------------------------User_Lib Exported Macro---------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup User_Lib_Exported_Functions User_Lib Exported Functions
  * @{
  */
/** @defgroup User_Lib_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
//CODE HERE
/** @}
*/
/*********************User_Lib Exported Functions Group1***************************/
/** @defgroup User_Lib_Exported_Functions_Group2 Operation Functions
  * @{
  */
int str_len(char *_str);
void str_cpy(char *_tar, char *_src);
int str_cmp(char * s1, char * s2);
void mem_set(char *_tar, char _data, int _len);

void int_to_str(int _iNumber, char *_pBuf, unsigned char _len);
int str_to_int(char *_pStr);

uint16_t BEBufToUint16(uint8_t *_pBuf);
uint16_t LEBufToUint16(uint8_t *_pBuf);

uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen) ;
int32_t  CaculTwoPoint(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x);
uint8_t char_upper(uint8_t c);

/** @}
*/
/**********************User_Lib Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------User_Lib Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------User_Lib------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __USER_LIB_H */

/**********************************************END OF FILE******************************************/
