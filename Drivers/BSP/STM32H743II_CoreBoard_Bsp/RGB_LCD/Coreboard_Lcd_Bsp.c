/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/RDB_LCD/Coreboard_Lcd_Bsp.c
  * @author  MCD Application Team
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
	The RGB_LCD canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/*******************************************APP/BSP*************************************************/
#include "stm32h743ii_Coreboard_Bsp.h"
#include "Coreboard_Lcd_Bsp.h"
#include "Coreboard_Mpu_Bsp.h"
#include "font.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup LCD LCD
  * @brief LCD
  * @{
  */
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup LCD_Private_Variables LCD Private Variables
  * @{
  */
/**
  * @brief  LCD Private Handle
	* @note		LCD
  */

LTDC_HandleTypeDef  LTDC_Handler;	    //LTDC句柄
DMA2D_HandleTypeDef DMA2D_Handler; 	  //DMA2D句柄
//根据不同的颜色格式,定义帧缓存数组
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	uint32_t ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
#else
	uint16_t ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
#endif

uint32_t *ltdc_framebuf[2];					//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
_ltdc_dev lcdltdc;						//管理LCD LTDC的重要参数
SRAM_HandleTypeDef SRAM_Handler;    //SRAM句柄(用于控制LCD)
//LCD的画笔颜色和背景色	   
uint32_t POINT_COLOR=0xFF000000;		//画笔颜色
uint32_t BACK_COLOR =0xFFFFFFFF;  	//背景色 
  
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
/** @}
*/		
/*----------------------------LCD Private Variables--------------------------------*/ 
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup LCD_Private_Functions_Prototypes RGB_LCD Private Functions Prototypes
  * @{
  */
//CODING HERE
/** @}
*/		
/*----------------------------LCD Private Functions Prototypes--------------------------------*/ 

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup LCD_Private_Functions RGB_LCD Private Functions
  * @{
  */
/**
  * @brief 
  * @param param: brief.
  * @retval status
  */
//CODING HERE Private Functions
/** @}
*/
/*--------------------------------------LCD Private Functions---------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup LCD_Exported_Functions RGB_LCD
  * @{
  */
/** @defgroup LCD_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize XXXX
  @endverbatim
  * @{
  */
	
void LCD_Init(void)
{ 	  
  GPIO_InitTypeDef GPIO_Initure;
	FMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
  FMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
    
  lcddev.id=LTDC_PanelID_Read();	//检查是否有RGB屏接入
	if(lcddev.id!=0)
	{
		RGB_LCD_Init();			    //ID非零,说明有RGB屏接入.
	}
  else
  {    
        __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
        GPIO_Initure.Pin=GPIO_PIN_5;           //PB5,背光控制
        GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
        GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
        GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //高速
        HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
        
        LCD_MPU_Config();                       //使能MPU保护LCD区域
        SRAM_Handler.Instance=FMC_NORSRAM_DEVICE; //BANK1
        SRAM_Handler.Extended=FMC_NORSRAM_EXTENDED_DEVICE;       
        
        SRAM_Handler.Init.NSBank=FMC_NORSRAM_BANK1;     //使用NE1
        SRAM_Handler.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;  //不复用数据线
        SRAM_Handler.Init.MemoryType=FMC_MEMORY_TYPE_SRAM;              //SRAM
        SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //16位数据宽度
        SRAM_Handler.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE; //是否使能突发访问,仅对同步突发存储器有效,此处未用到
        SRAM_Handler.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
        SRAM_Handler.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;   //存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
        SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;    //存储器写使能
        SRAM_Handler.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;           //等待使能位,此处未用到
        SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;        //读写使用不同的时序
        SRAM_Handler.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
        SRAM_Handler.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           //禁止突发写
        SRAM_Handler.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
        
        //FSMC读时序控制寄存器
        FSMC_ReadWriteTim.AddressSetupTime=0x11;    //地址建立时间(ADDSET)为17个HCLK 1/216M=4.6ns*17=78ns
        FSMC_ReadWriteTim.AddressHoldTime=0x00;
        FSMC_ReadWriteTim.DataSetupTime=0x55;       //数据保存时间(DATAST)为85个HCLK	=4.6*85=391ns
        FSMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //模式A
        //FSMC写时序控制寄存器
        FSMC_WriteTim.AddressSetupTime=0x15;        //地址建立时间(ADDSET)为21个HCLK=96ns
        FSMC_WriteTim.AddressHoldTime=0x00;
        FSMC_WriteTim.DataSetupTime=0x15;           //数据保存时间(DATAST)为4.6ns*21个HCLK=96ns
        FSMC_WriteTim.AccessMode=FMC_ACCESS_MODE_A;     //模式A
		
        HAL_SRAM_Init(&SRAM_Handler,&FSMC_ReadWriteTim,&FSMC_WriteTim);		
        delay_ms(50); // delay 50 ms 
        
        //尝试9341 ID的读取		
        LCD_WR_REG(0XD3);				   
        lcddev.id=LCD_RD_DATA();	//dummy read 	
        lcddev.id=LCD_RD_DATA();	//读到0X00
        lcddev.id=LCD_RD_DATA();   	//读取93								   
        lcddev.id<<=8;
        lcddev.id|=LCD_RD_DATA();  	//读取41 	   			   
        if(lcddev.id!=0X9341)		//非9341,尝试看看是不是NT35310
        {	 
            LCD_WR_REG(0XD4);				   
            lcddev.id=LCD_RD_DATA();//dummy read  
            lcddev.id=LCD_RD_DATA();//读回0X01	 
            lcddev.id=LCD_RD_DATA();//读回0X53	
            lcddev.id<<=8;	 
            lcddev.id|=LCD_RD_DATA();	//这里读回0X10	 
            if(lcddev.id!=0X5310)		//也不是NT35310,尝试看看是不是NT35510
            {
                LCD_WR_REG(0XDA00);	
                lcddev.id=LCD_RD_DATA();		//读回0X00	 
                LCD_WR_REG(0XDB00);	
                lcddev.id=LCD_RD_DATA();		//读回0X80
                lcddev.id<<=8;	
                LCD_WR_REG(0XDC00);	
                lcddev.id|=LCD_RD_DATA();		//读回0X00		
                if(lcddev.id==0x8000)lcddev.id=0x5510;//NT35510读回的ID是8000H,为方便区分,我们强制设置为5510
                if(lcddev.id!=0X5510)			//也不是NT5510,尝试看看是不是SSD1963
                {
                    LCD_WR_REG(0XA1);
                    lcddev.id=LCD_RD_DATA();
                    lcddev.id=LCD_RD_DATA();	//读回0X57
                    lcddev.id<<=8;	 
                    lcddev.id|=LCD_RD_DATA();	//读回0X61	
                    if(lcddev.id==0X5761)lcddev.id=0X1963;//SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963
                } 
            }
        }   
        if(lcddev.id==0X9341)	//9341初始化
        {	 
            LCD_WR_REG(0xCF);  
            LCD_WR_DATA(0x00); 
            LCD_WR_DATA(0xC1); 
            LCD_WR_DATA(0X30); 
            LCD_WR_REG(0xED);  
            LCD_WR_DATA(0x64); 
            LCD_WR_DATA(0x03); 
            LCD_WR_DATA(0X12); 
            LCD_WR_DATA(0X81); 
            LCD_WR_REG(0xE8);  
            LCD_WR_DATA(0x85); 
            LCD_WR_DATA(0x10); 
            LCD_WR_DATA(0x7A); 
            LCD_WR_REG(0xCB);  
            LCD_WR_DATA(0x39); 
            LCD_WR_DATA(0x2C); 
            LCD_WR_DATA(0x00); 
            LCD_WR_DATA(0x34); 
            LCD_WR_DATA(0x02); 
            LCD_WR_REG(0xF7);  
            LCD_WR_DATA(0x20); 
            LCD_WR_REG(0xEA);  
            LCD_WR_DATA(0x00); 
            LCD_WR_DATA(0x00); 
            LCD_WR_REG(0xC0);    //Power control 
            LCD_WR_DATA(0x1B);   //VRH[5:0] 
            LCD_WR_REG(0xC1);    //Power control 
            LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
            LCD_WR_REG(0xC5);    //VCM control 
            LCD_WR_DATA(0x30); 	 //3F
            LCD_WR_DATA(0x30); 	 //3C
            LCD_WR_REG(0xC7);    //VCM control2 
            LCD_WR_DATA(0XB7); 
            LCD_WR_REG(0x36);    // Memory Access Control 
            LCD_WR_DATA(0x48); 
            LCD_WR_REG(0x3A);   
            LCD_WR_DATA(0x55); 
            LCD_WR_REG(0xB1);   
            LCD_WR_DATA(0x00);   
            LCD_WR_DATA(0x1A); 
            LCD_WR_REG(0xB6);    // Display Function Control 
            LCD_WR_DATA(0x0A); 
            LCD_WR_DATA(0xA2); 
            LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
            LCD_WR_DATA(0x00); 
            LCD_WR_REG(0x26);    //Gamma curve selected 
            LCD_WR_DATA(0x01); 
            LCD_WR_REG(0xE0);    //Set Gamma 
            LCD_WR_DATA(0x0F); 
            LCD_WR_DATA(0x2A); 
            LCD_WR_DATA(0x28); 
            LCD_WR_DATA(0x08); 
            LCD_WR_DATA(0x0E); 
            LCD_WR_DATA(0x08); 
            LCD_WR_DATA(0x54); 
            LCD_WR_DATA(0XA9); 
            LCD_WR_DATA(0x43); 
            LCD_WR_DATA(0x0A); 
            LCD_WR_DATA(0x0F); 
            LCD_WR_DATA(0x00); 
            LCD_WR_DATA(0x00); 
            LCD_WR_DATA(0x00); 
            LCD_WR_DATA(0x00); 		 
            LCD_WR_REG(0XE1);    //Set Gamma 
            LCD_WR_DATA(0x00); 
            LCD_WR_DATA(0x15); 
            LCD_WR_DATA(0x17); 
            LCD_WR_DATA(0x07); 
            LCD_WR_DATA(0x11); 
            LCD_WR_DATA(0x06); 
            LCD_WR_DATA(0x2B); 
            LCD_WR_DATA(0x56); 
            LCD_WR_DATA(0x3C); 
            LCD_WR_DATA(0x05); 
            LCD_WR_DATA(0x10); 
            LCD_WR_DATA(0x0F); 
            LCD_WR_DATA(0x3F); 
            LCD_WR_DATA(0x3F); 
            LCD_WR_DATA(0x0F); 
            LCD_WR_REG(0x2B); 
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x01);
            LCD_WR_DATA(0x3f);
            LCD_WR_REG(0x2A); 
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xef);	 
            LCD_WR_REG(0x11); //Exit Sleep
            delay_ms(120);
            LCD_WR_REG(0x29); //display on	
        }else if(lcddev.id==0x5310)
        { 
            LCD_WR_REG(0xED);
            LCD_WR_DATA(0x01);
            LCD_WR_DATA(0xFE);

            LCD_WR_REG(0xEE);
            LCD_WR_DATA(0xDE);
            LCD_WR_DATA(0x21);

            LCD_WR_REG(0xF1);
            LCD_WR_DATA(0x01);
            LCD_WR_REG(0xDF);
            LCD_WR_DATA(0x10);

            //VCOMvoltage//
            LCD_WR_REG(0xC4);
            LCD_WR_DATA(0x8F);	  //5f

            LCD_WR_REG(0xC6);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xE2);
            LCD_WR_DATA(0xE2);
            LCD_WR_DATA(0xE2);
            LCD_WR_REG(0xBF);
            LCD_WR_DATA(0xAA);

            LCD_WR_REG(0xB0);
            LCD_WR_DATA(0x0D);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x0D);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x11);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x19);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x21);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x2D);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x3D);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x5D);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x5D);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xB1);
            LCD_WR_DATA(0x80);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x8B);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x96);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xB2);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x02);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x03);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xB3);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xB4);
            LCD_WR_DATA(0x8B);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x96);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA1);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xB5);
            LCD_WR_DATA(0x02);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x03);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x04);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xB6);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xB7);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x3F);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x5E);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x64);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x8C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xAC);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xDC);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x70);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x90);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xEB);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xDC);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xB8);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xBA);
            LCD_WR_DATA(0x24);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC1);
            LCD_WR_DATA(0x20);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x54);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xFF);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC2);
            LCD_WR_DATA(0x0A);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x04);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC3);
            LCD_WR_DATA(0x3C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x3A);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x39);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x37);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x3C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x36);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x32);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x2F);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x2C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x29);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x26);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x24);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x24);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x23);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x3C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x36);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x32);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x2F);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x2C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x29);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x26);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x24);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x24);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x23);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC4);
            LCD_WR_DATA(0x62);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x05);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x84);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xF0);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x18);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA4);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x18);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x50);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x0C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x17);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x95);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xF3);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xE6);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC5);
            LCD_WR_DATA(0x32);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x44);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x65);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x76);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x88);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC6);
            LCD_WR_DATA(0x20);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x17);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x01);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC7);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC8);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xC9);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xE0);
            LCD_WR_DATA(0x16);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x1C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x21);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x36);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x46);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x52);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x64);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x7A);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x8B);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x99);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA8);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xB9);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xC4);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xCA);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD2);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD9);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xE0);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xF3);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xE1);
            LCD_WR_DATA(0x16);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x1C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x22);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x36);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x45);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x52);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x64);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x7A);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x8B);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x99);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA8);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xB9);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xC4);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xCA);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD2);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD8);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xE0);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xF3);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xE2);
            LCD_WR_DATA(0x05);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x0B);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x1B);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x34);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x44);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x4F);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x61);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x79);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x88);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x97);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA6);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xB7);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xC2);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xC7);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD1);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD6);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xDD);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xF3);
            LCD_WR_DATA(0x00);
            LCD_WR_REG(0xE3);
            LCD_WR_DATA(0x05);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x1C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x33);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x44);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x50);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x62);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x78);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x88);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x97);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA6);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xB7);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xC2);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xC7);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD1);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD5);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xDD);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xF3);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xE4);
            LCD_WR_DATA(0x01);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x01);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x02);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x2A);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x3C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x4B);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x5D);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x74);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x84);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x93);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA2);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xB3);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xBE);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xC4);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xCD);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD3);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xDD);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xF3);
            LCD_WR_DATA(0x00);
            LCD_WR_REG(0xE5);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x02);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x29);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x3C);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x4B);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x5D);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x74);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x84);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x93);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xA2);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xB3);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xBE);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xC4);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xCD);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xD3);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xDC);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xF3);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xE6);
            LCD_WR_DATA(0x11);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x34);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x56);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x76);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x77);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x66);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x88);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x99);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xBB);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x99);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x66);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x55);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x55);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x45);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x43);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x44);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xE7);
            LCD_WR_DATA(0x32);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x55);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x76);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x66);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x67);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x67);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x87);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x99);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xBB);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x99);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x77);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x44);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x56);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x23); 
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x33);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x45);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xE8);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x99);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x87);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x88);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x77);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x66);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x88);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xAA);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0xBB);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x99);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x66);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x55);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x55);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x44);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x44);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x55);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xE9);
            LCD_WR_DATA(0xAA);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0x00);
            LCD_WR_DATA(0xAA);

            LCD_WR_REG(0xCF);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xF0);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x50);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xF3);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0xF9);
            LCD_WR_DATA(0x06);
            LCD_WR_DATA(0x10);
            LCD_WR_DATA(0x29);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0x3A);
            LCD_WR_DATA(0x55);	//66

            LCD_WR_REG(0x11);
            delay_ms(100);
            LCD_WR_REG(0x29);
            LCD_WR_REG(0x35);
            LCD_WR_DATA(0x00);

            LCD_WR_REG(0x51);
            LCD_WR_DATA(0xFF);
            LCD_WR_REG(0x53);
            LCD_WR_DATA(0x2C);
            LCD_WR_REG(0x55);
            LCD_WR_DATA(0x82);
            LCD_WR_REG(0x2c);
        }else if(lcddev.id==0x5510)
        {
            LCD_WriteReg(0xF000,0x55);
            LCD_WriteReg(0xF001,0xAA);
            LCD_WriteReg(0xF002,0x52);
            LCD_WriteReg(0xF003,0x08);
            LCD_WriteReg(0xF004,0x01);
            //AVDD Set AVDD 5.2V
            LCD_WriteReg(0xB000,0x0D);
            LCD_WriteReg(0xB001,0x0D);
            LCD_WriteReg(0xB002,0x0D);
            //AVDD ratio
            LCD_WriteReg(0xB600,0x34);
            LCD_WriteReg(0xB601,0x34);
            LCD_WriteReg(0xB602,0x34);
            //AVEE -5.2V
            LCD_WriteReg(0xB100,0x0D);
            LCD_WriteReg(0xB101,0x0D);
            LCD_WriteReg(0xB102,0x0D);
            //AVEE ratio
            LCD_WriteReg(0xB700,0x34);
            LCD_WriteReg(0xB701,0x34);
            LCD_WriteReg(0xB702,0x34);
            //VCL -2.5V
            LCD_WriteReg(0xB200,0x00);
            LCD_WriteReg(0xB201,0x00);
            LCD_WriteReg(0xB202,0x00);
            //VCL ratio
            LCD_WriteReg(0xB800,0x24);
            LCD_WriteReg(0xB801,0x24);
            LCD_WriteReg(0xB802,0x24);
            //VGH 15V (Free pump)
            LCD_WriteReg(0xBF00,0x01);
            LCD_WriteReg(0xB300,0x0F);
            LCD_WriteReg(0xB301,0x0F);
            LCD_WriteReg(0xB302,0x0F);
            //VGH ratio
            LCD_WriteReg(0xB900,0x34);
            LCD_WriteReg(0xB901,0x34);
            LCD_WriteReg(0xB902,0x34);
            //VGL_REG -10V
            LCD_WriteReg(0xB500,0x08);
            LCD_WriteReg(0xB501,0x08);
            LCD_WriteReg(0xB502,0x08);
            LCD_WriteReg(0xC200,0x03);
            //VGLX ratio
            LCD_WriteReg(0xBA00,0x24);
            LCD_WriteReg(0xBA01,0x24);
            LCD_WriteReg(0xBA02,0x24);
            //VGMP/VGSP 4.5V/0V
            LCD_WriteReg(0xBC00,0x00);
            LCD_WriteReg(0xBC01,0x78);
            LCD_WriteReg(0xBC02,0x00);
            //VGMN/VGSN -4.5V/0V
            LCD_WriteReg(0xBD00,0x00);
            LCD_WriteReg(0xBD01,0x78);
            LCD_WriteReg(0xBD02,0x00);
            //VCOM
            LCD_WriteReg(0xBE00,0x00);
            LCD_WriteReg(0xBE01,0x64);
            //Gamma Setting
            LCD_WriteReg(0xD100,0x00);
            LCD_WriteReg(0xD101,0x33);
            LCD_WriteReg(0xD102,0x00);
            LCD_WriteReg(0xD103,0x34);
            LCD_WriteReg(0xD104,0x00);
            LCD_WriteReg(0xD105,0x3A);
            LCD_WriteReg(0xD106,0x00);
            LCD_WriteReg(0xD107,0x4A);
            LCD_WriteReg(0xD108,0x00);
            LCD_WriteReg(0xD109,0x5C);
            LCD_WriteReg(0xD10A,0x00);
            LCD_WriteReg(0xD10B,0x81);
            LCD_WriteReg(0xD10C,0x00);
            LCD_WriteReg(0xD10D,0xA6);
            LCD_WriteReg(0xD10E,0x00);
            LCD_WriteReg(0xD10F,0xE5);
            LCD_WriteReg(0xD110,0x01);
            LCD_WriteReg(0xD111,0x13);
            LCD_WriteReg(0xD112,0x01);
            LCD_WriteReg(0xD113,0x54);
            LCD_WriteReg(0xD114,0x01);
            LCD_WriteReg(0xD115,0x82);
            LCD_WriteReg(0xD116,0x01);
            LCD_WriteReg(0xD117,0xCA);
            LCD_WriteReg(0xD118,0x02);
            LCD_WriteReg(0xD119,0x00);
            LCD_WriteReg(0xD11A,0x02);
            LCD_WriteReg(0xD11B,0x01);
            LCD_WriteReg(0xD11C,0x02);
            LCD_WriteReg(0xD11D,0x34);
            LCD_WriteReg(0xD11E,0x02);
            LCD_WriteReg(0xD11F,0x67);
            LCD_WriteReg(0xD120,0x02);
            LCD_WriteReg(0xD121,0x84);
            LCD_WriteReg(0xD122,0x02);
            LCD_WriteReg(0xD123,0xA4);
            LCD_WriteReg(0xD124,0x02);
            LCD_WriteReg(0xD125,0xB7);
            LCD_WriteReg(0xD126,0x02);
            LCD_WriteReg(0xD127,0xCF);
            LCD_WriteReg(0xD128,0x02);
            LCD_WriteReg(0xD129,0xDE);
            LCD_WriteReg(0xD12A,0x02);
            LCD_WriteReg(0xD12B,0xF2);
            LCD_WriteReg(0xD12C,0x02);
            LCD_WriteReg(0xD12D,0xFE);
            LCD_WriteReg(0xD12E,0x03);
            LCD_WriteReg(0xD12F,0x10);
            LCD_WriteReg(0xD130,0x03);
            LCD_WriteReg(0xD131,0x33);
            LCD_WriteReg(0xD132,0x03);
            LCD_WriteReg(0xD133,0x6D);
            LCD_WriteReg(0xD200,0x00);
            LCD_WriteReg(0xD201,0x33);
            LCD_WriteReg(0xD202,0x00);
            LCD_WriteReg(0xD203,0x34);
            LCD_WriteReg(0xD204,0x00);
            LCD_WriteReg(0xD205,0x3A);
            LCD_WriteReg(0xD206,0x00);
            LCD_WriteReg(0xD207,0x4A);
            LCD_WriteReg(0xD208,0x00);
            LCD_WriteReg(0xD209,0x5C);
            LCD_WriteReg(0xD20A,0x00);

            LCD_WriteReg(0xD20B,0x81);
            LCD_WriteReg(0xD20C,0x00);
            LCD_WriteReg(0xD20D,0xA6);
            LCD_WriteReg(0xD20E,0x00);
            LCD_WriteReg(0xD20F,0xE5);
            LCD_WriteReg(0xD210,0x01);
            LCD_WriteReg(0xD211,0x13);
            LCD_WriteReg(0xD212,0x01);
            LCD_WriteReg(0xD213,0x54);
            LCD_WriteReg(0xD214,0x01);
            LCD_WriteReg(0xD215,0x82);
            LCD_WriteReg(0xD216,0x01);
            LCD_WriteReg(0xD217,0xCA);
            LCD_WriteReg(0xD218,0x02);
            LCD_WriteReg(0xD219,0x00);
            LCD_WriteReg(0xD21A,0x02);
            LCD_WriteReg(0xD21B,0x01);
            LCD_WriteReg(0xD21C,0x02);
            LCD_WriteReg(0xD21D,0x34);
            LCD_WriteReg(0xD21E,0x02);
            LCD_WriteReg(0xD21F,0x67);
            LCD_WriteReg(0xD220,0x02);
            LCD_WriteReg(0xD221,0x84);
            LCD_WriteReg(0xD222,0x02);
            LCD_WriteReg(0xD223,0xA4);
            LCD_WriteReg(0xD224,0x02);
            LCD_WriteReg(0xD225,0xB7);
            LCD_WriteReg(0xD226,0x02);
            LCD_WriteReg(0xD227,0xCF);
            LCD_WriteReg(0xD228,0x02);
            LCD_WriteReg(0xD229,0xDE);
            LCD_WriteReg(0xD22A,0x02);
            LCD_WriteReg(0xD22B,0xF2);
            LCD_WriteReg(0xD22C,0x02);
            LCD_WriteReg(0xD22D,0xFE);
            LCD_WriteReg(0xD22E,0x03);
            LCD_WriteReg(0xD22F,0x10);
            LCD_WriteReg(0xD230,0x03);
            LCD_WriteReg(0xD231,0x33);
            LCD_WriteReg(0xD232,0x03);
            LCD_WriteReg(0xD233,0x6D);
            LCD_WriteReg(0xD300,0x00);
            LCD_WriteReg(0xD301,0x33);
            LCD_WriteReg(0xD302,0x00);
            LCD_WriteReg(0xD303,0x34);
            LCD_WriteReg(0xD304,0x00);
            LCD_WriteReg(0xD305,0x3A);
            LCD_WriteReg(0xD306,0x00);
            LCD_WriteReg(0xD307,0x4A);
            LCD_WriteReg(0xD308,0x00);
            LCD_WriteReg(0xD309,0x5C);
            LCD_WriteReg(0xD30A,0x00);

            LCD_WriteReg(0xD30B,0x81);
            LCD_WriteReg(0xD30C,0x00);
            LCD_WriteReg(0xD30D,0xA6);
            LCD_WriteReg(0xD30E,0x00);
            LCD_WriteReg(0xD30F,0xE5);
            LCD_WriteReg(0xD310,0x01);
            LCD_WriteReg(0xD311,0x13);
            LCD_WriteReg(0xD312,0x01);
            LCD_WriteReg(0xD313,0x54);
            LCD_WriteReg(0xD314,0x01);
            LCD_WriteReg(0xD315,0x82);
            LCD_WriteReg(0xD316,0x01);
            LCD_WriteReg(0xD317,0xCA);
            LCD_WriteReg(0xD318,0x02);
            LCD_WriteReg(0xD319,0x00);
            LCD_WriteReg(0xD31A,0x02);
            LCD_WriteReg(0xD31B,0x01);
            LCD_WriteReg(0xD31C,0x02);
            LCD_WriteReg(0xD31D,0x34);
            LCD_WriteReg(0xD31E,0x02);
            LCD_WriteReg(0xD31F,0x67);
            LCD_WriteReg(0xD320,0x02);
            LCD_WriteReg(0xD321,0x84);
            LCD_WriteReg(0xD322,0x02);
            LCD_WriteReg(0xD323,0xA4);
            LCD_WriteReg(0xD324,0x02);
            LCD_WriteReg(0xD325,0xB7);
            LCD_WriteReg(0xD326,0x02);
            LCD_WriteReg(0xD327,0xCF);
            LCD_WriteReg(0xD328,0x02);
            LCD_WriteReg(0xD329,0xDE);
            LCD_WriteReg(0xD32A,0x02);
            LCD_WriteReg(0xD32B,0xF2);
            LCD_WriteReg(0xD32C,0x02);
            LCD_WriteReg(0xD32D,0xFE);
            LCD_WriteReg(0xD32E,0x03);
            LCD_WriteReg(0xD32F,0x10);
            LCD_WriteReg(0xD330,0x03);
            LCD_WriteReg(0xD331,0x33);
            LCD_WriteReg(0xD332,0x03);
            LCD_WriteReg(0xD333,0x6D);
            LCD_WriteReg(0xD400,0x00);
            LCD_WriteReg(0xD401,0x33);
            LCD_WriteReg(0xD402,0x00);
            LCD_WriteReg(0xD403,0x34);
            LCD_WriteReg(0xD404,0x00);
            LCD_WriteReg(0xD405,0x3A);
            LCD_WriteReg(0xD406,0x00);
            LCD_WriteReg(0xD407,0x4A);
            LCD_WriteReg(0xD408,0x00);
            LCD_WriteReg(0xD409,0x5C);
            LCD_WriteReg(0xD40A,0x00);
            LCD_WriteReg(0xD40B,0x81);

            LCD_WriteReg(0xD40C,0x00);
            LCD_WriteReg(0xD40D,0xA6);
            LCD_WriteReg(0xD40E,0x00);
            LCD_WriteReg(0xD40F,0xE5);
            LCD_WriteReg(0xD410,0x01);
            LCD_WriteReg(0xD411,0x13);
            LCD_WriteReg(0xD412,0x01);
            LCD_WriteReg(0xD413,0x54);
            LCD_WriteReg(0xD414,0x01);
            LCD_WriteReg(0xD415,0x82);
            LCD_WriteReg(0xD416,0x01);
            LCD_WriteReg(0xD417,0xCA);
            LCD_WriteReg(0xD418,0x02);
            LCD_WriteReg(0xD419,0x00);
            LCD_WriteReg(0xD41A,0x02);
            LCD_WriteReg(0xD41B,0x01);
            LCD_WriteReg(0xD41C,0x02);
            LCD_WriteReg(0xD41D,0x34);
            LCD_WriteReg(0xD41E,0x02);
            LCD_WriteReg(0xD41F,0x67);
            LCD_WriteReg(0xD420,0x02);
            LCD_WriteReg(0xD421,0x84);
            LCD_WriteReg(0xD422,0x02);
            LCD_WriteReg(0xD423,0xA4);
            LCD_WriteReg(0xD424,0x02);
            LCD_WriteReg(0xD425,0xB7);
            LCD_WriteReg(0xD426,0x02);
            LCD_WriteReg(0xD427,0xCF);
            LCD_WriteReg(0xD428,0x02);
            LCD_WriteReg(0xD429,0xDE);
            LCD_WriteReg(0xD42A,0x02);
            LCD_WriteReg(0xD42B,0xF2);
            LCD_WriteReg(0xD42C,0x02);
            LCD_WriteReg(0xD42D,0xFE);
            LCD_WriteReg(0xD42E,0x03);
            LCD_WriteReg(0xD42F,0x10);
            LCD_WriteReg(0xD430,0x03);
            LCD_WriteReg(0xD431,0x33);
            LCD_WriteReg(0xD432,0x03);
            LCD_WriteReg(0xD433,0x6D);
            LCD_WriteReg(0xD500,0x00);
            LCD_WriteReg(0xD501,0x33);
            LCD_WriteReg(0xD502,0x00);
            LCD_WriteReg(0xD503,0x34);
            LCD_WriteReg(0xD504,0x00);
            LCD_WriteReg(0xD505,0x3A);
            LCD_WriteReg(0xD506,0x00);
            LCD_WriteReg(0xD507,0x4A);
            LCD_WriteReg(0xD508,0x00);
            LCD_WriteReg(0xD509,0x5C);
            LCD_WriteReg(0xD50A,0x00);
            LCD_WriteReg(0xD50B,0x81);

            LCD_WriteReg(0xD50C,0x00);
            LCD_WriteReg(0xD50D,0xA6);
            LCD_WriteReg(0xD50E,0x00);
            LCD_WriteReg(0xD50F,0xE5);
            LCD_WriteReg(0xD510,0x01);
            LCD_WriteReg(0xD511,0x13);
            LCD_WriteReg(0xD512,0x01);
            LCD_WriteReg(0xD513,0x54);
            LCD_WriteReg(0xD514,0x01);
            LCD_WriteReg(0xD515,0x82);
            LCD_WriteReg(0xD516,0x01);
            LCD_WriteReg(0xD517,0xCA);
            LCD_WriteReg(0xD518,0x02);
            LCD_WriteReg(0xD519,0x00);
            LCD_WriteReg(0xD51A,0x02);
            LCD_WriteReg(0xD51B,0x01);
            LCD_WriteReg(0xD51C,0x02);
            LCD_WriteReg(0xD51D,0x34);
            LCD_WriteReg(0xD51E,0x02);
            LCD_WriteReg(0xD51F,0x67);
            LCD_WriteReg(0xD520,0x02);
            LCD_WriteReg(0xD521,0x84);
            LCD_WriteReg(0xD522,0x02);
            LCD_WriteReg(0xD523,0xA4);
            LCD_WriteReg(0xD524,0x02);
            LCD_WriteReg(0xD525,0xB7);
            LCD_WriteReg(0xD526,0x02);
            LCD_WriteReg(0xD527,0xCF);
            LCD_WriteReg(0xD528,0x02);
            LCD_WriteReg(0xD529,0xDE);
            LCD_WriteReg(0xD52A,0x02);
            LCD_WriteReg(0xD52B,0xF2);
            LCD_WriteReg(0xD52C,0x02);
            LCD_WriteReg(0xD52D,0xFE);
            LCD_WriteReg(0xD52E,0x03);
            LCD_WriteReg(0xD52F,0x10);
            LCD_WriteReg(0xD530,0x03);
            LCD_WriteReg(0xD531,0x33);
            LCD_WriteReg(0xD532,0x03);
            LCD_WriteReg(0xD533,0x6D);
            LCD_WriteReg(0xD600,0x00);
            LCD_WriteReg(0xD601,0x33);
            LCD_WriteReg(0xD602,0x00);
            LCD_WriteReg(0xD603,0x34);
            LCD_WriteReg(0xD604,0x00);
            LCD_WriteReg(0xD605,0x3A);
            LCD_WriteReg(0xD606,0x00);
            LCD_WriteReg(0xD607,0x4A);
            LCD_WriteReg(0xD608,0x00);
            LCD_WriteReg(0xD609,0x5C);
            LCD_WriteReg(0xD60A,0x00);
            LCD_WriteReg(0xD60B,0x81);

            LCD_WriteReg(0xD60C,0x00);
            LCD_WriteReg(0xD60D,0xA6);
            LCD_WriteReg(0xD60E,0x00);
            LCD_WriteReg(0xD60F,0xE5);
            LCD_WriteReg(0xD610,0x01);
            LCD_WriteReg(0xD611,0x13);
            LCD_WriteReg(0xD612,0x01);
            LCD_WriteReg(0xD613,0x54);
            LCD_WriteReg(0xD614,0x01);
            LCD_WriteReg(0xD615,0x82);
            LCD_WriteReg(0xD616,0x01);
            LCD_WriteReg(0xD617,0xCA);
            LCD_WriteReg(0xD618,0x02);
            LCD_WriteReg(0xD619,0x00);
            LCD_WriteReg(0xD61A,0x02);
            LCD_WriteReg(0xD61B,0x01);
            LCD_WriteReg(0xD61C,0x02);
            LCD_WriteReg(0xD61D,0x34);
            LCD_WriteReg(0xD61E,0x02);
            LCD_WriteReg(0xD61F,0x67);
            LCD_WriteReg(0xD620,0x02);
            LCD_WriteReg(0xD621,0x84);
            LCD_WriteReg(0xD622,0x02);
            LCD_WriteReg(0xD623,0xA4);
            LCD_WriteReg(0xD624,0x02);
            LCD_WriteReg(0xD625,0xB7);
            LCD_WriteReg(0xD626,0x02);
            LCD_WriteReg(0xD627,0xCF);
            LCD_WriteReg(0xD628,0x02);
            LCD_WriteReg(0xD629,0xDE);
            LCD_WriteReg(0xD62A,0x02);
            LCD_WriteReg(0xD62B,0xF2);
            LCD_WriteReg(0xD62C,0x02);
            LCD_WriteReg(0xD62D,0xFE);
            LCD_WriteReg(0xD62E,0x03);
            LCD_WriteReg(0xD62F,0x10);
            LCD_WriteReg(0xD630,0x03);
            LCD_WriteReg(0xD631,0x33);
            LCD_WriteReg(0xD632,0x03);
            LCD_WriteReg(0xD633,0x6D);
            //LV2 Page 0 enable
            LCD_WriteReg(0xF000,0x55);
            LCD_WriteReg(0xF001,0xAA);
            LCD_WriteReg(0xF002,0x52);
            LCD_WriteReg(0xF003,0x08);
            LCD_WriteReg(0xF004,0x00);
            //Display control
            LCD_WriteReg(0xB100, 0xCC);
            LCD_WriteReg(0xB101, 0x00);
            //Source hold time
            LCD_WriteReg(0xB600,0x05);
            //Gate EQ control
            LCD_WriteReg(0xB700,0x70);
            LCD_WriteReg(0xB701,0x70);
            //Source EQ control (Mode 2)
            LCD_WriteReg(0xB800,0x01);
            LCD_WriteReg(0xB801,0x03);
            LCD_WriteReg(0xB802,0x03);
            LCD_WriteReg(0xB803,0x03);
            //Inversion mode (2-dot)
            LCD_WriteReg(0xBC00,0x02);
            LCD_WriteReg(0xBC01,0x00);
            LCD_WriteReg(0xBC02,0x00);
            //Timing control 4H w/ 4-delay
            LCD_WriteReg(0xC900,0xD0);
            LCD_WriteReg(0xC901,0x02);
            LCD_WriteReg(0xC902,0x50);
            LCD_WriteReg(0xC903,0x50);
            LCD_WriteReg(0xC904,0x50);
            LCD_WriteReg(0x3500,0x00);
            LCD_WriteReg(0x3A00,0x55);  //16-bit/pixel
            LCD_WR_REG(0x1100);
            delay_us(120);
            LCD_WR_REG(0x2900);  
        }else if(lcddev.id==0X1963)
        {
            LCD_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
            LCD_WR_DATA(0x1D);		//参数1 
            LCD_WR_DATA(0x02);		//参数2 Divider M = 2, PLL = 300/(M+1) = 100MHz
            LCD_WR_DATA(0x04);		//参数3 Validate M and N values   
            delay_us(100);
            LCD_WR_REG(0xE0);		// Start PLL command
            LCD_WR_DATA(0x01);		// enable PLL
            delay_ms(10);
            LCD_WR_REG(0xE0);		// Start PLL command again
            LCD_WR_DATA(0x03);		// now, use PLL output as system clock	
            delay_ms(12);  
            LCD_WR_REG(0x01);		//软复位
            delay_ms(10);
            
            LCD_WR_REG(0xE6);		//设置像素频率,33Mhz
            LCD_WR_DATA(0x2F);
            LCD_WR_DATA(0xFF);
            LCD_WR_DATA(0xFF);
		
            LCD_WR_REG(0xB0);		//设置LCD模式
            LCD_WR_DATA(0x20);		//24位模式
            LCD_WR_DATA(0x00);		//TFT 模式 
        
            LCD_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);//设置LCD水平像素
            LCD_WR_DATA(SSD_HOR_RESOLUTION-1);		 
            LCD_WR_DATA((SSD_VER_RESOLUTION-1)>>8);//设置LCD垂直像素
            LCD_WR_DATA(SSD_VER_RESOLUTION-1);		 
            LCD_WR_DATA(0x00);		//RGB序列 
            
            LCD_WR_REG(0xB4);		//Set horizontal period
            LCD_WR_DATA((SSD_HT-1)>>8);
            LCD_WR_DATA(SSD_HT-1);
            LCD_WR_DATA(SSD_HPS>>8);
            LCD_WR_DATA(SSD_HPS);
            LCD_WR_DATA(SSD_HOR_PULSE_WIDTH-1);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            LCD_WR_REG(0xB6);		//Set vertical period
            LCD_WR_DATA((SSD_VT-1)>>8);
            LCD_WR_DATA(SSD_VT-1);
            LCD_WR_DATA(SSD_VPS>>8);
            LCD_WR_DATA(SSD_VPS);
            LCD_WR_DATA(SSD_VER_FRONT_PORCH-1);
            LCD_WR_DATA(0x00);
            LCD_WR_DATA(0x00);
            
            LCD_WR_REG(0xF0);	//设置SSD1963与CPU接口为16bit  
            LCD_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp 

            LCD_WR_REG(0x29);	//开启显示
            //设置PWM输出  背光通过占空比可调 
            LCD_WR_REG(0xD0);	//设置自动白平衡DBC
            LCD_WR_DATA(0x00);	//disable
        
            LCD_WR_REG(0xBE);	//配置PWM输出
            LCD_WR_DATA(0x05);	//1设置PWM频率
            LCD_WR_DATA(0xFE);	//2设置PWM占空比
            LCD_WR_DATA(0x01);	//3设置C
            LCD_WR_DATA(0x00);	//4设置D
            LCD_WR_DATA(0x00);	//5设置E 
            LCD_WR_DATA(0x00);	//6设置F 
            
            LCD_WR_REG(0xB8);	//设置GPIO配置
            LCD_WR_DATA(0x03);	//2个IO口设置成输出
            LCD_WR_DATA(0x01);	//GPIO使用正常的IO功能 
            LCD_WR_REG(0xBA);
            LCD_WR_DATA(0X01);	//GPIO[1:0]=01,控制LCD方向
            
            LCD_SSD_BackLightSet(100);//背光设置为最亮
        }	    
    }	
	//初始化完成以后,提速
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//如果是这几个IC,则设置WR时序为最快
	{
		 //重新配置写时序控制寄存器的时序   	 							    
     FSMC_WriteTim.AddressSetupTime=5;      
     FSMC_WriteTim.DataSetupTime=5;         
     FMC_NORSRAM_Extended_Timing_Init(SRAM_Handler.Extended,&FSMC_WriteTim,SRAM_Handler.Init.NSBank,SRAM_Handler.Init.ExtendedMode);
	} 
  printf("LCD ID:%x\r\n",lcddev.id); //打印LCD ID  	
	LCD_Display_Dir(0);		//默认为竖屏
	LCD_LED(1);				//点亮背光
	LCD_Clear(WHITE);
}  	
	
/**
  * @brief Initialize the RGB_LCD according to the specified
  */
void RGB_LCD_Init(void)
{
	uint16_t lcdid=0;
	lcdid=LTDC_PanelID_Read();			//读取LCD面板ID	
	if(lcdid==0X4342)
	{
		lcdltdc.pwidth=480;			    //面板宽度,单位:像素
		lcdltdc.pheight=272;		    //面板高度,单位:像素
		lcdltdc.hsw=1;				    //水平同步宽度
		lcdltdc.vsw=1;				    //垂直同步宽度
		lcdltdc.hbp=40;				    //水平后廊
		lcdltdc.vbp=8;				    //垂直后廊
		lcdltdc.hfp=5;				    //水平前廊
		lcdltdc.vfp=8;				    //垂直前廊
		LTDC_Clk_Set(5,160,88);			//设置像素时钟 9Mhz 
		//其他参数待定.
	}else if(lcdid==0X7084)
	{
		lcdltdc.pwidth=800;			    //面板宽度,单位:像素
		lcdltdc.pheight=480;		    //面板高度,单位:像素
		lcdltdc.hsw=1;				    //水平同步宽度
		lcdltdc.vsw=1;				    //垂直同步宽度
		lcdltdc.hbp=46;				    //水平后廊
		lcdltdc.vbp=23;				    //垂直后廊
		lcdltdc.hfp=210;			    //水平前廊
		lcdltdc.vfp=22;				    //垂直前廊
		LTDC_Clk_Set(5,160,24);			//设置像素时钟 33M(如果开双显,需要降低DCLK到:18.75Mhz  pll3r=43,才会比较好)
	}else if(lcdid==0X7016)		
	{
		lcdltdc.pwidth=1024;			//面板宽度,单位:像素
		lcdltdc.pheight=600;			//面板高度,单位:像素
        lcdltdc.hsw=20;				    //水平同步宽度
		lcdltdc.vsw=3;				    //垂直同步宽度
		lcdltdc.hbp=140;			    //水平后廊
		lcdltdc.vbp=20;				    //垂直后廊
		lcdltdc.hfp=160;			    //水平前廊
		lcdltdc.vfp=12;				    //垂直前廊
		LTDC_Clk_Set(5,160,18);			//设置像素时钟  45Mhz 
		//其他参数待定.
	}else if(lcdid==0X7018)		
	{
		lcdltdc.pwidth=1280;			//面板宽度,单位:像素
		lcdltdc.pheight=800;			//面板高度,单位:像素
		//其他参数待定.
	}else if(lcdid==0X8016)		
	{
		lcdltdc.pwidth=1024;			//面板宽度,单位:像素
		lcdltdc.pheight=600;			//面板高度,单位:像素
		//其他参数待定.
	}else if(lcdid==0X1018)				//10.1寸1280*800 RGB屏	
	{
		lcdltdc.pwidth=1280;			//面板宽度,单位:像素
		lcdltdc.pheight=800;			//面板高度,单位:像素
		lcdltdc.hbp=140;				//水平后廊
		lcdltdc.hfp=10;			    	//水平前廊
		lcdltdc.hsw=10;					//水平同步宽度
		lcdltdc.vbp=10;					//垂直后廊
		lcdltdc.vfp=10;					//垂直前廊
		lcdltdc.vsw=3;					//垂直同步宽度
		LTDC_Clk_Set(5,160,16);			//设置像素时钟 50MHz
	}	else if(lcdid==0XA001)              //接VGA显示器
     { 
		lcdltdc.pwidth=1366;		    //面板宽 度,单位:像素
		lcdltdc.pheight=768;		    //面板高度,单位:像素
		lcdltdc.hsw=143;			    //水平同步宽度 
		lcdltdc.hfp=70;			        //水平前廊
		lcdltdc.hbp=213;			    //水平后廊
		lcdltdc.vsw=3;				    //垂直同步宽度 
		lcdltdc.vbp=24;				    //垂直后廊  
		lcdltdc.vfp=3;				    //垂直前廊
        LTDC_Clk_Set(5,160,28);//设置像素时钟 28.5Mhz
	 
	 }else if(lcdid==0XA002)            //接VGA显示器
     {  
        lcdltdc.pwidth=1280;		    //面板宽度,单位:像素
		lcdltdc.pheight=800;		    //面板高度,单位:像素
		lcdltdc.hsw=32;			        //水平同步宽度 
		lcdltdc.hfp=48;			        //水平前廊
		lcdltdc.hbp=80;			        //水平后廊
		lcdltdc.vsw=6;				    //垂直同步宽度
		lcdltdc.vbp=14;				    //垂直后廊 
		lcdltdc.vfp=3;				    //垂直前廊
		LTDC_Clk_Set(5,160,22);//设置像素时钟 35.5Mhz
	 }else if(lcdid==0XA003)	        //接VGA显示器
	 {
		lcdltdc.pwidth=1280;		    //面板宽度,单位:像素
		lcdltdc.pheight=768;		    //面板高度,单位:像素
		lcdltdc.hsw=32;				    //水平同步宽度 
		lcdltdc.hbp=80;			        //水平后廊
		lcdltdc.hfp=48;			        //水平前廊
		lcdltdc.vsw=7;				    //垂直同步宽度
		lcdltdc.vbp=12;				    //垂直后廊
		lcdltdc.vfp=3;				    //垂直前廊
		LTDC_Clk_Set(5,160,23);//设置像素时钟  34.25Mhz
	 }else if(lcdid==0XA004)            //接VGA显示器
	 {
		lcdltdc.pwidth=1024;		    //面板宽度,单位:像素
		lcdltdc.pheight=768;		    //面板高度,单位:像素
		lcdltdc.hsw=136;			    //水平同步宽度 
		lcdltdc.hfp=24;			        //水平前廊
		lcdltdc.hbp=160;			    //水平后廊
		lcdltdc.vsw=6;				    //垂直同步宽度
		lcdltdc.vbp=29;				    //垂直后廊 
		lcdltdc.vfp=3;				    //垂直前廊
		LTDC_Clk_Set(5,160,18);//设置像素时钟 43.25Mhz	
	 }else if(lcdid==0XA005)            //接VGA显示器
	 {
		lcdltdc.pwidth=848;		        //面板宽度,单位:像素 
		lcdltdc.pheight=480;		    //面板高度,单位:像素
		lcdltdc.hsw=112;		        //水平同步宽度
		lcdltdc.hbp=112;			    //水平后廊
		lcdltdc.hfp=16;			        //水平前廊
		lcdltdc.vsw=8;				    //垂直同步宽度
		lcdltdc.vbp=23;				    //垂直后廊
		lcdltdc.vfp=6;				    //垂直前廊
		LTDC_Clk_Set(5,160,47);//设置像素时钟 17Mhz 
	 }else if(lcdid==0XA006)            //接VGA显示器
	 {
		lcdltdc.pwidth=800;		        //面板宽度,单位:像素
		lcdltdc.pheight=600;		    //面板高度,单位:像素
		lcdltdc.hsw=128;			    //水平同步宽度
		lcdltdc.hbp=88;			        //水平后廊
		lcdltdc.hfp=40;			        //水平前廊
		lcdltdc.vsw=4;				    //垂直同步宽度
		lcdltdc.vbp=23;				    //垂直后廊
		lcdltdc.vfp=1;				    //垂直前廊
		LTDC_Clk_Set(5,160,40);//设置像素时钟 20mz		
	 }else if(lcdid==0XA007)            //接VGA显示器
	 {
        lcdltdc.pwidth=640;		        //面板宽度,单位:像素
		lcdltdc.pheight=480;		    //面板高度,单位:像素
		lcdltdc.hsw=96;				    //水平同步宽度
		lcdltdc.hfp=8;			        //水平前廊
		lcdltdc.hbp=56;			        //水平后廊
		lcdltdc.vsw=2;				    //垂直同步宽度
		lcdltdc.vbp=41;				    //垂直后廊
		lcdltdc.vfp=2;				    //垂直前廊
		LTDC_Clk_Set(5,160,62);//设置像素时钟 12.75Mhz
	 } 

	lcddev.width=lcdltdc.pwidth;
	lcddev.height=lcdltdc.pheight;
    
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888 
	ltdc_framebuf[0]=(uint32_t*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize=4;				//每个像素占4个字节
#else 
    lcdltdc.pixsize=2;				//每个像素占2个字节
	ltdc_framebuf[0]=(uint32_t*)&ltdc_lcd_framebuf;
#endif 	
    
    //LTDC配置
    LTDC_Handler.Instance=LTDC;
    LTDC_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         //水平同步极性
    LTDC_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         //垂直同步极性
    LTDC_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         //数据使能极性
    LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IPC;        //像素时钟极性
	 
		if(lcdid==0X1018)LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IIPC;        //像素时钟极性相反
	 
    LTDC_Handler.Init.HorizontalSync=lcdltdc.hsw-1;          //水平同步宽度
    LTDC_Handler.Init.VerticalSync=lcdltdc.vsw-1;            //垂直同步宽度
    LTDC_Handler.Init.AccumulatedHBP=lcdltdc.hsw+lcdltdc.hbp-1; //水平同步后沿宽度
    LTDC_Handler.Init.AccumulatedVBP=lcdltdc.vsw+lcdltdc.vbp-1; //垂直同步后沿高度
    LTDC_Handler.Init.AccumulatedActiveW=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth-1;//有效宽度
    LTDC_Handler.Init.AccumulatedActiveH=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight-1;//有效高度
    LTDC_Handler.Init.TotalWidth=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth+lcdltdc.hfp-1;   //总宽度
    LTDC_Handler.Init.TotalHeigh=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight+lcdltdc.vfp-1;  //总高度
    LTDC_Handler.Init.Backcolor.Red=0;           //屏幕背景层红色部分
    LTDC_Handler.Init.Backcolor.Green=0;         //屏幕背景层绿色部分
    LTDC_Handler.Init.Backcolor.Blue=0;          //屏幕背景色蓝色部分
    HAL_LTDC_Init(&LTDC_Handler);
 	
	//层配置
	LTDC_Layer_Parameter_Config(0,(uint32_t)ltdc_framebuf[0],LCD_PIXFORMAT,255,0,6,7,0X000000);//层参数配置
	LTDC_Layer_Window_Config(0,0,0,lcdltdc.pwidth,lcdltdc.pheight);	//层窗口配置,以LCD面板坐标系为基准,不要随便修改!	
	 	
 	LTDC_Display_Dir(1);			//默认竖屏
	LTDC_Select_Layer(0); 			//选择第1层
  LCD_LED(1);         		    //点亮背光
  LTDC_Clear(0XFFFFFFFF);			//清屏
}
//LTDC底层IO初始化和时钟使能
//此函数会被HAL_LTDC_Init()调用
//hltdc:LTDC句柄
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_LTDC_CLK_ENABLE();                //使能LTDC时钟
    __HAL_RCC_DMA2D_CLK_ENABLE();               //使能DMA2D时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();               //使能GPIOB时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();               //使能GPIOF时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟
    __HAL_RCC_GPIOH_CLK_ENABLE();               //使能GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();               //使能GPIOI时钟
    
    //初始化PB5，背光引脚
    GPIO_Initure.Pin=GPIO_PIN_5;                //PB5推挽输出，控制背光
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉        
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //初始化PF10
    GPIO_Initure.Pin=GPIO_PIN_10; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //复用
    GPIO_Initure.Pull=GPIO_NOPULL;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; //高速
    GPIO_Initure.Alternate=GPIO_AF14_LTDC;      //复用为LTDC
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //初始化PG6,7,11
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    //初始化PH9,10,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    //初始化PI0,1,2,4,5,6,7,9,10
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|\
                     GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI,&GPIO_Initure); 
}

/** @}
*/
/*********************RGB_LCD Group1**************************/

/** @defgroup RGB_LCD_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the XXXX
  @endverbatim
  * @{
  */
/**
  * @brief 
  * @param param: brief.
  * @retval status
  */

//打开LCD开关
//lcd_switch:1 打开,0，关闭
void LTDC_Switch(uint8_t sw)
{
	if(sw==1) __HAL_LTDC_ENABLE(&LTDC_Handler);
	else if(sw==0)__HAL_LTDC_DISABLE(&LTDC_Handler);
}

//开关指定层
//layerx:层号,0,第一层; 1,第二层
//sw:1 打开;0关闭
void LTDC_Layer_Switch(uint8_t layerx,uint8_t sw)
{
	if(sw==1) __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,layerx);
	else if(sw==0) __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,layerx);
	__HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
}

//选择层
//layerx:层号;0,第一层;1,第二层;
void LTDC_Select_Layer(uint8_t layerx)
{
	lcdltdc.activelayer=layerx;
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LTDC_Display_Dir(uint8_t dir)
{
    lcdltdc.dir=dir; 	//显示方向
	if(dir==0)			//竖屏
	{
		lcdltdc.width=lcdltdc.pheight;
		lcdltdc.height=lcdltdc.pwidth;	
	}else if(dir==1)	//横屏
	{
		lcdltdc.width=lcdltdc.pwidth;
		lcdltdc.height=lcdltdc.pheight;
	}
}

//画点函数
//x,y:坐标
//color:颜色值
void LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color)
{ 
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//横屏
	{
        *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
	}else 			//竖屏
	{
        *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color; 
	}
#else
	if(lcdltdc.dir)	//横屏
	{
        *(uint16_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
	}else 			//竖屏
	{
        *(uint16_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color; 
	}
#endif
}

//读点函数
//返回值:颜色值
uint32_t LTDC_Read_Point(uint16_t x,uint16_t y)
{ 
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//横屏
	{
		return *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}else 			//竖屏
	{
		return *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)); 
	}
#else
	if(lcdltdc.dir)	//横屏
	{
		return *(uint16_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}else 			//竖屏
	{
		return *(uint16_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)); 
	}
#endif 
}

//LTDC填充矩形,DMA2D填充
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色
void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
{ 
	uint32_t psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
	uint32_t timeout=0; 
	uint16_t offline;
	uint32_t addr; 
	//坐标系转换
	if(lcdltdc.dir)	//横屏
	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
	}else			//竖屏
	{
		psx=sy;psy=lcdltdc.pheight-ex-1;
		pex=ey;pey=lcdltdc.pheight-sx-1;
	} 
	offline=lcdltdc.pwidth-(pex-psx+1);
	addr=((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	RCC->AHB1ENR|=1<<23;			//使能DM2D时钟
	DMA2D->CR=3<<16;				//寄存器到存储器模式
	DMA2D->OPFCCR=LCD_PIXFORMAT;	//设置颜色格式
	DMA2D->OOR=offline;				//设置行偏移 
	DMA2D->CR&=~(1<<0);				//先停止DMA2D
	DMA2D->OMAR=addr;				//输出存储器地址
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器
	DMA2D->OCOLR=color;				//设定输出颜色寄存器 
	DMA2D->CR|=1<<0;				//启动DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//超时退出
	}  
	DMA2D->IFCR|=1<<1;				//清除传输完成标志 	
}
//在指定区域内填充指定颜色块,DMA2D填充	
//此函数仅支持uint16_t,RGB565格式的颜色数组填充.
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色数组首地址
void LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{
	uint32_t psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
	uint32_t timeout=0; 
	uint16_t offline;
	uint32_t addr; 
	//坐标系转换
	if(lcdltdc.dir)	//横屏
	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
	}else			//竖屏
	{
		psx=sy;psy=lcdltdc.pheight-ex-1;
		pex=ey;pey=lcdltdc.pheight-sx-1;
	}
	offline=lcdltdc.pwidth-(pex-psx+1);
	addr=((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	RCC->AHB1ENR|=1<<23;			//使能DM2D时钟
	DMA2D->CR=0<<16;				//存储器到存储器模式
	DMA2D->FGPFCCR=LCD_PIXFORMAT;	//设置颜色格式
	DMA2D->FGOR=0;					//前景层行偏移为0
	DMA2D->OOR=offline;				//设置行偏移 
	DMA2D->CR&=~(1<<0);				//先停止DMA2D
	DMA2D->FGMAR=(uint32_t)color;		//源地址
	DMA2D->OMAR=addr;				//输出存储器地址
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器 
	DMA2D->CR|=1<<0;				//启动DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//超时退出
	} 
	DMA2D->IFCR|=1<<1;				//清除传输完成标志  	
} 

//LCD清屏
//color:颜色值
void LTDC_Clear(uint32_t color)
{
	LTDC_Fill(0,0,lcdltdc.width-1,lcdltdc.height-1,color);
}

//LTDC时钟(Fdclk)设置函数
//PLL3_VCO Input=HSE_VALUE/PLL3M
//PLL3_VCO Output=PLL3_VCO Input * PLL3N
//PLLLCDCLK = PLL3_VCO Output/PLL3R
//假如HSE_VALUE=25MHz，PLL3M=5，PLL3N=160,PLL3R=88
//LTDCLK=PLLLCDCLK=25/5*160/88=9MHz

//返回值:0,成功;1,失败。
uint8_t LTDC_Clk_Set(uint32_t pll3m,uint32_t pll3n,uint32_t pll3r)
{
	RCC_PeriphCLKInitTypeDef PeriphClkIniture;
    PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkIniture.PLL3.PLL3M = pll3m;    
    PeriphClkIniture.PLL3.PLL3N = pll3n;
    PeriphClkIniture.PLL3.PLL3P = 2;
    PeriphClkIniture.PLL3.PLL3Q = 2;  
    PeriphClkIniture.PLL3.PLL3R = pll3r;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK) //配置像素时钟，这里配置为时钟为18.75MHZ
    {
        return 0;   //成功
    }
    else return 1;  //失败    
}

//LTDC,层颜窗口设置,窗口以LCD面板坐标系为基准
//注意:此函数必须在LTDC_Layer_Parameter_Config之后再设置.
//layerx:层值,0/1.
//sx,sy:起始坐标
//width,height:宽度和高度
void LTDC_Layer_Window_Config(uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
    HAL_LTDC_SetWindowPosition(&LTDC_Handler,sx,sy,layerx);  //设置窗口的位置
    HAL_LTDC_SetWindowSize(&LTDC_Handler,width,height,layerx);//设置窗口大小    
}

//LTDC,基本参数设置.
//注意:此函数,必须在LTDC_Layer_Window_Config之前设置.
//layerx:层值,0/1.
//bufaddr:层颜色帧缓存起始地址
//pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:层颜色Alpha值,0,全透明;255,不透明
//alpha0:默认颜色Alpha值,0,全透明;255,不透明
//bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
//bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
//bkcolor:层默认颜色,32位,低24位有效,RGB888格式
//返回值:无
void LTDC_Layer_Parameter_Config(uint8_t layerx,uint32_t bufaddr,uint8_t pixformat,uint8_t alpha,uint8_t alpha0,uint8_t bfac1,uint8_t bfac2,uint32_t bkcolor)
{
	LTDC_LayerCfgTypeDef pLayerCfg;
	
	pLayerCfg.WindowX0=0;                       //窗口起始X坐标
	pLayerCfg.WindowY0=0;                       //窗口起始Y坐标
	pLayerCfg.WindowX1=lcdltdc.pwidth;          //窗口终止X坐标
	pLayerCfg.WindowY1=lcdltdc.pheight;         //窗口终止Y坐标
	pLayerCfg.PixelFormat=pixformat;		    //像素格式
	pLayerCfg.Alpha=alpha;				        //Alpha值设置，0~255,255为完全不透明
	pLayerCfg.Alpha0=alpha0;			        //默认Alpha值
	pLayerCfg.BlendingFactor1=(uint32_t)bfac1<<8;    //设置层混合系数
	pLayerCfg.BlendingFactor2=(uint32_t)bfac2<<8;	//设置层混合系数
	pLayerCfg.FBStartAdress=bufaddr;	        //设置层颜色帧缓存起始地址
	pLayerCfg.ImageWidth=lcdltdc.pwidth;        //设置颜色帧缓冲区的宽度    
	pLayerCfg.ImageHeight=lcdltdc.pheight;      //设置颜色帧缓冲区的高度
	pLayerCfg.Backcolor.Red=(uint8_t)(bkcolor&0X00FF0000)>>16;   //背景颜色红色部分
	pLayerCfg.Backcolor.Green=(uint8_t)(bkcolor&0X0000FF00)>>8;  //背景颜色绿色部分
	pLayerCfg.Backcolor.Blue=(uint8_t)bkcolor&0X000000FF;        //背景颜色蓝色部分
	HAL_LTDC_ConfigLayer(&LTDC_Handler,&pLayerCfg,layerx);   //设置所选中的层
}  

//读取面板参数
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0	//4.3寸480*272 RGB屏,ID=0X4342
//0 :0 :1	//7寸800*480 RGB屏,ID=0X7084
//0 :1 :0	//7寸1024*600 RGB屏,ID=0X7016
//0 :1 :1	//7寸1280*800 RGB屏,ID=0X7018
//1 :0 :0	//8寸1024*768 RGB屏,ID=0X8017 
//返回值:LCD ID:0,非法;其他值,ID;
uint16_t LTDC_PanelID_Read(void)
{
	uint8_t idx=0;
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();       //使能GPIOG时钟
	__HAL_RCC_GPIOI_CLK_ENABLE();       //使能GPIOI时钟
    
    GPIO_Initure.Pin=GPIO_PIN_6;        //PG6
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //输入
    GPIO_Initure.Pull=GPIO_PULLUP;      //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; //高速
    HAL_GPIO_Init(GPIOG,&GPIO_Initure); //初始化
    
    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_7; //PI2,7
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化
    
    idx=(uint8_t)HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_6); //读取M0
    idx|=(uint8_t)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_2)<<1;//读取M1
    idx|=(uint8_t)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_7)<<2;//读取M2
	if(idx==0)return 0X4342;		//4.3寸屏,480*272分辨率
	else if(idx==1)return 0X7084;	//7寸屏,800*480分辨率
	else if(idx==2)return 0X7016;	//7寸屏,1024*600分辨率
	else if(idx==3)return 0X7018;	//7寸屏,1280*800分辨率
	else if(idx==4)return 0X8016;	//8寸屏,1024*600分辨率
	else if(idx==5)return 0X1018;	//10.1寸屏,1280*800分辨率		
	else return 0;
}

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(__IO uint16_t regval)
{   
	regval=regval;		//使用-O2优化的时候,必须插入的延时
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(__IO uint16_t data)
{	  
	data=data;			//使用-O2优化的时候,必须插入的延时
	LCD->LCD_RAM=data;		 
}
//读LCD数据
//返回值:读到的值
uint16_t LCD_RD_DATA(void)
{
	__IO uint16_t ram;			//防止被优化
	ram=LCD->LCD_RAM;	
	return ram;	 
}					   
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}	   
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}   
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(uint16_t RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
uint16_t LCD_BGR2RGB(uint16_t c)
{
	uint16_t  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(uint8_t i)
{
	while(i--);
}
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
uint32_t LCD_ReadPoint(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回
	if(lcdltdc.pwidth!=0)							//如果是RGB屏
	{
		return LTDC_Read_Point(x,y);
	}
	LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X2E);//9341/3510/1963 发送读GRAM指令
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 发送读GRAM指令
 	r=LCD_RD_DATA();								//dummy Read	   
	if(lcddev.id==0X1963)return r;					//1963直接读就可以 
	opt_delay(2);	  
 	r=LCD_RD_DATA();  		  						//实际坐标颜色
	//9341/NT35310/NT35510要分2次读出 
	opt_delay(2);	  
	b=LCD_RD_DATA(); 
	g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8; 
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310/NT35510需要公式转换一下
}			 
//LCD开启显示
void LCD_DisplayOn(void)
{					   
	if(lcdltdc.pwidth!=0)LTDC_Switch(1);//开启LCD
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X29);	//开启显示
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2900);	//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	if(lcdltdc.pwidth!=0)LTDC_Switch(0);//关闭LCD
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X28);	//关闭显示
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2800);	//关闭显示
}   
//设置光标位置(对RGB屏无效)
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 
 	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{		    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
	}else if(lcddev.id==0X1963)
	{  			 		
		if(lcddev.dir==0)//x坐标需要变换
		{
			Xpos=lcddev.width-1-Xpos;
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0); 		
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF);		 	 
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 		
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);		 	 			
		}	
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF); 			 		
		
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(Xpos>>8); 		
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(Xpos&0XFF);			 
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(Ypos>>8);  		
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(Ypos&0XFF);			
	} 
} 		 
//设置LCD的自动扫描方向(对RGB屏无效)
//注意:其他函数可能会受到此函数设置的影响(尤其是9341),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9341/5310/5510/1963等IC已经实际测试	   	   
void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;  
	if((lcddev.dir==1&&lcddev.id!=0X1963)||(lcddev.dir==0&&lcddev.id==0X1963))//横屏时，对1963不改变扫描方向！竖屏时1963改变方向
	{			   
		switch(dir)//方向转换
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcddev.id==0x9341||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//9341/5310/5510/1963,特殊处理
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))regval|=0X08;//5310/5510/1963不需要BGR   
 		LCD_WriteReg(dirreg,regval);
		if(lcddev.id!=0X1963)//1963不做坐标处理
		{
			if(regval&0X20)
			{
				if(lcddev.width<lcddev.height)//交换X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}else  
			{
				if(lcddev.width>lcddev.height)//交换X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}  
		}
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	} 
}     
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	if(lcdltdc.pwidth!=0)//如果是RGB屏
	{
		LTDC_Draw_Point(x,y,POINT_COLOR);
	}else
	{
		LCD_SetCursor(x,y);		//设置光标位置 
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		LCD->LCD_RAM=POINT_COLOR; 
	}
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{	   
	if(lcdltdc.pwidth!=0)//如果是RGB屏
	{
		LTDC_Draw_Point(x,y,color);
		return;
	}else if(lcddev.id==0X9341||lcddev.id==0X5310)
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		 	 
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(x>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(x&0XFF);	  
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(y>>8);  
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(y&0XFF); 
	}else if(lcddev.id==0X1963)
	{
		if(lcddev.dir==0)x=lcddev.width-1-x;
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 		
		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 		
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		
		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		
	}		 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=color; 
}	 
//SSD1963 背光设置
//pwm:背光等级,0~100.越大越亮.
void LCD_SSD_BackLightSet(uint8_t pwm)
{	
	LCD_WR_REG(0xBE);	//配置PWM输出
	LCD_WR_DATA(0x05);	//1设置PWM频率
	LCD_WR_DATA(pwm*2.55);//2设置PWM占空比
	LCD_WR_DATA(0x01);	//3设置C
	LCD_WR_DATA(0xFF);	//4设置D
	LCD_WR_DATA(0x00);	//5设置E
	LCD_WR_DATA(0x00);	//6设置F
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir)
{
    lcddev.dir=dir;         //横屏/竖屏
	if(lcdltdc.pwidth!=0)   //如果是RGB屏
	{
		LTDC_Display_Dir(dir);
		lcddev.width=lcdltdc.width;
		lcddev.height=lcdltdc.height;
		return;
	}
	if(dir==0)			//竖屏
	{
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X5310)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=480;
			lcddev.height=800;
		}else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//设置写入GRAM的指令 
			lcddev.setxcmd=0X2B;	//设置写X坐标指令
			lcddev.setycmd=0X2A;	//设置写Y坐标指令
			lcddev.width=480;		//设置宽度480
			lcddev.height=800;		//设置高度800  
		}
	}else 				//横屏
	{	  				
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=800;
			lcddev.height=480;
		}else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//设置写入GRAM的指令 
			lcddev.setxcmd=0X2A;	//设置写X坐标指令
			lcddev.setycmd=0X2B;	//设置写Y坐标指令
			lcddev.width=800;		//设置宽度800
			lcddev.height=480;		//设置高度480  
		}
		if(lcddev.id==0X5310)
		{ 	 
			lcddev.width=480;
			lcddev.height=320; 			
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}	 
//设置窗口(对RGB屏无效),并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{     
	uint16_t twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcddev.id==0X9341||lcddev.id==0X5310||(lcddev.dir==1&&lcddev.id==0X1963))
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(twidth>>8); 
		LCD_WR_DATA(twidth&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(theight>>8); 
		LCD_WR_DATA(theight&0XFF); 
	}else if(lcddev.id==0X1963)//1963竖屏特殊处理
	{
		sx=lcddev.width-width-sx; 
		height=sy+height-1; 
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA((sx+width-1)>>8); 
		LCD_WR_DATA((sx+width-1)&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(height>>8); 
		LCD_WR_DATA(height&0XFF); 		
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(sx>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(sx&0XFF);	  
		LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA(twidth>>8);   
		LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA(twidth&0XFF);   
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(sy>>8);   
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(sy&0XFF);  
		LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA(theight>>8);   
		LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA(theight&0XFF);  
	} 
}

//配置MPU的region,外部SRAM区配置为透写模式
void LCD_MPU_Config(void)
{	
	MPU_Region_InitTypeDef MPU_Initure;

	HAL_MPU_Disable();							//配置MPU之前先关闭MPU,配置完成以后在使能MPU	
	//外部SRAM为region0，大小为2MB，此区域可读写
	MPU_Initure.Enable=MPU_REGION_ENABLE;	    //使能region
	MPU_Initure.Number=LCD_REGION_NUMBER;		//设置region，外部SRAM使用的region0
	MPU_Initure.BaseAddress=LCD_ADDRESS_START;	//region基地址
	MPU_Initure.Size=LCD_REGION_SIZE;			//region大小
	MPU_Initure.SubRegionDisable=0X00;
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;
	MPU_Initure.AccessPermission=MPU_REGION_FULL_ACCESS;	//此region可读写
	MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;	//允许读取此区域中的指令
	MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;
	MPU_Initure.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
	MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;
	HAL_MPU_ConfigRegion(&MPU_Initure);
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);     //开启MPU
}
//SRAM底层驱动，时钟使能，引脚分配
//此函数会被HAL_SRAM_Init()调用
//hsram:SRAM句柄
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
  GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_FMC_CLK_ENABLE();				//使能FMC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();			//使能GPIOD时钟
	__HAL_RCC_GPIOE_CLK_ENABLE();			//使能GPIOE时钟
	
	//初始化PD0,1,4,5,7,8,9,10,13,14,15
	GPIO_Initure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode = GPIO_MODE_AF_PP; 		//复用
	GPIO_Initure.Pull = GPIO_PULLUP;			//上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;	//高速
	GPIO_Initure.Alternate = GPIO_AF12_FMC;	//复用为FMC
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	//初始化PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}
//清屏函数
//color:要清屏的填充色
void LCD_Clear(uint32_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	if(lcdltdc.pwidth!=0)	//如果是RGB屏
	{
		LTDC_Clear(color);
	}else
	{
		totalpoint*=lcddev.height; 			//得到总点数
		LCD_SetCursor(0x00,0x0000);			//设置光标位置 
		LCD_WriteRAM_Prepare();     		//开始写入GRAM	 	  
		for(index=0;index<totalpoint;index++)
		{
			LCD->LCD_RAM=color;	
		}
	}
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	if(lcdltdc.pwidth!=0)	//如果是RGB屏
	{
		LTDC_Fill(sx,sy,ex,ey,color);
	}else
	{	
		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
			LCD_SetCursor(sx,i);      				//设置光标位置 
			LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
			for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//显示颜色 	    
		} 
	}
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	if(lcdltdc.pwidth!=0)	//如果是RGB屏
	{
		LTDC_Color_Fill(sx,sy,ex,ey,color);
	}else
	{
		width=ex-sx+1; 			//得到填充的宽度
		height=ey-sy+1;			//高度
		for(i=0;i<height;i++)
		{
			LCD_SetCursor(sx,sy+i);   	//设置光标位置 
			LCD_WriteRAM_Prepare();     //开始写入GRAM
			for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];//写入数据 
		}
	}	
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}        
/** @}
*/
/****************************XXXX Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------LCD------------------------------------*/
/** @}
*/
/*----------------------------------------------LCD-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
