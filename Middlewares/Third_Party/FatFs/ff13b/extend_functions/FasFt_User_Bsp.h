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
#define FILE_MAX_TYPE_NUM		7	//最多FILE_MAX_TYPE_NUM个大类
#define FILE_MAX_SUBT_NUM		4	//最多FILE_MAX_SUBT_NUM个小类
/* 用于测试读写速度 */

#define TEST_FILE_LEN			(2*1024*1024)	/* 用于测试的文件长度 */
#define BUF_SIZE				(1024)			/* 每次读写SD卡的最大数据长度 */

//Bsp_File_Tell返回的类型定义
//根据表FILE_TYPE_TBL获得.在Bsp_FatFs_ExtendFunctions.c里面定义
#define T_BIN		0X00	//bin文件
#define T_LRC		0X10	//lrc文件

#define T_NES		0X20	//nes文件
#define T_SMS		0X21	//sms文件

#define T_TEXT		0X30	//.txt文件
#define T_C			0X31	//.c文件
#define T_H			0X32    //.h文件

#define T_WAV		0X40	//WAV文件
#define T_MP3		0X41	//MP3文件 
#define T_APE		0X42	//APE文件
#define T_FLAC		0X43	//FLAC文件

#define T_BMP		0X50	//bmp文件
#define T_JPG		0X51	//jpg文件
#define T_JPEG		0X52	//jpeg文件		 
#define T_GIF		0X53	//gif文件  
 
#define T_AVI		0X60	//avi文件  
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
extern uint8_t *fatbuf;//SD卡数据缓存区
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
uint8_t Bsp_FatFs_Init(void);													//申请内存
/** @}
*/
/*********************FasFt_User Exported Functions Group1***************************/
/** @defgroup FasFt_User_Exported_Functions_Group2 Operation Functions
  * @{
  */
uint8_t Bsp_FatFs_File_Tell(uint8_t *fname);											//识别文件类型
uint8_t Bsp_FatFs_GetDisk_Volume(uint8_t *drv,uint32_t *total,uint32_t *free);		//得到磁盘总容量和剩余容量
uint8_t Bsp_FatFs_CopyFile(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t totsize,uint32_t cpdsize,uint8_t fwmode);	   //文件复制
uint8_t Bsp_FatFs_CopyFolder(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t *totsize,uint32_t *cpdsize,uint8_t fwmode);//文件夹复制	  
uint8_t* Bsp_FatFs_GetSrc_Dname(uint8_t* dpfn);																		   
uint32_t Bsp_FatFs_GetFile_Size(uint8_t *fdname);										//得到文件夹大小

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
