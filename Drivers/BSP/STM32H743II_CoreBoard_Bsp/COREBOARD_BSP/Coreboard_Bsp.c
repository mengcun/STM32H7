/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/COREBOARD_BSP/Coreboard_Bsp.c
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
	The Coreboard_Bsp canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
#include <stdio.h>
/*******************************************APP/BSP*************************************************/
#include "Coreboard_Bsp.h"
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
/** @defgroup CoreBoard_BSP_Private_Variables CoreBoard_BSP Private Variables
  * @{
  */
/**
  * @brief	The struct of the information of COreBoard
  */  
Infor_CoreBoard CoreBoard_Infor;

/** @}
*/		
/*----------------------------CoreBoard_BSP Private Variables--------------------------------*/
/* Private constants ------------------------------------------------------------------------------*/
/** @defgroup CoreBoard_BSP_Private_Defines CoreBoard_BSP Private constants
  * @{
  */
/**
  * @brief  RTC_Calendar buffer
  */
 

/** @}
*/
/*----------------------------------------CoreBoard_BSP Private constants---------------------------------------*/
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

	/* -3- Configure the system clock to 400 MHz*/
	SystemClock_Config();
	
	/* -4- Initialize LEDs mounted on STM32H743II_CoreBoard */
	Bsp_InitLed(LED1_Blue);
	Bsp_InitLed(LED1_Green);
	Bsp_InitLed(LED1_Red);
	Bsp_InitLed(LED2_Blue);
	Bsp_InitLed(LED2_Green);
	Bsp_InitLed(LED2_Red);
			
	/* -5- Initialize USART1 as baund 115200mounted on STM32H743II_CoreBoard for */
	Bsp_InitUsart1(115200);
	
	/* -6- Initialize SysTick (use system clock 400MHz) mounted on STM32H743II_CoreBoard*/
	Bsp_InitSoftTimer(400);
	
#if HARDWARE_TIM5_DEBUG == 1
	/* -7- Initialize General Hard_Timer_ TIM5 mounted on STM32H743II_CoreBoard*/
	Bsp_InitHardTimer_TIM5(1000000, 200);	//���Ƶ��Frequence = 200 000 000 / 1000000 / 200 = 1Hz	 
#endif	/*HARDWARE_TIM5_DEBUG*/

#if IWDG_DEBUG == 1
	/* -8- Initialize IWatchDog as 1s timeout mounted on STM32H743II_CoreBoard*/			
	Bsp_IWDG_Init(IWDG_PRESCALER_64, 500);
#endif	/*IWDG_DEBUG*/

#if WWDG_DEBUG == 1
	/* -8- Initialize Window WatchDog as 11ms WWDG_Timeout mounted on STM32H743II_CoreBoard*/			
	Bsp_WWDG_Init(0X7F, 0X5F, WWDG_PRESCALER_8);
#endif	/*WWDG_DEBUG*/

#if RTC_GET_TIME == 1 
	/* -9- Initialize RTC mounted on STM32H743II_CoreBoard*/			
	Bsp_RTC_Init();
#endif	/*RTC_GET_TIME*/

#if RTC_WAKEUP_DEBUG == 1 
	/* -10- Initialize RTC WAKE UP as 1s mounted on STM32H743II_CoreBoard*/			
	Bsp_RTC_Set_WakeUp(RTC_WAKEUPCLOCK_CK_SPRE_16BITS,0); 
#endif	/*RTC_WAKEUP_DEBUG*/	

#if RTC_ALARMA_DEBUG == 1 
	/* -11- Initialize RTC ALARM as Wednesday AM 11:00:00 mounted on STM32H743II_CoreBoard*/			
    Bsp_RTC_Set_AlarmA(3,11,00,00);	
#endif	/*RTC_ALARMA_DEBUG*/

#if RTC_TIMESTAMP_DEBUG == 1 
	/* -12- Initialize RTC TIME STAMP as rising edge on PC.13 mounted on STM32H743II_CoreBoard*/			
	Bsp_RTC_Set_TimeStamp();
#endif	/*RTC_TIMESTAMP_DEBUG*/

#if RTC_TAMPER_DEBUG == 1 
	/* -13- Initialize RTC TIME STAMP as failing edge on PC.13 mounted on STM32H743II_CoreBoard*/				
	Bsp_RTC_Set_Tamper();
#endif	/*RTC_TAMPER_DEBUG*/

#if RTC_BKRAM_DEBUG == 1
	/* -14- Initialize RTC BackUp RAM mounted on STM32H743II_CoreBoard*/				
	Bsp_RTC_Set_BackupRAM();
#endif	/*RTC_BKRAM_DEBUG*/

#if HARDWARE_TIM2PWM_DEBUG == 1
	/* -15.1- Initialize TIM2_CHANNEL3_PWM as 100 000 Hz and remaping to PA2 mounted on STM32H743II_CoreBoard*/	
	/*Frequence(Hz) = TIM2CLK / Period / Prescaler = 200 000 000 / param1 / param2 */
	Bsp_InitHardTimer_TIM2(100, 20, 20, TIM_CHANNEL_3);		//���Ƶ��Frequence = 200 000 000 / 100 / 20 = 100 000Hz
	
	/* -15.2- Initialize TIM2_CHANNEL4_PWM as 100 000 Hz and remaping to PA2 mounted on STM32H743II_CoreBoard*/	
	/*Frequence(Hz) = TIM2CLK / Period / Prescaler = 200 000 000 / param1 / param2 */
	Bsp_InitHardTimer_TIM2(100, 20, 50, TIM_CHANNEL_4);		//���Ƶ��Frequence = 200 000 000 / 100 / 20 = 100 000Hz
		
#endif	/*HARDWARE_TIM2PWM_DEBUG*/

#if CRC_DEBUG == 1
	/* -16- Initialize CRC and Cumpute CRC by re-initialized default polynomial 0x4C11DB7, and default init value mounted on STM32H743II_CoreBoard*/				
	Bsp_InitDefautCRC();
	Bsp_ComputeCRCDefault();
	
	/* -17- Initialize CRC and Cumpute CRC by user define polynomial 0x9B without re-initialized, default init value mounted on STM32H743II_CoreBoard*/					
	Bsp_InitUserDefineCRC();
	Bsp_ComputeCRCAccumulate();
#endif	/*CRC_DEBUG*/

#if DAC_WAVE_DEBUG == 1	
	/* -18- Initialize TIM6 for DAC_CHANNEL2 on PA4 and PA5 mounted on STM32H743II_CoreBoard*/								
	Bsp_InitHardTimer_TIM6(2000, 0);		//���Ƶ��Frequence = 200 000 000 / 2000 / 1 = 100 000Hz
	
	/* -19- Initialize DAC_CHANNEL1 on PA4 as NOISE mode mounted on STM32H743II_CoreBoard*/						
	Bsp_InitNoiseDAC(DAC_LFSRUNMASK_BITS11_0);
	
	/* -20- Initialize DAC_CHANNEL1 on PA4 as TRIANGLE mode mounted on STM32H743II_CoreBoard*/							
	Bsp_InitTriangleDAC(DAC_TRIANGLEAMPLITUDE_4095);
	
	/* -21- Initialize DAC_CHANNEL1 Triangle and DAC_CHANNEL2 Noise on PA4 and PA5 mounted on STM32H743II_CoreBoard*/								
	Bsp_InitDualWaveDAC(DAC_TRIANGLEAMPLITUDE_4095,DAC_LFSRUNMASK_BITS11_0);
	
#endif	/*DAC_WAVE_DEBUG*/

#if SINWAVE_GEN_FOR_TEST == 1	
	/* -22- Initialize TIM6 for DAC_CHANNEL2 on PA4 and PA5 mounted on STM32H743II_CoreBoard*/								
	Bsp_InitHardTimer_TIM6(20000, 2000);		//���Ƶ��Frequence = 200 000 000 / 20000 / 2000 = 5Hz
	
	/* -23- Initialize DAC_CHANNEL1 on PA4 as ESCALATOR DMA_STREAM5 mode mounted on STM32H743II_CoreBoard*/							
	//Bsp_InitEscalatorDAC();		//The Escalator and the Sin wave are used DMA,can not test them at the same time.
	
	/* -24- Initialize DAC_CHANNEL1 on PA4 as SIN DMA_STREAM5 mode mounted on STM32H743II_CoreBoard*/							
	Bsp_InitSinWaveDAC();
	
#endif	/*SINWAVE_GEN_FOR_TEST*/


#if ADC3_SINGLE_CHANNEL_CONVERT == 1
	/* -24- Initialize ADC12 CHANNEL 6 Connected on PA6 mounted on STM32H743II_CoreBoard*/							
	Bsp_Init_ADC3_SINGLE_CHANNEL(ADC_CHANNEL_TEMPSENSOR);
	Bsp_Init_ADC3_SINGLE_CHANNEL(ADC_CHANNEL_VREFINT);
	Bsp_Init_ADC3_SINGLE_CHANNEL(ADC_CHANNEL_VBAT_DIV4);
#endif	/*ADC3_SINGLE_CHANNEL_CONVERT*/

#if ADC3_MULTI_CHANNEL_CONVERT == 1
	/* -24- Initialize ADC12 CHANNEL 6 Connected on PA6 mounted on STM32H743II_CoreBoard*/							
	Bsp_Init_ADC3_MULTI_CHANNEL();
#endif	/*ADC3_SINGLE_CHANNEL_CONVERT*/

#if ADC_DualMode_Interleaved == 1
	/* -25- Initialize ADC12 CHANNEL 18 Connected on PA4 as Dual Mode mounted on STM32H743II_CoreBoard*/							
	Bsp_Init_DualModeADC(ADC1, ADC_CHANNEL_18, ADC2, ADC_CHANNEL_18);
	
#if ADC_DUAL_TRIGGER_FROM_TIMER == 1
	
	Bsp_InitHardTimer_TIM3(1000, 200);
	if (HAL_TIM_Base_Start(&TIM2_Handler) != HAL_OK)
	{
		Error_Handler();
	}
#endif	/*ADC_DUAL_TRIGGER_FROM_TIMER*/
	  
	if (HAL_ADCEx_MultiModeStart_DMA(&ADC_Handler_Master, (uint32_t *)aADCDualConvertedValues, ADCCONVERTEDVALUES_BUFFER_SIZE) != HAL_OK)
	{
		Error_Handler();
	}	
	
#endif	/*ADC_DualMode_Interleaved*/

#if USMART_DEBUG == 1			
	/* -26- Initialize USER_DEBUG mounted on STM32H743II_CoreBoard*/			
	usmart_dev.init(); 	
#endif	/*USMART_DEBUG*/	

#if SDRAM_DEBUG == 1			
	/* -27- Initialize SDRAM mounted on STM32H743II_CoreBoard*/
	Bsp_SDRAM_Init();   
#endif	/*SDRAM_DEBUG*/

#if RGBLCD_DEBUG == 1			
	/* -28- Initialize RGB_LCD mounted on STM32H743II_CoreBoard*/
	Bsp_RGB_LCD_Init(); 
#endif	/*RGBLCD_DEBUG*/

#if RNG_DEBUG == 1	
	/* -28- Initialize and enable RNG by Interrupt Mode mounted on STM32H743II_CoreBoard*/					
	Bsp_InitRNG();		/*It produces four 32-bit random samples every 16*FAHB/FRNG AHB clock cycles, if value is higher than 213 cycles (213 cycles otherwise).*/
						/*After enabling the RNG for the first time, random data is first available after either */
						/*128 RNG clock cycles + 426 AHB cycles, if fAHB < fthreshold;	192 RNG clock cycles + 213 AHB cycles, if fAHB >= fthreshold*/
#endif	/*RNG_DEBUG*/	

	/* -29- Get the Information about STM32H743II_CoreBoard*/
	GetInfo_CoreBoard();
	POINT_COLOR=RED; 		
	while(1)
	{	
		Bsp_LCD_Clear(BLUE);
		
		GetInfo_Calendar();
		
		Bsp_LCD_ShowString(10,40,800,32,32,CoreBoard_Infor.BOARD_NAME); 	
		Bsp_LCD_ShowString(10,80,800,24,24,CoreBoard_Infor.CPU_NAME);
		Bsp_LCD_ShowString(10,120,800,24,24,CoreBoard_Infor.CPU_ID);
		Bsp_LCD_ShowString(10,160,800,24,24,CoreBoard_Infor.BSP_VERSION);	
		Bsp_LCD_ShowString(10,200,800,24,24,CoreBoard_Infor.LCD_ID);  				 
		Bsp_LCD_ShowString(10,240,800,24,24,aShowTime);
		Bsp_LCD_ShowString(10,280,800,24,24,aShowDate);
		Bsp_LCD_ShowString(10,320,800,24,24,aShowWeek);
		
		Bsp_Delay_ms(1000);	
		
#if DAC_WAVE_DEBUG == 1	
		DAC_Ch1_NoiseConfig(DAC_LFSRUnmaskTbl[iTestDAC]);
		Bsp_Printf("The DAC LFSR Noise Config Selected as %X! \r\n",DAC_LFSRUnmaskTbl[iTestDAC]);
		
		Bsp_Delay_ms(5000);	
		
		DAC_Ch1_TriangleConfig(DAC_TriangleAmpTbl[iTestDAC]);
		Bsp_Printf("The DAC Triangle Config Selected as %X! \r\n",DAC_TriangleAmpTbl[iTestDAC]);
		
		Bsp_Delay_ms(5000);	
		
		iTestDAC ++;
		if(iTestDAC == 11)
		{
			iTestDAC = 0;
		}

		Bsp_Ch12_DualWaveConfig(DAC_TriangleAmpTbl[8],DAC_LFSRUnmaskTbl[8]);
		
		Bsp_Printf("The CH1 is generate the triangle as %X! \r\n",DAC_TriangleAmpTbl[8]);
		Bsp_Printf("The CH2 is generate the Noise as %X! \r\n",DAC_LFSRUnmaskTbl[8]);
		
		Bsp_Delay_ms(5000);	
		
#endif	/*DAC_WAVE_DEBUG*/
		
#if ADC3_SINGLE_CHANNEL_CONVERT == 1	

		Bsp_Init_ADC3_SINGLE_CHANNEL(ADC_CHANNEL_TEMPSENSOR);
		Bsp_Get_CPU_Intern_Temperature();
		
		Bsp_Init_ADC3_SINGLE_CHANNEL(ADC_CHANNEL_VREFINT);
		Bsp_Get_CPU_Intern_VerfInt();	
		
		Bsp_Init_ADC3_SINGLE_CHANNEL(ADC_CHANNEL_VBAT_DIV4);
		Bsp_Get_Board_VBAT();
#endif	/*ADC3_SINGLE_CHANNEL_CONVERT*/

#if ADC3_MULTI_CHANNEL_CONVERT == 1
		Bsp_Printf("The Temperature in CPU is %d degress! \r\n",ADCxConvertedData[0]);
		Bsp_Printf("The CPU_Intern_VerfInt is %d mV! \r\n",ADCxConvertedData[1]);
		Bsp_Printf("The Board Vbat is %d mV! \r\n",ADCxConvertedData[2]);
		Bsp_Printf("The ADC Value in PA4 ADC1 is %d! \r\n",ADCxConvertedData[3]);
		Bsp_Printf("The ADC Value in PA6 ADC2 is %d! \r\n",ADCxConvertedData[4]);

#endif	/*ADC3_SINGLE_CHANNEL_CONVERT*/

#if RNG_DEBUG == 1		
		RNG_Get_RandomNum();
		Bsp_Printf("The Random32bit is generated in Polling Mode, RNG= %X! \r\n",POLL_Random32bit);
		RNG_Get_RandomRange(0x00000000,0x00000020);
		Bsp_Printf("The Random32bit is generated in Polling Mode, RNG = %X! \r\n",POLL_Random32bit);
		Bsp_Printf("Computed The CRC by re-initialized with the default polynomial 0x4C11DB7! CRC = %X \r\n", DefaultCRCValue);
		Bsp_Printf("Computed The CRC by the previously computed CRC! CRC = %X \r\n", AccumulateCRCValue);
#endif	/*RNG_DEBUG*/
		
#if RNG_IT_ENABLE == 1	
		__HAL_RNG_ENABLE_IT(&Rng_Handler);/*��ΪRNG���ж����ȼ������ڴ��ڿ��Ź�,��RNG����һ�������ͻ�����ж�,Ϊ�˱�֤ϵͳ�ȶ�����,���ж��н����жϹر�,ֻ������Ҫ�����ʱ�����¿���*/
		Bsp_Printf("The Random32bit is generated with Interrupt, RNG = %X! \r\n",IT_Random32bit);
#endif	/*RNG_IT_ENABLE*/
		
	}
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
/****
	* @brief  System Clock Configuration
	*         The system Clock is configured as follow : 
    *            System Clock source            = PLL (HSE)
    *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
    *            HCLK(Hz)                       = 200000000 (For D1 Domain AXI and AHB3; D2 Domain AHB1/AHB2; D3 Domain AHB4 )
    *            AHB Prescaler                  = 2
    *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
    *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
    *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
    *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
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
	*    				 	Fsys = 800/2 = 400Mhz
	*    				 	Fq 	 = 800/2 = 400Mhz
****/
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
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
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

/**
  * @brief 	�ú���ÿ��10ms��Systick�жϵ���1�Ρ���� Software_Timer_BSP.c�Ķ�ʱ�жϷ������һЩ��Ҫ�����Դ���
  *		   	��������Է��ڴ˺��������磺����ɨ�衢���������п��Ƶȡ�
  */
void BSP_RunPer10ms(void)
{
	CountEvery10ms ++ ;
	if(CountEvery10ms == 60)	//����LSIʱ�ӷ�ΧΪ17KHz-47KHz, ����ʹ��32KHz���������ۼ��㣬
	{							//�����Ϊ0.68s - 1.88s,Ϊ�˼�ʱι����֤ϵͳ�������У�����ÿ��0.6sι��
		CountEvery10ms = 0;
		Bsp_LED_Toggled(LED1_Green); 	//ÿ��LED1_Green��˸����ϵͳ��������
#if IWDG_DEBUG == 1
		/* --- ι�� */
		Bsp_IWDG_Feed();
		Bsp_Printf("IWDG feed in BSP_RunPer10ms() every 0.6s!\r\n");
#endif
	}	
}

/**
  * @brief 	�ú���ÿ��1ms��Systick�жϵ���1�Ρ���� bsp_timer.c�Ķ�ʱ�жϷ������һЩ��Ҫ�����Դ����
  *			������Է��ڴ˺��������磺��������ɨ�衣
  */
void BSP_RunPer1ms(void)
{
}

/**
  * @brief 	����ʱִ�еĺ�����һ����������for��whileѭ������������Ҫ���� CPU_IDLE() �������ñ�������
  *			������ȱʡΪ�ղ������û��������ι��������CPU��������ģʽ�Ĺ��ܡ�
  */
void BSP_Idle(void)
{
#if IWDG_DEBUG == 1
	/* --- ι�� */
    Bsp_IWDG_Feed();
	Bsp_Printf("IWDG feed in BSP_Idle() when calling the Bsp_Delay_ms(), This time is less then 1ms!\r\n");
#endif
	/* --- ��CPU�������ߣ���Systick��ʱ�жϻ��ѻ��������жϻ��� */

	/* ���� emWin ͼ�ο⣬���Բ���ͼ�ο���Ҫ����ѯ���� */
	//GUI_Exec();

	/* ���� uIP Э��ʵ�֣����Բ���uip��ѯ���� */
}

/**
  * @brief  This method returns the STM32H743I EVAL BSP Driver revision
  */
void GetInfo_CoreBoard(void)
{
	/* ���CPU ID */
	/* �ο��ֲ᣺
		60.1 Unique device ID register (96 bits)
	*/
	uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

	CPU_Sn0 = *(__IO uint32_t*)(0x1FF1E800);
	CPU_Sn1 = *(__IO uint32_t*)(0x1FF1E800 + 4);
	CPU_Sn2 = *(__IO uint32_t*)(0x1FF1E800 + 8);
	
	sprintf((char*)CoreBoard_Infor.BOARD_NAME, "BOARD NAME : STM32H7 COREBOARD");
	sprintf((char*)CoreBoard_Infor.CPU_NAME, "CPU NAME : STM32H743II16 LQFP176");
	sprintf((char*)CoreBoard_Infor.CPU_ID, "CPU ID : %08X %08X %08X", CPU_Sn2, CPU_Sn1, CPU_Sn0);
	sprintf((char*)CoreBoard_Infor.BSP_VERSION,"BSP VERSION : %08X", __STM32H743II_COREBOARD_BSP_VERSION);
	sprintf((char*)CoreBoard_Infor.LCD_ID,"LCD ID : %04X",lcddev.id);	
	
	Bsp_Printf("%s \r\n",CoreBoard_Infor.BOARD_NAME);
	Bsp_Printf("%s \r\n",CoreBoard_Infor.CPU_NAME);
	Bsp_Printf("%s \r\n",CoreBoard_Infor.CPU_ID);
	Bsp_Printf("%s \r\n",CoreBoard_Infor.BSP_VERSION);
	Bsp_Printf("%s \r\n",CoreBoard_Infor.LCD_ID);
}

 /**
  * @brief  This method returns the status of I_Cache
  * @retval 0 : closed  1: open
  */
uint8_t Bsp_Get_ICahceSta(void)
{
    uint8_t sta;
    sta = ((SCB->CCR)>>17)&0X01;
    return sta;
}

 /**
  * @brief  This method returns the status of D_Cache
	* @retval 0 : closed  1: open
  */
uint8_t Bsp_Get_DCahceSta(void)
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
/* 
	ST�⺯��ʹ����C�������Ķ��Թ��ܣ����������USE_FULL_ASSERT����ô���е�ST�⺯������麯���β�
	�Ƿ���ȷ���������ȷ������ assert_failed() ���������������һ����ѭ���������û������롣
	
	�ؼ��� __LINE__ ��ʾԴ�����кš�
	�ؼ���__FILE__��ʾԴ�����ļ�����
	
	���Թ���ʹ�ܺ���������С���Ƽ��û����ڵ���ʱʹ�ܣ�����ʽ��������ǽ�ֹ��

	�û�����ѡ���Ƿ�ʹ��ST�̼���Ķ��Թ��ܡ�ʹ�ܶ��Եķ��������֣�
	(1) ��C��������Ԥ�����ѡ���ж���USE_FULL_ASSERT��
	(2) �ڱ��ļ�ȡ��"#define USE_FULL_ASSERT    1"�е�ע�͡�	
*/
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,*/
	Bsp_Printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	/* Infinite loop */
	while (1)
	{
	}
}
#endif	/*USE_FULL_ASSERT*/

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
