/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Middlewares/Third_Party/FatFs/ff13b/extend_functions/FasFt_User_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    28-September-2018
  * @brief   The template of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __FASFT_USER_BSP_H
#define __FASFT_USER_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h7xx_hal.h"
#include "ff.h"
/*******************************************APP/BSP*************************************************/
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup FasFt_User
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup FasFt_User_Exported_define FasFt_User Exported define
  * @{
  */
/**
  * @brief  FasFt_User definition
  * @note	FasFt_User
  */
#define FILE_MAX_TYPE_NUM		7	//���FILE_MAX_TYPE_NUM������
#define FILE_MAX_SUBT_NUM		4	//���FILE_MAX_SUBT_NUM��С��
/* ���ڲ��Զ�д�ٶ� */

#define TEST_FILE_LEN			(2*1024*1024)	/* ���ڲ��Ե��ļ����� */
#define BUF_SIZE				(1024)			/* ÿ�ζ�дSD����������ݳ��� */

//Bsp_File_Tell���ص����Ͷ���
//���ݱ�FILE_TYPE_TBL���.��Bsp_FatFs_ExtendFunctions.c���涨��
#define T_BIN		0X00	//bin�ļ�
#define T_LRC		0X10	//lrc�ļ�

#define T_NES		0X20	//nes�ļ�
#define T_SMS		0X21	//sms�ļ�

#define T_TEXT		0X30	//.txt�ļ�
#define T_C			0X31	//.c�ļ�
#define T_H			0X32    //.h�ļ�

#define T_WAV		0X40	//WAV�ļ�
#define T_MP3		0X41	//MP3�ļ� 
#define T_APE		0X42	//APE�ļ�
#define T_FLAC		0X43	//FLAC�ļ�

#define T_BMP		0X50	//bmp�ļ�
#define T_JPG		0X51	//jpg�ļ�
#define T_JPEG		0X52	//jpeg�ļ�		 
#define T_GIF		0X53	//gif�ļ�  
 
#define T_AVI		0X60	//avi�ļ�  
/** @}
*/
/*------------------------------------FasFt_User Exported define-----------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/** @defgroup FasFt_User_Exported_constants FasFt_User Exported constants
  * @{
  */
/**
  * @brief  FasFt_User constants definition
  * @note	FasFt_User
  */
extern FATFS *fs[FF_VOLUMES];  
extern FIL *file;	 
extern FIL *ftemp;	 
extern UINT br,bw;
extern FILINFO fileinfo;
extern DIR dir;
extern uint8_t *fatbuf;//SD�����ݻ�����
extern BYTE Buff[4096] __attribute__ ((aligned (4))) ;	/* Working buffer */
extern char ReadDataBuff[BUF_SIZE];
extern char WriteDataBuff[BUF_SIZE];
/** @}
*/
/*----------------------------------------FasFt_User Exported constants--------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup FasFt_User_Exported_Functions FasFt_User Exported Functions
  * @{
  */
/** @defgroup FasFt_User_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
uint8_t Bsp_FatFs_Init(void);													//�����ڴ�
/** @}
*/
/*********************FasFt_User Exported Functions Group1***************************/
/** @defgroup FasFt_User_Exported_Functions_Group2 Operation Functions
  * @{
  */
uint8_t Bsp_FatFs_File_Tell(uint8_t *fname);											//ʶ���ļ�����
uint8_t Bsp_FatFs_GetDisk_Volume(uint8_t *drv,uint32_t *total,uint32_t *free);		//�õ�������������ʣ������
uint8_t Bsp_FatFs_CopyFile(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t totsize,uint32_t cpdsize,uint8_t fwmode);	   //�ļ�����
uint8_t Bsp_FatFs_CopyFolder(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t *totsize,uint32_t *cpdsize,uint8_t fwmode);//�ļ��и���	  
uint8_t* Bsp_FatFs_GetSrc_Dname(uint8_t* dpfn);																		   
uint32_t Bsp_FatFs_GetFile_Size(uint8_t *fdname);										//�õ��ļ��д�С

uint8_t Bsp_FatFs_ViewDir(const TCHAR* path);
uint8_t Bsp_FatFs_CreateNewFile(const TCHAR* path, const TCHAR* fname);
uint8_t Bsp_FatFs_CreateDir(const TCHAR* path);
uint8_t Bsp_FatFs_DeleteDirFile(const TCHAR* path, const TCHAR* fname);
uint8_t Bsp_FatFs_ReadFileData(const TCHAR* path, const TCHAR* fname, char* ReadDataBuff);
uint8_t Bsp_FatFs_WriteFileData(const TCHAR* path, const TCHAR* fname, char* WriteDataBuff);
/** @}
*/
/**********************FasFt_User Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------FasFt_User Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------FasFt_User------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __FASFT_USER_BSP_H */

/**********************************************END OF FILE******************************************/
