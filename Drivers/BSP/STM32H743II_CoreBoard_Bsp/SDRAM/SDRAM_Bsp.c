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
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM���
/**
  * @brief  SDRAM Test Array
  *			
  */
#if SDRM_TEST == 1
uint16_t TestSdram[250000] __attribute__((at(0XC0000000)));//����������, ��LCD֡�������׵�ַ��һ����.��������0XC0000000,�������ʹ�ò��Թ�����Ҫ��ֹLCD
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
                                                     
    SDRAM_Handler.Instance=FMC_SDRAM_DEVICE;                          	 //SDRAM��BANK5,6  
    SDRAM_Handler.Init.SDBank=FMC_SDRAM_BANK1;                           //SDRAM����BANK5��
    SDRAM_Handler.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_9;     //������
    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_13;          //������
    SDRAM_Handler.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;       //���ݿ��Ϊ16λ
    SDRAM_Handler.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;  //һ��4��BANK
    SDRAM_Handler.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_2;               //CASΪ2
    SDRAM_Handler.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;//ʧ��д����
    SDRAM_Handler.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;           //SDRAMʱ��ΪHCLK/2=200M/2=100M=10ns
    SDRAM_Handler.Init.ReadBurst=FMC_SDRAM_RBURST_ENABLE;                //ʹ��ͻ��
    SDRAM_Handler.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_0;            //��ͨ����ʱ
    
    SDRAM_Timing.LoadToActiveDelay=2;                                   //����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������
    SDRAM_Timing.ExitSelfRefreshDelay=8;                                //�˳���ˢ���ӳ�Ϊ8��ʱ������
    SDRAM_Timing.SelfRefreshTime=6;                                     //��ˢ��ʱ��Ϊ6��ʱ������                                 
    SDRAM_Timing.RowCycleDelay=6;                                       //��ѭ���ӳ�Ϊ6��ʱ������
    SDRAM_Timing.WriteRecoveryTime=2;                                   //�ָ��ӳ�Ϊ2��ʱ������
    SDRAM_Timing.RPDelay=2;                                             //��Ԥ����ӳ�Ϊ2��ʱ������
    SDRAM_Timing.RCDDelay=2;                                            //�е����ӳ�Ϊ2��ʱ������
    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);  
	
	Bsp_SDRAM_Initialization_Sequence(&SDRAM_Handler);//����SDRAM��ʼ������
}
/**
  * @brief The sequence of Initialize the SDRAM according to the specified
  */
void Bsp_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
	uint32_t temp=0;

	//SDRAM��������ʼ������Ժ���Ҫ��������˳���ʼ��SDRAM
	Bsp_SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //ʱ������ʹ��
	Bsp_Delay_us(500);                                  //������ʱ200us
	Bsp_SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_PALL,1,0);       //�����д洢��Ԥ���
	Bsp_SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//������ˢ�´��� 
	//����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
	//bit3Ϊָ��ͻ�����ʵ����ͣ�bit4~bit6ΪCASֵ��bit7��bit8Ϊ����ģʽ
	//bit9Ϊָ����дͻ��ģʽ��bit10��bit11λ����λ
	temp =(uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |	//����ͻ������:1(������1/2/4/8)
					SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	//����ͻ������:����(����������/����)
					SDRAM_MODEREG_CAS_LATENCY_2           |	//����CASֵ:3(������2/3)
					SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //���ò���ģʽ:0,��׼ģʽ
					SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //����ͻ��дģʽ:1,�������
					Bsp_SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //����SDRAM��ģʽ�Ĵ���
    
	//ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
	//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
	//����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=200/2=100Mhz,����Ϊ8192(2^13).
	//����,COUNT=64*1000*100/8192-20=677
	HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler,677);	
}	
/**
  * @brief SDRAM hardware layer initial
  * @param hsdram: SDRAM handle pointer
 */
//SDRAM�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_SDRAM_Init()����
//hsdram:SDRAM���
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;
    
	__HAL_RCC_SYSCFG_CLK_ENABLE();				//ʹ��SYSCFGʱ��
    __HAL_RCC_FMC_CLK_ENABLE();                 //ʹ��FMCʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();               //ʹ��GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();               //ʹ��GPIODʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();               //ʹ��GPIOEʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();               //ʹ��GPIOFʱ��
    __HAL_RCC_GPIOG_CLK_ENABLE();               //ʹ��GPIOGʱ��
    
	//PC2,3����ģ�⿪�عرգ���������
	//HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC2,SYSCFG_SWITCH_PC2_CLOSE);
	//HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PC3,SYSCFG_SWITCH_PC3_CLOSE);
    
    //��ʼ��PC0,2,3
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;  
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;         //����
    GPIO_Initure.Alternate=GPIO_AF12_FMC;       //����ΪFMC    
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);         //��ʼ��
    
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOD,&GPIO_Initure); //��ʼ��PD0,1,8,9,10,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOE,&GPIO_Initure); //��ʼ��PE0,1,7,8,9,10,11,12,13,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOF,&GPIO_Initure); //��ʼ��PF0,1,2,3,4,5,11,12,13,14,15
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);	//��ʼ��PG0,1,2,4,5,8,15 
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
// bankx:0,��BANK5�����SDRAM����ָ��
//		 1,��BANK6�����SDRAM����ָ��
// cmd:ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
// refresh:��ˢ�´���
// regval:ģʽ�Ĵ����Ķ���
uint8_t Bsp_SDRAM_Send_Cmd(uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    uint32_t target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;
    
    if(bankx==0) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;       
    else if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;   
    Command.CommandMode=cmd;                //����
    Command.CommandTarget=target_bank;      //Ŀ��SDRAM�洢����
    Command.AutoRefreshNumber=refresh;      //��ˢ�´���
    Command.ModeRegisterDefinition=regval;  //Ҫд��ģʽ�Ĵ�����ֵ
    if(HAL_SDRAM_SendCommand(&SDRAM_Handler,&Command,0XFFFF)==HAL_OK) //��SDRAM��������
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
//��ָ����ַ(WriteAddr+Bank5_SDRAM_ADDR)��ʼ,����д��n���ֽ�.
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
//��ָ����ַ((WriteAddr+Bank5_SDRAM_ADDR))��ʼ,��������n���ֽ�.
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
//SDRAM�ڴ����	    
void Bsp_Fsmc_Sdram_Test(void)
{ 	
	__IO uint32_t i=0;  	  
	__IO uint32_t Ts=0;  	  
	__IO uint32_t temp=0;	   
	__IO uint32_t sval=0;	
	
    for(Ts=0;Ts<250000;Ts++)
	{
		TestSdram[Ts]=Ts;												//Ԥ���������	
		Bsp_Printf("TestSdram[%d]:%d\r\n",Ts,TestSdram[Ts]);			//��ӡ��������
		
  	}
	for(i=0;i<32*1024*1024;i+=16*1024)									//ÿ��16K�ֽ�,д��һ������,�ܹ�д��2048������,�պ���32M�ֽ�
	{
		*(__IO uint32_t*)(Bank5_SDRAM_ADDR+i)=temp; 
		temp++;
	}
 	for(i=0;i<32*1024*1024;i+=16*1024) 									//���ζ���֮ǰд�������,����У��		  
	{	
  		temp=*(__IO uint32_t*)(Bank5_SDRAM_ADDR+i);
		if(i==0)
		{
			sval=temp;
		}
 		else if(temp<=sval)
			break;														//�������������һ��Ҫ�ȵ�һ�ζ��������ݴ�.	   		   
		Bsp_Printf("SDRAM Capacity:%dKB\r\n",(uint16_t)(temp-sval+1)*16);//��ӡSDRAM����
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
