/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/Soft_Timer/Software_Timer_Bsp.c
  * @author  MCD Application Team
  * @version SW:V1.0.0 HW:V1.0
  * @date    21-August-2018
  * @brief   This file provides set of firmware functions to manage:
  *			 ����systick��ʱ����Ϊϵͳ�δ�ʱ����ȱʡ��ʱ����Ϊ1ms
  *			 ��������ʱ����������ʹ��(����1ms)�� ����ͨ���޸� TMR_COUNT ������ʱ������
  *			 ms�����ӳٺ���������1ms�� ��us���ӳٺ���
  *			 ϵͳ����ʱ�亯����1ms��λ��
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
#include "stm32h743ii_Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup Software_Timer Software_Timer
  * @brief ������������ժȡ.��Bsp_Delay_usΪ��,����Bsp_Delay_us(50),�ڸս��� Bsp_Delay_us��ʱ���ȼ�����������Ҫ�ȴ��� 
  *		   systick��������,����Ϊ50*400(����ϵͳʱ��Ϊ 400Mhz,��Ϊ�������� systick��Ƶ��Ϊϵͳʱ��,��ôsystickÿ����1��
  *        ����1/400us)��Ȼ�����Ǿ�һֱͳ��systick�ļ����仯,ֱ�����ֵ�仯��50*400,һ����⵽�仯��򳬹����ֵ����ô50usʱ�䵽�ˡ�
  *        ��������ֻ��ץȡSysTick�������ı仯,������Ҫ�޸ļ�����,������Ҫ�޸�SysTick���κ�״̬,��ȫ��Ӱ��̬,��ȫ��Ӱ��SysTick��ΪUCOSʱ�ӽ��ĵĹ���,
  *        �����ʵ��delay�Ͳ���ϵͳ����SysTick��ʱ����ԭ��.
  * @{
  */
/* Private Static Variable -------------------------------------------------------------------*/
/** @defgroup Software_Timer_Static_Variable Software_Timer Static Variable
  * @{
  */
static uint32_t fac_us=0;							//us��ʱ������
/* ��2��ȫ�ֱ���ת���� bsp_DelayMS() ���� */
/*��������Ϊ s_ucTimeOutFlag = 0�����Կ����Ż�������� s_ucTimeOutFlag ������������Ϊ volatile*/
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/* ���������ʱ���ṹ����� */
static SOFT_TMR s_tTmr[TMR_COUNT];

/*
	ȫ������ʱ�䣬��λ1ms
	����Ա�ʾ 24.85�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������
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
  * @param  SYSCLK: SYSCLKʱ��Ƶ��
  */
void Bsp_InitSoftTimer(uint16_t SYSCLK)
{
 	uint8_t i;

	/* �������е������ʱ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
	}
	/*  SysTick��ʱ��Դ��HCLK,���������ⲿ����Ϊ25M,Ȼ��Ƶ��400MHZ,��ôSysTick��ʱ�Ӽ�Ϊ400Mhz,SysTick�ļ������ļ�����VALÿ��1,�ʹ���ʱ��1/400us��
		���� fac_us=SYSCLK;��仰���Ǽ�����SYSCLKʱ��Ƶ������ʱ1us��Ҫ���ٸ�SysTickʱ�����ڡ�
		SysTick_Config() �������βα�ʾ�ں�ʱ�Ӷ��ٸ����ں󴥷�һ��Systick��ʱ�ж�.��� stm32h7xx_hal��
	    -- SystemCoreClock / 1000  ��ʾ��ʱƵ��Ϊ 1000Hz�� Ҳ���Ƕ�ʱ����Ϊ  1ms 
	    -- SystemCoreClock / 500   ��ʾ��ʱƵ��Ϊ 500Hz��  Ҳ���Ƕ�ʱ����Ϊ  2ms
	    -- SystemCoreClock / 2000  ��ʾ��ʱƵ��Ϊ 2000Hz�� Ҳ���Ƕ�ʱ����Ϊ  500us
		���ڳ����Ӧ�ã�����һ��ȡ��ʱ����1ms�����ڵ���CPU���ߵ͹���Ӧ��,�������ö�ʱ����Ϊ10ms
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
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
  * @brief 
  * @param param: brief.
  * @retval status
  */
/**
  * @brief  This function handles SysTick Handler. ÿ��1ms����1��
  */
void SysTick_Handler(void)
{
	static uint8_t s_count = 0;
	uint8_t i;
	/* ÿ��1ms����1�� �������� bsp_DelayMS�� */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
		{
			s_ucTimeOutFlag = 1;
		}
	}
	/* ÿ��1ms���������ʱ���ļ��������м�һ���� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		Bsp_SoftTimerDec(&s_tTmr[i]);
	}
	/* ȫ������ʱ��ÿ1ms��1 */
	g_iRunTime++;
	if (g_iRunTime == 0x7FFFFFFF)	/* ��������� int32_t ���ͣ������Ϊ 0x7FFFFFFF */
	{
		g_iRunTime = 0;
	}
	BSP_RunPer1ms();		/* ÿ��1ms����һ�δ˺������˺����� stm32h743ii_Coreboard_Bsp.c */
	
	if (++s_count >= 10)
	{
		s_count = 0;
		BSP_RunPer10ms();	/* ÿ��10ms����һ�δ˺������˺����� stm32h743ii_Coreboard_Bsp.c */
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
	uint32_t reload = SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks = nus * fac_us; 						//��Ҫ�Ľ����� 
	told = SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow < told)
			{
				tcnt += told - tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			}
			else 
			{
				tcnt += reload - tnow + told;
			}	    
			told = tnow;
			if(tcnt >= ticks)
			{
				break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
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

	DISABLE_INT();  			/* ���ж� */

	s_uiDelayCount = nms;
	s_ucTimeOutFlag = 0;

	ENABLE_INT();  				/* ���ж� */

	while (1)
	{
		BSP_Idle();				/* CPU����ִ�еĲ����� �� stm32h743ii_Coreboard_Bsp.c �� stm32h743ii_Coreboard_Bsp.h �ļ� */
		/*
			�ȴ��ӳ�ʱ�䵽
			ע�⣺��������Ϊ s_ucTimeOutFlag = 0�����Կ����Ż�������� s_ucTimeOutFlag ������������Ϊ volatile
		*/
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}	
}

/**
  * @brief  ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
  *	@param	 _tmr : ��ʱ������ָ��
  */
static void Bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0)
	{
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--_tmr->Count == 0)
		{
			_tmr->Flag = 1;

			/* ������Զ�ģʽ�����Զ���װ������ */
			if(_tmr->Mode == TMR_AUTO_MODE)
			{
				_tmr->Count = _tmr->PreLoad;
			}
		}
	}
}

/**
  * @brief  ����һ����ʱ���������ö�ʱ���ڡ�
  *	@param	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
  *			_period : ��ʱ���ڣ���λ1ms
  */
void Bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT();  			/* ���ж� */

	s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	ENABLE_INT();  				/* ���ж� */
}

/**
  * @brief  ����һ���Զ���ʱ���������ö�ʱ���ڡ�
  *	@param	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
  *			_period : ��ʱ���ڣ���λ1ms
  */
void Bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT();  		/* ���ж� */

	s_tTmr[_id].Count = _period;			/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */

	ENABLE_INT();  			/* ���ж� */
}

/**
  * @brief  ֹͣһ����ʱ����
  *	@param	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
  */
void Bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT();  	/* ���ж� */

	s_tTmr[_id].Count = 0;				/* ʵʱ��������ֵ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* �Զ�����ģʽ */

	ENABLE_INT();  		/* ���ж� */
}

/**
  * @brief  ��ⶨʱ���Ƿ�ʱ.
  *	@param	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
  *			_period : ��ʱ���ڣ���λ1ms
  * @return 0 ��ʾ��ʱδ���� 1��ʾ��ʱ��
*/
uint8_t Bsp_CheckTimer(uint8_t _id)
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
  * @brief  ��ȡCPU����ʱ�䣬��λ1ms������Ա�ʾ 24.85�죬�����Ĳ�Ʒ��������ʱ�䳬�������������뿼���������.
  *	@return CPU����ʱ�䣬��λ1ms
*/
int32_t Bsp_GetRunTime(void)
{
	int32_t runtime;

	DISABLE_INT();  	/* ���ж� */

	runtime = g_iRunTime;	/* ���������Systick�ж��б���д�������Ҫ���жϽ��б��� */

	ENABLE_INT();  		/* ���ж� */

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
