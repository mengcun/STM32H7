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
SD_HandleTypeDef        SDCARD_Handler;             //SD�����  

/**
  * @brief The Infor Struct for SD
  */ 
HAL_SD_CardInfoTypeDef  SDCardInfo;					//SD����Ϣ�ṹ��

/**
  * @brief The DMA_Handle for SD
  */ 
DMA_HandleTypeDef SDTxDMAHandler,SDRxDMAHandler;    //SD��DMA���ͺͽ��վ��

/**
  * @brief ���ʹ��DMA�Ļ��������������������SD����д�Ƿ����
  */ 
static volatile uint8_t SDCardWriteStatus=0,SDCardReadStatus=0; 

/**
  * @brief SD_ReadDisk/SD_WriteDisk����ר��buf,�����������������ݻ�������ַ����4�ֽڶ����ʱ��,��Ҫ�õ�������,ȷ�����ݻ�������ַ��4�ֽڶ����.
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
  * @brief SD����ʼ��,SDMMCƵ��Ϊ200MHz,SD�����Ƶ��25MHz,��ʼ��ʱ��ʱ�Ӳ��ܴ���400KHZ 
  * @retval 0 ��ʼ����ȷ������ֵ����ʼ������
  */
uint8_t Bsp_SD_Init(void)
{
    uint8_t SD_Error;	
    SDCARD_Handler.Instance = SDMMC1;
    SDCARD_Handler.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;              		//������     
    SDCARD_Handler.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;  		//����ʱ���ر�ʱ�ӵ�Դ
    SDCARD_Handler.Init.BusWide = SDMMC_BUS_WIDE_4B;                      		//4λ������
    SDCARD_Handler.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;//�ر�Ӳ������
    SDCARD_Handler.Init.ClockDiv = SDMMC_NSpeed_CLK_DIV;               			//SD����ʱ��Ƶ�����25MHZ
    
    SD_Error = HAL_SD_Init(&SDCARD_Handler);
    if(SD_Error!=HAL_OK) 
	{
		return 1;
	}
	//��ȡSD����Ϣ
	HAL_SD_GetCardInfo(&SDCARD_Handler,&SDCardInfo);
    return 0;
}

/**
  * @brief SDMMC�ײ�������ʱ��ʹ�ܣ��������ã�DMA����,�˺����ᱻHAL_SD_Init()����
  * @param hsd:SD�����
  */
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_SDMMC1_CLK_ENABLE();  //ʹ��SDMMC1ʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();   //ʹ��GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();   //ʹ��GPIODʱ��
    
    //PC8,9,10,11,12
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF12_SDIO1; //����ΪSDIO
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //��ʼ��
    
    //PD2
    GPIO_Initure.Pin=GPIO_PIN_2;            
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     //��ʼ��

#if (SD_DMA_MODE==1)                        //ʹ��DMAģʽ
    HAL_NVIC_SetPriority(SDMMC1_IRQn,2,0);  						//����SDMMC1�ж�����SD����DMA���䣬��ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);        //ʹ��SDMMC1�ж�
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
  * @brief �õ�������Ϣ
  * @param cardinfo:����Ϣ�洢��
  * @retval ����״̬
  */
uint8_t Bsp_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    uint8_t sta;
	sta=HAL_SD_GetCardInfo(&SDCARD_Handler,cardinfo);
    return sta;
}

/**
  * @brief 	�ж�SD���Ƿ���Դ���(��д)����
  * @param 	cardinfo:����Ϣ�洢��
  * @retval SD_TRANSFER_OK ������ɣ����Լ�����һ�δ���
			SD_TRANSFER_BUSY SD����æ�������Խ�����һ�δ���
  */

uint8_t Bsp_SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&SDCARD_Handler)==HAL_SD_CARD_TRANSFER )?SD_TRANSFER_OK:SD_TRANSFER_BUSY);
}

#if (SD_DMA_MODE==1)        //DMAģʽ

/**
  * @brief 	DMA��ʽ��SD��
  * @param 	buf:�����ݻ�����
  * @param 	sector:������ַ
  * @param 	cnt:��������
  * @retval 0,����;����,�������;
  */

uint8_t Bsp_SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_ERROR;
	SDCardReadStatus=0;
	
	if(HAL_SD_ReadBlocks_DMA(&SDCARD_Handler,(uint8_t*)buf,(uint32_t)sector,(uint32_t)cnt)==HAL_OK)
	{
		while(SDCardReadStatus==0){};	//�ȴ������
		
		SDCardReadStatus=0;
		while(Bsp_SD_GetCardState()){};		//�ȴ�SD������
		sta=HAL_OK;
	}
	
    return sta;
}  

/**
  * @brief 	DMA��ʽдSD��
  * @param 	buf:�����ݻ�����
  * @param 	sector:������ַ
  * @param 	cnt:��������
  * @retval 0,����;����,�������;
  */

uint8_t Bsp_SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{   
    uint8_t sta=HAL_ERROR;
	
	SDCardWriteStatus=0;
	
	if(HAL_SD_WriteBlocks_DMA(&SDCARD_Handler,(uint8_t*)buf,(uint32_t)sector,(uint32_t)cnt)==HAL_OK)
	{
		while(SDCardWriteStatus==0){};	//�ȴ�д���
		
		SDCardWriteStatus=0;
		while(Bsp_SD_GetCardState()){};		//�ȴ�SD������
		sta=HAL_OK;
	}
		
    return sta;
} 

/**
  * @brief SDMMC1�жϷ�����
  */

void SDMMC1_IRQHandler(void)
{
    HAL_SD_IRQHandler(&SDCARD_Handler);
}

/**
  * @brief SDMMC1д��ɻص�����
  */

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
	SDCardWriteStatus=1; //���д���
}

/**
  * @brief SDMMC1����ɻص�����
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
	SDCardReadStatus=1;	//��Ƕ����
}

#else  //��ѯģʽ

/**
  * @brief 	LOOPING��ʽ��SD��
  * @param 	buf:�����ݻ�����
  * @param 	sector:������ַ
  * @param 	cnt:��������
  * @retval 0,����;����,�������;
  */

uint8_t Bsp_SD_ReadDisk(uint8_t* buf,uint32_t sector,uint32_t cnt)
{
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    DISABLE_INT();//�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
	sta=HAL_SD_ReadBlocks(&SDCARD_Handler, (uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//���sector�Ķ�����
	
	//�ȴ�SD������
	while(Bsp_SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
		}
    }
    ENABLE_INT();//�������ж�
    return sta;
}  

/**
  * @brief 	LOOPING��ʽдSD��
  * @param 	buf:�����ݻ�����
  * @param 	sector:������ַ
  * @param 	cnt:��������
  * @retval 0,����;����,�������;
  */

uint8_t Bsp_SD_WriteDisk(uint8_t *buf,uint32_t sector,uint32_t cnt)
{   
    uint8_t sta=HAL_OK;
	uint32_t timeout=SD_TIMEOUT;
    long long lsector=sector;
    DISABLE_INT();//�ر����ж�(POLLINGģʽ,�Ͻ��жϴ��SDIO��д����!!!)
	sta=HAL_SD_WriteBlocks(&SDCARD_Handler,(uint8_t*)buf,lsector,cnt,SD_TIMEOUT);//���sector��д����
		
	//�ȴ�SD��д��
	while(Bsp_SD_GetCardState()!=SD_TRANSFER_OK)
    {
		if(timeout-- == 0)
		{	
			sta=SD_TRANSFER_BUSY;
		}
    }    
	ENABLE_INT();//�������ж�
    return sta;
}
#endif

/**
  * @brief SD�������Ϣ
  */
void Bsp_Show_SD_Info(void)
{
	uint64_t CardCap;	//SD������
	HAL_SD_CardCIDTypedef SDCard_CID;

	HAL_SD_GetCardCID(&SDCARD_Handler,&SDCard_CID);	//��ȡCID
	Bsp_SD_GetCardInfo(&SDCardInfo);				//��ȡSD����Ϣ
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
	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);		//����SD������
  	Bsp_Printf("Card ManufacturerID:%d\r\n",SDCard_CID.ManufacturerID);						//������ID
 	Bsp_Printf("Card RCA:%d\r\n",SDCardInfo.RelCardAdd);									//����Ե�ַ
	Bsp_Printf("Logic BlockNbr:%d \r\n",(uint32_t)(SDCardInfo.LogBlockNbr));				//��ʾ�߼�������
	Bsp_Printf("Logic BlockSize:%d \r\n",(uint32_t)(SDCardInfo.LogBlockSize));				//��ʾ�߼����С
	Bsp_Printf("Card Capacity:%d MB\r\n",(uint32_t)(CardCap>>20));							//��ʾ����
 	Bsp_Printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.BlockSize);							//��ʾ���С
}

/**
  * @brief 	����SD���Ķ�ȡ,	��secaddr��ַ��ʼ,��ȡseccnt������������
  * @param 	secaddr:������ַ
  * @param 	seccnt:������
  */

void Bsp_SD_ReadTest(uint32_t secaddr,uint32_t seccnt)
{
	uint32_t i; uint8_t *buf; uint8_t sta=0;
	buf = Bsp_mymalloc(SRAMEX,seccnt*512);		//�����ڴ�
	sta = Bsp_SD_ReadDisk(buf,secaddr,seccnt);	//��ȡsecaddr������ʼ������
	if(sta==0)						
	{	 
		Bsp_Printf("SECTOR %d DATA:\r\n",secaddr);
		for(i=0;i<seccnt*512;i++)
		{
			Bsp_Printf("%x ",buf[i]);				//��ӡsecaddr��ʼ���������� 
		}			
		Bsp_Printf("\r\nDATA ENDED\r\n"); 
	}
	else 
	{
		Bsp_Printf("err:%d\r\n",sta);
	}
	Bsp_myfree(SRAMEX,buf);						//�ͷ��ڴ�	   
}

/**
  * @brief 	����SD����д��(����,���дȫ��0XFF������,���������SD��.)��secaddr��ַ��ʼ,д��seccnt������������
  * @param 	secaddr:������ַ
  * @param 	seccnt:������
  */

void Bsp_SD_WriteTest(uint32_t secaddr,uint32_t seccnt)
{
	uint32_t i;
	uint8_t *buf; uint8_t sta=0; 
	buf=Bsp_mymalloc(SRAMEX,seccnt*512);		    //��SDRAM�����ڴ�
	for(i=0;i<seccnt*512;i++) 
	{
		buf[i]=i*3; 		//��ʼ��д�������,��3�ı���.
	}
	sta=Bsp_SD_WriteDisk(buf,secaddr,seccnt);		//��secaddr������ʼд��seccnt����������
	if(sta==0) 
	{
		Bsp_Printf("Write over!\r\n");
	}		
    else
	{
		Bsp_Printf("err:%d\r\n",sta);
	}
	Bsp_myfree(SRAMEX,buf);					    //�ͷ��ڴ�	   
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
