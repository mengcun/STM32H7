/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/FLASH/Spi_Flash_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 14-September-2018
  * @brief   Flash�ͺ�Ϊ W25Q256, ����Ϊ256Mbit = 32MByte, ������СΪ4096�ֽ�(4kB)��8192������,16������Ϊһ���飬��512���飬 ���ʱ��(104MHz)
  *			 ʹ��Quad SPI Mode:
  * 		 PB2/QSPI_BK1_SCK
  * 		 PB6/QSPI_BK1_CS
  * 		 PF8/QSPI_BK1_IO0
  * 		 PF9/QSPI_BK1_IO1
  * 		 PF7/QSPI_BK1_IO2
  * 		 PF6/QSPI_BK1_IO3
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The SPI_FLASH canbe used as follows:
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
/** @defgroup SPI_FLASH SPI_FLASH
  * @brief SPI_FLASH 
  * @{
  */


/* Private variables -------------------------------------------------------------------*/
/** @defgroup SPI_FLASH SPI_FLASH Private variables
  * @{
  */
/**
  * @brief The Handler for QSPI
  */  
QSPI_HandleTypeDef 		QSPI_Handler;

uint16_t W25QXX_TYPE	=	W25Q256;	//Ĭ����W25Q256
uint8_t  W25QXX_QPI_MODE	=	0;		//QSPIģʽ��־:0,SPIģʽ;1,QPIģʽ.

uint32_t TotalSize;						/* ������ */
uint16_t PageSize;						/* ҳ���С */

uint8_t TEXT_Buffer[4 * 1024];
	
uint32_t iTime_Start, iTime_End;		//	��¼��ʼ�ͽ���ʱ��

/** @}
*/		
/*--------------------------------------SPI_FLASH Private variables--------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup SPI_FLASH_Exported_Functions SPI_FLASH Exported Functions
  * @{
  */
/** @defgroup SPI_FLASH_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize QSPI
  @endverbatim
  * @{
  */
/**
  * @brief Initialize the W25QXX according to the specified
  */  
uint8_t Bsp_W25QXX_Init(void)
{ 
    uint8_t temp;    
	Bsp_QSPI_Init();							//��ʼ��QSPI
 	Bsp_W25QXX_QSPI_Enable();					//ʹ��QSPIģʽ
	W25QXX_TYPE	=	Bsp_W25QXX_ReadID();		//��ȡFLASH ID.
#if W25QXX_DEBUG == 1
	Bsp_Printf("The ID of the W25QXX mounted on board is:%x\r\n",W25QXX_TYPE);
#endif
	if(W25QXX_TYPE==W25Q256)       		 		//SPI FLASHΪW25Q256
    {
        temp=Bsp_W25QXX_ReadSR(3);      		//��ȡ״̬�Ĵ���3���жϵ�ַģʽ
        if((temp&0X01)==0)						//�������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ
		{ 
			Bsp_W25QXX_Write_Enable();			//дʹ��
			Bsp_QSPI_Send_CMD(W25X_Enable4ByteAddr,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);//QPI,ʹ��4�ֽڵ�ַָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ����� 
		}
		Bsp_W25QXX_Write_Enable();				//дʹ��
		Bsp_QSPI_Send_CMD(W25X_SetReadParam,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES); 		//QPI,���ö�����ָ��,��ַΪ0,4�ߴ�����_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,1���ֽ�����
		temp=3<<4;								//����P4&P5=11,8��dummy clocks,104M
		Bsp_QSPI_Transmit(&temp,1);				//����1���ֽ�	
		return 0;
    }
	else
	{
		return 1;
	}
} 

/**
  * @brief Initialize the QSPI according to the specified
  */
uint8_t Bsp_QSPI_Init(void)
{
    QSPI_Handler.Instance				=	QUADSPI;                          		//QSPI
    QSPI_Handler.Init.ClockPrescaler	=	1;                     					//QPSI��Ƶ�ȣ�W25Q256���Ƶ��Ϊ104M�����Դ˴�Ӧ��Ϊ1��QSPIƵ�ʾ�Ϊ200/(1+1)=100MHZ
    QSPI_Handler.Init.FifoThreshold		=	4;                      				//FIFO��ֵΪ4���ֽ�
    QSPI_Handler.Init.SampleShifting	=	QSPI_SAMPLE_SHIFTING_HALFCYCLE;			//������λ�������(DDRģʽ��,��������Ϊ0)
    QSPI_Handler.Init.FlashSize			=	POSITION_VAL(0X2000000)-1;  			//SPI FLASH��С��W25Q256��СΪ32M�ֽ�
    QSPI_Handler.Init.ChipSelectHighTime=	QSPI_CS_HIGH_TIME_5_CYCLE;				//Ƭѡ�ߵ�ƽʱ��Ϊ5��ʱ��(10*5=55ns),���ֲ������tSHSL����
    QSPI_Handler.Init.ClockMode			=	QSPI_CLOCK_MODE_0;          			//ģʽ0
    QSPI_Handler.Init.FlashID			=	QSPI_FLASH_ID_1;              			//��һƬflash
    QSPI_Handler.Init.DualFlash			=	QSPI_DUALFLASH_DISABLE;     			//��ֹ˫����ģʽ
	
    if(HAL_QSPI_Init(&QSPI_Handler)==HAL_OK)
	{
		return 0;      //QSPI��ʼ���ɹ�
	}		
    else 
	{
		return 1;
	}
}

/**
  * @brief Initialize the QSPI according to the specified
  * @param hqspi: QSPI ���
  */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_QSPI_CLK_ENABLE();        //ʹ��QSPIʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();       //ʹ��GPIOBʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();       //ʹ��GPIOFʱ��
    
    //��ʼ��PB6 Ƭѡ�ź�
    GPIO_Initure.Pin=GPIO_PIN_6;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          	//����
    GPIO_Initure.Pull=GPIO_PULLUP;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//����
    GPIO_Initure.Alternate=GPIO_AF10_QUADSPI;   	//����ΪQSPI
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //PF8,9
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Pull=GPIO_NOPULL;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;   //����
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //PB2
    GPIO_Initure.Pin=GPIO_PIN_2;
    GPIO_Initure.Alternate=GPIO_AF9_QUADSPI;   		//����ΪQSPI
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //PF6,7
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}
/** @}
*/
/*********************SPI_FLASH Exported Functions Group1**************************/

/** @defgroup SPI_FLASH_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the QSPI
  @endverbatim
  * @{
  */
/**
  * @brief QSPI��������
  * @param param: brief.
  * @param instruction:Ҫ���͵�ָ��
  * @param address:���͵���Ŀ�ĵ�ַ
  * @param dummyCycles:��ָ��������
  * @param instructionMode:ָ��ģʽ;QSPI_INSTRUCTION_NONE,QSPI_INSTRUCTION_1_LINE,QSPI_INSTRUCTION_2_LINE,QSPI_INSTRUCTION_4_LINE  
  * @param addressMode:��ַģʽ; QSPI_ADDRESS_NONE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_2_LINE,QSPI_ADDRESS_4_LINE
  * @param addressSize:��ַ����;QSPI_ADDRESS_8_BITS,QSPI_ADDRESS_16_BITS,QSPI_ADDRESS_24_BITS,QSPI_ADDRESS_32_BITS
  * @param dataMode:����ģʽ; QSPI_DATA_NONE,QSPI_DATA_1_LINE,QSPI_DATA_2_LINE,QSPI_DATA_4_LINE
  */

void Bsp_QSPI_Send_CMD(uint32_t instruction,uint32_t address,uint32_t dummyCycles,uint32_t instructionMode,uint32_t addressMode,uint32_t addressSize,uint32_t dataMode)
{
    QSPI_CommandTypeDef Cmdhandler;
    
    Cmdhandler.Instruction=instruction;                 	//ָ��
    Cmdhandler.Address=address;                            	//��ַ
    Cmdhandler.DummyCycles=dummyCycles;                     //���ÿ�ָ��������
    Cmdhandler.InstructionMode=instructionMode;				//ָ��ģʽ
    Cmdhandler.AddressMode=addressMode;   					//��ַģʽ
    Cmdhandler.AddressSize=addressSize;   					//��ַ����
    Cmdhandler.DataMode=dataMode;             				//����ģʽ
    Cmdhandler.SIOOMode=QSPI_SIOO_INST_EVERY_CMD;       	//ÿ�ζ�����ָ��
    Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_NONE; //�޽����ֽ�
    Cmdhandler.DdrMode=QSPI_DDR_MODE_DISABLE;           	//�ر�DDRģʽ
    Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_ANALOG_DELAY;

    HAL_QSPI_Command(&QSPI_Handler,&Cmdhandler,5000);
}

/**
  * @brief Receive data from the QSPI ����ָ�����ȵ�����
  * @param buf:�������ݻ������׵�ַ
  * @param datalen:Ҫ��������ݳ���
  * @retval status:0,����,����,�������
  */

uint8_t Bsp_QSPI_Receive(uint8_t* buf,uint32_t datalen)
{
    QSPI_Handler.Instance->DLR = datalen-1;                         //�������ݳ���
    if(HAL_QSPI_Receive(&QSPI_Handler,buf,5000)==HAL_OK)
	{
		return 0;
	}
    else 
	{
		return 1;
	}
}

/**
  * @brief Send data through the QSPI ����ָ�����ȵ�����
  * @param buf:�������ݻ������׵�ַ
  * @param datalen:Ҫ��������ݳ���
  * @retval status:0,����,����,�������
  */

uint8_t Bsp_QSPI_Transmit(uint8_t* buf,uint32_t datalen)
{
    QSPI_Handler.Instance->DLR=datalen-1;                            //�������ݳ���
    if(HAL_QSPI_Transmit(&QSPI_Handler,buf,5000)==HAL_OK) 
	{
		return 0;
	}
    else 
	{
		return 1;
	}
}

/**
  * @brief W25QXX����QSPIģʽ 
  */

void Bsp_W25QXX_QSPI_Enable(void)
{
	uint8_t 	stareg2;
	stareg2	=	Bsp_W25QXX_ReadSR(2);		//�ȶ���״̬�Ĵ���2��ԭʼֵ
	if((stareg2&0X02)==0)					//QEλδʹ��
	{
		Bsp_W25QXX_Write_Enable();			//дʹ�� 
		stareg2|=1<<1;						//ʹ��QEλ		
		Bsp_W25QXX_Write_SR(2,stareg2);		//д״̬�Ĵ���2
	}
	//дcommandָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,0���ֽ�����
	Bsp_QSPI_Send_CMD(W25X_EnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
	W25QXX_QPI_MODE=1;					//���QSPIģʽ
}

/**
  * @brief W25QXX�˳�QSPIģʽ 
  */

void W25QXX_QSPI_Disable(void)
{ 
	//дcommandָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ�����
	Bsp_QSPI_Send_CMD(W25X_ExitQPIMode,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
	W25QXX_QPI_MODE=0;				//���SPIģʽ
}

/**
  * @brief ��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ��� 
	       ״̬�Ĵ���1��
	       BIT7  6   5   4   3   2   1   0
	       SPR   RV  TB BP2 BP1 BP0 WEL BUSY
	       SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
	       TB,BP2,BP1,BP0:FLASH����д��������
	       WEL:дʹ������
	       BUSY:æ���λ(1,æ;0,����)
	       Ĭ��:0x00
	       ״̬�Ĵ���2��
	       BIT7  6   5   4   3   2   1   0
	       SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
	       ״̬�Ĵ���3��
	       BIT7      6    5    4   3   2   1   0
	       HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
  * @param regno:״̬�Ĵ����ţ���:1~3
  * @return ����ֵ:״̬�Ĵ���ֵ
  */

uint8_t Bsp_W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //��״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //��״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //��״̬�Ĵ���3ָ��
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }   
	if(W25QXX_QPI_MODE)
	{
		//QPI,дcommandָ��,��ַΪ0,4�ߴ�����_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,1���ֽ�����	
		Bsp_QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);	
	}
	else 
	{
		//SPI,дcommandָ��,��ַΪ0,���ߴ�����_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,1���ֽ�����
		Bsp_QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);					
	}
	
	Bsp_QSPI_Receive(&byte,1);	        
	return byte;   
}   

/**
  * @brief дW25QXX״̬�Ĵ��� 
  * @param regno:״̬�Ĵ����ţ���Χ:1~3
  * @param sr: Ҫд���ֵ
  */

void Bsp_W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{   
    uint8_t command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //д״̬�Ĵ���1ָ��
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //д״̬�Ĵ���2ָ��
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //д״̬�Ĵ���3ָ��
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    }   
	if(W25QXX_QPI_MODE)
	{
		//QPI,дcommandָ��,��ַΪ0,4�ߴ�����_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,1���ֽ�����
		Bsp_QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);	
	}
	else 
	{
		//SPI,дcommandָ��,��ַΪ0,���ߴ�����_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,1���ֽ�����
		Bsp_QSPI_Send_CMD(command,0,0, QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);				
	}
	
	Bsp_QSPI_Transmit(&sr,1);	         	      
}  

/**
  * @brief W25QXXдʹ�ܣ���S1�Ĵ�����WEL��λ	
  */

void Bsp_W25QXX_Write_Enable(void)   
{
	if(W25QXX_QPI_MODE)
	{
		//QPI,дʹ��ָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ�����
		Bsp_QSPI_Send_CMD(W25X_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);	
	}
	else 
	{
		//SPI,дʹ��ָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,0���ֽ�����	
		Bsp_QSPI_Send_CMD(W25X_WriteEnable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);	
	}
} 

/**
  * @brief W25QXXд��ֹ��S1�Ĵ�����WEL����
  */

void Bsp_W25QXX_Write_Disable(void)   
{  
	if(W25QXX_QPI_MODE)
	{
		//QPI,д��ָֹ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ�����
		Bsp_QSPI_Send_CMD(W25X_WriteDisable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
	}
	else 
	{
		//SPI,д��ָֹ��,��ַΪ0,������_8λ��ַ_�޵�ַ_���ߴ���ָ��,�޿�����,0���ֽ����� 
		Bsp_QSPI_Send_CMD(W25X_WriteDisable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);	
	}
} 

/**
  * @brief  ��ȡW25QXX ��ID
  * @return 0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
		    0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
			0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
			0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
			0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
			0XEF18,��ʾоƬ�ͺ�ΪW25Q256
  */

uint16_t Bsp_W25QXX_ReadID(void)
{
	uint8_t temp[2];
	uint16_t deviceid;
	if(W25QXX_QPI_MODE)
	{
		//QPI,��id,��ַΪ0,�޿�����,4�ߴ���ָ��_4�ߴ����ַ_24λ��ַ_4�ߴ�������,2���ֽ�����
		Bsp_QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
	}
	else 
	{
		//SPI,��id,��ַΪ0,�޿�����,1�ߴ���ָ��_1�ߴ����ַ_24λ��ַ_1�ߴ�������,2���ֽ�����
		Bsp_QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE);			
	}
	
	Bsp_QSPI_Receive(temp,2);
	deviceid=(temp[0]<<8)|temp[1];
	return deviceid;
}    

/**
  * @brief  ��ȡW25QXX ����Ϣ
  * @return 0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
		    0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
			0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
			0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
			0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
			0XEF18,��ʾоƬ�ͺ�ΪW25Q256
  */

void Bsp_Read_FLASH_Info(void)
{
	uint16_t Flash_ID = 0;
	Flash_ID = Bsp_W25QXX_ReadID();
	switch (Flash_ID)
	{
		case 0XEF13:
			TotalSize = 1 * 1024 * 1024;
			PageSize = 4 * 1024;
			Bsp_Printf("The Flash mounted on board is W25Q80! TotalSize is 8M bits(1M BYTES), The PageSize is 4KB\r\n");
			break;
		case 0XEF14:
			TotalSize = 2 * 1024 * 1024;
			PageSize = 4 * 1024;
			Bsp_Printf("The Flash mounted on board is W25Q16! TotalSize is 16M bits(2M BYTES), The PageSize is 4KB\r\n");
			break;
		case 0XEF15:
			TotalSize = 4 * 1024 * 1024;
			PageSize = 4 * 1024;
			Bsp_Printf("The Flash mounted on board is W25Q32! TotalSize is 32M bits(4M BYTES), The PageSize is 4KB\r\n");
			break;
		case 0XEF16:
			TotalSize = 8 * 1024 * 1024;
			PageSize = 4 * 1024;
			Bsp_Printf("The Flash mounted on board is W25Q64! TotalSize is 64M bits(8M BYTES), The PageSize is 4KB\r\n");
			break;
		case 0XEF17:
			TotalSize = 16 * 1024 * 1024;
			PageSize = 4 * 1024;
			Bsp_Printf("The Flash mounted on board is W25Q128! TotalSize is 128M bits(16M BYTES), The PageSize is 4KB\r\n");
			break;
		case 0XEF18:
			TotalSize = 32 * 1024 * 1024;
			PageSize = 4 * 1024;
			Bsp_Printf("The Flash mounted on board is W25Q256! TotalSize is 256M bits(32M BYTES), The PageSize is 4KB\r\n");
			break;		
		default:
			Bsp_Printf("Unknow Flash, Please check the FLASH!\r\n");
			break;
	}
}

/**
  * @brief ��ȡSPI FLASH,��ָ����ַ��ʼ��ȡָ�����ȵ�����,��֧��QPIģʽ
  * @param pBuffer:���ݴ洢��
  * @param ReadAddr:��ʼ��ȡ�ĵ�ַ(���32bit)
  * @param NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
  */

void Bsp_W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
	//QPI,���ٶ�����,��ַΪReadAddr,8������,4�ߴ���ָ��_4�ߴ����ַ_32λ��ַ_4�ߴ�������,NumByteToRead������
	Bsp_QSPI_Send_CMD(W25X_FastReadData,ReadAddr,8,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES);	
	Bsp_QSPI_Receive(pBuffer,NumByteToRead); 
}  

/**
  * @brief SPI��һҳ(0~65535)��д������256���ֽڵ�����,��ָ����ַ��ʼд�����256�ֽڵ�����
  * @param pBuffer:���ݴ洢��
  * @param WriteAddr:��ʼд��ĵ�ַ(���32bit)
  * @param NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	
  */

void Bsp_W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	Bsp_W25QXX_Write_Enable();
	//QPI,ҳдָ��,��ַΪWriteAddr,�޿�����,4�ߴ���ָ��_4�ߴ����ַ_32λ��ַ_4�ߴ�������,NumByteToWrite������
	Bsp_QSPI_Send_CMD(W25X_PageProgram,WriteAddr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES);	
	Bsp_QSPI_Transmit(pBuffer,NumByteToWrite);	         	      
	Bsp_W25QXX_Wait_Busy();
} 

/**
  * @brief 	�޼���дSPI FLASH,����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
  * 		�����Զ���ҳ����,��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
  * @param  pBuffer:���ݴ洢��
  * @param  WriteAddr:��ʼд��ĵ�ַ(���32bit)
  * @param  NumByteToWrite:Ҫд����ֽ���(���65535)	
  */

void Bsp_W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	 	
	pageremain = 256 - WriteAddr%256; 						//��ҳʣ����ֽ���		 	    
	if(NumByteToWrite <= pageremain)
	{
		pageremain=NumByteToWrite;							//������256���ֽ�
	}
	while(1)
	{	   
		Bsp_W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite == pageremain)
		{
			break;											//д�������
		}
	 	else 												//NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	

			NumByteToWrite -= pageremain;			  			//��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite > 256)pageremain = 256; 			//һ�ο���д��256���ֽ�
			else pageremain = NumByteToWrite; 	  				//����256���ֽ���
		}
	}   
} 

/**
  * @brief 	дSPI FLASH, ��ָ����ַ��ʼд��ָ�����ȵ�����,�ú�������������!
  * @param  pBuffer:���ݴ洢��
  * @param  WriteAddr:��ʼд��ĵ�ַ(���32bit)
  * @param  NumByteToWrite:Ҫд����ֽ���(���65535)	
  */ 
uint8_t W25QXX_BUFFER[4096];		 
void Bsp_W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t *W25QXX_BUF;	  
   	W25QXX_BUF = W25QXX_BUFFER;	     
 	secpos = WriteAddr/4096;											//������ַ  
	secoff = WriteAddr%4096;											//�������ڵ�ƫ��
	secremain = 4096 - secoff;											//����ʣ��ռ��С   
#if W25QXX_DEBUG == 1
 	Bsp_Printf("Write address:%X,Number of bytes to write:%X\r\n",WriteAddr,NumByteToWrite);//������
#endif
 	if(NumByteToWrite<=secremain)secremain = NumByteToWrite;			//������4096���ֽ�
	while(1) 
	{	
		Bsp_W25QXX_Read(W25QXX_BUF,secpos*4096,4096);					//������������������
		for(i=0;i<secremain;i++)										//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;						//��Ҫ����  	  
		}
		if(i<secremain)													//��Ҫ����
		{
			Bsp_W25QXX_Erase_Sector(secpos);							//�����������
			for(i=0;i<secremain;i++)	   								//����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			Bsp_W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);		//д����������  
		}
		else 
		{
			Bsp_W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);		//д�Ѿ������˵�,ֱ��д������ʣ������.	
		} 				   
		if(NumByteToWrite==secremain)
		{
			break;														//д�������
		}
		else															//д��δ����
		{
			secpos++;													//������ַ��1
			secoff=0;													//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  										//ָ��ƫ��
			WriteAddr+=secremain;										//д��ַƫ��	   
		   	NumByteToWrite-=secremain;									//�ֽ����ݼ�
			if(NumByteToWrite>4096)
			{
				secremain=4096;											//��һ����������д����
			}
			else 
			{
				secremain=NumByteToWrite;								//��һ����������д����
			}
		}	 
	}	 
}

/**
  * @brief 	��������оƬ,�ȴ�ʱ�䳬��...	
  */ 

void Bsp_W25QXX_Erase_Chip(void)   
{                                   
    Bsp_W25QXX_Write_Enable();
    Bsp_W25QXX_Wait_Busy();   
	//QPI,дȫƬ����ָ��,��ַΪ0,������_8λ��ַ_�޵�ַ_4�ߴ���ָ��,�޿�����,0���ֽ�����
	Bsp_QSPI_Send_CMD(W25X_ChipErase,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
	Bsp_W25QXX_Wait_Busy();						//�ȴ�оƬ��������
} 

/**
  * @brief 	����һ������,����һ������������ʱ��:150ms
  * @param  st_Addr:������ַ ����ʵ����������
  */ 

void Bsp_W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
#if W25QXX_DEBUG == 1	 
 	Bsp_Printf("Erase the Sector in:%x\r\n",Dst_Addr);		//����falsh�������,������  	  
#endif
 	Dst_Addr*=4096;
    Bsp_W25QXX_Write_Enable();                  	 
    Bsp_W25QXX_Wait_Busy();  
	//QPI,д��������ָ��,��ַΪ0,������_32λ��ַ_4�ߴ����ַ_4�ߴ���ָ��,�޿�����,0���ֽ�����
	Bsp_QSPI_Send_CMD(W25X_SectorErase,Dst_Addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
    Bsp_W25QXX_Wait_Busy();   				    			//�ȴ��������
}

/**
  * @brief 	�ж�дPAGEǰ�Ƿ���Ҫ�Ȳ�����
  * @param  _ucpOldBuf:������
  * @param  _ucpNewBuf:������
  * @param  _uiLen:���ݸ��������ܳ���ҳ���С
  * @return 0:����Ҫ������ 1����Ҫ����
  */ 

uint8_t Bsp_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucOld;

	/*
	�㷨��1����old ��, new ����
	      old    new
		  1101   0101
	~     1111
		= 0010   0101

	�㷨��2��: old �󷴵Ľ���� new λ��
		  0010   old
	&	  0101   new
		 =0000

	�㷨��3��: ���Ϊ0,���ʾ�������. �����ʾ��Ҫ����
	*/

	for (i = 0; i < _usLen; i++)
	{
		ucOld = *_ucpOldBuf++;
		ucOld = ~ucOld;

		/* ע������д��: if (ucOld & (*_ucpNewBuf++) != 0) */
		if ((ucOld & (*_ucpNewBuf++)) != 0)
		{
			return 1;
		}
	}
	return 0;
}
/**
  * @brief 	�ȴ�����
  */ 

void Bsp_W25QXX_Wait_Busy(void)   
{   
	while((Bsp_W25QXX_ReadSR(1)&0x01)==0x01);   // �ȴ�BUSYλ���
}   

/**
  * @brief 	����QSPI�Ķ�FLASH,����ʾ��
  * @param  pBuffer:���ݴ洢��
  * @param  ReadAddr:��ʼд��ĵ�ַ(���32bit)
  * @param  NumByteToRead:Ҫд����ֽ���(���65535)	
  */
void Bsp_ReadTest(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
	uint16_t i;
	iTime_Start = Bsp_GetCPURunTime();	/* ���¿�ʼʱ�� */
	Bsp_W25QXX_Read(TEXT_Buffer, ReadAddr, NumByteToRead);
	iTime_End = Bsp_GetCPURunTime();	/* ���½���ʱ�� */
	Bsp_Printf("Write Flash success,The datas are:\r\n");

	/* ��ӡ���� */
	for (i = 0; i < NumByteToRead; i++)
	{
		Bsp_Printf(" %02X", TEXT_Buffer[i]);

		if ((i & 31) == 31)
		{
			Bsp_Printf("\r\n");	/* ÿ����ʾ16�ֽ����� */
		}
		else if ((i & 31) == 15)
		{
			Bsp_Printf(" - ");
		}
	}

	/* ��ӡ���ٶ� */
	Bsp_Printf("Read Test: From %x, Data length: %d Bytes, Read time: %dms, Read speed: %d Bytes/s\r\n", ReadAddr, NumByteToRead,  iTime_End - iTime_Start, (NumByteToRead * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	����QSPI�Ķ�FLASH���ٶȡ�
  * @param  pBuffer:���ݴ洢��
  * @param  ReadAddr:��ʼд��ĵ�ַ(���32bit)
  * @param  NumByteToRead:Ҫд����ֽ���(���65535)	
  */
void Bsp_ReadSpeedTest(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
	uint16_t i;
	uint32_t uiAddr;
	
	iTime_Start = Bsp_GetCPURunTime();	/* ���¿�ʼʱ�� */
	uiAddr = 0;
	for (i = 0; i < TotalSize / NumByteToRead; i++, uiAddr += NumByteToRead)
	{
		Bsp_W25QXX_Read(TEXT_Buffer, ReadAddr, NumByteToRead);
	}
	iTime_End = Bsp_GetCPURunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	Bsp_Printf("Read The whole chip, Data length: %d Bytes, Read time: %dms, Read speed: %d Bytes/s\r\n", TotalSize,  iTime_End - iTime_Start, (TotalSize * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	����QSPI��дFLASH,������д�롣
  * @param  pBuffer:���ݴ洢��
  * @param  WriteAddr:��ʼд��ĵ�ַ(���32bit)
  * @param  NumByteToWrite:Ҫд����ֽ���(���65535)	
  */
void Bsp_Write_With_Check_Test(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t i;
	/* �����Ի����� */
	for (i = 0; i < NumByteToWrite; i++)
	{
		TEXT_Buffer[i] = i;
	}

	iTime_Start = Bsp_GetCPURunTime();	/* ���¿�ʼʱ�� */
	Bsp_W25QXX_Write(TEXT_Buffer,WriteAddr, NumByteToWrite);
	iTime_End = Bsp_GetCPURunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	Bsp_Printf("Write with check Test: From %x, Data length: %d Bytes, Write time: %dms, Write speed: %d Bytes/s\r\n", WriteAddr, NumByteToWrite,  iTime_End - iTime_Start, (NumByteToWrite * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	����QSPI��дFLASH,��������д�롣
  * @param  pBuffer:���ݴ洢��
  * @param  WriteAddr:��ʼд��ĵ�ַ(���32bit)
  * @param  NumByteToWrite:Ҫд����ֽ���(���65535)	
  */
void Bsp_Write_Without_Check_Test(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t i;
	/* �����Ի����� */
	for (i = 0; i < NumByteToWrite; i++)
	{
		TEXT_Buffer[i] = i;
	}

	iTime_Start = Bsp_GetCPURunTime();	/* ���¿�ʼʱ�� */
	Bsp_W25QXX_Write_NoCheck(TEXT_Buffer,WriteAddr, NumByteToWrite);
	iTime_End = Bsp_GetCPURunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	Bsp_Printf("Write without check Test: From %x, Data length: %d Bytes, Write time: %dms, Write speed: %d Bytes/s\r\n", WriteAddr, NumByteToWrite,  iTime_End - iTime_Start, (NumByteToWrite * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	����QSPI��ȫдFLASH,��ҳ��д��ÿ��д��4K Bytes
  * @param _data: Ҫд�������
  * @param  pBuffer:���ݴ洢��
  * @param  WriteAddr:��ʼд��ĵ�ַ(���32bit)
  * @param  NumByteToWrite:Ҫд����ֽ���(���65535)	
  */
void Bsp_Write_All_Page_Test(uint8_t _data, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t i;
	/* �����Ի����� */
	for (i = 0; i < NumByteToWrite; i++)
	{
		TEXT_Buffer[i] = _data;
	}

	iTime_Start = Bsp_GetCPURunTime();	/* ���¿�ʼʱ�� */
	for (i = 0; i < TotalSize / PageSize; i++)
	{
		Bsp_W25QXX_Write_Page(TEXT_Buffer, i * PageSize, PageSize);
		Bsp_Printf(".");
		if (((i + 1) % 128) == 0)
		{
			Bsp_Printf("\r\n");
		}
	}
	iTime_End = Bsp_GetCPURunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	Bsp_Printf("Data length: %dK Bytes, Write time: %dms, Write seed: %d Bytes/s\r\n", TotalSize / 1024,  iTime_End - iTime_Start, (TotalSize * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	����QSPI��ȫ����FLASH��
  */
void Bsp_Erase_ChipTest(void)
{
	iTime_Start = Bsp_GetCPURunTime();	/* ���¿�ʼʱ�� */
	Bsp_W25QXX_Erase_Chip();
	iTime_End = Bsp_GetCPURunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	Bsp_Printf("Erash the whole Flash complete! Spand Time: %dms\r\n",  iTime_End - iTime_Start);
	return;
}

/**
  * @brief 	����QSPI�Ĳ���һ��������FLASH��
  * @param _data: Ҫ��������ʼ��ַ
  */
void Bsp_Erase_Page_Test(uint32_t Dst_Addr)
{
	iTime_Start = Bsp_GetCPURunTime();	/* ���¿�ʼʱ�� */
	Bsp_W25QXX_Erase_Sector(Dst_Addr);
	iTime_End = Bsp_GetCPURunTime();	/* ���½���ʱ�� */

	/* ��ӡ���ٶ� */
	Bsp_Printf("Erash the whole Flash complete! Spand Time: %dms\r\n",  iTime_End - iTime_Start);
	return;
}


/** @}
*/
/****************************QSPI Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------SPI_FLASH Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------SPI_FLASH-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
