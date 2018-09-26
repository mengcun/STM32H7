/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/FLASH/Spi_Flash_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 14-September-2018
  * @brief   Flash型号为 W25Q256, 容量为256Mbit = 32MByte, 扇区大小为4096字节(4kB)共8192个扇区,16个扇区为一个块，共512个块， 最大时钟(104MHz)
  *			 使用Quad SPI Mode:
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

uint16_t W25QXX_TYPE	=	W25Q256;	//默认是W25Q256
uint8_t  W25QXX_QPI_MODE	=	0;		//QSPI模式标志:0,SPI模式;1,QPI模式.

uint32_t TotalSize;						/* 总容量 */
uint16_t PageSize;						/* 页面大小 */

uint8_t TEXT_Buffer[4 * 1024];
	
uint32_t iTime_Start, iTime_End;		//	记录起始和结束时间

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
	Bsp_QSPI_Init();							//初始化QSPI
 	Bsp_W25QXX_QSPI_Enable();					//使能QSPI模式
	W25QXX_TYPE	=	Bsp_W25QXX_ReadID();		//读取FLASH ID.
#if W25QXX_DEBUG == 1
	Bsp_Printf("The ID of the W25QXX mounted on board is:%x\r\n",W25QXX_TYPE);
#endif
	if(W25QXX_TYPE==W25Q256)       		 		//SPI FLASH为W25Q256
    {
        temp=Bsp_W25QXX_ReadSR(3);      		//读取状态寄存器3，判断地址模式
        if((temp&0X01)==0)						//如果不是4字节地址模式,则进入4字节地址模式
		{ 
			Bsp_W25QXX_Write_Enable();			//写使能
			Bsp_QSPI_Send_CMD(W25X_Enable4ByteAddr,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);//QPI,使能4字节地址指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据 
		}
		Bsp_W25QXX_Write_Enable();				//写使能
		Bsp_QSPI_Send_CMD(W25X_SetReadParam,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES); 		//QPI,设置读参数指令,地址为0,4线传数据_8位地址_无地址_4线传输指令,无空周期,1个字节数据
		temp=3<<4;								//设置P4&P5=11,8个dummy clocks,104M
		Bsp_QSPI_Transmit(&temp,1);				//发送1个字节	
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
    QSPI_Handler.Init.ClockPrescaler	=	1;                     					//QPSI分频比，W25Q256最大频率为104M，所以此处应该为1，QSPI频率就为200/(1+1)=100MHZ
    QSPI_Handler.Init.FifoThreshold		=	4;                      				//FIFO阈值为4个字节
    QSPI_Handler.Init.SampleShifting	=	QSPI_SAMPLE_SHIFTING_HALFCYCLE;			//采样移位半个周期(DDR模式下,必须设置为0)
    QSPI_Handler.Init.FlashSize			=	POSITION_VAL(0X2000000)-1;  			//SPI FLASH大小，W25Q256大小为32M字节
    QSPI_Handler.Init.ChipSelectHighTime=	QSPI_CS_HIGH_TIME_5_CYCLE;				//片选高电平时间为5个时钟(10*5=55ns),即手册里面的tSHSL参数
    QSPI_Handler.Init.ClockMode			=	QSPI_CLOCK_MODE_0;          			//模式0
    QSPI_Handler.Init.FlashID			=	QSPI_FLASH_ID_1;              			//第一片flash
    QSPI_Handler.Init.DualFlash			=	QSPI_DUALFLASH_DISABLE;     			//禁止双闪存模式
	
    if(HAL_QSPI_Init(&QSPI_Handler)==HAL_OK)
	{
		return 0;      //QSPI初始化成功
	}		
    else 
	{
		return 1;
	}
}

/**
  * @brief Initialize the QSPI according to the specified
  * @param hqspi: QSPI 句柄
  */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_QSPI_CLK_ENABLE();        //使能QSPI时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();       //使能GPIOF时钟
    
    //初始化PB6 片选信号
    GPIO_Initure.Pin=GPIO_PIN_6;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          	//复用
    GPIO_Initure.Pull=GPIO_PULLUP;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  	//高速
    GPIO_Initure.Alternate=GPIO_AF10_QUADSPI;   	//复用为QSPI
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //PF8,9
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Pull=GPIO_NOPULL;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;   //高速
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //PB2
    GPIO_Initure.Pin=GPIO_PIN_2;
    GPIO_Initure.Alternate=GPIO_AF9_QUADSPI;   		//复用为QSPI
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
  * @brief QSPI发送命令
  * @param param: brief.
  * @param instruction:要发送的指令
  * @param address:发送到的目的地址
  * @param dummyCycles:空指令周期数
  * @param instructionMode:指令模式;QSPI_INSTRUCTION_NONE,QSPI_INSTRUCTION_1_LINE,QSPI_INSTRUCTION_2_LINE,QSPI_INSTRUCTION_4_LINE  
  * @param addressMode:地址模式; QSPI_ADDRESS_NONE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_2_LINE,QSPI_ADDRESS_4_LINE
  * @param addressSize:地址长度;QSPI_ADDRESS_8_BITS,QSPI_ADDRESS_16_BITS,QSPI_ADDRESS_24_BITS,QSPI_ADDRESS_32_BITS
  * @param dataMode:数据模式; QSPI_DATA_NONE,QSPI_DATA_1_LINE,QSPI_DATA_2_LINE,QSPI_DATA_4_LINE
  */

void Bsp_QSPI_Send_CMD(uint32_t instruction,uint32_t address,uint32_t dummyCycles,uint32_t instructionMode,uint32_t addressMode,uint32_t addressSize,uint32_t dataMode)
{
    QSPI_CommandTypeDef Cmdhandler;
    
    Cmdhandler.Instruction=instruction;                 	//指令
    Cmdhandler.Address=address;                            	//地址
    Cmdhandler.DummyCycles=dummyCycles;                     //设置空指令周期数
    Cmdhandler.InstructionMode=instructionMode;				//指令模式
    Cmdhandler.AddressMode=addressMode;   					//地址模式
    Cmdhandler.AddressSize=addressSize;   					//地址长度
    Cmdhandler.DataMode=dataMode;             				//数据模式
    Cmdhandler.SIOOMode=QSPI_SIOO_INST_EVERY_CMD;       	//每次都发送指令
    Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_NONE; //无交替字节
    Cmdhandler.DdrMode=QSPI_DDR_MODE_DISABLE;           	//关闭DDR模式
    Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_ANALOG_DELAY;

    HAL_QSPI_Command(&QSPI_Handler,&Cmdhandler,5000);
}

/**
  * @brief Receive data from the QSPI 接收指定长度的数据
  * @param buf:接收数据缓冲区首地址
  * @param datalen:要传输的数据长度
  * @retval status:0,正常,其他,错误代码
  */

uint8_t Bsp_QSPI_Receive(uint8_t* buf,uint32_t datalen)
{
    QSPI_Handler.Instance->DLR = datalen-1;                         //配置数据长度
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
  * @brief Send data through the QSPI 发送指定长度的数据
  * @param buf:发送数据缓冲区首地址
  * @param datalen:要传输的数据长度
  * @retval status:0,正常,其他,错误代码
  */

uint8_t Bsp_QSPI_Transmit(uint8_t* buf,uint32_t datalen)
{
    QSPI_Handler.Instance->DLR=datalen-1;                            //配置数据长度
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
  * @brief W25QXX进入QSPI模式 
  */

void Bsp_W25QXX_QSPI_Enable(void)
{
	uint8_t 	stareg2;
	stareg2	=	Bsp_W25QXX_ReadSR(2);		//先读出状态寄存器2的原始值
	if((stareg2&0X02)==0)					//QE位未使能
	{
		Bsp_W25QXX_Write_Enable();			//写使能 
		stareg2|=1<<1;						//使能QE位		
		Bsp_W25QXX_Write_SR(2,stareg2);		//写状态寄存器2
	}
	//写command指令,地址为0,无数据_8位地址_无地址_单线传输指令,无空周期,0个字节数据
	Bsp_QSPI_Send_CMD(W25X_EnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
	W25QXX_QPI_MODE=1;					//标记QSPI模式
}

/**
  * @brief W25QXX退出QSPI模式 
  */

void W25QXX_QSPI_Disable(void)
{ 
	//写command指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
	Bsp_QSPI_Send_CMD(W25X_ExitQPIMode,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
	W25QXX_QPI_MODE=0;				//标记SPI模式
}

/**
  * @brief 读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器 
	       状态寄存器1：
	       BIT7  6   5   4   3   2   1   0
	       SPR   RV  TB BP2 BP1 BP0 WEL BUSY
	       SPR:默认0,状态寄存器保护位,配合WP使用
	       TB,BP2,BP1,BP0:FLASH区域写保护设置
	       WEL:写使能锁定
	       BUSY:忙标记位(1,忙;0,空闲)
	       默认:0x00
	       状态寄存器2：
	       BIT7  6   5   4   3   2   1   0
	       SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
	       状态寄存器3：
	       BIT7      6    5    4   3   2   1   0
	       HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
  * @param regno:状态寄存器号，范:1~3
  * @return 返回值:状态寄存器值
  */

uint8_t Bsp_W25QXX_ReadSR(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //读状态寄存器1指令
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //读状态寄存器2指令
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //读状态寄存器3指令
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }   
	if(W25QXX_QPI_MODE)
	{
		//QPI,写command指令,地址为0,4线传数据_8位地址_无地址_4线传输指令,无空周期,1个字节数据	
		Bsp_QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);	
	}
	else 
	{
		//SPI,写command指令,地址为0,单线传数据_8位地址_无地址_单线传输指令,无空周期,1个字节数据
		Bsp_QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);					
	}
	
	Bsp_QSPI_Receive(&byte,1);	        
	return byte;   
}   

/**
  * @brief 写W25QXX状态寄存器 
  * @param regno:状态寄存器号，范围:1~3
  * @param sr: 要写入的值
  */

void Bsp_W25QXX_Write_SR(uint8_t regno,uint8_t sr)   
{   
    uint8_t command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //写状态寄存器1指令
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //写状态寄存器2指令
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //写状态寄存器3指令
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    }   
	if(W25QXX_QPI_MODE)
	{
		//QPI,写command指令,地址为0,4线传数据_8位地址_无地址_4线传输指令,无空周期,1个字节数据
		Bsp_QSPI_Send_CMD(command,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);	
	}
	else 
	{
		//SPI,写command指令,地址为0,单线传数据_8位地址_无地址_单线传输指令,无空周期,1个字节数据
		Bsp_QSPI_Send_CMD(command,0,0, QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);				
	}
	
	Bsp_QSPI_Transmit(&sr,1);	         	      
}  

/**
  * @brief W25QXX写使能，将S1寄存器的WEL置位	
  */

void Bsp_W25QXX_Write_Enable(void)   
{
	if(W25QXX_QPI_MODE)
	{
		//QPI,写使能指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
		Bsp_QSPI_Send_CMD(W25X_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);	
	}
	else 
	{
		//SPI,写使能指令,地址为0,无数据_8位地址_无地址_单线传输指令,无空周期,0个字节数据	
		Bsp_QSPI_Send_CMD(W25X_WriteEnable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);	
	}
} 

/**
  * @brief W25QXX写禁止将S1寄存器的WEL清零
  */

void Bsp_W25QXX_Write_Disable(void)   
{  
	if(W25QXX_QPI_MODE)
	{
		//QPI,写禁止指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
		Bsp_QSPI_Send_CMD(W25X_WriteDisable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
	}
	else 
	{
		//SPI,写禁止指令,地址为0,无数据_8位地址_无地址_单线传输指令,无空周期,0个字节数据 
		Bsp_QSPI_Send_CMD(W25X_WriteDisable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);	
	}
} 

/**
  * @brief  读取W25QXX 的ID
  * @return 0XEF13,表示芯片型号为W25Q80  
		    0XEF14,表示芯片型号为W25Q16    
			0XEF15,表示芯片型号为W25Q32  
			0XEF16,表示芯片型号为W25Q64 
			0XEF17,表示芯片型号为W25Q128 	  
			0XEF18,表示芯片型号为W25Q256
  */

uint16_t Bsp_W25QXX_ReadID(void)
{
	uint8_t temp[2];
	uint16_t deviceid;
	if(W25QXX_QPI_MODE)
	{
		//QPI,读id,地址为0,无空周期,4线传输指令_4线传输地址_24位地址_4线传输数据,2个字节数据
		Bsp_QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
	}
	else 
	{
		//SPI,读id,地址为0,无空周期,1线传输指令_1线传输地址_24位地址_1线传输数据,2个字节数据
		Bsp_QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE);			
	}
	
	Bsp_QSPI_Receive(temp,2);
	deviceid=(temp[0]<<8)|temp[1];
	return deviceid;
}    

/**
  * @brief  读取W25QXX 的信息
  * @return 0XEF13,表示芯片型号为W25Q80  
		    0XEF14,表示芯片型号为W25Q16    
			0XEF15,表示芯片型号为W25Q32  
			0XEF16,表示芯片型号为W25Q64 
			0XEF17,表示芯片型号为W25Q128 	  
			0XEF18,表示芯片型号为W25Q256
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
  * @brief 读取SPI FLASH,在指定地址开始读取指定长度的数据,仅支持QPI模式
  * @param pBuffer:数据存储区
  * @param ReadAddr:开始读取的地址(最大32bit)
  * @param NumByteToRead:要读取的字节数(最大65535)
  */

void Bsp_W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
	//QPI,快速读数据,地址为ReadAddr,8空周期,4线传输指令_4线传输地址_32位地址_4线传输数据,NumByteToRead个数据
	Bsp_QSPI_Send_CMD(W25X_FastReadData,ReadAddr,8,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES);	
	Bsp_QSPI_Receive(pBuffer,NumByteToRead); 
}  

/**
  * @brief SPI在一页(0~65535)内写入少于256个字节的数据,在指定地址开始写入最大256字节的数据
  * @param pBuffer:数据存储区
  * @param WriteAddr:开始写入的地址(最大32bit)
  * @param NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	
  */

void Bsp_W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	Bsp_W25QXX_Write_Enable();
	//QPI,页写指令,地址为WriteAddr,无空周期,4线传输指令_4线传输地址_32位地址_4线传输数据,NumByteToWrite个数据
	Bsp_QSPI_Send_CMD(W25X_PageProgram,WriteAddr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES);	
	Bsp_QSPI_Transmit(pBuffer,NumByteToWrite);	         	      
	Bsp_W25QXX_Wait_Busy();
} 

/**
  * @brief 	无检验写SPI FLASH,必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
  * 		具有自动换页功能,在指定地址开始写入指定长度的数据,但是要确保地址不越界!
  * @param  pBuffer:数据存储区
  * @param  WriteAddr:开始写入的地址(最大32bit)
  * @param  NumByteToWrite:要写入的字节数(最大65535)	
  */

void Bsp_W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	 	
	pageremain = 256 - WriteAddr%256; 						//单页剩余的字节数		 	    
	if(NumByteToWrite <= pageremain)
	{
		pageremain=NumByteToWrite;							//不大于256个字节
	}
	while(1)
	{	   
		Bsp_W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite == pageremain)
		{
			break;											//写入结束了
		}
	 	else 												//NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	

			NumByteToWrite -= pageremain;			  			//减去已经写入了的字节数
			if(NumByteToWrite > 256)pageremain = 256; 			//一次可以写入256个字节
			else pageremain = NumByteToWrite; 	  				//不够256个字节了
		}
	}   
} 

/**
  * @brief 	写SPI FLASH, 在指定地址开始写入指定长度的数据,该函数带擦除操作!
  * @param  pBuffer:数据存储区
  * @param  WriteAddr:开始写入的地址(最大32bit)
  * @param  NumByteToWrite:要写入的字节数(最大65535)	
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
 	secpos = WriteAddr/4096;											//扇区地址  
	secoff = WriteAddr%4096;											//在扇区内的偏移
	secremain = 4096 - secoff;											//扇区剩余空间大小   
#if W25QXX_DEBUG == 1
 	Bsp_Printf("Write address:%X,Number of bytes to write:%X\r\n",WriteAddr,NumByteToWrite);//测试用
#endif
 	if(NumByteToWrite<=secremain)secremain = NumByteToWrite;			//不大于4096个字节
	while(1) 
	{	
		Bsp_W25QXX_Read(W25QXX_BUF,secpos*4096,4096);					//读出整个扇区的内容
		for(i=0;i<secremain;i++)										//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;						//需要擦除  	  
		}
		if(i<secremain)													//需要擦除
		{
			Bsp_W25QXX_Erase_Sector(secpos);							//擦除这个扇区
			for(i=0;i<secremain;i++)	   								//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			Bsp_W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);		//写入整个扇区  
		}
		else 
		{
			Bsp_W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);		//写已经擦除了的,直接写入扇区剩余区间.	
		} 				   
		if(NumByteToWrite==secremain)
		{
			break;														//写入结束了
		}
		else															//写入未结束
		{
			secpos++;													//扇区地址增1
			secoff=0;													//偏移位置为0 	 

		   	pBuffer+=secremain;  										//指针偏移
			WriteAddr+=secremain;										//写地址偏移	   
		   	NumByteToWrite-=secremain;									//字节数递减
			if(NumByteToWrite>4096)
			{
				secremain=4096;											//下一个扇区还是写不完
			}
			else 
			{
				secremain=NumByteToWrite;								//下一个扇区可以写完了
			}
		}	 
	}	 
}

/**
  * @brief 	擦除整个芯片,等待时间超长...	
  */ 

void Bsp_W25QXX_Erase_Chip(void)   
{                                   
    Bsp_W25QXX_Write_Enable();
    Bsp_W25QXX_Wait_Busy();   
	//QPI,写全片擦除指令,地址为0,无数据_8位地址_无地址_4线传输指令,无空周期,0个字节数据
	Bsp_QSPI_Send_CMD(W25X_ChipErase,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
	Bsp_W25QXX_Wait_Busy();						//等待芯片擦除结束
} 

/**
  * @brief 	擦除一个扇区,擦除一个扇区的最少时间:150ms
  * @param  st_Addr:扇区地址 根据实际容量设置
  */ 

void Bsp_W25QXX_Erase_Sector(uint32_t Dst_Addr)   
{  
#if W25QXX_DEBUG == 1	 
 	Bsp_Printf("Erase the Sector in:%x\r\n",Dst_Addr);		//监视falsh擦除情况,测试用  	  
#endif
 	Dst_Addr*=4096;
    Bsp_W25QXX_Write_Enable();                  	 
    Bsp_W25QXX_Wait_Busy();  
	//QPI,写扇区擦除指令,地址为0,无数据_32位地址_4线传输地址_4线传输指令,无空周期,0个字节数据
	Bsp_QSPI_Send_CMD(W25X_SectorErase,Dst_Addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
    Bsp_W25QXX_Wait_Busy();   				    			//等待擦除完成
}

/**
  * @brief 	判断写PAGE前是否需要先擦除。
  * @param  _ucpOldBuf:旧数据
  * @param  _ucpNewBuf:新数据
  * @param  _uiLen:数据个数，不能超过页面大小
  * @return 0:不需要擦除， 1：需要擦除
  */ 

uint8_t Bsp_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t ucOld;

	/*
	算法第1步：old 求反, new 不变
	      old    new
		  1101   0101
	~     1111
		= 0010   0101

	算法第2步: old 求反的结果与 new 位与
		  0010   old
	&	  0101   new
		 =0000

	算法第3步: 结果为0,则表示无需擦除. 否则表示需要擦除
	*/

	for (i = 0; i < _usLen; i++)
	{
		ucOld = *_ucpOldBuf++;
		ucOld = ~ucOld;

		/* 注意错误的写法: if (ucOld & (*_ucpNewBuf++) != 0) */
		if ((ucOld & (*_ucpNewBuf++)) != 0)
		{
			return 1;
		}
	}
	return 0;
}
/**
  * @brief 	等待空闲
  */ 

void Bsp_W25QXX_Wait_Busy(void)   
{   
	while((Bsp_W25QXX_ReadSR(1)&0x01)==0x01);   // 等待BUSY位清空
}   

/**
  * @brief 	测试QSPI的读FLASH,并显示。
  * @param  pBuffer:数据存储区
  * @param  ReadAddr:开始写入的地址(最大32bit)
  * @param  NumByteToRead:要写入的字节数(最大65535)	
  */
void Bsp_ReadTest(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
	uint16_t i;
	iTime_Start = Bsp_GetCPURunTime();	/* 记下开始时间 */
	Bsp_W25QXX_Read(TEXT_Buffer, ReadAddr, NumByteToRead);
	iTime_End = Bsp_GetCPURunTime();	/* 记下结束时间 */
	Bsp_Printf("Write Flash success,The datas are:\r\n");

	/* 打印数据 */
	for (i = 0; i < NumByteToRead; i++)
	{
		Bsp_Printf(" %02X", TEXT_Buffer[i]);

		if ((i & 31) == 31)
		{
			Bsp_Printf("\r\n");	/* 每行显示16字节数据 */
		}
		else if ((i & 31) == 15)
		{
			Bsp_Printf(" - ");
		}
	}

	/* 打印读速度 */
	Bsp_Printf("Read Test: From %x, Data length: %d Bytes, Read time: %dms, Read speed: %d Bytes/s\r\n", ReadAddr, NumByteToRead,  iTime_End - iTime_Start, (NumByteToRead * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	测试QSPI的读FLASH的速度。
  * @param  pBuffer:数据存储区
  * @param  ReadAddr:开始写入的地址(最大32bit)
  * @param  NumByteToRead:要写入的字节数(最大65535)	
  */
void Bsp_ReadSpeedTest(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
	uint16_t i;
	uint32_t uiAddr;
	
	iTime_Start = Bsp_GetCPURunTime();	/* 记下开始时间 */
	uiAddr = 0;
	for (i = 0; i < TotalSize / NumByteToRead; i++, uiAddr += NumByteToRead)
	{
		Bsp_W25QXX_Read(TEXT_Buffer, ReadAddr, NumByteToRead);
	}
	iTime_End = Bsp_GetCPURunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	Bsp_Printf("Read The whole chip, Data length: %d Bytes, Read time: %dms, Read speed: %d Bytes/s\r\n", TotalSize,  iTime_End - iTime_Start, (TotalSize * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	测试QSPI的写FLASH,带检查的写入。
  * @param  pBuffer:数据存储区
  * @param  WriteAddr:开始写入的地址(最大32bit)
  * @param  NumByteToWrite:要写入的字节数(最大65535)	
  */
void Bsp_Write_With_Check_Test(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t i;
	/* 填充测试缓冲区 */
	for (i = 0; i < NumByteToWrite; i++)
	{
		TEXT_Buffer[i] = i;
	}

	iTime_Start = Bsp_GetCPURunTime();	/* 记下开始时间 */
	Bsp_W25QXX_Write(TEXT_Buffer,WriteAddr, NumByteToWrite);
	iTime_End = Bsp_GetCPURunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	Bsp_Printf("Write with check Test: From %x, Data length: %d Bytes, Write time: %dms, Write speed: %d Bytes/s\r\n", WriteAddr, NumByteToWrite,  iTime_End - iTime_Start, (NumByteToWrite * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	测试QSPI的写FLASH,不带检查的写入。
  * @param  pBuffer:数据存储区
  * @param  WriteAddr:开始写入的地址(最大32bit)
  * @param  NumByteToWrite:要写入的字节数(最大65535)	
  */
void Bsp_Write_Without_Check_Test(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t i;
	/* 填充测试缓冲区 */
	for (i = 0; i < NumByteToWrite; i++)
	{
		TEXT_Buffer[i] = i;
	}

	iTime_Start = Bsp_GetCPURunTime();	/* 记下开始时间 */
	Bsp_W25QXX_Write_NoCheck(TEXT_Buffer,WriteAddr, NumByteToWrite);
	iTime_End = Bsp_GetCPURunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	Bsp_Printf("Write without check Test: From %x, Data length: %d Bytes, Write time: %dms, Write speed: %d Bytes/s\r\n", WriteAddr, NumByteToWrite,  iTime_End - iTime_Start, (NumByteToWrite * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	测试QSPI的全写FLASH,按页来写。每次写入4K Bytes
  * @param _data: 要写入的数据
  * @param  pBuffer:数据存储区
  * @param  WriteAddr:开始写入的地址(最大32bit)
  * @param  NumByteToWrite:要写入的字节数(最大65535)	
  */
void Bsp_Write_All_Page_Test(uint8_t _data, uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint16_t i;
	/* 填充测试缓冲区 */
	for (i = 0; i < NumByteToWrite; i++)
	{
		TEXT_Buffer[i] = _data;
	}

	iTime_Start = Bsp_GetCPURunTime();	/* 记下开始时间 */
	for (i = 0; i < TotalSize / PageSize; i++)
	{
		Bsp_W25QXX_Write_Page(TEXT_Buffer, i * PageSize, PageSize);
		Bsp_Printf(".");
		if (((i + 1) % 128) == 0)
		{
			Bsp_Printf("\r\n");
		}
	}
	iTime_End = Bsp_GetCPURunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	Bsp_Printf("Data length: %dK Bytes, Write time: %dms, Write seed: %d Bytes/s\r\n", TotalSize / 1024,  iTime_End - iTime_Start, (TotalSize * 1000) / ( iTime_End - iTime_Start));
}

/**
  * @brief 	测试QSPI的全擦除FLASH。
  */
void Bsp_Erase_ChipTest(void)
{
	iTime_Start = Bsp_GetCPURunTime();	/* 记下开始时间 */
	Bsp_W25QXX_Erase_Chip();
	iTime_End = Bsp_GetCPURunTime();	/* 记下结束时间 */

	/* 打印读速度 */
	Bsp_Printf("Erash the whole Flash complete! Spand Time: %dms\r\n",  iTime_End - iTime_Start);
	return;
}

/**
  * @brief 	测试QSPI的擦除一个扇区的FLASH。
  * @param _data: 要擦除的起始地址
  */
void Bsp_Erase_Page_Test(uint32_t Dst_Addr)
{
	iTime_Start = Bsp_GetCPURunTime();	/* 记下开始时间 */
	Bsp_W25QXX_Erase_Sector(Dst_Addr);
	iTime_End = Bsp_GetCPURunTime();	/* 记下结束时间 */

	/* 打印读速度 */
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
