/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/RDB_LCD/Lcd_Bsp.c
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
/********************************************Macro**************************************************/
#include "font.h"
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

LTDC_HandleTypeDef  LTDC_Handler;	    //LTDC���
DMA2D_HandleTypeDef DMA2D_Handler; 	  //DMA2D���
//���ݲ�ͬ����ɫ��ʽ,����֡��������
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	uint32_t ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
#else
	uint16_t ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
#endif

uint32_t *ltdc_framebuf[2];					//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
_ltdc_dev lcdltdc;						//����LCD LTDC����Ҫ����
SRAM_HandleTypeDef SRAM_Handler;    //SRAM���(���ڿ���LCD)
//LCD�Ļ�����ɫ�ͱ���ɫ	   
uint32_t POINT_COLOR=0xFF000000;		//������ɫ
uint32_t BACK_COLOR =0xFFFFFFFF;  	//����ɫ 
  
//����LCD��Ҫ����
//Ĭ��Ϊ����
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
	
void Bsp_LCD_Init(void)
{ 	  
	GPIO_InitTypeDef GPIO_Initure;
	FMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
	FMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
    
	lcddev.id=Bsp_LTDC_PanelID_Read();	//����Ƿ���RGB������
	if(lcddev.id!=0)
	{
		Bsp_RGB_LCD_Init();			    //ID����,˵����RGB������.
	}
	else
	{    
        __HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
        GPIO_Initure.Pin=GPIO_PIN_5;           //PB5,�������
        GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
        GPIO_Initure.Pull=GPIO_PULLUP;          //����
        GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //����
        HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
        
        Bsp_Lcd_Mpu_Config();                       //ʹ��MPU����LCD����
        SRAM_Handler.Instance=FMC_NORSRAM_DEVICE; //BANK1
        SRAM_Handler.Extended=FMC_NORSRAM_EXTENDED_DEVICE;       
        
        SRAM_Handler.Init.NSBank=FMC_NORSRAM_BANK1;     //ʹ��NE1
        SRAM_Handler.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;  //������������
        SRAM_Handler.Init.MemoryType=FMC_MEMORY_TYPE_SRAM;              //SRAM
        SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //16λ���ݿ���
        SRAM_Handler.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE; //�Ƿ�ʹ��ͻ������,����ͬ��ͻ���洢����Ч,�˴�δ�õ�
        SRAM_Handler.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;//�ȴ��źŵļ���,����ͻ��ģʽ����������
        SRAM_Handler.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;   //�洢�����ڵȴ�����֮ǰ��һ��ʱ�����ڻ��ǵȴ������ڼ�ʹ��NWAIT
        SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;    //�洢��дʹ��
        SRAM_Handler.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;           //�ȴ�ʹ��λ,�˴�δ�õ�
        SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;        //��дʹ�ò�ͬ��ʱ��
        SRAM_Handler.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;//�Ƿ�ʹ��ͬ������ģʽ�µĵȴ��ź�,�˴�δ�õ�
        SRAM_Handler.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           //��ֹͻ��д
        SRAM_Handler.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
        
        //FSMC��ʱ����ƼĴ���
        FSMC_ReadWriteTim.AddressSetupTime=0x11;    //��ַ����ʱ��(ADDSET)Ϊ17��HCLK 1/216M=4.6ns*17=78ns
        FSMC_ReadWriteTim.AddressHoldTime=0x00;
        FSMC_ReadWriteTim.DataSetupTime=0x55;       //���ݱ���ʱ��(DATAST)Ϊ85��HCLK	=4.6*85=391ns
        FSMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //ģʽA
        //FSMCдʱ����ƼĴ���
        FSMC_WriteTim.AddressSetupTime=0x15;        //��ַ����ʱ��(ADDSET)Ϊ21��HCLK=96ns
        FSMC_WriteTim.AddressHoldTime=0x00;
        FSMC_WriteTim.DataSetupTime=0x15;           //���ݱ���ʱ��(DATAST)Ϊ4.6ns*21��HCLK=96ns
        FSMC_WriteTim.AccessMode=FMC_ACCESS_MODE_A;     //ģʽA
		
        HAL_SRAM_Init(&SRAM_Handler,&FSMC_ReadWriteTim,&FSMC_WriteTim);		
        Bsp_Delay_ms(50); // delay 50 ms 
        
        //����9341 ID�Ķ�ȡ		
        Bsp_Lcd_WR_REG(0XD3);				   
        lcddev.id=Bsp_Lcd_RD_DATA();	//dummy read 	
        lcddev.id=Bsp_Lcd_RD_DATA();	//����0X00
        lcddev.id=Bsp_Lcd_RD_DATA();   	//��ȡ93								   
        lcddev.id<<=8;
        lcddev.id|=Bsp_Lcd_RD_DATA();  	//��ȡ41 	   			   
        if(lcddev.id!=0X9341)		//��9341,���Կ����ǲ���NT35310
        {	 
            Bsp_Lcd_WR_REG(0XD4);				   
            lcddev.id=Bsp_Lcd_RD_DATA();//dummy read  
            lcddev.id=Bsp_Lcd_RD_DATA();//����0X01	 
            lcddev.id=Bsp_Lcd_RD_DATA();//����0X53	
            lcddev.id<<=8;	 
            lcddev.id|=Bsp_Lcd_RD_DATA();	//�������0X10	 
            if(lcddev.id!=0X5310)		//Ҳ����NT35310,���Կ����ǲ���NT35510
            {
                Bsp_Lcd_WR_REG(0XDA00);	
                lcddev.id=Bsp_Lcd_RD_DATA();		//����0X00	 
                Bsp_Lcd_WR_REG(0XDB00);	
                lcddev.id=Bsp_Lcd_RD_DATA();		//����0X80
                lcddev.id<<=8;	
                Bsp_Lcd_WR_REG(0XDC00);	
                lcddev.id|=Bsp_Lcd_RD_DATA();		//����0X00		
                if(lcddev.id==0x8000)lcddev.id=0x5510;//NT35510���ص�ID��8000H,Ϊ��������,����ǿ������Ϊ5510
                if(lcddev.id!=0X5510)			//Ҳ����NT5510,���Կ����ǲ���SSD1963
                {
                    Bsp_Lcd_WR_REG(0XA1);
                    lcddev.id=Bsp_Lcd_RD_DATA();
                    lcddev.id=Bsp_Lcd_RD_DATA();	//����0X57
                    lcddev.id<<=8;	 
                    lcddev.id|=Bsp_Lcd_RD_DATA();	//����0X61	
                    if(lcddev.id==0X5761)lcddev.id=0X1963;//SSD1963���ص�ID��5761H,Ϊ��������,����ǿ������Ϊ1963
                } 
            }
        }   
        if(lcddev.id==0X9341)	//9341��ʼ��
        {	 
            Bsp_Lcd_WR_REG(0xCF);  
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_DATA(0xC1); 
            Bsp_Lcd_WR_DATA(0X30); 
            Bsp_Lcd_WR_REG(0xED);  
            Bsp_Lcd_WR_DATA(0x64); 
            Bsp_Lcd_WR_DATA(0x03); 
            Bsp_Lcd_WR_DATA(0X12); 
            Bsp_Lcd_WR_DATA(0X81); 
            Bsp_Lcd_WR_REG(0xE8);  
            Bsp_Lcd_WR_DATA(0x85); 
            Bsp_Lcd_WR_DATA(0x10); 
            Bsp_Lcd_WR_DATA(0x7A); 
            Bsp_Lcd_WR_REG(0xCB);  
            Bsp_Lcd_WR_DATA(0x39); 
            Bsp_Lcd_WR_DATA(0x2C); 
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_DATA(0x34); 
            Bsp_Lcd_WR_DATA(0x02); 
            Bsp_Lcd_WR_REG(0xF7);  
            Bsp_Lcd_WR_DATA(0x20); 
            Bsp_Lcd_WR_REG(0xEA);  
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_REG(0xC0);    //Power control 
            Bsp_Lcd_WR_DATA(0x1B);   //VRH[5:0] 
            Bsp_Lcd_WR_REG(0xC1);    //Power control 
            Bsp_Lcd_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
            Bsp_Lcd_WR_REG(0xC5);    //VCM control 
            Bsp_Lcd_WR_DATA(0x30); 	 //3F
            Bsp_Lcd_WR_DATA(0x30); 	 //3C
            Bsp_Lcd_WR_REG(0xC7);    //VCM control2 
            Bsp_Lcd_WR_DATA(0XB7); 
            Bsp_Lcd_WR_REG(0x36);    // Memory Access Control 
            Bsp_Lcd_WR_DATA(0x48); 
            Bsp_Lcd_WR_REG(0x3A);   
            Bsp_Lcd_WR_DATA(0x55); 
            Bsp_Lcd_WR_REG(0xB1);   
            Bsp_Lcd_WR_DATA(0x00);   
            Bsp_Lcd_WR_DATA(0x1A); 
            Bsp_Lcd_WR_REG(0xB6);    // Display Function Control 
            Bsp_Lcd_WR_DATA(0x0A); 
            Bsp_Lcd_WR_DATA(0xA2); 
            Bsp_Lcd_WR_REG(0xF2);    // 3Gamma Function Disable 
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_REG(0x26);    //Gamma curve selected 
            Bsp_Lcd_WR_DATA(0x01); 
            Bsp_Lcd_WR_REG(0xE0);    //Set Gamma 
            Bsp_Lcd_WR_DATA(0x0F); 
            Bsp_Lcd_WR_DATA(0x2A); 
            Bsp_Lcd_WR_DATA(0x28); 
            Bsp_Lcd_WR_DATA(0x08); 
            Bsp_Lcd_WR_DATA(0x0E); 
            Bsp_Lcd_WR_DATA(0x08); 
            Bsp_Lcd_WR_DATA(0x54); 
            Bsp_Lcd_WR_DATA(0XA9); 
            Bsp_Lcd_WR_DATA(0x43); 
            Bsp_Lcd_WR_DATA(0x0A); 
            Bsp_Lcd_WR_DATA(0x0F); 
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_DATA(0x00); 		 
            Bsp_Lcd_WR_REG(0XE1);    //Set Gamma 
            Bsp_Lcd_WR_DATA(0x00); 
            Bsp_Lcd_WR_DATA(0x15); 
            Bsp_Lcd_WR_DATA(0x17); 
            Bsp_Lcd_WR_DATA(0x07); 
            Bsp_Lcd_WR_DATA(0x11); 
            Bsp_Lcd_WR_DATA(0x06); 
            Bsp_Lcd_WR_DATA(0x2B); 
            Bsp_Lcd_WR_DATA(0x56); 
            Bsp_Lcd_WR_DATA(0x3C); 
            Bsp_Lcd_WR_DATA(0x05); 
            Bsp_Lcd_WR_DATA(0x10); 
            Bsp_Lcd_WR_DATA(0x0F); 
            Bsp_Lcd_WR_DATA(0x3F); 
            Bsp_Lcd_WR_DATA(0x3F); 
            Bsp_Lcd_WR_DATA(0x0F); 
            Bsp_Lcd_WR_REG(0x2B); 
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x01);
            Bsp_Lcd_WR_DATA(0x3f);
            Bsp_Lcd_WR_REG(0x2A); 
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xef);	 
            Bsp_Lcd_WR_REG(0x11); //Exit Sleep
            Bsp_Delay_ms(120);
            Bsp_Lcd_WR_REG(0x29); //display on	
        }else if(lcddev.id==0x5310)
        { 
            Bsp_Lcd_WR_REG(0xED);
            Bsp_Lcd_WR_DATA(0x01);
            Bsp_Lcd_WR_DATA(0xFE);

            Bsp_Lcd_WR_REG(0xEE);
            Bsp_Lcd_WR_DATA(0xDE);
            Bsp_Lcd_WR_DATA(0x21);

            Bsp_Lcd_WR_REG(0xF1);
            Bsp_Lcd_WR_DATA(0x01);
            Bsp_Lcd_WR_REG(0xDF);
            Bsp_Lcd_WR_DATA(0x10);

            //VCOMvoltage//
            Bsp_Lcd_WR_REG(0xC4);
            Bsp_Lcd_WR_DATA(0x8F);	  //5f

            Bsp_Lcd_WR_REG(0xC6);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xE2);
            Bsp_Lcd_WR_DATA(0xE2);
            Bsp_Lcd_WR_DATA(0xE2);
            Bsp_Lcd_WR_REG(0xBF);
            Bsp_Lcd_WR_DATA(0xAA);

            Bsp_Lcd_WR_REG(0xB0);
            Bsp_Lcd_WR_DATA(0x0D);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x0D);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x11);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x19);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x21);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x2D);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x3D);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x5D);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x5D);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xB1);
            Bsp_Lcd_WR_DATA(0x80);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x8B);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x96);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xB2);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x02);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x03);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xB3);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xB4);
            Bsp_Lcd_WR_DATA(0x8B);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x96);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA1);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xB5);
            Bsp_Lcd_WR_DATA(0x02);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x03);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x04);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xB6);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xB7);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x3F);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x5E);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x64);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x8C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xAC);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xDC);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x70);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x90);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xEB);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xDC);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xB8);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xBA);
            Bsp_Lcd_WR_DATA(0x24);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC1);
            Bsp_Lcd_WR_DATA(0x20);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x54);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xFF);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC2);
            Bsp_Lcd_WR_DATA(0x0A);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x04);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC3);
            Bsp_Lcd_WR_DATA(0x3C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x3A);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x39);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x37);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x3C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x36);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x32);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x2F);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x2C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x29);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x26);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x24);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x24);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x23);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x3C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x36);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x32);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x2F);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x2C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x29);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x26);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x24);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x24);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x23);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC4);
            Bsp_Lcd_WR_DATA(0x62);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x05);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x84);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xF0);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x18);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA4);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x18);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x50);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x0C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x17);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x95);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xF3);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xE6);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC5);
            Bsp_Lcd_WR_DATA(0x32);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x44);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x65);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x76);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x88);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC6);
            Bsp_Lcd_WR_DATA(0x20);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x17);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x01);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC7);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC8);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xC9);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xE0);
            Bsp_Lcd_WR_DATA(0x16);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x1C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x21);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x36);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x46);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x52);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x64);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x7A);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x8B);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x99);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA8);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xB9);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xC4);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xCA);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD2);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD9);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xE0);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xF3);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xE1);
            Bsp_Lcd_WR_DATA(0x16);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x1C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x22);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x36);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x45);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x52);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x64);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x7A);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x8B);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x99);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA8);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xB9);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xC4);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xCA);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD2);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD8);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xE0);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xF3);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xE2);
            Bsp_Lcd_WR_DATA(0x05);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x0B);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x1B);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x34);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x44);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x4F);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x61);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x79);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x88);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x97);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA6);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xB7);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xC2);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xC7);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD1);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD6);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xDD);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xF3);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_REG(0xE3);
            Bsp_Lcd_WR_DATA(0x05);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x1C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x33);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x44);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x50);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x62);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x78);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x88);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x97);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA6);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xB7);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xC2);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xC7);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD1);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD5);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xDD);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xF3);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xE4);
            Bsp_Lcd_WR_DATA(0x01);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x01);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x02);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x2A);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x3C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x4B);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x5D);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x74);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x84);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x93);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA2);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xB3);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xBE);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xC4);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xCD);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD3);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xDD);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xF3);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_REG(0xE5);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x02);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x29);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x3C);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x4B);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x5D);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x74);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x84);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x93);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xA2);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xB3);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xBE);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xC4);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xCD);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xD3);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xDC);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xF3);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xE6);
            Bsp_Lcd_WR_DATA(0x11);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x34);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x56);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x76);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x77);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x66);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x88);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x99);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xBB);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x99);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x66);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x55);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x55);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x45);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x43);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x44);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xE7);
            Bsp_Lcd_WR_DATA(0x32);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x55);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x76);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x66);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x67);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x67);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x87);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x99);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xBB);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x99);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x77);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x44);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x56);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x23); 
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x33);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x45);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xE8);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x99);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x87);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x88);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x77);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x66);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x88);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xAA);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0xBB);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x99);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x66);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x55);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x55);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x44);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x44);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x55);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xE9);
            Bsp_Lcd_WR_DATA(0xAA);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0x00);
            Bsp_Lcd_WR_DATA(0xAA);

            Bsp_Lcd_WR_REG(0xCF);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xF0);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x50);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xF3);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0xF9);
            Bsp_Lcd_WR_DATA(0x06);
            Bsp_Lcd_WR_DATA(0x10);
            Bsp_Lcd_WR_DATA(0x29);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0x3A);
            Bsp_Lcd_WR_DATA(0x55);	//66

            Bsp_Lcd_WR_REG(0x11);
            Bsp_Delay_ms(100);
            Bsp_Lcd_WR_REG(0x29);
            Bsp_Lcd_WR_REG(0x35);
            Bsp_Lcd_WR_DATA(0x00);

            Bsp_Lcd_WR_REG(0x51);
            Bsp_Lcd_WR_DATA(0xFF);
            Bsp_Lcd_WR_REG(0x53);
            Bsp_Lcd_WR_DATA(0x2C);
            Bsp_Lcd_WR_REG(0x55);
            Bsp_Lcd_WR_DATA(0x82);
            Bsp_Lcd_WR_REG(0x2c);
        }else if(lcddev.id==0x5510)
        {
            Bsp_LCD_WriteReg(0xF000,0x55);
            Bsp_LCD_WriteReg(0xF001,0xAA);
            Bsp_LCD_WriteReg(0xF002,0x52);
            Bsp_LCD_WriteReg(0xF003,0x08);
            Bsp_LCD_WriteReg(0xF004,0x01);
            //AVDD Set AVDD 5.2V
            Bsp_LCD_WriteReg(0xB000,0x0D);
            Bsp_LCD_WriteReg(0xB001,0x0D);
            Bsp_LCD_WriteReg(0xB002,0x0D);
            //AVDD ratio
            Bsp_LCD_WriteReg(0xB600,0x34);
            Bsp_LCD_WriteReg(0xB601,0x34);
            Bsp_LCD_WriteReg(0xB602,0x34);
            //AVEE -5.2V
            Bsp_LCD_WriteReg(0xB100,0x0D);
            Bsp_LCD_WriteReg(0xB101,0x0D);
            Bsp_LCD_WriteReg(0xB102,0x0D);
            //AVEE ratio
            Bsp_LCD_WriteReg(0xB700,0x34);
            Bsp_LCD_WriteReg(0xB701,0x34);
            Bsp_LCD_WriteReg(0xB702,0x34);
            //VCL -2.5V
            Bsp_LCD_WriteReg(0xB200,0x00);
            Bsp_LCD_WriteReg(0xB201,0x00);
            Bsp_LCD_WriteReg(0xB202,0x00);
            //VCL ratio
            Bsp_LCD_WriteReg(0xB800,0x24);
            Bsp_LCD_WriteReg(0xB801,0x24);
            Bsp_LCD_WriteReg(0xB802,0x24);
            //VGH 15V (Free pump)
            Bsp_LCD_WriteReg(0xBF00,0x01);
            Bsp_LCD_WriteReg(0xB300,0x0F);
            Bsp_LCD_WriteReg(0xB301,0x0F);
            Bsp_LCD_WriteReg(0xB302,0x0F);
            //VGH ratio
            Bsp_LCD_WriteReg(0xB900,0x34);
            Bsp_LCD_WriteReg(0xB901,0x34);
            Bsp_LCD_WriteReg(0xB902,0x34);
            //VGL_REG -10V
            Bsp_LCD_WriteReg(0xB500,0x08);
            Bsp_LCD_WriteReg(0xB501,0x08);
            Bsp_LCD_WriteReg(0xB502,0x08);
            Bsp_LCD_WriteReg(0xC200,0x03);
            //VGLX ratio
            Bsp_LCD_WriteReg(0xBA00,0x24);
            Bsp_LCD_WriteReg(0xBA01,0x24);
            Bsp_LCD_WriteReg(0xBA02,0x24);
            //VGMP/VGSP 4.5V/0V
            Bsp_LCD_WriteReg(0xBC00,0x00);
            Bsp_LCD_WriteReg(0xBC01,0x78);
            Bsp_LCD_WriteReg(0xBC02,0x00);
            //VGMN/VGSN -4.5V/0V
            Bsp_LCD_WriteReg(0xBD00,0x00);
            Bsp_LCD_WriteReg(0xBD01,0x78);
            Bsp_LCD_WriteReg(0xBD02,0x00);
            //VCOM
            Bsp_LCD_WriteReg(0xBE00,0x00);
            Bsp_LCD_WriteReg(0xBE01,0x64);
            //Gamma Setting
            Bsp_LCD_WriteReg(0xD100,0x00);
            Bsp_LCD_WriteReg(0xD101,0x33);
            Bsp_LCD_WriteReg(0xD102,0x00);
            Bsp_LCD_WriteReg(0xD103,0x34);
            Bsp_LCD_WriteReg(0xD104,0x00);
            Bsp_LCD_WriteReg(0xD105,0x3A);
            Bsp_LCD_WriteReg(0xD106,0x00);
            Bsp_LCD_WriteReg(0xD107,0x4A);
            Bsp_LCD_WriteReg(0xD108,0x00);
            Bsp_LCD_WriteReg(0xD109,0x5C);
            Bsp_LCD_WriteReg(0xD10A,0x00);
            Bsp_LCD_WriteReg(0xD10B,0x81);
            Bsp_LCD_WriteReg(0xD10C,0x00);
            Bsp_LCD_WriteReg(0xD10D,0xA6);
            Bsp_LCD_WriteReg(0xD10E,0x00);
            Bsp_LCD_WriteReg(0xD10F,0xE5);
            Bsp_LCD_WriteReg(0xD110,0x01);
            Bsp_LCD_WriteReg(0xD111,0x13);
            Bsp_LCD_WriteReg(0xD112,0x01);
            Bsp_LCD_WriteReg(0xD113,0x54);
            Bsp_LCD_WriteReg(0xD114,0x01);
            Bsp_LCD_WriteReg(0xD115,0x82);
            Bsp_LCD_WriteReg(0xD116,0x01);
            Bsp_LCD_WriteReg(0xD117,0xCA);
            Bsp_LCD_WriteReg(0xD118,0x02);
            Bsp_LCD_WriteReg(0xD119,0x00);
            Bsp_LCD_WriteReg(0xD11A,0x02);
            Bsp_LCD_WriteReg(0xD11B,0x01);
            Bsp_LCD_WriteReg(0xD11C,0x02);
            Bsp_LCD_WriteReg(0xD11D,0x34);
            Bsp_LCD_WriteReg(0xD11E,0x02);
            Bsp_LCD_WriteReg(0xD11F,0x67);
            Bsp_LCD_WriteReg(0xD120,0x02);
            Bsp_LCD_WriteReg(0xD121,0x84);
            Bsp_LCD_WriteReg(0xD122,0x02);
            Bsp_LCD_WriteReg(0xD123,0xA4);
            Bsp_LCD_WriteReg(0xD124,0x02);
            Bsp_LCD_WriteReg(0xD125,0xB7);
            Bsp_LCD_WriteReg(0xD126,0x02);
            Bsp_LCD_WriteReg(0xD127,0xCF);
            Bsp_LCD_WriteReg(0xD128,0x02);
            Bsp_LCD_WriteReg(0xD129,0xDE);
            Bsp_LCD_WriteReg(0xD12A,0x02);
            Bsp_LCD_WriteReg(0xD12B,0xF2);
            Bsp_LCD_WriteReg(0xD12C,0x02);
            Bsp_LCD_WriteReg(0xD12D,0xFE);
            Bsp_LCD_WriteReg(0xD12E,0x03);
            Bsp_LCD_WriteReg(0xD12F,0x10);
            Bsp_LCD_WriteReg(0xD130,0x03);
            Bsp_LCD_WriteReg(0xD131,0x33);
            Bsp_LCD_WriteReg(0xD132,0x03);
            Bsp_LCD_WriteReg(0xD133,0x6D);
            Bsp_LCD_WriteReg(0xD200,0x00);
            Bsp_LCD_WriteReg(0xD201,0x33);
            Bsp_LCD_WriteReg(0xD202,0x00);
            Bsp_LCD_WriteReg(0xD203,0x34);
            Bsp_LCD_WriteReg(0xD204,0x00);
            Bsp_LCD_WriteReg(0xD205,0x3A);
            Bsp_LCD_WriteReg(0xD206,0x00);
            Bsp_LCD_WriteReg(0xD207,0x4A);
            Bsp_LCD_WriteReg(0xD208,0x00);
            Bsp_LCD_WriteReg(0xD209,0x5C);
            Bsp_LCD_WriteReg(0xD20A,0x00);

            Bsp_LCD_WriteReg(0xD20B,0x81);
            Bsp_LCD_WriteReg(0xD20C,0x00);
            Bsp_LCD_WriteReg(0xD20D,0xA6);
            Bsp_LCD_WriteReg(0xD20E,0x00);
            Bsp_LCD_WriteReg(0xD20F,0xE5);
            Bsp_LCD_WriteReg(0xD210,0x01);
            Bsp_LCD_WriteReg(0xD211,0x13);
            Bsp_LCD_WriteReg(0xD212,0x01);
            Bsp_LCD_WriteReg(0xD213,0x54);
            Bsp_LCD_WriteReg(0xD214,0x01);
            Bsp_LCD_WriteReg(0xD215,0x82);
            Bsp_LCD_WriteReg(0xD216,0x01);
            Bsp_LCD_WriteReg(0xD217,0xCA);
            Bsp_LCD_WriteReg(0xD218,0x02);
            Bsp_LCD_WriteReg(0xD219,0x00);
            Bsp_LCD_WriteReg(0xD21A,0x02);
            Bsp_LCD_WriteReg(0xD21B,0x01);
            Bsp_LCD_WriteReg(0xD21C,0x02);
            Bsp_LCD_WriteReg(0xD21D,0x34);
            Bsp_LCD_WriteReg(0xD21E,0x02);
            Bsp_LCD_WriteReg(0xD21F,0x67);
            Bsp_LCD_WriteReg(0xD220,0x02);
            Bsp_LCD_WriteReg(0xD221,0x84);
            Bsp_LCD_WriteReg(0xD222,0x02);
            Bsp_LCD_WriteReg(0xD223,0xA4);
            Bsp_LCD_WriteReg(0xD224,0x02);
            Bsp_LCD_WriteReg(0xD225,0xB7);
            Bsp_LCD_WriteReg(0xD226,0x02);
            Bsp_LCD_WriteReg(0xD227,0xCF);
            Bsp_LCD_WriteReg(0xD228,0x02);
            Bsp_LCD_WriteReg(0xD229,0xDE);
            Bsp_LCD_WriteReg(0xD22A,0x02);
            Bsp_LCD_WriteReg(0xD22B,0xF2);
            Bsp_LCD_WriteReg(0xD22C,0x02);
            Bsp_LCD_WriteReg(0xD22D,0xFE);
            Bsp_LCD_WriteReg(0xD22E,0x03);
            Bsp_LCD_WriteReg(0xD22F,0x10);
            Bsp_LCD_WriteReg(0xD230,0x03);
            Bsp_LCD_WriteReg(0xD231,0x33);
            Bsp_LCD_WriteReg(0xD232,0x03);
            Bsp_LCD_WriteReg(0xD233,0x6D);
            Bsp_LCD_WriteReg(0xD300,0x00);
            Bsp_LCD_WriteReg(0xD301,0x33);
            Bsp_LCD_WriteReg(0xD302,0x00);
            Bsp_LCD_WriteReg(0xD303,0x34);
            Bsp_LCD_WriteReg(0xD304,0x00);
            Bsp_LCD_WriteReg(0xD305,0x3A);
            Bsp_LCD_WriteReg(0xD306,0x00);
            Bsp_LCD_WriteReg(0xD307,0x4A);
            Bsp_LCD_WriteReg(0xD308,0x00);
            Bsp_LCD_WriteReg(0xD309,0x5C);
            Bsp_LCD_WriteReg(0xD30A,0x00);

            Bsp_LCD_WriteReg(0xD30B,0x81);
            Bsp_LCD_WriteReg(0xD30C,0x00);
            Bsp_LCD_WriteReg(0xD30D,0xA6);
            Bsp_LCD_WriteReg(0xD30E,0x00);
            Bsp_LCD_WriteReg(0xD30F,0xE5);
            Bsp_LCD_WriteReg(0xD310,0x01);
            Bsp_LCD_WriteReg(0xD311,0x13);
            Bsp_LCD_WriteReg(0xD312,0x01);
            Bsp_LCD_WriteReg(0xD313,0x54);
            Bsp_LCD_WriteReg(0xD314,0x01);
            Bsp_LCD_WriteReg(0xD315,0x82);
            Bsp_LCD_WriteReg(0xD316,0x01);
            Bsp_LCD_WriteReg(0xD317,0xCA);
            Bsp_LCD_WriteReg(0xD318,0x02);
            Bsp_LCD_WriteReg(0xD319,0x00);
            Bsp_LCD_WriteReg(0xD31A,0x02);
            Bsp_LCD_WriteReg(0xD31B,0x01);
            Bsp_LCD_WriteReg(0xD31C,0x02);
            Bsp_LCD_WriteReg(0xD31D,0x34);
            Bsp_LCD_WriteReg(0xD31E,0x02);
            Bsp_LCD_WriteReg(0xD31F,0x67);
            Bsp_LCD_WriteReg(0xD320,0x02);
            Bsp_LCD_WriteReg(0xD321,0x84);
            Bsp_LCD_WriteReg(0xD322,0x02);
            Bsp_LCD_WriteReg(0xD323,0xA4);
            Bsp_LCD_WriteReg(0xD324,0x02);
            Bsp_LCD_WriteReg(0xD325,0xB7);
            Bsp_LCD_WriteReg(0xD326,0x02);
            Bsp_LCD_WriteReg(0xD327,0xCF);
            Bsp_LCD_WriteReg(0xD328,0x02);
            Bsp_LCD_WriteReg(0xD329,0xDE);
            Bsp_LCD_WriteReg(0xD32A,0x02);
            Bsp_LCD_WriteReg(0xD32B,0xF2);
            Bsp_LCD_WriteReg(0xD32C,0x02);
            Bsp_LCD_WriteReg(0xD32D,0xFE);
            Bsp_LCD_WriteReg(0xD32E,0x03);
            Bsp_LCD_WriteReg(0xD32F,0x10);
            Bsp_LCD_WriteReg(0xD330,0x03);
            Bsp_LCD_WriteReg(0xD331,0x33);
            Bsp_LCD_WriteReg(0xD332,0x03);
            Bsp_LCD_WriteReg(0xD333,0x6D);
            Bsp_LCD_WriteReg(0xD400,0x00);
            Bsp_LCD_WriteReg(0xD401,0x33);
            Bsp_LCD_WriteReg(0xD402,0x00);
            Bsp_LCD_WriteReg(0xD403,0x34);
            Bsp_LCD_WriteReg(0xD404,0x00);
            Bsp_LCD_WriteReg(0xD405,0x3A);
            Bsp_LCD_WriteReg(0xD406,0x00);
            Bsp_LCD_WriteReg(0xD407,0x4A);
            Bsp_LCD_WriteReg(0xD408,0x00);
            Bsp_LCD_WriteReg(0xD409,0x5C);
            Bsp_LCD_WriteReg(0xD40A,0x00);
            Bsp_LCD_WriteReg(0xD40B,0x81);

            Bsp_LCD_WriteReg(0xD40C,0x00);
            Bsp_LCD_WriteReg(0xD40D,0xA6);
            Bsp_LCD_WriteReg(0xD40E,0x00);
            Bsp_LCD_WriteReg(0xD40F,0xE5);
            Bsp_LCD_WriteReg(0xD410,0x01);
            Bsp_LCD_WriteReg(0xD411,0x13);
            Bsp_LCD_WriteReg(0xD412,0x01);
            Bsp_LCD_WriteReg(0xD413,0x54);
            Bsp_LCD_WriteReg(0xD414,0x01);
            Bsp_LCD_WriteReg(0xD415,0x82);
            Bsp_LCD_WriteReg(0xD416,0x01);
            Bsp_LCD_WriteReg(0xD417,0xCA);
            Bsp_LCD_WriteReg(0xD418,0x02);
            Bsp_LCD_WriteReg(0xD419,0x00);
            Bsp_LCD_WriteReg(0xD41A,0x02);
            Bsp_LCD_WriteReg(0xD41B,0x01);
            Bsp_LCD_WriteReg(0xD41C,0x02);
            Bsp_LCD_WriteReg(0xD41D,0x34);
            Bsp_LCD_WriteReg(0xD41E,0x02);
            Bsp_LCD_WriteReg(0xD41F,0x67);
            Bsp_LCD_WriteReg(0xD420,0x02);
            Bsp_LCD_WriteReg(0xD421,0x84);
            Bsp_LCD_WriteReg(0xD422,0x02);
            Bsp_LCD_WriteReg(0xD423,0xA4);
            Bsp_LCD_WriteReg(0xD424,0x02);
            Bsp_LCD_WriteReg(0xD425,0xB7);
            Bsp_LCD_WriteReg(0xD426,0x02);
            Bsp_LCD_WriteReg(0xD427,0xCF);
            Bsp_LCD_WriteReg(0xD428,0x02);
            Bsp_LCD_WriteReg(0xD429,0xDE);
            Bsp_LCD_WriteReg(0xD42A,0x02);
            Bsp_LCD_WriteReg(0xD42B,0xF2);
            Bsp_LCD_WriteReg(0xD42C,0x02);
            Bsp_LCD_WriteReg(0xD42D,0xFE);
            Bsp_LCD_WriteReg(0xD42E,0x03);
            Bsp_LCD_WriteReg(0xD42F,0x10);
            Bsp_LCD_WriteReg(0xD430,0x03);
            Bsp_LCD_WriteReg(0xD431,0x33);
            Bsp_LCD_WriteReg(0xD432,0x03);
            Bsp_LCD_WriteReg(0xD433,0x6D);
            Bsp_LCD_WriteReg(0xD500,0x00);
            Bsp_LCD_WriteReg(0xD501,0x33);
            Bsp_LCD_WriteReg(0xD502,0x00);
            Bsp_LCD_WriteReg(0xD503,0x34);
            Bsp_LCD_WriteReg(0xD504,0x00);
            Bsp_LCD_WriteReg(0xD505,0x3A);
            Bsp_LCD_WriteReg(0xD506,0x00);
            Bsp_LCD_WriteReg(0xD507,0x4A);
            Bsp_LCD_WriteReg(0xD508,0x00);
            Bsp_LCD_WriteReg(0xD509,0x5C);
            Bsp_LCD_WriteReg(0xD50A,0x00);
            Bsp_LCD_WriteReg(0xD50B,0x81);

            Bsp_LCD_WriteReg(0xD50C,0x00);
            Bsp_LCD_WriteReg(0xD50D,0xA6);
            Bsp_LCD_WriteReg(0xD50E,0x00);
            Bsp_LCD_WriteReg(0xD50F,0xE5);
            Bsp_LCD_WriteReg(0xD510,0x01);
            Bsp_LCD_WriteReg(0xD511,0x13);
            Bsp_LCD_WriteReg(0xD512,0x01);
            Bsp_LCD_WriteReg(0xD513,0x54);
            Bsp_LCD_WriteReg(0xD514,0x01);
            Bsp_LCD_WriteReg(0xD515,0x82);
            Bsp_LCD_WriteReg(0xD516,0x01);
            Bsp_LCD_WriteReg(0xD517,0xCA);
            Bsp_LCD_WriteReg(0xD518,0x02);
            Bsp_LCD_WriteReg(0xD519,0x00);
            Bsp_LCD_WriteReg(0xD51A,0x02);
            Bsp_LCD_WriteReg(0xD51B,0x01);
            Bsp_LCD_WriteReg(0xD51C,0x02);
            Bsp_LCD_WriteReg(0xD51D,0x34);
            Bsp_LCD_WriteReg(0xD51E,0x02);
            Bsp_LCD_WriteReg(0xD51F,0x67);
            Bsp_LCD_WriteReg(0xD520,0x02);
            Bsp_LCD_WriteReg(0xD521,0x84);
            Bsp_LCD_WriteReg(0xD522,0x02);
            Bsp_LCD_WriteReg(0xD523,0xA4);
            Bsp_LCD_WriteReg(0xD524,0x02);
            Bsp_LCD_WriteReg(0xD525,0xB7);
            Bsp_LCD_WriteReg(0xD526,0x02);
            Bsp_LCD_WriteReg(0xD527,0xCF);
            Bsp_LCD_WriteReg(0xD528,0x02);
            Bsp_LCD_WriteReg(0xD529,0xDE);
            Bsp_LCD_WriteReg(0xD52A,0x02);
            Bsp_LCD_WriteReg(0xD52B,0xF2);
            Bsp_LCD_WriteReg(0xD52C,0x02);
            Bsp_LCD_WriteReg(0xD52D,0xFE);
            Bsp_LCD_WriteReg(0xD52E,0x03);
            Bsp_LCD_WriteReg(0xD52F,0x10);
            Bsp_LCD_WriteReg(0xD530,0x03);
            Bsp_LCD_WriteReg(0xD531,0x33);
            Bsp_LCD_WriteReg(0xD532,0x03);
            Bsp_LCD_WriteReg(0xD533,0x6D);
            Bsp_LCD_WriteReg(0xD600,0x00);
            Bsp_LCD_WriteReg(0xD601,0x33);
            Bsp_LCD_WriteReg(0xD602,0x00);
            Bsp_LCD_WriteReg(0xD603,0x34);
            Bsp_LCD_WriteReg(0xD604,0x00);
            Bsp_LCD_WriteReg(0xD605,0x3A);
            Bsp_LCD_WriteReg(0xD606,0x00);
            Bsp_LCD_WriteReg(0xD607,0x4A);
            Bsp_LCD_WriteReg(0xD608,0x00);
            Bsp_LCD_WriteReg(0xD609,0x5C);
            Bsp_LCD_WriteReg(0xD60A,0x00);
            Bsp_LCD_WriteReg(0xD60B,0x81);

            Bsp_LCD_WriteReg(0xD60C,0x00);
            Bsp_LCD_WriteReg(0xD60D,0xA6);
            Bsp_LCD_WriteReg(0xD60E,0x00);
            Bsp_LCD_WriteReg(0xD60F,0xE5);
            Bsp_LCD_WriteReg(0xD610,0x01);
            Bsp_LCD_WriteReg(0xD611,0x13);
            Bsp_LCD_WriteReg(0xD612,0x01);
            Bsp_LCD_WriteReg(0xD613,0x54);
            Bsp_LCD_WriteReg(0xD614,0x01);
            Bsp_LCD_WriteReg(0xD615,0x82);
            Bsp_LCD_WriteReg(0xD616,0x01);
            Bsp_LCD_WriteReg(0xD617,0xCA);
            Bsp_LCD_WriteReg(0xD618,0x02);
            Bsp_LCD_WriteReg(0xD619,0x00);
            Bsp_LCD_WriteReg(0xD61A,0x02);
            Bsp_LCD_WriteReg(0xD61B,0x01);
            Bsp_LCD_WriteReg(0xD61C,0x02);
            Bsp_LCD_WriteReg(0xD61D,0x34);
            Bsp_LCD_WriteReg(0xD61E,0x02);
            Bsp_LCD_WriteReg(0xD61F,0x67);
            Bsp_LCD_WriteReg(0xD620,0x02);
            Bsp_LCD_WriteReg(0xD621,0x84);
            Bsp_LCD_WriteReg(0xD622,0x02);
            Bsp_LCD_WriteReg(0xD623,0xA4);
            Bsp_LCD_WriteReg(0xD624,0x02);
            Bsp_LCD_WriteReg(0xD625,0xB7);
            Bsp_LCD_WriteReg(0xD626,0x02);
            Bsp_LCD_WriteReg(0xD627,0xCF);
            Bsp_LCD_WriteReg(0xD628,0x02);
            Bsp_LCD_WriteReg(0xD629,0xDE);
            Bsp_LCD_WriteReg(0xD62A,0x02);
            Bsp_LCD_WriteReg(0xD62B,0xF2);
            Bsp_LCD_WriteReg(0xD62C,0x02);
            Bsp_LCD_WriteReg(0xD62D,0xFE);
            Bsp_LCD_WriteReg(0xD62E,0x03);
            Bsp_LCD_WriteReg(0xD62F,0x10);
            Bsp_LCD_WriteReg(0xD630,0x03);
            Bsp_LCD_WriteReg(0xD631,0x33);
            Bsp_LCD_WriteReg(0xD632,0x03);
            Bsp_LCD_WriteReg(0xD633,0x6D);
            //LV2 Page 0 enable
            Bsp_LCD_WriteReg(0xF000,0x55);
            Bsp_LCD_WriteReg(0xF001,0xAA);
            Bsp_LCD_WriteReg(0xF002,0x52);
            Bsp_LCD_WriteReg(0xF003,0x08);
            Bsp_LCD_WriteReg(0xF004,0x00);
            //Display control
            Bsp_LCD_WriteReg(0xB100, 0xCC);
            Bsp_LCD_WriteReg(0xB101, 0x00);
            //Source hold time
            Bsp_LCD_WriteReg(0xB600,0x05);
            //Gate EQ control
            Bsp_LCD_WriteReg(0xB700,0x70);
            Bsp_LCD_WriteReg(0xB701,0x70);
            //Source EQ control (Mode 2)
            Bsp_LCD_WriteReg(0xB800,0x01);
            Bsp_LCD_WriteReg(0xB801,0x03);
            Bsp_LCD_WriteReg(0xB802,0x03);
            Bsp_LCD_WriteReg(0xB803,0x03);
            //Inversion mode (2-dot)
            Bsp_LCD_WriteReg(0xBC00,0x02);
            Bsp_LCD_WriteReg(0xBC01,0x00);
            Bsp_LCD_WriteReg(0xBC02,0x00);
            //Timing control 4H w/ 4-delay
            Bsp_LCD_WriteReg(0xC900,0xD0);
            Bsp_LCD_WriteReg(0xC901,0x02);
            Bsp_LCD_WriteReg(0xC902,0x50);
            Bsp_LCD_WriteReg(0xC903,0x50);
            Bsp_LCD_WriteReg(0xC904,0x50);
            Bsp_LCD_WriteReg(0x3500,0x00);
            Bsp_LCD_WriteReg(0x3A00,0x55);  //16-bit/pixel
            Bsp_Lcd_WR_REG(0x1100);
            Bsp_Delay_us(120);
            Bsp_Lcd_WR_REG(0x2900);  
        }else if(lcddev.id==0X1963)
        {
            Bsp_Lcd_WR_REG(0xE2);		//Set PLL with OSC = 10MHz (hardware),	Multiplier N = 35, 250MHz < VCO < 800MHz = OSC*(N+1), VCO = 300MHz
            Bsp_Lcd_WR_DATA(0x1D);		//����1 
            Bsp_Lcd_WR_DATA(0x02);		//����2 Divider M = 2, PLL = 300/(M+1) = 100MHz
            Bsp_Lcd_WR_DATA(0x04);		//����3 Validate M and N values   
            Bsp_Delay_us(100);
            Bsp_Lcd_WR_REG(0xE0);		// Start PLL command
            Bsp_Lcd_WR_DATA(0x01);		// enable PLL
            Bsp_Delay_ms(10);
            Bsp_Lcd_WR_REG(0xE0);		// Start PLL command again
            Bsp_Lcd_WR_DATA(0x03);		// now, use PLL output as system clock	
            Bsp_Delay_ms(12);  
            Bsp_Lcd_WR_REG(0x01);		//����λ
            Bsp_Delay_ms(10);
            
            Bsp_Lcd_WR_REG(0xE6);		//��������Ƶ��,33Mhz
            Bsp_Lcd_WR_DATA(0x2F);
            Bsp_Lcd_WR_DATA(0xFF);
            Bsp_Lcd_WR_DATA(0xFF);
		
            Bsp_Lcd_WR_REG(0xB0);		//����LCDģʽ
            Bsp_Lcd_WR_DATA(0x20);		//24λģʽ
            Bsp_Lcd_WR_DATA(0x00);		//TFT ģʽ 
        
            Bsp_Lcd_WR_DATA((SSD_HOR_RESOLUTION-1)>>8);//����LCDˮƽ����
            Bsp_Lcd_WR_DATA(SSD_HOR_RESOLUTION-1);		 
            Bsp_Lcd_WR_DATA((SSD_VER_RESOLUTION-1)>>8);//����LCD��ֱ����
            Bsp_Lcd_WR_DATA(SSD_VER_RESOLUTION-1);		 
            Bsp_Lcd_WR_DATA(0x00);		//RGB���� 
            
            Bsp_Lcd_WR_REG(0xB4);		//Set horizontal period
            Bsp_Lcd_WR_DATA((SSD_HT-1)>>8);
            Bsp_Lcd_WR_DATA(SSD_HT-1);
            Bsp_Lcd_WR_DATA(SSD_HPS>>8);
            Bsp_Lcd_WR_DATA(SSD_HPS);
            Bsp_Lcd_WR_DATA(SSD_HOR_PULSE_WIDTH-1);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_REG(0xB6);		//Set vertical period
            Bsp_Lcd_WR_DATA((SSD_VT-1)>>8);
            Bsp_Lcd_WR_DATA(SSD_VT-1);
            Bsp_Lcd_WR_DATA(SSD_VPS>>8);
            Bsp_Lcd_WR_DATA(SSD_VPS);
            Bsp_Lcd_WR_DATA(SSD_VER_FRONT_PORCH-1);
            Bsp_Lcd_WR_DATA(0x00);
            Bsp_Lcd_WR_DATA(0x00);
            
            Bsp_Lcd_WR_REG(0xF0);	//����SSD1963��CPU�ӿ�Ϊ16bit  
            Bsp_Lcd_WR_DATA(0x03);	//16-bit(565 format) data for 16bpp 

            Bsp_Lcd_WR_REG(0x29);	//������ʾ
            //����PWM���  ����ͨ��ռ�ձȿɵ� 
            Bsp_Lcd_WR_REG(0xD0);	//�����Զ���ƽ��DBC
            Bsp_Lcd_WR_DATA(0x00);	//disable
        
            Bsp_Lcd_WR_REG(0xBE);	//����PWM���
            Bsp_Lcd_WR_DATA(0x05);	//1����PWMƵ��
            Bsp_Lcd_WR_DATA(0xFE);	//2����PWMռ�ձ�
            Bsp_Lcd_WR_DATA(0x01);	//3����C
            Bsp_Lcd_WR_DATA(0x00);	//4����D
            Bsp_Lcd_WR_DATA(0x00);	//5����E 
            Bsp_Lcd_WR_DATA(0x00);	//6����F 
            
            Bsp_Lcd_WR_REG(0xB8);	//����GPIO����
            Bsp_Lcd_WR_DATA(0x03);	//2��IO�����ó����
            Bsp_Lcd_WR_DATA(0x01);	//GPIOʹ��������IO���� 
            Bsp_Lcd_WR_REG(0xBA);
            Bsp_Lcd_WR_DATA(0X01);	//GPIO[1:0]=01,����LCD����
            
            Bsp_LCD_SSD_BackLightSet(100);//��������Ϊ����
        }	    
    }	
	//��ʼ������Ժ�,����
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//������⼸��IC,������WRʱ��Ϊ���
	{
		 //��������дʱ����ƼĴ�����ʱ��   	 							    
     FSMC_WriteTim.AddressSetupTime=5;      
     FSMC_WriteTim.DataSetupTime=5;         
     FMC_NORSRAM_Extended_Timing_Init(SRAM_Handler.Extended,&FSMC_WriteTim,SRAM_Handler.Init.NSBank,SRAM_Handler.Init.ExtendedMode);
	} 
	BSP_Printf("LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID  	
	Bsp_LCD_Display_Dir(0);		//Ĭ��Ϊ����
	LCD_LED(1);				//��������
	Bsp_LCD_Clear(WHITE);
}  	
	
/**
  * @brief Initialize the RGB_LCD according to the specified
  */
void Bsp_RGB_LCD_Init(void)
{
	uint16_t lcdid=0;
	lcdid=Bsp_LTDC_PanelID_Read();			//��ȡLCD���ID	
	if(lcdid==0X4342)
	{
		lcdltdc.pwidth=480;			    //������,��λ:����
		lcdltdc.pheight=272;		    //���߶�,��λ:����
		lcdltdc.hsw=1;				    //ˮƽͬ������
		lcdltdc.vsw=1;				    //��ֱͬ������
		lcdltdc.hbp=40;				    //ˮƽ����
		lcdltdc.vbp=8;				    //��ֱ����
		lcdltdc.hfp=5;				    //ˮƽǰ��
		lcdltdc.vfp=8;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,88);			//��������ʱ�� 9Mhz 
		//������������.
	}
	else if(lcdid==0X7084)
	{
		lcdltdc.pwidth=800;			    //������,��λ:����
		lcdltdc.pheight=480;		    //���߶�,��λ:����
		lcdltdc.hsw=1;				    //ˮƽͬ������
		lcdltdc.vsw=1;				    //��ֱͬ������
		lcdltdc.hbp=46;				    //ˮƽ����
		lcdltdc.vbp=23;				    //��ֱ����
		lcdltdc.hfp=210;			    //ˮƽǰ��
		lcdltdc.vfp=22;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,24);			//��������ʱ�� 33M(�����˫��,��Ҫ����DCLK��:18.75Mhz  pll3r=43,�Ż�ȽϺ�)
	}
	else if(lcdid==0X7016)		
	{
		lcdltdc.pwidth=1024;			//������,��λ:����
		lcdltdc.pheight=600;			//���߶�,��λ:����
        lcdltdc.hsw=20;				    //ˮƽͬ������
		lcdltdc.vsw=3;				    //��ֱͬ������
		lcdltdc.hbp=140;			    //ˮƽ����
		lcdltdc.vbp=20;				    //��ֱ����
		lcdltdc.hfp=160;			    //ˮƽǰ��
		lcdltdc.vfp=12;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,18);			//��������ʱ��  45Mhz 
		//������������.
	}
	else if(lcdid==0X7018)		
	{
		lcdltdc.pwidth=1280;			//������,��λ:����
		lcdltdc.pheight=800;			//���߶�,��λ:����
		//������������.
	}
	else if(lcdid==0X8016)		
	{
		lcdltdc.pwidth=1024;			//������,��λ:����
		lcdltdc.pheight=600;			//���߶�,��λ:����
		//������������.
	}
	else if(lcdid==0X1018)				//10.1��1280*800 RGB��	
	{
		lcdltdc.pwidth=1280;			//������,��λ:����
		lcdltdc.pheight=800;			//���߶�,��λ:����
		lcdltdc.hbp=140;				//ˮƽ����
		lcdltdc.hfp=10;			    	//ˮƽǰ��
		lcdltdc.hsw=10;					//ˮƽͬ������
		lcdltdc.vbp=10;					//��ֱ����
		lcdltdc.vfp=10;					//��ֱǰ��
		lcdltdc.vsw=3;					//��ֱͬ������
		Bsp_LTDC_Clk_Set(5,160,16);			//��������ʱ�� 50MHz
	}	
	else if(lcdid==0XA001)              //��VGA��ʾ��
	{ 
		lcdltdc.pwidth=1366;		    //���� ��,��λ:����
		lcdltdc.pheight=768;		    //���߶�,��λ:����
		lcdltdc.hsw=143;			    //ˮƽͬ������ 
		lcdltdc.hfp=70;			        //ˮƽǰ��
		lcdltdc.hbp=213;			    //ˮƽ����
		lcdltdc.vsw=3;				    //��ֱͬ������ 
		lcdltdc.vbp=24;				    //��ֱ����  
		lcdltdc.vfp=3;				    //��ֱǰ��
        Bsp_LTDC_Clk_Set(5,160,28);//��������ʱ�� 28.5Mhz
	 
	}
	else if(lcdid==0XA002)            //��VGA��ʾ��
    {  
        lcdltdc.pwidth=1280;		    //������,��λ:����
		lcdltdc.pheight=800;		    //���߶�,��λ:����
		lcdltdc.hsw=32;			        //ˮƽͬ������ 
		lcdltdc.hfp=48;			        //ˮƽǰ��
		lcdltdc.hbp=80;			        //ˮƽ����
		lcdltdc.vsw=6;				    //��ֱͬ������
		lcdltdc.vbp=14;				    //��ֱ���� 
		lcdltdc.vfp=3;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,22);//��������ʱ�� 35.5Mhz
	}
	else if(lcdid==0XA003)	        //��VGA��ʾ��
	{
		lcdltdc.pwidth=1280;		    //������,��λ:����
		lcdltdc.pheight=768;		    //���߶�,��λ:����
		lcdltdc.hsw=32;				    //ˮƽͬ������ 
		lcdltdc.hbp=80;			        //ˮƽ����
		lcdltdc.hfp=48;			        //ˮƽǰ��
		lcdltdc.vsw=7;				    //��ֱͬ������
		lcdltdc.vbp=12;				    //��ֱ����
		lcdltdc.vfp=3;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,23);//��������ʱ��  34.25Mhz
	}
	else if(lcdid==0XA004)            //��VGA��ʾ��
	{
		lcdltdc.pwidth=1024;		    //������,��λ:����
		lcdltdc.pheight=768;		    //���߶�,��λ:����
		lcdltdc.hsw=136;			    //ˮƽͬ������ 
		lcdltdc.hfp=24;			        //ˮƽǰ��
		lcdltdc.hbp=160;			    //ˮƽ����
		lcdltdc.vsw=6;				    //��ֱͬ������
		lcdltdc.vbp=29;				    //��ֱ���� 
		lcdltdc.vfp=3;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,18);//��������ʱ�� 43.25Mhz	
	}
	else if(lcdid==0XA005)            //��VGA��ʾ��
	{
		lcdltdc.pwidth=848;		        //������,��λ:���� 
		lcdltdc.pheight=480;		    //���߶�,��λ:����
		lcdltdc.hsw=112;		        //ˮƽͬ������
		lcdltdc.hbp=112;			    //ˮƽ����
		lcdltdc.hfp=16;			        //ˮƽǰ��
		lcdltdc.vsw=8;				    //��ֱͬ������
		lcdltdc.vbp=23;				    //��ֱ����
		lcdltdc.vfp=6;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,47);//��������ʱ�� 17Mhz 
	}
	else if(lcdid==0XA006)            //��VGA��ʾ��
	{
		lcdltdc.pwidth=800;		        //������,��λ:����
		lcdltdc.pheight=600;		    //���߶�,��λ:����
		lcdltdc.hsw=128;			    //ˮƽͬ������
		lcdltdc.hbp=88;			        //ˮƽ����
		lcdltdc.hfp=40;			        //ˮƽǰ��
		lcdltdc.vsw=4;				    //��ֱͬ������
		lcdltdc.vbp=23;				    //��ֱ����
		lcdltdc.vfp=1;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,40);//��������ʱ�� 20mz		
	}
	else if(lcdid==0XA007)            //��VGA��ʾ��
	{
        lcdltdc.pwidth=640;		        //������,��λ:����
		lcdltdc.pheight=480;		    //���߶�,��λ:����
		lcdltdc.hsw=96;				    //ˮƽͬ������
		lcdltdc.hfp=8;			        //ˮƽǰ��
		lcdltdc.hbp=56;			        //ˮƽ����
		lcdltdc.vsw=2;				    //��ֱͬ������
		lcdltdc.vbp=41;				    //��ֱ����
		lcdltdc.vfp=2;				    //��ֱǰ��
		Bsp_LTDC_Clk_Set(5,160,62);//��������ʱ�� 12.75Mhz
	} 

	lcddev.width=lcdltdc.pwidth;
	lcddev.height=lcdltdc.pheight;
    
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888 
	ltdc_framebuf[0]=(uint32_t*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize=4;				//ÿ������ռ4���ֽ�
#else 
    lcdltdc.pixsize=2;				//ÿ������ռ2���ֽ�
	ltdc_framebuf[0]=(uint32_t*)&ltdc_lcd_framebuf;
#endif 	
    
    //LTDC����
    LTDC_Handler.Instance=LTDC;
    LTDC_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         //ˮƽͬ������
    LTDC_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         //��ֱͬ������
    LTDC_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         //����ʹ�ܼ���
    LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IPC;        //����ʱ�Ӽ���
	 
	if(lcdid==0X1018)LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IIPC;        //����ʱ�Ӽ����෴
	 
    LTDC_Handler.Init.HorizontalSync=lcdltdc.hsw-1;          //ˮƽͬ������
    LTDC_Handler.Init.VerticalSync=lcdltdc.vsw-1;            //��ֱͬ������
    LTDC_Handler.Init.AccumulatedHBP=lcdltdc.hsw+lcdltdc.hbp-1; //ˮƽͬ�����ؿ���
    LTDC_Handler.Init.AccumulatedVBP=lcdltdc.vsw+lcdltdc.vbp-1; //��ֱͬ�����ظ߶�
    LTDC_Handler.Init.AccumulatedActiveW=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth-1;//��Ч����
    LTDC_Handler.Init.AccumulatedActiveH=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight-1;//��Ч�߶�
    LTDC_Handler.Init.TotalWidth=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth+lcdltdc.hfp-1;   //�ܿ���
    LTDC_Handler.Init.TotalHeigh=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight+lcdltdc.vfp-1;  //�ܸ߶�
    LTDC_Handler.Init.Backcolor.Red=0;           //��Ļ�������ɫ����
    LTDC_Handler.Init.Backcolor.Green=0;         //��Ļ��������ɫ����
    LTDC_Handler.Init.Backcolor.Blue=0;          //��Ļ����ɫ��ɫ����
    HAL_LTDC_Init(&LTDC_Handler);
 	
	//������
	Bsp_LTDC_Layer_Parameter_Config(0,(uint32_t)ltdc_framebuf[0],LCD_PIXFORMAT,255,0,6,7,0X000000);//���������
	Bsp_LTDC_Layer_Window_Config(0,0,0,lcdltdc.pwidth,lcdltdc.pheight);	//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!	
	 	
 	Bsp_LTDC_Display_Dir(1);			//Ĭ������
	Bsp_LTDC_Select_Layer(0); 			//ѡ���1��
	LCD_LED(1);         		    //��������
	Bsp_LTDC_Clear(0XFFFFFFFF);			//����
}
//LTDC�ײ�IO��ʼ����ʱ��ʹ��
//�˺����ᱻHAL_LTDC_Init()����
//hltdc:LTDC���
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_LTDC_CLK_ENABLE();                //ʹ��LTDCʱ��
    __HAL_RCC_DMA2D_CLK_ENABLE();               //ʹ��DMA2Dʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();               //ʹ��GPIOBʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();               //ʹ��GPIOFʱ��
    __HAL_RCC_GPIOG_CLK_ENABLE();               //ʹ��GPIOGʱ��
    __HAL_RCC_GPIOH_CLK_ENABLE();               //ʹ��GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();               //ʹ��GPIOIʱ��
    
    //��ʼ��PB5����������
    GPIO_Initure.Pin=GPIO_PIN_5;                //PB5������������Ʊ���
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //�������
    GPIO_Initure.Pull=GPIO_PULLUP;              //����        
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //��ʼ��PF10
    GPIO_Initure.Pin=GPIO_PIN_10; 
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //����
    GPIO_Initure.Pull=GPIO_NOPULL;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; //����
    GPIO_Initure.Alternate=GPIO_AF14_LTDC;      //����ΪLTDC
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //��ʼ��PG6,7,11
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    //��ʼ��PH9,10,11,12,13,14,15
    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    //��ʼ��PI0,1,2,4,5,6,7,9,10
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

//��LCD����
//lcd_switch:1 ��,0���ر�
void Bsp_LTDC_Switch(uint8_t sw)
{
	if(sw==1) __HAL_LTDC_ENABLE(&LTDC_Handler);
	else if(sw==0)__HAL_LTDC_DISABLE(&LTDC_Handler);
}

//����ָ����
//layerx:���,0,��һ��; 1,�ڶ���
//sw:1 ��;0�ر�
void Bsp_LTDC_Layer_Switch(uint8_t layerx,uint8_t sw)
{
	if(sw==1) __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,layerx);
	else if(sw==0) __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,layerx);
	__HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
}

//ѡ���
//layerx:���;0,��һ��;1,�ڶ���;
void Bsp_LTDC_Select_Layer(uint8_t layerx)
{
	lcdltdc.activelayer=layerx;
}

//����LCD��ʾ����
//dir:0,������1,����
void Bsp_LTDC_Display_Dir(uint8_t dir)
{
    lcdltdc.dir=dir; 	//��ʾ����
	if(dir==0)			//����
	{
		lcdltdc.width=lcdltdc.pheight;
		lcdltdc.height=lcdltdc.pwidth;	
	}
	else if(dir==1)	//����
	{
		lcdltdc.width=lcdltdc.pwidth;
		lcdltdc.height=lcdltdc.pheight;
	}
}

//���㺯��
//x,y:����
//color:��ɫֵ
void Bsp_LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color)
{ 
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//����
	{
        *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
	}
	else 			//����
	{
        *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color; 
	}
#else
	if(lcdltdc.dir)	//����
	{
        *(uint16_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
	}
	else 			//����
	{
        *(uint16_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color; 
	}
#endif
}

//���㺯��
//����ֵ:��ɫֵ
uint32_t Bsp_LTDC_Read_Point(uint16_t x,uint16_t y)
{ 
#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//����
	{
		return *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}
	else 			//����
	{
		return *(uint32_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)); 
	}
#else
	if(lcdltdc.dir)	//����
	{
		return *(uint16_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}
	else 			//����
	{
		return *(uint16_t*)((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)); 
	}
#endif 
}

//LTDC������,DMA2D���
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ
void Bsp_LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
{ 
	uint32_t psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	uint32_t timeout=0; 
	uint16_t offline;
	uint32_t addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
	}
	else			//����
	{
		psx=sy;psy=lcdltdc.pheight-ex-1;
		pex=ey;pey=lcdltdc.pheight-sx-1;
	} 
	offline=lcdltdc.pwidth-(pex-psx+1);
	addr=((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	RCC->AHB1ENR|=1<<23;			//ʹ��DM2Dʱ��
	DMA2D->CR=3<<16;				//�Ĵ������洢��ģʽ
	DMA2D->OPFCCR=LCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->OOR=offline;				//������ƫ�� 
	DMA2D->CR&=~(1<<0);				//��ֹͣDMA2D
	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ���
	DMA2D->OCOLR=color;				//�趨�����ɫ�Ĵ��� 
	DMA2D->CR|=1<<0;				//����DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	}  
	DMA2D->IFCR|=1<<1;				//���������ɱ�־ 	
}
//��ָ�����������ָ����ɫ��,DMA2D���	
//�˺�����֧��uint16_t,RGB565��ʽ����ɫ�������.
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)  
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ�����׵�ַ
void Bsp_LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{
	uint32_t psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	uint32_t timeout=0; 
	uint16_t offline;
	uint32_t addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
	}
	else			//����
	{
		psx=sy;psy=lcdltdc.pheight-ex-1;
		pex=ey;pey=lcdltdc.pheight-sx-1;
	}
	offline=lcdltdc.pwidth-(pex-psx+1);
	addr=((uint32_t)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	RCC->AHB1ENR|=1<<23;			//ʹ��DM2Dʱ��
	DMA2D->CR=0<<16;				//�洢�����洢��ģʽ
	DMA2D->FGPFCCR=LCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->FGOR=0;					//ǰ������ƫ��Ϊ0
	DMA2D->OOR=offline;				//������ƫ�� 
	DMA2D->CR&=~(1<<0);				//��ֹͣDMA2D
	DMA2D->FGMAR=(uint32_t)color;		//Դ��ַ
	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ��� 
	DMA2D->CR|=1<<0;				//����DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	} 
	DMA2D->IFCR|=1<<1;				//���������ɱ�־  	
} 

//LCD����
//color:��ɫֵ
void Bsp_LTDC_Clear(uint32_t color)
{
	Bsp_LTDC_Fill(0,0,lcdltdc.width-1,lcdltdc.height-1,color);
}

//LTDCʱ��(Fdclk)���ú���
//PLL3_VCO Input=HSE_VALUE/PLL3M
//PLL3_VCO Output=PLL3_VCO Input * PLL3N
//PLLLCDCLK = PLL3_VCO Output/PLL3R
//����HSE_VALUE=25MHz��PLL3M=5��PLL3N=160,PLL3R=88
//LTDCLK=PLLLCDCLK=25/5*160/88=9MHz

//����ֵ:0,�ɹ�;1,ʧ�ܡ�
uint8_t Bsp_LTDC_Clk_Set(uint32_t pll3m,uint32_t pll3n,uint32_t pll3r)
{
	RCC_PeriphCLKInitTypeDef PeriphClkIniture;
    PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkIniture.PLL3.PLL3M = pll3m;    
    PeriphClkIniture.PLL3.PLL3N = pll3n;
    PeriphClkIniture.PLL3.PLL3P = 2;
    PeriphClkIniture.PLL3.PLL3Q = 2;  
    PeriphClkIniture.PLL3.PLL3R = pll3r;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK) //��������ʱ�ӣ���������Ϊʱ��Ϊ18.75MHZ
    {
        return 0;   //�ɹ�
    }
    else return 1;  //ʧ��    
}

//LTDC,���մ�������,������LCD�������ϵΪ��׼
//ע��:�˺���������Bsp_LTDC_Layer_Parameter_Config֮��������.
//layerx:��ֵ,0/1.
//sx,sy:��ʼ����
//width,height:���Ⱥ͸߶�
void Bsp_LTDC_Layer_Window_Config(uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
    HAL_LTDC_SetWindowPosition(&LTDC_Handler,sx,sy,layerx);  //���ô��ڵ�λ��
    HAL_LTDC_SetWindowSize(&LTDC_Handler,width,height,layerx);//���ô��ڴ�С    
}

//LTDC,������������.
//ע��:�˺���,������Bsp_LTDC_Layer_Window_Config֮ǰ����.
//layerx:��ֵ,0/1.
//bufaddr:����ɫ֡������ʼ��ַ
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//alpha0:Ĭ����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//bfac1:���ϵ��1,4(100),�㶨��Alpha;6(101),����Alpha*�㶨Alpha
//bfac2:���ϵ��2,5(101),�㶨��Alpha;7(111),����Alpha*�㶨Alpha
//bkcolor:��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
//����ֵ:��
void Bsp_LTDC_Layer_Parameter_Config(uint8_t layerx,uint32_t bufaddr,uint8_t pixformat,uint8_t alpha,uint8_t alpha0,uint8_t bfac1,uint8_t bfac2,uint32_t bkcolor)
{
	LTDC_LayerCfgTypeDef pLayerCfg;
	
	pLayerCfg.WindowX0=0;                       //������ʼX����
	pLayerCfg.WindowY0=0;                       //������ʼY����
	pLayerCfg.WindowX1=lcdltdc.pwidth;          //������ֹX����
	pLayerCfg.WindowY1=lcdltdc.pheight;         //������ֹY����
	pLayerCfg.PixelFormat=pixformat;		    //���ظ�ʽ
	pLayerCfg.Alpha=alpha;				        //Alphaֵ���ã�0~255,255Ϊ��ȫ��͸��
	pLayerCfg.Alpha0=alpha0;			        //Ĭ��Alphaֵ
	pLayerCfg.BlendingFactor1=(uint32_t)bfac1<<8;    //���ò���ϵ��
	pLayerCfg.BlendingFactor2=(uint32_t)bfac2<<8;	//���ò���ϵ��
	pLayerCfg.FBStartAdress=bufaddr;	        //���ò���ɫ֡������ʼ��ַ
	pLayerCfg.ImageWidth=lcdltdc.pwidth;        //������ɫ֡�������Ŀ���    
	pLayerCfg.ImageHeight=lcdltdc.pheight;      //������ɫ֡�������ĸ߶�
	pLayerCfg.Backcolor.Red=(uint8_t)(bkcolor&0X00FF0000)>>16;   //������ɫ��ɫ����
	pLayerCfg.Backcolor.Green=(uint8_t)(bkcolor&0X0000FF00)>>8;  //������ɫ��ɫ����
	pLayerCfg.Backcolor.Blue=(uint8_t)bkcolor&0X000000FF;        //������ɫ��ɫ����
	HAL_LTDC_ConfigLayer(&LTDC_Handler,&pLayerCfg,layerx);   //������ѡ�еĲ�
}  

//��ȡ������
//PG6=R7(M0);PI2=G7(M1);PI7=B7(M2);
//M2:M1:M0
//0 :0 :0	//4.3��480*272 RGB��,ID=0X4342
//0 :0 :1	//7��800*480 RGB��,ID=0X7084
//0 :1 :0	//7��1024*600 RGB��,ID=0X7016
//0 :1 :1	//7��1280*800 RGB��,ID=0X7018
//1 :0 :0	//8��1024*768 RGB��,ID=0X8017 
//����ֵ:LCD ID:0,�Ƿ�;����ֵ,ID;
uint16_t Bsp_LTDC_PanelID_Read(void)
{
	uint8_t idx=0;
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOG_CLK_ENABLE();       //ʹ��GPIOGʱ��
	__HAL_RCC_GPIOI_CLK_ENABLE();       //ʹ��GPIOIʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_6;        //PG6
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //����
    GPIO_Initure.Pull=GPIO_PULLUP;      //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; //����
    HAL_GPIO_Init(GPIOG,&GPIO_Initure); //��ʼ��
    
    GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_7; //PI2,7
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //��ʼ��
    
    idx=(uint8_t)HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_6); //��ȡM0
    idx|=(uint8_t)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_2)<<1;//��ȡM1
    idx|=(uint8_t)HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_7)<<2;//��ȡM2
	if(idx==0)return 0X4342;		//4.3����,480*272�ֱ���
	else if(idx==1)return 0X7084;	//7����,800*480�ֱ���
	else if(idx==2)return 0X7016;	//7����,1024*600�ֱ���
	else if(idx==3)return 0X7018;	//7����,1280*800�ֱ���
	else if(idx==4)return 0X8016;	//8����,1024*600�ֱ���
	else if(idx==5)return 0X1018;	//10.1����,1280*800�ֱ���		
	else return 0;
}

//д�Ĵ�������
//regval:�Ĵ���ֵ
void Bsp_Lcd_WR_REG(__IO uint16_t regval)
{   
	regval=regval;		//ʹ��-O2�Ż���ʱ��,����������ʱ
	LCD->LCD_REG=regval;//д��Ҫд�ļĴ������	 
}
//дLCD����
//data:Ҫд���ֵ
void Bsp_Lcd_WR_DATA(__IO uint16_t data)
{	  
	data=data;			//ʹ��-O2�Ż���ʱ��,����������ʱ
	LCD->LCD_RAM=data;		 
}
//��LCD����
//����ֵ:������ֵ
uint16_t Bsp_Lcd_RD_DATA(void)
{
	__IO uint16_t ram;			//��ֹ���Ż�
	ram=LCD->LCD_RAM;	
	return ram;	 
}					   
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void Bsp_LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	LCD->LCD_RAM = LCD_RegValue;//д������	    		 
}	   
//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
uint16_t Bsp_LCD_ReadReg(uint16_t LCD_Reg)
{										   
	Bsp_Lcd_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	Bsp_Delay_us(5);		  
	return Bsp_Lcd_RD_DATA();		//���ض�����ֵ
}   
//��ʼдGRAM
void Bsp_LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCDдGRAM
//RGB_Code:��ɫֵ
void Bsp_LCD_WriteRAM(uint16_t RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
uint16_t LCD_BGR2RGB(uint16_t c)
{
	uint16_t  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(uint8_t i)
{
	while(i--);
}
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
uint32_t Bsp_LCD_ReadPoint(uint16_t x,uint16_t y)
{
 	uint16_t r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���
	if(lcdltdc.pwidth!=0)							//�����RGB��
	{
		return Bsp_LTDC_Read_Point(x,y);
	}
	Bsp_LCD_SetCursor(x,y);	    
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X1963)Bsp_Lcd_WR_REG(0X2E);//9341/3510/1963 ���Ͷ�GRAMָ��
	else if(lcddev.id==0X5510)Bsp_Lcd_WR_REG(0X2E00);	//5510 ���Ͷ�GRAMָ��
 	r=Bsp_Lcd_RD_DATA();								//dummy Read	   
	if(lcddev.id==0X1963)return r;					//1963ֱ�Ӷ��Ϳ��� 
	opt_delay(2);	  
 	r=Bsp_Lcd_RD_DATA();  		  						//ʵ��������ɫ
	//9341/NT35310/NT35510Ҫ��2�ζ��� 
	opt_delay(2);	  
	b=Bsp_Lcd_RD_DATA(); 
	g=r&0XFF;		//����9341/5310/5510,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
	g<<=8; 
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
}			 
//LCD������ʾ
void Bsp_LCD_DisplayOn(void)
{					   
	if(lcdltdc.pwidth!=0)Bsp_LTDC_Switch(1);//����LCD
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X1963)Bsp_Lcd_WR_REG(0X29);	//������ʾ
	else if(lcddev.id==0X5510)Bsp_Lcd_WR_REG(0X2900);	//������ʾ
}	 
//LCD�ر���ʾ
void Bsp_LCD_DisplayOff(void)
{	   
	if(lcdltdc.pwidth!=0)Bsp_LTDC_Switch(0);//�ر�LCD
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X1963)Bsp_Lcd_WR_REG(0X28);	//�ر���ʾ
	else if(lcddev.id==0X5510)Bsp_Lcd_WR_REG(0X2800);	//�ر���ʾ
}   
//���ù��λ��(��RGB����Ч)
//Xpos:������
//Ypos:������
void Bsp_LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 
 	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{		    
		Bsp_Lcd_WR_REG(lcddev.setxcmd); 
		Bsp_Lcd_WR_DATA(Xpos>>8);Bsp_Lcd_WR_DATA(Xpos&0XFF); 			 
		Bsp_Lcd_WR_REG(lcddev.setycmd); 
		Bsp_Lcd_WR_DATA(Ypos>>8);Bsp_Lcd_WR_DATA(Ypos&0XFF); 		
	}
	else if(lcddev.id==0X1963)
	{  			 		
		if(lcddev.dir==0)//x������Ҫ�任
		{
			Xpos=lcddev.width-1-Xpos;
			Bsp_Lcd_WR_REG(lcddev.setxcmd); 
			Bsp_Lcd_WR_DATA(0);Bsp_Lcd_WR_DATA(0); 		
			Bsp_Lcd_WR_DATA(Xpos>>8);Bsp_Lcd_WR_DATA(Xpos&0XFF);		 	 
		}
		else
		{
			Bsp_Lcd_WR_REG(lcddev.setxcmd); 
			Bsp_Lcd_WR_DATA(Xpos>>8);Bsp_Lcd_WR_DATA(Xpos&0XFF); 		
			Bsp_Lcd_WR_DATA((lcddev.width-1)>>8);Bsp_Lcd_WR_DATA((lcddev.width-1)&0XFF);		 	 			
		}	
		Bsp_Lcd_WR_REG(lcddev.setycmd); 
		Bsp_Lcd_WR_DATA(Ypos>>8);Bsp_Lcd_WR_DATA(Ypos&0XFF); 		
		Bsp_Lcd_WR_DATA((lcddev.height-1)>>8);Bsp_Lcd_WR_DATA((lcddev.height-1)&0XFF); 			 		
		
	}
	else if(lcddev.id==0X5510)
	{
		Bsp_Lcd_WR_REG(lcddev.setxcmd);Bsp_Lcd_WR_DATA(Xpos>>8); 		
		Bsp_Lcd_WR_REG(lcddev.setxcmd+1);Bsp_Lcd_WR_DATA(Xpos&0XFF);			 
		Bsp_Lcd_WR_REG(lcddev.setycmd);Bsp_Lcd_WR_DATA(Ypos>>8);  		
		Bsp_Lcd_WR_REG(lcddev.setycmd+1);Bsp_Lcd_WR_DATA(Ypos&0XFF);			
	} 
} 		 
//����LCD���Զ�ɨ�跽��(��RGB����Ч)
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���	   	   
void Bsp_LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;  
	if((lcddev.dir==1&&lcddev.id!=0X1963)||(lcddev.dir==0&&lcddev.id==0X1963))//����ʱ����1963���ı�ɨ�跽������ʱ1963�ı䷽��
	{			   
		switch(dir)//����ת��
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
	if(lcddev.id==0x9341||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//9341/5310/5510/1963,���⴦��
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))regval|=0X08;//5310/5510/1963����ҪBGR   
 		Bsp_LCD_WriteReg(dirreg,regval);
		if(lcddev.id!=0X1963)//1963�������괦��
		{
			if(regval&0X20)
			{
				if(lcddev.width<lcddev.height)//����X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}
			else  
			{
				if(lcddev.width>lcddev.height)//����X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}  
		}
		if(lcddev.id==0X5510)
		{
			Bsp_Lcd_WR_REG(lcddev.setxcmd);Bsp_Lcd_WR_DATA(0); 
			Bsp_Lcd_WR_REG(lcddev.setxcmd+1);Bsp_Lcd_WR_DATA(0); 
			Bsp_Lcd_WR_REG(lcddev.setxcmd+2);Bsp_Lcd_WR_DATA((lcddev.width-1)>>8); 
			Bsp_Lcd_WR_REG(lcddev.setxcmd+3);Bsp_Lcd_WR_DATA((lcddev.width-1)&0XFF); 
			Bsp_Lcd_WR_REG(lcddev.setycmd);Bsp_Lcd_WR_DATA(0); 
			Bsp_Lcd_WR_REG(lcddev.setycmd+1);Bsp_Lcd_WR_DATA(0); 
			Bsp_Lcd_WR_REG(lcddev.setycmd+2);Bsp_Lcd_WR_DATA((lcddev.height-1)>>8); 
			Bsp_Lcd_WR_REG(lcddev.setycmd+3);Bsp_Lcd_WR_DATA((lcddev.height-1)&0XFF);
		}
		else
		{
			Bsp_Lcd_WR_REG(lcddev.setxcmd); 
			Bsp_Lcd_WR_DATA(0);Bsp_Lcd_WR_DATA(0);
			Bsp_Lcd_WR_DATA((lcddev.width-1)>>8);Bsp_Lcd_WR_DATA((lcddev.width-1)&0XFF);
			Bsp_Lcd_WR_REG(lcddev.setycmd); 
			Bsp_Lcd_WR_DATA(0);Bsp_Lcd_WR_DATA(0);
			Bsp_Lcd_WR_DATA((lcddev.height-1)>>8);Bsp_Lcd_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	} 
}     
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void Bsp_LCD_DrawPoint(uint16_t x,uint16_t y)
{
	if(lcdltdc.pwidth!=0)//�����RGB��
	{
		Bsp_LTDC_Draw_Point(x,y,POINT_COLOR);
	}
	else
	{
		Bsp_LCD_SetCursor(x,y);		//���ù��λ�� 
		Bsp_LCD_WriteRAM_Prepare();	//��ʼд��GRAM
		LCD->LCD_RAM=POINT_COLOR; 
	}
}
//���ٻ���
//x,y:����
//color:��ɫ
void Bsp_LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{	   
	if(lcdltdc.pwidth!=0)//�����RGB��
	{
		Bsp_LTDC_Draw_Point(x,y,color);
		return;
	}
	else if(lcddev.id==0X9341||lcddev.id==0X5310)
	{
		Bsp_Lcd_WR_REG(lcddev.setxcmd); 
		Bsp_Lcd_WR_DATA(x>>8);Bsp_Lcd_WR_DATA(x&0XFF);  			 
		Bsp_Lcd_WR_REG(lcddev.setycmd); 
		Bsp_Lcd_WR_DATA(y>>8);Bsp_Lcd_WR_DATA(y&0XFF); 		 	 
	}
	else if(lcddev.id==0X5510)
	{
		Bsp_Lcd_WR_REG(lcddev.setxcmd);Bsp_Lcd_WR_DATA(x>>8);  
		Bsp_Lcd_WR_REG(lcddev.setxcmd+1);Bsp_Lcd_WR_DATA(x&0XFF);	  
		Bsp_Lcd_WR_REG(lcddev.setycmd);Bsp_Lcd_WR_DATA(y>>8);  
		Bsp_Lcd_WR_REG(lcddev.setycmd+1);Bsp_Lcd_WR_DATA(y&0XFF); 
	}
	else if(lcddev.id==0X1963)
	{
		if(lcddev.dir==0)x=lcddev.width-1-x;
		Bsp_Lcd_WR_REG(lcddev.setxcmd); 
		Bsp_Lcd_WR_DATA(x>>8);Bsp_Lcd_WR_DATA(x&0XFF); 		
		Bsp_Lcd_WR_DATA(x>>8);Bsp_Lcd_WR_DATA(x&0XFF); 		
		Bsp_Lcd_WR_REG(lcddev.setycmd); 
		Bsp_Lcd_WR_DATA(y>>8);Bsp_Lcd_WR_DATA(y&0XFF); 		
		Bsp_Lcd_WR_DATA(y>>8);Bsp_Lcd_WR_DATA(y&0XFF); 		
	}		 
	LCD->LCD_REG=lcddev.wramcmd; 
	LCD->LCD_RAM=color; 
}	 
//SSD1963 ��������
//pwm:����ȼ�,0~100.Խ��Խ��.
void Bsp_LCD_SSD_BackLightSet(uint8_t pwm)
{	
	Bsp_Lcd_WR_REG(0xBE);	//����PWM���
	Bsp_Lcd_WR_DATA(0x05);	//1����PWMƵ��
	Bsp_Lcd_WR_DATA(pwm*2.55);//2����PWMռ�ձ�
	Bsp_Lcd_WR_DATA(0x01);	//3����C
	Bsp_Lcd_WR_DATA(0xFF);	//4����D
	Bsp_Lcd_WR_DATA(0x00);	//5����E
	Bsp_Lcd_WR_DATA(0x00);	//6����F
}

//����LCD��ʾ����
//dir:0,������1,����
void Bsp_LCD_Display_Dir(uint8_t dir)
{
    lcddev.dir=dir;         //����/����
	if(lcdltdc.pwidth!=0)   //�����RGB��
	{
		Bsp_LTDC_Display_Dir(dir);
		lcddev.width=lcdltdc.width;
		lcddev.height=lcdltdc.height;
		return;
	}
	if(dir==0)			//����
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
		}
		else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=480;
			lcddev.height=800;
		}
		else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//����д��GRAM��ָ�� 
			lcddev.setxcmd=0X2B;	//����дX����ָ��
			lcddev.setycmd=0X2A;	//����дY����ָ��
			lcddev.width=480;		//���ÿ���480
			lcddev.height=800;		//���ø߶�800  
		}
	}
	else 				//����
	{	  				
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}
		else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=800;
			lcddev.height=480;
		}
		else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//����д��GRAM��ָ�� 
			lcddev.setxcmd=0X2A;	//����дX����ָ��
			lcddev.setycmd=0X2B;	//����дY����ָ��
			lcddev.width=800;		//���ÿ���800
			lcddev.height=480;		//���ø߶�480  
		}
		if(lcddev.id==0X5310)
		{ 	 
			lcddev.width=480;
			lcddev.height=320; 			
		}
	} 
	Bsp_LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}	 
//���ô���(��RGB����Ч),���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ��Ⱥ͸߶�,�������0!!
//�����С:width*height. 
void Bsp_LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{     
	uint16_t twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcddev.id==0X9341||lcddev.id==0X5310||(lcddev.dir==1&&lcddev.id==0X1963))
	{
		Bsp_Lcd_WR_REG(lcddev.setxcmd); 
		Bsp_Lcd_WR_DATA(sx>>8); 
		Bsp_Lcd_WR_DATA(sx&0XFF);	 
		Bsp_Lcd_WR_DATA(twidth>>8); 
		Bsp_Lcd_WR_DATA(twidth&0XFF);  
		Bsp_Lcd_WR_REG(lcddev.setycmd); 
		Bsp_Lcd_WR_DATA(sy>>8); 
		Bsp_Lcd_WR_DATA(sy&0XFF); 
		Bsp_Lcd_WR_DATA(theight>>8); 
		Bsp_Lcd_WR_DATA(theight&0XFF); 
	}
	else if(lcddev.id==0X1963)//1963�������⴦��
	{
		sx=lcddev.width-width-sx; 
		height=sy+height-1; 
		Bsp_Lcd_WR_REG(lcddev.setxcmd); 
		Bsp_Lcd_WR_DATA(sx>>8); 
		Bsp_Lcd_WR_DATA(sx&0XFF);	 
		Bsp_Lcd_WR_DATA((sx+width-1)>>8); 
		Bsp_Lcd_WR_DATA((sx+width-1)&0XFF);  
		Bsp_Lcd_WR_REG(lcddev.setycmd); 
		Bsp_Lcd_WR_DATA(sy>>8); 
		Bsp_Lcd_WR_DATA(sy&0XFF); 
		Bsp_Lcd_WR_DATA(height>>8); 
		Bsp_Lcd_WR_DATA(height&0XFF); 		
	}
	else if(lcddev.id==0X5510)
	{
		Bsp_Lcd_WR_REG(lcddev.setxcmd);Bsp_Lcd_WR_DATA(sx>>8);  
		Bsp_Lcd_WR_REG(lcddev.setxcmd+1);Bsp_Lcd_WR_DATA(sx&0XFF);	  
		Bsp_Lcd_WR_REG(lcddev.setxcmd+2);Bsp_Lcd_WR_DATA(twidth>>8);   
		Bsp_Lcd_WR_REG(lcddev.setxcmd+3);Bsp_Lcd_WR_DATA(twidth&0XFF);   
		Bsp_Lcd_WR_REG(lcddev.setycmd);Bsp_Lcd_WR_DATA(sy>>8);   
		Bsp_Lcd_WR_REG(lcddev.setycmd+1);Bsp_Lcd_WR_DATA(sy&0XFF);  
		Bsp_Lcd_WR_REG(lcddev.setycmd+2);Bsp_Lcd_WR_DATA(theight>>8);   
		Bsp_Lcd_WR_REG(lcddev.setycmd+3);Bsp_Lcd_WR_DATA(theight&0XFF);  
	} 
}

//����MPU��region,�ⲿSRAM������Ϊ͸дģʽ
void Bsp_Lcd_Mpu_Config(void)
{	
	MPU_Region_InitTypeDef MPU_Initure;

	HAL_MPU_Disable();							//����MPU֮ǰ�ȹر�MPU,��������Ժ���ʹ��MPU	
	//�ⲿSRAMΪregion0����СΪ2MB��������ɶ�д
	MPU_Initure.Enable=MPU_REGION_ENABLE;	    //ʹ��region
	MPU_Initure.Number=LCD_REGION_NUMBER;		//����region���ⲿSRAMʹ�õ�region0
	MPU_Initure.BaseAddress=LCD_ADDRESS_START;	//region����ַ
	MPU_Initure.Size=LCD_REGION_SIZE;			//region��С
	MPU_Initure.SubRegionDisable=0X00;
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;
	MPU_Initure.AccessPermission=MPU_REGION_FULL_ACCESS;	//��region�ɶ�д
	MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;	//������ȡ�������е�ָ��
	MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;
	MPU_Initure.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
	MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;
	HAL_MPU_ConfigRegion(&MPU_Initure);
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);     //����MPU
}
//SRAM�ײ�������ʱ��ʹ�ܣ����ŷ���
//�˺����ᱻHAL_SRAM_Init()����
//hsram:SRAM���
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_FMC_CLK_ENABLE();				//ʹ��FMCʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIODʱ��
	__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOEʱ��
	
	//��ʼ��PD0,1,4,5,7,8,9,10,13,14,15
	GPIO_Initure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode = GPIO_MODE_AF_PP; 		//����
	GPIO_Initure.Pull = GPIO_PULLUP;			//����
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;	//����
	GPIO_Initure.Alternate = GPIO_AF12_FMC;	//����ΪFMC
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	//��ʼ��PE7,8,9,10,11,12,13,14,15
	GPIO_Initure.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}
//��������
//color:Ҫ���������ɫ
void Bsp_LCD_Clear(uint32_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	if(lcdltdc.pwidth!=0)	//�����RGB��
	{
		Bsp_LTDC_Clear(color);
	}
	else
	{
		totalpoint*=lcddev.height; 			//�õ��ܵ���
		Bsp_LCD_SetCursor(0x00,0x0000);			//���ù��λ�� 
		Bsp_LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	 	  
		for(index=0;index<totalpoint;index++)
		{
			LCD->LCD_RAM=color;	
		}
	}
}  
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void Bsp_LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	if(lcdltdc.pwidth!=0)	//�����RGB��
	{
		Bsp_LTDC_Fill(sx,sy,ex,ey,color);
	}
	else
	{	
		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
			Bsp_LCD_SetCursor(sx,i);      				//���ù��λ�� 
			Bsp_LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
			for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//��ʾ��ɫ 	    
		} 
	}
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void Bsp_LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	if(lcdltdc.pwidth!=0)	//�����RGB��
	{
		Bsp_LTDC_Color_Fill(sx,sy,ex,ey,color);
	}
	else
	{
		width=ex-sx+1; 			//�õ����Ŀ���
		height=ey-sy+1;			//�߶�
		for(i=0;i<height;i++)
		{
			Bsp_LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
			Bsp_LCD_WriteRAM_Prepare();     //��ʼд��GRAM
			for(j=0;j<width;j++)LCD->LCD_RAM=color[i*width+j];//д������ 
		}
	}	
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void Bsp_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		Bsp_LCD_DrawPoint(uRow,uCol);//���� 
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
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void Bsp_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	Bsp_LCD_DrawLine(x1,y1,x2,y1);
	Bsp_LCD_DrawLine(x1,y1,x1,y2);
	Bsp_LCD_DrawLine(x1,y2,x2,y2);
	Bsp_LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Bsp_LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		Bsp_LCD_DrawPoint(x0+a,y0-b);             //5
 		Bsp_LCD_DrawPoint(x0+b,y0-a);             //0           
		Bsp_LCD_DrawPoint(x0+b,y0+a);             //4               
		Bsp_LCD_DrawPoint(x0+a,y0+b);             //6 
		Bsp_LCD_DrawPoint(x0-a,y0+b);             //1       
 		Bsp_LCD_DrawPoint(x0-b,y0+a);             
		Bsp_LCD_DrawPoint(x0-a,y0-b);             //2             
  		Bsp_LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24/32
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void Bsp_LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else if(size==32)temp=asc2_3216[num][t];	//����3216����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)Bsp_LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)Bsp_LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void Bsp_LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
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
				Bsp_LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}
			else enshow=1; 
		 	 
		}
	 	Bsp_LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void Bsp_LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
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
				if(mode&0X80)Bsp_LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else Bsp_LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}
			else enshow=1; 
		 	 
		}
	 	Bsp_LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void Bsp_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        Bsp_LCD_ShowChar(x,y,*p,size,0);
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