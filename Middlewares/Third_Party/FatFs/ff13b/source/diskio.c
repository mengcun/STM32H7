/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"				/* FatFs lower layer API */
#include "Sd_Bsp.h"				/* SD CARD lower layer API */
#include "Spi_Flash_Bsp.h"		/* FLASH W25Q256 lower layer API */
#include "Malloc_Bsp.h"			/* Memory Management lower layer API */

/* Definitions of physical drive number for each drive */
#define DEV_SDCARD	0	/* Example: Map SD CARD to physical drive 0 */
#define DEV_W25Q256		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

//对于W25Q256
//前25M字节给fatfs用,25M字节后,用于存放字库,字库占用6.01M.	剩余部分,自己用	 
#define FATFS_FLASH_SECTOR_SIZE 	512	
#define FATFS_FLASH_SECTOR_COUNT 	1024*25*2	//W25Q256,前25M字节给FATFS占用	
#define FATFS_FLASH_BLOCK_SIZE   	8     		//每个BLOCK有8个扇区		
  
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) 
	{
		case DEV_SDCARD :
			result = Bsp_SD_GetCardState();
			stat = result;
			return stat;
		
		case DEV_W25Q256 :
			result = Bsp_W25QXX_ReadSR(1);
			stat = (result & 0x01);
			return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	int result;

	switch (pdrv) 
	{
		case DEV_SDCARD :
			result = Bsp_SD_Init();
			break;

		case DEV_W25Q256 :
			result = Bsp_W25QXX_Init();
			break;
		
		default:
			result=1; 	
			break;
	}
	if(result)
	{
		return  STA_NOINIT;
	}
	else
	{		
		return 0;
	}
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	int result;
	
    if (!count)
	{
		return RES_PARERR;		//count不能等于0，否则返回参数错误		 	 
	}
	switch (pdrv) 
	{
		case DEV_SDCARD :
			result = Bsp_SD_ReadDisk(buff, sector, count);
			while(result)			//读出错
			{
				Bsp_SD_Init();		//重新初始化SD卡
				result=Bsp_SD_ReadDisk(buff,sector,count);	
			}
			break;
			
		case DEV_W25Q256 :
			for(;count>0;count--)
			{
				Bsp_W25QXX_Read(buff,sector*FATFS_FLASH_SECTOR_SIZE,FATFS_FLASH_SECTOR_SIZE);
				sector++;
				buff += FATFS_FLASH_SECTOR_SIZE;
			}
			result = 0;
			break;
		default:
			result = 1; 
			break;
		
		}

	if(result == 0x00)
	{
		return  RES_OK;
	}
	else
	{		
		return RES_ERROR;
	}
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	int result;

	switch (pdrv) 
	{
		case DEV_SDCARD :
			result = Bsp_SD_WriteDisk((uint8_t*)buff, sector, count);
			while(result)			//读出错
			{
				Bsp_SD_Init();		//重新初始化SD卡
				result=Bsp_SD_WriteDisk((uint8_t*)buff,sector,count);	
			}
			break;

		case DEV_W25Q256 :
			for(;count>0;count--)
			{
				Bsp_W25QXX_Write((uint8_t*)buff,sector*FATFS_FLASH_SECTOR_SIZE,FATFS_FLASH_SECTOR_SIZE);
				sector++;
				buff += FATFS_FLASH_SECTOR_SIZE;
			}
			result = 0;
			break;

	}

	if(result == 0x00)
	{
		return  RES_OK;
	}
	else
	{		
		return RES_ERROR;
	}
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	if(pdrv == DEV_SDCARD)
	{
		switch(cmd)
		{
			case CTRL_SYNC:
				res = RES_OK; 
				break;
			case GET_SECTOR_SIZE:
				*(DWORD*)buff = 512; 
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				*(WORD*)buff = SDCardInfo.LogBlockSize;
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff = SDCardInfo.LogBlockNbr;
				res = RES_OK;
				break;
			default:
				res = RES_PARERR;
				break;
		}
	}	
	else if(pdrv == DEV_W25Q256)
	{	
		switch(cmd)
		{
			case CTRL_SYNC:
				res = RES_OK; 
				break;
			case GET_SECTOR_SIZE:
				*(WORD*)buff = FATFS_FLASH_SECTOR_SIZE;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				*(WORD*)buff = FATFS_FLASH_BLOCK_SIZE;
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff = FATFS_FLASH_SECTOR_COUNT;
				res = RES_OK;
				break;
			default:
				res = RES_PARERR;
				break;
		}
	}
	else 
	{
		res=RES_ERROR;
	}
	
    return res;

}

/* User defined function to give a current time to fatfs module      */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}			 
//动态分配内存
void *ff_memalloc (UINT size)			
{
	return (void*)Bsp_mymalloc(SRAMIN,size);
}
//释放内存
void ff_memfree (void* mf)		 
{
	Bsp_myfree(SRAMIN,mf);
}
