/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/SD_MMC/Adc_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 26-September-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The SD_MMC canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup SD_MMC SD_MMC
  * @brief SD_MMC
	* @note	SD_MMC
  * @{
  */
/* Private variables -------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Private_c DAC_LFSR_NOISE Private variables
  * @{
  */
/**
  * @brief The Handler for SD
  */  
SD_HandleTypeDef        SDCARD_Handler;             //SD卡句柄  

/**
  * @brief The Infor Struct for SD
  */ 
HAL_SD_CardInfoTypeDef  SDCardInfo;					//SD卡信息结构体

/**
  * @brief The DMA_Handle for SD
  */ 
DMA_HandleTypeDef SDTxDMAHandler,SDRxDMAHandler;    //SD卡DMA发送和接收句柄

/**
  * @brief 如果使用DMA的话下面两个变量用来标记SD卡读写是否完成
  */ 
static volatile uint8_t SDCardWriteStatus=0,SDCardReadStatus=0; 

/**
  * @brief SD_ReadDisk/SD_WriteDisk函数专用buf,当这两个函数的数据缓存区地址不是4字节对齐的时候,需要用到该数组,确保数据缓存区地址是4字节对齐的.
  */
__align(4) uint8_t SDIO_DATA_BUFFER[512];

/** @}
*/		
/*----------------------------ADC Private variables--------------------------------*/ 
  
/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup SD_MMC_Exported_Functions SD_MMC Exported Functions
  * @{
  */
/** @defgroup SD_MMC_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize SD_MMC
  @endverbatim
  * @{
  */
/**
  * @brief SD卡初始化,SDMMC频率为200MHz,SD卡最大频率25MHz,初始化时的时钟不能大于400KHZ 
  * @retval 0 初始化正确；其他值，初始化错误
  */
uint8_t Bsp_SD_Init(void)
{
    uint8_t SD_Error;	
    SDCARD_Handler.Instance = SDMMC1;
    SDCARD_Handler.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;              		//上升沿     
    SDCARD_Handler.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;  		//空闲时不关闭时钟电源
    SDCARD_Handler.Init.BusWide = SDMMC_BUS_WIDE_4B;                      		//4位数据线
    SDCARD_Handler.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;//关闭硬件流控
    SDCARD_Handler.Init.ClockDiv = SDMMC_NSpeed_CLK_DIV;               			//SD传输时钟频率最大25MHZ
    
    SD_Error = HAL_SD_Init(&SDCARD_Handler);
    if(SD_Error!=HAL_OK) 
	{
		return 1;
	}
	//获取SD卡信息
	HAL_SD_GetCardInfo(&SDCARD_Handler,&SDCardInfo);
    return 0;
}

/**
  * @brief SDMMC底层驱动，时钟使能，引脚配置，DMA配置,此函数会被HAL_SD_Init()调用
  * @param hsd:SD卡句柄
  */
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_SDMMC1_CLK_ENABLE();  //使能SDMMC1时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
    
    //PC8,9,10,11,12
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF12_SDIO1; //复用为SDIO
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //初始化
    
    //PD2
    GPIO_Initure.Pin=GPIO_PIN_2;            
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //初始化

#if (SD_DMA_MODE==1)                        //使用DMA模式
    HAL_NVIC_SetPriority(SDMMC1_IRQn,2,0);  						//设置SDMMC1中断用于SD卡的DMA传输，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);        //使能SDMMC1中断
#endif
}

/** @}
*/
/*********************SD_MMC Exported Functions Group1**************************/

/** @defgroup SD_MMC_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the SD_MMC
  @endverbatim
  * @{
  */
/**
  * @brief 得到卡的信息
  * @param cardinfo:卡信息存储区
  * @retval 错误状态
  */
uint8_t Bsp_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    uint8_t sta;
	sta=HAL_SD_GetCardInfo(&SDCARD_Handler,cardinfo);
    return sta;
}

/**
  * @brief 	判断SD卡是否可以传输(读写)数据
  * @param 	cardinfo:卡信息存储区
  * @retval SD_TRANSFER_OK 传输完成，可以继续下一次传输
			SD_TRANSFER_BUSY SD卡正忙，不可以进行下一次传输
  */

uint8_t Bsp_SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&SDCARD_Handler)==HAL_SD_CARD_TRANSFER )?SD_TRANSFER_OK:SD_TRANSFER_BUSY);
}

#if (SD_DMA_MODE==1)        //DMA模式

/**
  * @brief 	DMA方式读SD卡
  * @param 	buf:读数据缓存区
  * @param 	sector:扇区地址
  * @param 	cnt:扇区个数
  * @retval 0,正常;其他,错误代码;
  */

uint8_t Bsp_SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_ERROR;
	SDCardReadStatus=0;
	
	if(HAL_SD_ReadBlocks_DMA(&SDCARD_Handler,(uint8_t*)buf,(uint32_t)sector,(uint32_t)cnt)==HAL_OK)
	{
		while(SDCardReadStatus==0){};	//等待读完成
		
		SDCardReadStatus=0;
		while(Bsp_SD_GetCardState()){};		//等待SD卡空闲
		sta=HAL_OK;
	}
	
    return sta;
}  

/**
  * @brief 	DMA方式写SD卡
  * @param 	buf:读数据缓存区
  * @param 	sector:扇区地址
  * @param 	cnt:扇区个数
  * @retval 0,正常;其他,错误代码;
  */

uint8_t Bsp_SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{   
    uint8_t sta=HAL_ERROR;
	
	SDCardWriteStatus=0;
	
	if(HAL_SD_WriteBlocks_DMA(&SDCARD_Handler,(uint8_t*)buf,(uint32_t)sector,(uint32_t)cnt)==HAL_OK)
	{
		while(SDCardWriteStatus==0){};	//等待写完成
		
		SDCardWriteStatus=0;
		while(Bsp_SD_GetCardState()){};		//等待SD卡空闲
		sta=HAL_OK;
	}
		
    return sta;
} 

/**
  * @brief SDMMC1中断服务函数
  */

void SDMMC1_IRQHandler(void)
{
    HAL_SD_IRQHandler(&SDCARD_Handler);
}

/**
  * @brief SDMMC1写完成回调函数
  */

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
	SDCardWriteStatus=1; //标记写完成
}

/**
  * @brief SDMMC1读完成回调函数
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
	SDCardReadStatus=1;	//标记读完成
}

#else  //轮询模式

/**
  * @brief 	LOOPING方式读SD卡
  * @param 	buf:读数据缓存区
  * @param 	sector:扇区地址
  * @param 	cnt:扇区个数
  * @retval 0,正常;其他,错误代码;
  */

uint8_t Bsp_SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    DISABLE_INT();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_ReadBlocks(&SDCARD_Handler, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的读操作
	
	//等待SD卡读完
	while(Bsp_SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
		}
    }
    ENABLE_INT();//开启总中断
    return sta;
}  

/**
  * @brief 	LOOPING方式写SD卡
  * @param 	buf:读数据缓存区
  * @param 	sector:扇区地址
  * @param 	cnt:扇区个数
  * @retval 0,正常;其他,错误代码;
  */

uint8_t Bsp_SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{   
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    DISABLE_INT();//关闭总中断(POLLING模式,严禁中断打断SDIO读写操作!!!)
	sta=HAL_SD_WriteBlocks(&SDCARD_Handler,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//多个sector的写操作
		
	//等待SD卡写完
	while(Bsp_SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
		}
    }    
	ENABLE_INT();//开启总中断
    return sta;
}
#endif

/**
  * @brief SD卡相关信息
  */
void Bsp_Show_SD_Info(void)
{
	uint64_t CardCap;	//SD卡容量
	HAL_SD_CardCIDTypedef SDCard_CID;

	HAL_SD_GetCardCID(&SDCARD_Handler,&SDCard_CID);	//获取CID
	Bsp_SD_GetCardInfo(&SDCardInfo);				//获取SD卡信息
	switch(SDCardInfo.CardType)
	{
		case CARD_SDSC:
		{
			if(SDCardInfo.CardVersion == CARD_V1_X)
			{
				Bsp_Printf("Card Type:SDSC V1\r\n");
			}
			else if(SDCardInfo.CardVersion == CARD_V2_X)
			{
				Bsp_Printf("Card Type:SDSC V2\r\n");
			}
		}
		break;
		case CARD_SDHC_SDXC:
			Bsp_Printf("Card Type:SDHC\r\n");
		break;
	}	
	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);		//计算SD卡容量
  	Bsp_Printf("Card ManufacturerID:%d\r\n",SDCard_CID.ManufacturerID);						//制造商ID
 	Bsp_Printf("Card RCA:%d\r\n",SDCardInfo.RelCardAdd);									//卡相对地址
	Bsp_Printf("Logic BlockNbr:%d \r\n",(uint32_t)(SDCardInfo.LogBlockNbr));				//显示逻辑块数量
	Bsp_Printf("Logic BlockSize:%d \r\n",(uint32_t)(SDCardInfo.LogBlockSize));				//显示逻辑块大小
	Bsp_Printf("Card Capacity:%d MB\r\n",(uint32_t)(CardCap>>20));							//显示容量
 	Bsp_Printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.BlockSize);							//显示块大小
}

/**
  * @brief 	测试SD卡的读取,	从secaddr地址开始,读取seccnt个扇区的数据
  * @param 	secaddr:扇区地址
  * @param 	seccnt:扇区数
  */

void Bsp_SD_ReadTest(uint32_t secaddr,uint32_t seccnt)
{
	uint32_t i; uint8_t *buf; uint8_t sta=0;
	buf = Bsp_mymalloc(SRAMEX,seccnt*512);		//申请内存
	sta = Bsp_SD_ReadDisk(buf,secaddr,seccnt);	//读取secaddr扇区开始的内容
	if(sta==0)						
	{	 
		Bsp_Printf("SECTOR %d DATA:\r\n",secaddr);
		for(i=0;i<seccnt*512;i++)
		{
			Bsp_Printf("%x ",buf[i]);				//打印secaddr开始的扇区数据 
		}			
		Bsp_Printf("\r\nDATA ENDED\r\n"); 
	}
	else 
	{
		Bsp_Printf("err:%d\r\n",sta);
	}
	Bsp_myfree(SRAMEX,buf);						//释放内存	   
}

/**
  * @brief 	测试SD卡的写入(慎用,最好写全是0XFF的扇区,否则可能损坏SD卡.)从secaddr地址开始,写入seccnt个扇区的数据
  * @param 	secaddr:扇区地址
  * @param 	seccnt:扇区数
  */

void Bsp_SD_WriteTest(uint32_t secaddr,uint32_t seccnt)
{
	uint32_t i;
	uint8_t *buf; uint8_t sta=0; 
	buf=Bsp_mymalloc(SRAMEX,seccnt*512);		    //从SDRAM申请内存
	for(i=0;i<seccnt*512;i++) 
	{
		buf[i]=i*3; 		//初始化写入的数据,是3的倍数.
	}
	sta=Bsp_SD_WriteDisk(buf,secaddr,seccnt);		//从secaddr扇区开始写入seccnt个扇区内容
	if(sta==0) 
	{
		Bsp_Printf("Write over!\r\n");
	}		
    else
	{
		Bsp_Printf("err:%d\r\n",sta);
	}
	Bsp_myfree(SRAMEX,buf);					    //释放内存	   
}
/** @}
*/
/****************************SD_MMC Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------SD_MMC Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------SD_MMC-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
