#ifndef __BSP_FATFS_BASICFUNCTIONS_H
#define __BSP_FATFS_BASICFUNCTIONS_H 			   
#include "ff.h"
#include "Coreboard_Bsp.h"	   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//FATFS 测试代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
    
extern BYTE Buff[4096] __attribute__ ((aligned (4))) ;	/* Working buffer */

uint8_t Bsp_fmount_SDCARD(uint8_t* path,uint8_t mt);
uint8_t Bsp_fmount_FLASH(uint8_t* path,uint8_t mt);

uint8_t Bsp_fopen(uint8_t*path,uint8_t mode);
uint8_t Bsp_fclose(void);
uint8_t Bsp_fread(uint16_t len);
uint8_t Bsp_fwrite(uint8_t*dat,uint16_t len);

uint8_t Bsp_fopendir(uint8_t* path);
uint8_t Bsp_fclosedir(void);
uint8_t Bsp_freaddir(void);

uint8_t Bsp_fscan_files(uint8_t * path);
uint8_t Bsp_flseek(uint32_t offset);

uint8_t Bsp_fputc(uint8_t c);
uint8_t Bsp_fputs(uint8_t*c);

uint8_t Bsp_fmkdir(uint8_t*pname);
uint8_t Bsp_fmkfs(uint8_t* path,uint8_t mode,uint16_t au, void* Buff, uint16_t SizeOfBuff);
uint8_t Bsp_funlink(uint8_t *pname);
uint8_t Bsp_frename(uint8_t *oldname,uint8_t* newname);

uint32_t Bsp_fshowfree(uint8_t *drv);
uint32_t Bsp_ftell(void);
uint32_t Bsp_fsize(void); 
void Bsp_fgetlabel(uint8_t *path);
void Bsp_fsetlabel(uint8_t *path); 
void Bsp_fgets(uint16_t size);

#endif





























