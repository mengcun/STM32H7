/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/RTC/RTC_Bsp.c
  * @author  MCD Application Team
  * @version SW:V1.0.0 HW:V1.0
  * @date    28-August-2018
  * @brief   This file provides set of firmware functions to manage: RTC
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The RTC_Calendar canbe used as follows:
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
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup RTC_Calendar RTC_Calendar
  * @brief RTC_Calendar
	* @note	RTC_Calendar
  * @{
  */
/** @defgroup RTC_Private_Variables RTC Private Variables
  * @{
  */
/**
  * @brief	RTC_Calendar Private Handle
  */
RTC_HandleTypeDef 		RTC_Handler;  //RTC句柄
/**
  * @brief	RTC_Calendar Time Struct
  */
RTC_TimeTypeDef 		RTC_TimeStruct;
/**
  * @brief	RTC_Calendar Date Struct
  */
RTC_DateTypeDef 		RTC_DateStruct;
/**
  * @brief	RTC_Calendar Alarm Struct
  */
RTC_AlarmTypeDef		RTC_AlarmStruct;
/**
  * @brief	RTC_Calendar Tamper Struct
  */
RTC_TamperTypeDef  		RTC_TamperStruct;

/**
  * @brief	Buffers used for displaying Time and Date
  */
uint8_t aShowTime[40] = {0};
uint8_t aShowDate[40] = {0}; 
uint8_t aShowWeek[10] = {0};
uint8_t aShowTimeStamp[40] = {0};
uint8_t aShowDateStamp[40] = {0};

/**
  * @brief	TamperStatus
  */
__IO FlagStatus TamperStatus = RESET;

/**
  * @brief	RTC BACKUP Reg defines. there are 32 RTC_BKPxR.
  *	@note	当系统发生故障或掉电时，使用HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister, uint32_t Data)将重要信息写入BKP寄存器中
  */

uint32_t aBKPDataReg[32] = 
{ 	RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR3, RTC_BKP_DR3, RTC_BKP_DR4, 
	RTC_BKP_DR5, RTC_BKP_DR6, RTC_BKP_DR7, RTC_BKP_DR8, RTC_BKP_DR9, 
	RTC_BKP_DR10, RTC_BKP_DR11, RTC_BKP_DR12, RTC_BKP_DR13, RTC_BKP_DR14, 
	RTC_BKP_DR15, RTC_BKP_DR16, RTC_BKP_DR17, RTC_BKP_DR18, RTC_BKP_DR19,
	RTC_BKP_DR20, RTC_BKP_DR21, RTC_BKP_DR22, RTC_BKP_DR23, RTC_BKP_DR24,
	RTC_BKP_DR25, RTC_BKP_DR26, RTC_BKP_DR27, RTC_BKP_DR28, RTC_BKP_DR29,
	RTC_BKP_DR30, RTC_BKP_DR31
};

/** @}
*/		
/*----------------------------RTC Private Variables--------------------------------*/
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup RTC_Calendar_Private_Functions_Prototypes RTC_Calendar Private Functions Prototypes
  * @{
  */

/** @}
*/		
/*----------------------------RTC_Calendar Private Functions Prototypes--------------------------------*/ 

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup RTC_Calendar_Private_Functions RTC_Calendar Private Functions
  * @{
  */
//CODING HERE Private Functions
/** @}
*/
/*--------------------------------------RTC_Calendar Private Functions---------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup RTC_Calendar_Exported_Functions RTC_Calendar Exported Functions
  * @{
  */
/** @defgroup RTC_Calendar_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize RTC_Calendar
  @endverbatim
  * @{
  */
/**
* @brief 	RTC初始化:这里使用LSE外部32.768KHz时钟; AsynchPrediv = 128;SynchPrediv = 256; 正好得到1Hz
*			ck_apre(1Hz) = RTCCLK(LSE) /(AsynchPrediv + 1) : 用于驱动二进制RTV_SSR向下递减的亚秒计数器，当计数到0的时候，RTC_SSR寄存器重新加载AsynchPrediv的值
*			ck_spre(1Hz) = RTCCLK(LSE) /(AsynchPrediv + 1)*(SynchPrediv + 1) : 用于更新万年历或者16位自动重装唤醒定时器的时基
  *			RTC最小分频1，最大分频2^22(7+15),所以RTC最大输入时钟为4MHz
  * @retval 0,初始化成功
  */
uint8_t Bsp_RTC_Init(void)
{ 	
    /*Configure the RTC peripheral*/
	RTC_Handler.Instance=RTC;
    RTC_Handler.Init.HourFormat=RTC_HOURFORMAT_24;				//24小时格式 
    RTC_Handler.Init.AsynchPrediv=0X7F;           				//7位异步分频系数(1~0X7F)如果两个分频器都是用，推荐将异步分频器配置为最大，降低功耗
    RTC_Handler.Init.SynchPrediv=0XFF;            				//步分频系数(0~7FFF)   
    RTC_Handler.Init.OutPut=RTC_OUTPUT_DISABLE;     
    RTC_Handler.Init.OutPutPolarity=RTC_OUTPUT_POLARITY_HIGH;
    RTC_Handler.Init.OutPutType=RTC_OUTPUT_TYPE_OPENDRAIN;
   
	if(HAL_RTC_Init(&RTC_Handler)!=HAL_OK)	
	{
		/* Initialization Error */
		Error_Handler();
	}

    if(HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0)!=0X5050)	//用于检测是否已经配置过RTC,如果配置过的话,会在配置结束时设置RTC备份寄存器为0x32F2。
																//如果检测RTC备份寄存器不0X5050,那么表示没有配置过,需要配置RTC.
    { 
        Bsp_RTC_Set_Time(10,50,0,0,RTC_HOURFORMAT12_AM);	        //设置时间,根据实际时间修改
		Bsp_RTC_Set_Date(18,8,29,35);		                    //设置日期
        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050);	//标记已经初始化过了
    }
    return 0;
}

/**
  * @brief 	RTC底层驱动,时钟配置,此函数会被HAL_RTC_Init()调用
  * @param  hrtc:RTC句柄
  */

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    __HAL_RCC_RTC_CLK_ENABLE();     							//使能RTC时钟
	HAL_PWR_EnableBkUpAccess();     							//取消备份区域写保护
		
    RCC_OscInitStruct.OscillatorType=RCC_OSCILLATORTYPE_LSE;	//LSE配置
    RCC_OscInitStruct.PLL.PLLState=RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState=RCC_LSE_ON;                  	//RTC使用LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_RTC;	//外设为RTC
    PeriphClkInitStruct.RTCClockSelection=RCC_RTCCLKSOURCE_LSE;	//RTC时钟源为LSE
   
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	    
    __HAL_RCC_RTC_ENABLE();										//RTC时钟使能
}

/** @}
*/
/*********************RTC_Calendar Exported Functions Group1**************************/

/** @defgroup RTC_Calendar_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the RTC_Calendar
  @endverbatim
  * @{
  */
/**
  * @brief 	RTC时间设置
  * @param 	hour,min,sec:小时,分钟,秒钟
  *			ampm:RTC_HOURFORMAT12_AM or RTC_HOURFORMAT12_PM
  * @retval SUCEE(1),成功;ERROR(0),进入初始化模式失败
  */
    
HAL_StatusTypeDef Bsp_RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t subsec,uint8_t ampm)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_TimeStructure.Hours=hour;
	RTC_TimeStructure.Minutes=min;
	RTC_TimeStructure.Seconds=sec;
	RTC_TimeStructure.SubSeconds=subsec;
	RTC_TimeStructure.TimeFormat=ampm;
	RTC_TimeStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation=RTC_STOREOPERATION_RESET;
	return HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStructure,RTC_FORMAT_BIN);	
}

/**
  * @brief 	RTC日期设置
  * @param 	year,month,date:年(0~99),月(1~12),日(0~31)
  *			week:星期(1~7,0,非法!)
  * @retval SUCEE(1),成功;ERROR(0),进入初始化模式失败
  */
 
HAL_StatusTypeDef Bsp_RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week)
{
	RTC_DateTypeDef RTC_DateStructure;
    
	RTC_DateStructure.Year=year;
	RTC_DateStructure.Month=month;
	RTC_DateStructure.Date=date;
	RTC_DateStructure.WeekDay=week;
	return HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStructure,RTC_FORMAT_BIN);
}

/**
  * @brief 	设置闹钟时间(按星期闹铃,24小时制)
  * @param 	week:星期几(1~7) RTC_WeekDay_Definitions
  *			hour,min,sec:小时,分钟,秒钟
  */
 
void Bsp_RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  								//小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min; 								//分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; 								//秒
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;							//精确匹配星期，时分秒
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;	//按星期
    RTC_AlarmSturuct.AlarmDateWeekDay=week; 								//星期
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     								//闹钟A
	
    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_ALRBF);
    if(HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN) != HAL_OK)
  	{	
		/* Initialization Error */
		Error_Handler();
	}
       
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,4,3);//设置RTC闹钟中断优先级,抢占优先级4,子优先级3,注意比串口优先级低
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
  * @brief 	周期性唤醒定时器设置
  * @param 	wksel: RTCEx_Wakeup_Timer_Definitions
  *			cnt:自动重装载值.减到0,产生中断.
  */
 
void Bsp_RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF);	//清除RTC WAKE UP的标志
	if(HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel) != HAL_OK)            //设置重装载值和时钟 
 	{	
		/* Initialization Error */
		Error_Handler();
	}
    
	HAL_NVIC_SetPriority(RTC_WKUP_IRQn,4,4);//设置RTC周期唤醒中断优先级,抢占优先级4,子优先级4,注意比串口优先级低
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

/**
  * @brief 	RTC时间戳功能设置TimeStamp Rising Edge on PC.13 Pin
  */

void Bsp_RTC_Set_TimeStamp(void)
{ 
    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_TSF);	//清除 Time Stamp 标志
	if(HAL_RTCEx_SetTimeStamp_IT(&RTC_Handler, RTC_TIMESTAMPEDGE_RISING, RTC_TIMESTAMPPIN_DEFAULT) != HAL_OK)															
 	{	
		/* Initialization Error */
		Error_Handler();
	}
	
	HAL_NVIC_SetPriority(TAMP_STAMP_IRQn, 4, 0);//设置RTC时间戳中断优先级,抢占优先级4,子优先级0, 注意比串口优先级低
	HAL_NVIC_EnableIRQ(TAMP_STAMP_IRQn);
}

/**
  * @brief 	RTC入侵检测功能设置 TimeStamp Failing Edge on PC.13 Pin
  * 		检测到的入侵事件将复位所有备份寄存器的值
  */

void Bsp_RTC_Set_Tamper(void)
{
	/* Configure RTC Tamper */
	RTC_TamperStruct.Tamper                       = RTC_TAMPER_1;
	RTC_TamperStruct.Trigger                      = RTC_TAMPERTRIGGER_FALLINGEDGE;
	RTC_TamperStruct.Filter                       = RTC_TAMPERFILTER_DISABLE;
	RTC_TamperStruct.SamplingFrequency            = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV32768;
	RTC_TamperStruct.PrechargeDuration            = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
	RTC_TamperStruct.TamperPullUp                 = RTC_TAMPER_PULLUP_ENABLE;
	RTC_TamperStruct.TimeStampOnTamperDetection   = RTC_TIMESTAMPONTAMPERDETECTION_DISABLE;
	RTC_TamperStruct.NoErase                      = RTC_TAMPER_ERASE_BACKUP_ENABLE;
	RTC_TamperStruct.MaskFlag                     = RTC_TAMPERMASK_FLAG_DISABLE;
	RTC_TamperStruct.Interrupt                    = RTC_TAMPER1_INTERRUPT;

    __HAL_RTC_TAMPER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TAMP1F);
	if (HAL_RTCEx_SetTamper_IT(&RTC_Handler, &RTC_TamperStruct) != HAL_OK)
	{
	/* Initialization Error */
	Error_Handler();
	}
	
	HAL_NVIC_SetPriority(TAMP_STAMP_IRQn, 4, 0);//设置RTC入侵加测 中断优先级,抢占优先级4,子优先级0, 注意比串口优先级低
	HAL_NVIC_EnableIRQ(TAMP_STAMP_IRQn);	
}

/**
  * @brief 	配置备份RAM并启用
  */

void Bsp_RTC_Set_BackupRAM(void)
{
	/* 使能 BKPRAM 时钟 */
	__HAL_RCC_BKPRAM_CLK_ENABLE();
	/* 启用备份SRAM的低功耗稳压器，在VBAT模式仍保持其内*/
	HAL_PWREx_EnableBkUpReg();

	/* 等待直到备份SRAM的低功耗稳压器就绪 */
	while(__HAL_PWR_GET_FLAG(PWR_FLAG_BRR) == RESET)
	{
	}
}
/**
  * @brief 	得到当前时间
  */
 
void Bsp_RTC_GetTime(void)
{
	/* 得到当前时分秒时间 */
	HAL_RTC_GetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
}

/**
  * @brief 	得到当前日期
  */
 
void Bsp_RTC_GetDate(void)
{
	/* 得到当前年月日时间 */
	HAL_RTC_GetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
}

/**
  * @brief 	得到闹钟时间
  */
 
void Bsp_RTC_GetAlarm(void)
{
	/* 得到当前闹钟时间 */
	HAL_RTC_GetAlarm(&RTC_Handler,&RTC_AlarmStruct,RTC_ALARM_A,RTC_FORMAT_BIN);
}

/**
  * @brief  This method Display the current time
  */

void GetInfo_Calendar(void)
{
	Bsp_RTC_GetTime();
	Bsp_RTC_GetDate();
	Bsp_RTC_GetAlarm();
	sprintf((char*)aShowTime,"Time:%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds); 
	sprintf((char*)aShowDate,"Date:20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); 
	sprintf((char*)aShowWeek,"Week:%d",RTC_DateStruct.WeekDay); 
}

/**
  * @brief	写数据到RTC的备份数据寄存器
  * @param  FirstRTCBackupData:写入的数据
  */

void Bsp_WriteToRTC_BKP_DR(uint32_t FirstRTCBackupData)
{
	uint32_t index = 0;

	for (index = 0; index < 32; index++)
	{
		/* 写数据到备份数据寄存器 */
		HAL_RTCEx_BKUPWrite(&RTC_Handler,aBKPDataReg[index], FirstRTCBackupData + (index * 0x5A));
	}
}

/**
  * @brief	检测写入的数据是否正确
  * @param  FirstRTCBackupData:写入的数据
  * @return 0  写入和读出的全部相同
  *         !0 写入的数据有错误，立即返回
  */

uint32_t Bsp_CheckRTC_BKP_DR(uint32_t FirstRTCBackupData)
{
	uint32_t index = 0;

	for (index = 0; index < 32; index++)
	{
		/* 从备份数据寄存器中读数据 */
		if (HAL_RTCEx_BKUPRead(&RTC_Handler,aBKPDataReg[index]) != (FirstRTCBackupData + (index * 0x5A)))
		{
		  return (index + 1);
		}
	}
	return 0;
}

/**
  * @brief	检测写入到备份寄存器的数据是否清除
  * @return 0  该备份寄存器已经清零
  *         !0 该备份寄存器中数据没有清零，立即返回
  */

uint32_t Bsp_IsBackupRegReset(void)
{
	uint32_t index = 0;

	for (index = 0; index < 32; index++)
	{
		/* 读备份数据寄存器 */
		if (HAL_RTCEx_BKUPRead(&RTC_Handler,aBKPDataReg[index]) != 0)
		{
		  return (index + 1);
		}
	}
	return 0;
}

/**
  * @brief	向备份SRAM里面写入数据并检查写入是否正确
  *			Backup SRAM(4 KB) over AXI->AHB Bridge
  * @return 0: 正确写入 !0:写入错误
  */

uint32_t Bsp_WriteToSRAM(void)
{
	uint32_t uwIndex = 0;
	uint32_t uwErrorIndex = 0;
	for (uwIndex = 0x0; uwIndex < 0x1000; uwIndex += 4)
	{
		*(__IO uint32_t *) (D3_BKPSRAM_BASE + uwIndex) = uwIndex;
	}
	/* 检测写入的数据 */
	for (uwIndex = 0x0; uwIndex < 0x1000; uwIndex += 4)
	{
		if ((*(__IO uint32_t *) (D3_BKPSRAM_BASE + uwIndex)) != uwIndex)
		{
		  uwErrorIndex++;
		}
	}
	/* 检测是否有错误 */
	if(uwErrorIndex)
	{
		BSP_Printf ("BKP SRAM Number of errors = %d\r\n", uwErrorIndex);
		uwErrorIndex = 0;
		return 1;
	}
	else
	{
		BSP_Printf ("BKP SRAM write OK \r\n");
		uwErrorIndex = 0;
		return 0;
	}
}

/**
  * @brief	RTC闹钟中断服务函数
  */

void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
}
 
/**
  * @brief	RTC WAKE UP中断服务函数

  */

void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 
}

/**
  * @brief  This function handles Tamper interrupt request.
  */

void TAMP_STAMP_IRQHandler(void)
{
  HAL_RTCEx_TamperTimeStampIRQHandler(&RTC_Handler);
  TamperStatus = SET;
}

/**
  * @brief	RTC闹钟A中断处理回调函数
  */

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    BSP_Printf("RTC_AlarmAEventCallback!\r\n");
}

/**
  * @brief	RTC WAKE UP中断处理

  */

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    BSP_Printf("RTCEx_WakeUpTimerEventCallback!\r\n");
}

/**
  * @brief  Timestamp callback 
  * @param  hrtc : hrtc handle
  */

void HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc)
{  
  HAL_RTCEx_GetTimeStamp(&RTC_Handler, &RTC_TimeStruct, &RTC_DateStruct, RTC_FORMAT_BIN);
  BSP_Printf("TimeStamp Event detected, the time is saved! \r\n");
  BSP_Printf("%.2d:%.2d:%.2d \r\n", RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
  BSP_Printf("%.2d-%.2d-%.2d \r\n", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date);	
}

/**
  * @brief  Tamper event callback function
  * @param  RTC handle
  * @retval None
  */

void HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc)
{
  /* LED1 On: Tamper button pressed */
  BSP_Printf("Tamper1 Event detected, all the BKR will be reset! \r\n");
	if(Bsp_IsBackupRegReset() == 0)
	{
		BSP_Printf("\n\r RTC备份寄存器复位成功 \n\r");				/* 清除成功 */
	}
	else
	{
		BSP_Printf("\n\r RTC备份寄存器复位失败 \n\r");				/*清除失败 */
	}
	/*在进入中断后要关闭中断，否则会一直进中断*/
    __HAL_RTC_TAMPER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TAMP1F); 	/* 清除标志 */
	__HAL_RTC_TAMPER1_DISABLE(&RTC_Handler);					/* 禁止 Tamper 引脚1 */
	__HAL_RTC_TAMPER_DISABLE_IT(&RTC_Handler,RTC_TAMPER_1);		/* 禁止 Tamper 中断 */
}

/** @}
*/
/****************************RTC_Calendar Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------RTC_Calendar Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------RTC_Calendar-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
