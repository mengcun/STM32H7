/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/SDRAM/Sdram_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    20-July-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The SDRAM canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/*******************************************APP/BSP*************************************************/
#include "Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup SDRAM_BSP SDRAM_BSP
  * @brief SDRAM_BSP
  * @{
  */
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup SDRAM_Private_Variables SDRAM Private Variables
  * @{
  */
/**
  * @brief  SDRAM Private Handle
  */
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM句柄
/**
  * @brief  SDRAM Test Array
  *			
  */
#if SDRM_TEST == 1
uint16_t TestSdram[250000] __attribute__((at(0XC0000000)));//测试用数组, 与LCD帧缓冲区首地址是一样的.定义在了0XC0000000,所以如果使用测试功能需要禁止LCD
#endif
/** @}
*/		
/*----------------------------SDRAM Private Variables--------------------------------*/ 
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup SDRAM_Private_Functions_Prototypes SDRAM Private Functions Prototypes
  * @{
  */
/** @}
*/		
/*----------------------------SDRAM Private Functions Prototypes--------------------------------*/ 

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup SDRAM_Private_Functions SDRAM Private Functions
  * @{
  */
/** @}
*/
/*--------------------------------------SDRAM Private Functions---------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup SDRAM_Exported_Functions SDRAM Exported Functions
  * @{
  */
/** @defgroup SDRAM_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize SDRAM
  @endverbatim
  * @{
  */
/**
  * @brief Initialize the SDRAM according to the specified
  */
void Bsp_SDRAM_Init(void)
{
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;
                                                     
    SDRAM_Handler.Instance=FMC_SDRAM_DEVICE;                          	 //SDRAM在BANK5,6  
    SDRAM_Handler.Init.SDBank=FMC_SDRAM_BANK1;                           //SDRAM接在BANK5上
    SDRAM_Handler.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_9;     //列数量
    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_13;          //行数量
    SDRAM_Handler.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;       //数据宽度为16位
    SDRAM_Handler.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;  //一共4个BANK
    SDRAM_Handler.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_2;               //CAS为2
    SDRAM_Handler.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;//失能写保护
    SDRAM_Handler.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;           //SDRAM时钟为HCLK/2=200M/2=100M=10ns
    SDRAM_Handler.Init.ReadBurst=FMC_SDRAM_RBURST_ENABLE;                //使能突发
    SDRAM_Handler.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_0;            //读通道延时
    
    SDRAM_Timing.LoadToActiveDelay=2;                                   //加载模式寄存器到激活时间的延迟为2个时钟周期
    SDRAM_Timing.ExitSelfRefreshDelay=8;                                //退出自刷新延迟为8个时钟周期
    SDRAM_Timing.SelfRefreshTime=6;                                     //自刷新时间为6个时钟周期                                 
    SDRAM_Timing.RowCycleDelay=6;                                       //行循环延迟为6个时钟周期
    SDRAM_Timing.WriteRecoveryTime=2;                                   //恢复延迟为2个时钟周期
    SDRAM_Timing.RPDelay=2;                                             //行预充电延迟为2个时钟周期
    SDRAM_Timing.RCDDelay=2;                                            //行到列延迟为2个时钟周期
    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);  
	
	Bsp_SDRAM_Initialization_Sequence(&SDRAM_Handler);//发送SDRAM初始化序列
}
/**
  * @brief The sequence of Initialize the SDRAM according to the specified
  */
void Bsp_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
	uint32_t temp=0;

	//SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
	Bsp_SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //时钟配置使能
	Bsp_Delay_us(500);                                  //至少延时200us
	Bsp_SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_PALL,1,0);       //对所有存储区预充电
	Bsp_SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//设置自刷新次数 
	//配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
	//bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
	//bit9为指定的写突发模式，bit10和bit11位保留位
	temp =(uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |	//设置突发长度:1(可以是1/2/4/8)
					SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	//设置突发类型:连续(可以是连续/交错)
					SDRAM_MODEREG_CAS_LATENCY_2           |	//设置CAS值:3(可以是2/3)
					SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //设置操作模式:0,标准模式
					SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //设置突发写模式:1,单点访问
					Bsp_SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //设置SDRAM的模式寄存器
    
	//刷新频率计数器(以SDCLK频率计数),计算方法:
	//COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
	//我们使用的SDRAM刷新周期为64ms,SDCLK=200/2=100Mhz,行数为8192(2^13).
	//所以,COUNT=64*1000*100/8192-20=677
	HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler,677);	
}	
/**
  * @brief SDRAM hardware layer initial
  * @param hsdram: SDRAM handle pointer
 */
//SDRAM底层驱动，引脚配置，时钟使能
//此函数会被HAL_SDRAM_Init()调用
//hsdram:SDRAM句柄
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;
    
	__HAL_RCC_SYSCFG_CLK_ENABLE();				//使能SYSCFG时钟
    __HAL_RCC_FMC_CLK_ENABLE();                 //使能FMC时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();               //使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();               //使能GPIOD时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();               //使能GPIOE时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();               //使能GPIOF时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟
    
	//PC2,3引脚模拟开关关闭！！！！！
	//HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2,SYSCFG_SWITCH_PC2_CLOSE);
	//HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3,SYSCFG_SWITCH_PC3_CLOSE);
    
    //初始化PC0,2,3
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;  
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;         //高速
    GPIO_Initure.Alternate=GPIO_AF12_FMC;       //复用为FMC    
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //初始化
    
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOD,&GPIO_Initure); //初始化PD0,1,8,9,10,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOE,&GPIO_Initure); //初始化PE0,1,7,8,9,10,11,12,13,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOF,&GPIO_Initure); //初始化PF0,1,2,3,4,5,11,12,13,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);	//初始化PG0,1,2,4,5,8,15 
}

/** @}
*/
/*********************SDRAM Exported Functions Group1**************************/

/** @defgroup SDRAM_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the SDRAM
  @endverbatim
  * @{
  */
/**
  * @brief 	Send Operation to SDRAM
  * @param 	bankx:
  *			@arg 0,Send instruction to SDRAM ON bank5
  *		    @arg 1,Send instruction to SDRAM ON bank5
  * @param 	cmd: Instructions(0,Normal mode/1,Enable the clk/2,Precharged all storage area/3,Auto refresh/4,Load mode register/5,Self-refresh/6,Power-off)
  * @param 	refresh: Number of self-refreshes
  * @param 	regval: Definition of mode registers
  * @return 0: success; 1: fail.
 */
// bankx:0,向BANK5上面的SDRAM发送指令
//		 1,向BANK6上面的SDRAM发送指令
// cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
// refresh:自刷新次数
// regval:模式寄存器的定义
uint8_t Bsp_SDRAM_Send_Cmd(uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    uint32_t target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;
    
    if(bankx==0) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;       
    else if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;   
    Command.CommandMode=cmd;                //命令
    Command.CommandTarget=target_bank;      //目标SDRAM存储区域
    Command.AutoRefreshNumber=refresh;      //自刷新次数
    Command.ModeRegisterDefinition=regval;  //要写入模式寄存器的值
    if(HAL_SDRAM_SendCommand(&SDRAM_Handler,&Command,0XFFFF)==HAL_OK) //向SDRAM发送命令
    {
        return 0;  
    }
    else return 1;    
}
/**
  * @brief Write bytes to SDRAM at specified address
  * @param pBuffer: Pointer for Bytes needed to write
  * @param WriteAddr: The address to write
  * @param n: The number of bytes to write
 */
//在指定地址(WriteAddr+Bank5_SDRAM_ADDR)开始,连续写入n个字节.
void Bsp_FMC_SDRAM_WriteBuffer(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t n)
{
	for(;n!=0;n--)
	{
		*(__IO uint8_t*)(Bank5_SDRAM_ADDR+WriteAddr)=*pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}
/**
  * @brief Read bytes from SDRAM at specified address
  * @param pBuffer: Pointer for bytes need to read
  * @param ReadAddr: The address to read
  * @param n: The number of bytes to read
 */
//在指定地址((WriteAddr+Bank5_SDRAM_ADDR))开始,连续读出n个字节.
void Bsp_FMC_SDRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n)
{
	for(;n!=0;n--)
	{
		*pBuffer++=*(__IO uint8_t*)(Bank5_SDRAM_ADDR+ReadAddr);
		ReadAddr++;
	}
}

/**
  * @brief Test the SDRAM
 */
#if SDRM_TEST == 1
//SDRAM内存测试	    
void Bsp_Fsmc_Sdram_Test(void)
{ 	
	__IO uint32_t i=0;  	  
	__IO uint32_t Ts=0;  	  
	__IO uint32_t temp=0;	   
	__IO uint32_t sval=0;	
	
    for(Ts=0;Ts<250000;Ts++)
	{
		TestSdram[Ts]=Ts;												//预存测试数据	
		Bsp_Printf("TestSdram[%d]:%d\r\n",Ts,TestSdram[Ts]);			//打印测试数据
		
  	}
	for(i=0;i<32*1024*1024;i+=16*1024)									//每隔16K字节,写入一个数据,总共写入2048个数据,刚好是32M字节
	{
		*(__IO uint32_t*)(Bank5_SDRAM_ADDR+i)=temp; 
		temp++;
	}
 	for(i=0;i<32*1024*1024;i+=16*1024) 									//依次读出之前写入的数据,进行校验		  
	{	
  		temp=*(__IO uint32_t*)(Bank5_SDRAM_ADDR+i);
		if(i==0)
		{
			sval=temp;
		}
 		else if(temp<=sval)
			break;														//后面读出的数据一定要比第一次读到的数据大.	   		   
		Bsp_Printf("SDRAM Capacity:%dKB\r\n",(uint16_t)(temp-sval+1)*16);//打印SDRAM容量
 	}					 
}
#endif
/** @}
*/
/****************************SDRAM Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------SDRAM Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------SDRAM-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
