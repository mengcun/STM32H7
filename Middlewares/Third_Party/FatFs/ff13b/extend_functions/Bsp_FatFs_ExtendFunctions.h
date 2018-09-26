#ifndef __BSP_FATFS_EXTENDFUNCTIONS_H
#define __BSP_FATFS_EXTENDFUNCTIONS_H 					   
#include "Coreboard_Bsp.h"
#include "ff.h"

extern FATFS *fs[FF_VOLUMES];  
extern FIL *file;	 
extern FIL *ftemp;	 
extern UINT br,bw;
extern FILINFO fileinfo;
extern DIR dir;
extern uint8_t *fatbuf;//SD�����ݻ�����


#define FILE_MAX_TYPE_NUM		7	//���FILE_MAX_TYPE_NUM������
#define FILE_MAX_SUBT_NUM		4	//���FILE_MAX_SUBT_NUM��С��

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

 
uint8_t Bsp_FatFs_Init(void);													//�����ڴ�
uint8_t Bsp_File_Tell(uint8_t *fname);											//ʶ���ļ�����
uint8_t Bsp_GetDisk_Volume(uint8_t *drv,uint32_t *total,uint32_t *free);		//�õ�������������ʣ������
uint8_t Bsp_CopyFile(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t totsize,uint32_t cpdsize,uint8_t fwmode);	   //�ļ�����
uint8_t Bsp_CopyFolder(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t *totsize,uint32_t *cpdsize,uint8_t fwmode);//�ļ��и���	  
uint8_t* Bsp_GetSrc_Dname(uint8_t* dpfn);																		   
uint32_t Bsp_GetFile_Size(uint8_t *fdname);										//�õ��ļ��д�С	
#endif


