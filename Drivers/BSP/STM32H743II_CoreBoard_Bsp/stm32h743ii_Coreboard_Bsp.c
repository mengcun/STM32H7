/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/stm32h743ii_Coreboard_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    15-July-2018
  * @brief   This file provides set of firmware functions to manage:
  *          - 
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The stm32h743ii_coreboard_bsp canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h743ii_Coreboard_Bsp.h"
#include "Coreboard_Led_Bsp.h"
#include "CoreBoard_Usart1_Bsp.h"
#include "CoreBoard_Lcd_Bsp.h"
#include "CoreBoard_SDRAM_Bsp.h" 
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup CoreBoard_BSP CoreBoard_BSP
  * @brief It contains the functions to init the coreboard.
	* @note	This is the import file to operate the coreboard.
  * @{
  */
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Private_Functions_Prototypes CoreBoard_BSP Private Functions Prototypes
  * @{
  */
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
/** @}
*/	
/*----------------------------CoreBoard_BSP Private Functions Prototypes--------------------------------*/ 

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Private_Functions CoreBoard_BSP Private Functions
  * @{
  */
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Domain1 frequency)
  *            HCLK(Hz)                       = 20000000  (Domain2 frequency)
  *            D1AHB Prescaler                = 2
  *            D1APB1 Prescaler               = 2
  *            D2APB1 Prescaler               = 2
  *            D2APB2 Prescaler               = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
	*
	*						 Fs:PLL����ʱ��Ƶ��,������HSI,CSI,HSE��. 
	*						 Fvco:VCOƵ������PLL1���໷
	*						 Fsys:ϵͳʱ��Ƶ��,PLL1��p��Ƶ���ʱ��Ƶ��
	*						 Fq:PLL1��q��Ƶ���ʱ��Ƶ��
	*
	*						 Fvco = Fs*(PLL_N/PLL_M);
	*						 Fsys = Fvco/PLL_P = Fs*(PLL_N/(PLL_M*PLL_P));
	*						 Fq 	 = Fvco/PLL_Q = Fs*(PLL_N/(PLL_M*PLL_P));
  *
	*						 PLL_N:PLL1��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:4~512.
	*						 PLL_M:PLL1Ԥ��Ƶϵ��(��PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
	*						 PLL_P:PLL1��P��Ƶϵ��(PLL֮��ķ�Ƶ),��Ƶ����Ϊϵͳʱ��,ȡֵ��Χ:2~128.(�ұ�����2�ı���)
	*						 PLL_Q:PLL1��Q��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:1~128.
	*						 PLL_R:PLL1��R��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:1~128.
	*
	*					 	 CPUƵ��(rcc_c_ck) = sys_d1cpre_ck = 400Mhz 
	*						 rcc_aclk = rcc_hclk3 = 200Mhz
	*						 AHB1/2/3/4(rcc_hclk1/2/3/4) = rcc_aclk / 1 = 200Mhz  
	*						 APB1/2/3/4(rcc_pclk1/2/3/4) = rcc_aclk / 2 = 100Mhz  
	*						 FMCʱ��Ƶ�� = pll2_r_ck = ((25/25)*512/2) = 256Mhz (Page340)
	*
	*					 	 �ⲿ����Ϊ25M��ʱ��,�Ƽ�ֵ:PLLN = 160,PLLM = 5,PLLP = 2,PLLQ = 4.
	*						 �õ�:	Fvco = 20*(160/4) = 800Mhz
	*    				 Fsys = 800/2 = 400Mhz
	*    				 Fq 	 = 800/2 = 400Mhz
**/
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /*!< Supply configuration update enable */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
   
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);
  
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV4; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
 
  /*activate CSI clock mondatory for I/O Compensation Cell*/  
  __HAL_RCC_CSI_ENABLE() ;
    
  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;
  
  /* Enables the I/O Compensation Cell */    
  HAL_EnableCompensationCell();  
}
/**
* @brief  CPU L1-Cache enable.
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();
  /* Enable D-Cache */
  SCB_EnableDCache();
//ǿ��D-Cache͸д,�粻����,ʵ��ʹ���п���������������		
	Write_Through();
}

/** @}
*/
/*--------------------------------------CoreBoard_BSP Private Functions ---------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Exported_Functions CoreBoard_BSP Exported Functions
  * @{
  */
/** @defgroup CoreBoard_BSP_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize the hardware of CoreBoard
  @endverbatim
  * @{
  */
/**
  * @brief Initialize the all hardware of the coreboard according to the specified
  */
void Coreboard_BSP_Init(void)
{
  uint8_t x=0;
  uint8_t lcd_id[12];
	
  /* -1- Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* -2- STM32H7xx HAL library initialization:
      - Systick timer is configured by default as source of time base, but user
      can eventually implement his proper time base source (a general purpose
      timer for example or other time source), keeping in mind that Time base
      duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
      handled in milliseconds basis.
      - Set NVIC Group Priority to 4
      - Low Level Initialization
  */
  HAL_Init();

  /* -3- Configure the system clock to 400 MHz */
  SystemClock_Config();
	
  /* -4- Initialize LEDs mounted on STM32H743II_CoreBoard */
	Bsp_InitLed(LED1_Blue);
	Bsp_InitLed(LED1_Green);
	Bsp_InitLed(LED1_Red);
	Bsp_InitLed(LED2_Blue);
	Bsp_InitLed(LED2_Green);
	Bsp_InitLed(LED2_Red);
	
	Bsp_LED_On(LED1_Green); /*For debug, Indect that the system is runnig*/
	
  /* -5- Initialize USART1 mounted on STM32H743II_CoreBoard for */
	Bsp_InitUsart1(115200);
  /* -6- Initialize SDRAM mounted on STM32H743II_CoreBoard*/
	SDRAM_Init();          
  /* -7- Initialize RGB_LCD mounted on STM32H743II_CoreBoard*/
	RGB_LCD_Init(); 
/*	POINT_COLOR=RED; 
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣	
  while(1)
  {
    switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			case 2:LCD_Clear(BLUE);break;
			case 3:LCD_Clear(RED);break;
			case 4:LCD_Clear(MAGENTA);break;
			case 5:LCD_Clear(GREEN);break;
			case 6:LCD_Clear(CYAN);break; 
			case 7:LCD_Clear(YELLOW);break;
			case 8:LCD_Clear(BRRED);break;
			case 9:LCD_Clear(GRAY);break;
			case 10:LCD_Clear(LGRAY);break;
			case 11:LCD_Clear(BROWN);break;
		}
		POINT_COLOR=RED;	  
		LCD_ShowString(10,40,260,32,32,"Apollo STM32H7"); 	
		LCD_ShowString(10,80,240,24,24,"LTDC TEST");
		LCD_ShowString(10,110,240,16,16,"ATOM@ALIENTEK");
 		LCD_ShowString(10,130,240,16,16,lcd_id);		//��ʾLCD ID	      					 
		LCD_ShowString(10,150,240,12,12,"2017/8/12");
		
	  x++;
		if(x==12)x=0;      
		Bsp_LED_Toggled(LED2_Blue); 
		delay_ms(1000);	
	}
*/
}

/*********************CoreBoard_BSP Exported Functions_Group1**************************/

/** @defgroup CoreBoard_BSP_Exported_Functions_Group2 Operation Functions
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
  * @brief  This method returns the STM32H743I EVAL BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32H743II_COREBOARD_BSP_VERSION;
}
 /**
  * @brief  This method returns the status of I_Cache
	* @retval 0 : closed  1: open
  */
uint8_t Get_ICahceSta(void)
{
    uint8_t sta;
    sta = ((SCB->CCR)>>17)&0X01;
    return sta;
}

 /**
  * @brief  This method returns the status of D_Cache
	* @retval 0 : closed  1: open
  */
uint8_t Get_DCahceSta(void)
{
    uint8_t sta;
    sta = ((SCB->CCR)>>16)&0X01;
    return sta;
}
/**
  * @brief  This function is executed in case of error occurrence.
  */
void Error_Handler(void)
{
	/*Coding here when there is an error*/
  while(1)
  {
		Bsp_LED_Off(LED1_Green);
		Bsp_LED_On(LED1_Red);
  }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void f_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,*/
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);

  /* Infinite loop */
  while (1)
  {
  }
}
#endif	/*USE_FULL_ASSERT*/
/**
  * @brief  This function is delay for nus, and the nus must be less then 1000.
  * @param  nus: The time need to be delayed
  */
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	 
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
	
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks = nus * 400; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ

	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
}
/**
  * @brief  This function is delay for nms.
  * @param  nms: The time need to be delayed
  */
void delay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

/** @}
*/
/** @}
*/
/****************************CoreBoard_BSP Exported FunctionsGroup2*********************/
/** @}
*/
/*----------------------------------CoreBoard_BSP Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------CoreBoard_BSP-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
