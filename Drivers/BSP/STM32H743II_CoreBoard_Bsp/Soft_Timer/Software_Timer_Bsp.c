/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/SOFT_TIMER/Software_Timer_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    21-August-2018
  * @brief   This file provides set of firmware functions to manage:
  *			 配置systick定时器作为系统滴答定时器。缺省定时周期为1ms
  *			 多个软件定时器供主程序使用(精度1ms)， 可以通过修改 TMR_COUNT 增减定时器个数
  *			 ms级别延迟函数（精度1ms） 和us级延迟函数
  *			 系统运行时间函数（1ms单位）
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The Software_Timer canbe used as follows:
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
/** @defgroup Software_Timer Software_Timer
  * @brief	Creat the Software timer by systick
  * @{
  */
// 我们利用是钟摘取.以Bsp_Delay_us为例,比如Bsp_Delay_us(50),在刚进入 Bsp_Delay_us的时候先计算好这段延需要等待的 
// systick计数次数,这里为50*400(假设系统时钟为 400Mhz,因为我们设置 systick的频率为系统时钟,那么systick每增加1，
// 就是1/400us)，然后我们就一直统计systick的计数变化,直到这个值变化了50*400,一旦检测到变化达或超过这个值，那么50us时间到了。
// 这样我们只是抓取SysTick计数器的变化,并不需要修改计数器,并不需要修改SysTick的任何状态,完全不影响态,完全不影响SysTick作为UCOS时钟节拍的功能,
// 这就是实现delay和操作系统共用SysTick定时器的原理.

/* Private Static Variable -------------------------------------------------------------------*/
/** @defgroup Software_Timer_Static_Variable Software_Timer Static Variable
  * @{
  */
static uint32_t fac_us=0;							//us延时倍乘数
/* 这2个全局变量转用于 bsp_DelayMS() 函数 */
/*编译器认为 s_ucTimeOutFlag = 0，所以可能优化错误，因此 s_ucTimeOutFlag 变量必须申明为 volatile*/
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* 定于软件定时器结构体变量 */
static SOFT_TMR s_tTmr[TMR_COUNT];

/*
	全局运行时间，单位1ms
	最长可以表示 24.85天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*/
__IO int32_t g_iRunTime = 0;

/** @}
*/	
/*----------------------------Software_Timer Static Variable--------------------------------*/ 
/* Private functions Prototypes -------------------------------------------------------------------*/
/** @defgroup Software_Timer_Private_Functions_Prototypes Software_Timer Private Functions Prototypes
  * @{
  */
static void Bsp_SoftTimerDec(SOFT_TMR *_tmr);

/** @}
*/		
/*----------------------------Software_Timer Private Functions Prototypes--------------------------------*/ 

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup Software_Timer_Private_Functions Software_Timer Private Functions
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
/*--------------------------------------Software_Timer Private Functions---------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup Software_Timer_Exported_Functions Software_Timer Exported Functions
  * @{
  */
/** @defgroup Software_Timer_Exported_Functions_Group1 Initialization and de-initialization functions
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
/**
  * @brief  This function is initial the Systick.
  * @param  SYSCLK: SYSCLK
  */
void Bsp_InitSoftTimer(uint16_t SYSCLK)
{
 	uint8_t i;

	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
	}
	/*  SysTick的时钟源自HCLK,假设我们外部晶振为25M,然后倍频到400MHZ,那么SysTick的时钟即为400Mhz,SysTick的计数器的计数器VAL每减1,就代表时间1/400us。
		所以 fac_us=SYSCLK;这句话就是计算在SYSCLK时钟频率下延时1us需要多少个SysTick时钟周期。
		SysTick_Config() 函数的形参表示内核时钟多少个周期后触发一次Systick定时中断.详见 stm32h7xx_hal中
	    -- SystemCoreClock / 1000  表示定时频率为 1000Hz， 也就是定时周期为  1ms 
	    -- SystemCoreClock / 500   表示定时频率为 500Hz，  也就是定时周期为  2ms
	    -- SystemCoreClock / 2000  表示定时频率为 2000Hz， 也就是定时周期为  500us
		对于常规的应用，我们一般取定时周期1ms。对于低速CPU或者低功耗应用,可以设置定时周期为10ms
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	fac_us=SYSCLK;	
}
/** @}
*/
/*********************Software_Timer Exported Functions Group1**************************/

/** @defgroup Software_Timer_Exported_Functions_Group2 Operation Functions
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
  * @brief  This function handles SysTick Handler. Every 1ms interrupt
  */
void SysTick_Handler(void)
{
	static uint8_t s_count = 0;
	uint8_t i;
	/* 每隔1ms进来1次 （仅用于 bsp_DelayMS） */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
		{
			s_ucTimeOutFlag = 1;
		}
	}
	/* 每隔1ms，对软件定时器的计数器进行减一操作 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		Bsp_SoftTimerDec(&s_tTmr[i]);
	}
	/* 全局运行时间每1ms增1 */
	g_iRunTime++;
	if (g_iRunTime == 0x7FFFFFFF)	/* 这个变量是 int32_t 类型，最大数为 0x7FFFFFFF */
	{
		g_iRunTime = 0;
	}
	BSP_RunPer1ms();		/* 每隔1ms调用一次此函数，此函数在 Coreboard_Bsp.c */
	
	if (++s_count >= 10)
	{
		s_count = 0;
		BSP_RunPer10ms();	/* 每隔10ms调用一次此函数，此函数在 Coreboard_Bsp.c */
	}	
}
/**
  * @brief  This function is delay for nus, and the nus must be less then 1000.
  * @param  nus: The time need to be delayed
  */
void Bsp_Delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told;
	uint32_t tnow;
	uint32_t tcnt=0;
	uint32_t reload = SysTick->LOAD;				//LOAD的值	    	 
	ticks = nus * fac_us; 						//需要的节拍数 
	told = SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow < told)
			{
				tcnt += told - tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			}
			else 
			{
				tcnt += reload - tnow + told;
			}	    
			told = tnow;
			if(tcnt >= ticks)
			{
				break;			//时间超过/等于要延迟的时间,则退出.
			}
		}  
	};
}

/**
  * @brief  This function is delay for nms.
  * @param  nms: The time need to be delayed
  */
void Bsp_Delay_ms(uint32_t nms)
{
//	uint32_t i;
//	for(i=0;i<nms;i++)
//		Bsp_Delay_us(1000);
	if (nms == 0)
	{
		return;
	}
	else if (nms == 1)
	{
		nms = 2;
	}

	DISABLE_INT();  			/* 关中断 */

	s_uiDelayCount = nms;
	s_ucTimeOutFlag = 0;

	ENABLE_INT();  				/* 开中断 */

	while (1)
	{
		BSP_Idle();				/* CPU空闲执行的操作， 见 Coreboard_Bsp.c 和 Coreboard_Bsp.h 文件 */
		/*
			等待延迟时间到
			注意：编译器认为 s_ucTimeOutFlag = 0，所以可能优化错误，因此 s_ucTimeOutFlag 变量必须申明为 volatile
		*/
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}	
}

/**
  * @brief  The _tmr will decrease 1 ervery 1ms, this should be called in SysTick_ISR period
  *	@param	 _tmr : The pointer for Timer Variables
  */
//每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
static void Bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* 如果是自动模式，则自动重装计数器 */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

/**
  * @brief  Start a software Timer once with the Period
  *	@param	_id     : Timer ID，should be [0,TMR_COUNT-1]。
 *	@param	_period : timerout period (ms)
  */

//启动一个一次性定时器，并设置定时周期。用户必须自行维护定时器ID，以避免定时器ID冲突。
void Bsp_StartSoftwareTimerOnce(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		Bsp_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT();  			/* 关中断 */

	s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	ENABLE_INT();  				/* 开中断 */
}

/**
  * @brief  Start a software Timer auto reload with the Period
  *	@param	_id     : Timer ID，should be [0,TMR_COUNT-1]。
 *	@param	_period : timerout period (ms)
  */
//启动一个自动重装定时器，并设置定时周期。用户必须自行维护定时器ID，以避免定时器ID冲突。
void Bsp_StartSoftwareTimerAuto(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		Bsp_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT();  		/* 关中断 */

	s_tTmr[_id].Count = _period;			/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */

	ENABLE_INT();  			/* 开中断 */
}

/**
  * @brief  Stop the software timer
  *	@param	_id     : Timer ID，should be [0,TMR_COUNT-1]。
  */
void Bsp_StopSoftwareTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		Bsp_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT();  	/* 关中断 */

	s_tTmr[_id].Count = 0;				/* 实时计数器初值 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 自动工作模式 */

	ENABLE_INT();  		/* 开中断 */
}

/**
  * @brief  Check whether is timeout or not.
  *	@param	_id  : Timer ID，should be [0,TMR_COUNT-1]。
  * @return 0: Time no out; 1: Timeout
*/
uint8_t Bsp_CheckSoftwareTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (s_tTmr[_id].Flag == 1)
	{
		s_tTmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
  * @brief  Get the CPU  run time (ms), the longest time is 24.85 days
  *	@return CPU run time (1ms)
*/

//获取CPU运行时间，单位1ms。最长可以表示 24.85天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题.
int32_t Bsp_GetCPURunTime(void)
{
	int32_t runtime;

	DISABLE_INT();  	/* 关中断 */

	runtime = g_iRunTime;	/* 这个变量在Systick中断中被改写，因此需要关中断进行保护 */

	ENABLE_INT();  		/* 开中断 */

	return runtime;
}

/** @}
*/
/****************************XXXX Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------Software_Timer Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------Software_Timer-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
