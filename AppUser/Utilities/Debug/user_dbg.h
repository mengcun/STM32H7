/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/Utilities/Debug/user_dbg.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 14-Junly-2018
  * @brief   The template of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __USER_DBG_H
#define __USER_DBG_H	  		  
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "user_dbg_str.h"
/*******************************************APP/BSP*************************************************/
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup USER_DEBUG
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Private_Defines USER_DEBUG Private Defines
  * @{
  */
/**
  * @brief USER_DEBUG Defines
  */
#define USMART_ENTIMX_SCAN 	1	//使用TIM的定时中断来扫描SCAN函数,如果设置为0,需要自己实现隔一段时间扫描一次scan函数.
								//注意:如果要用runtime统计功能,必须设置USMART_ENTIMX_SCAN为1!!!!
								
#define USMART_USE_HELP		1	//使用帮助，该值设为0，可以节省近700个字节，但是将导致无法显示帮助信息。
#define USMART_USE_WRFUNS	0	//使用读写函数,使能这里,可以读取任何地址的值,还可以写寄存器的值.为防止误操作，需要时开启
  
#define MAX_FNAME_LEN 		30	//函数名最大长度，应该设置为不小于最长函数名的长度。											   
#define MAX_PARM 			10	//最大为10个参数 ,修改此参数,必须修改usmart_exe与之对应.
#define PARM_LEN 			200	//所有参数之和的长度不超过PARM_LEN个字节,注意串口接收部分要与之对应(不小于PARM_LEN)


#define USMART_OK 			0  //无错误
#define USMART_FUNCERR 		1  //函数错误
#define USMART_PARMERR 		2  //参数错误
#define USMART_PARMOVER 	3  //参数溢出
#define USMART_NOFUNCFIND 	4  //未找到匹配函数

#define SP_TYPE_DEC      	0  //10进制参数显示
#define SP_TYPE_HEX       	1  //16进制参数显示


/** @}
*/
/*----------------------------------------USER_DEBUG Defines---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Exported_types USER_DEBUG Exported types
  * @{
  */
  
/**
  * @brief  USER_DEBUG Function list
  */
  
struct _m_usmart_nametab
{
	void* func;			//函数指针
	const uint8_t* name;		//函数名(查找串)	 
};

/**
  * @brief  usmart controler
  */

struct _m_usmart_dev
{
	struct _m_usmart_nametab *funs;	//函数名指针

	void (*init)(void);				//初始化
	uint8_t (*cmd_rec)(uint8_t*str);			//识别函数名及参数
	void (*exe)(void); 				//执行 
	void (*scan)(void);             //扫描
	uint8_t fnum; 				  		//函数数量
	uint8_t pnum;                        //参数数量
	uint8_t id;							//函数id
	uint8_t sptype;						//参数显示类型(非字符串参数):0,10进制;1,16进制;
	uint16_t parmtype;					//参数的类型
	uint8_t  plentbl[MAX_PARM];  		//每个参数的长度暂存表
	uint8_t  parm[PARM_LEN];  			//函数的参数
	uint8_t runtimeflag;					//0,不统计函数执行时间;1,统计函数执行时间,注意:此功能必须在USMART_ENTIMX_SCAN使能的时候,才有用
	uint32_t runtime;					//运行时间,单位:0.1ms,最大延时时间为定时器CNT值的2倍*0.1ms
};

/** @}
*/
/*----------------------------------------USER_DEBUG Exported types---------------------------------------*/

/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Exported_macro USER_DEBUG Exported macro
  * @{
  */
  
extern struct _m_usmart_nametab usmart_nametab[];	//Defined in user_dbg_config
extern struct _m_usmart_dev usmart_dev;				//Defined in user_dbg_config

/** @}
*/
/*----------------------------------------USER_DEBUG Exported macro---------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Exported_Functions USER_DEBUG Exported Functions
  * @{
  */
/** @defgroup USER_DEBUG_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
void usmart_init(void);
/** @}
*/
/*********************USER_DEBUG Exported Functions Group1***************************/
/** @defgroup USER_DEBUG_Exported_Functions_Group2 Operation Functions
  * @{
  */
uint8_t usmart_cmd_rec(uint8_t*str);	
void usmart_exe(void);			
void usmart_scan(void);    		
uint32_t read_addr(uint32_t addr);		
void write_addr(uint32_t addr,uint32_t val);
uint32_t usmart_get_runtime(void);			
void usmart_reset_runtime(void);	
/** @}
*/
/**********************USER_DEBUG Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------USER_DEBUG Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------USER_DEBUG------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __TEMPLATE_H */

/**********************************************END OF FILE******************************************/
