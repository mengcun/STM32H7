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
  
#define USMART_ENTIMX_SCAN 	1	//ʹӃTIM4Մ֨ʱ֐֏4ɨèSCANگʽ,ȧٻɨփΪ0,ШҪהܺʵϖشһ֎ʱܤɨèһԎscanگʽ,ҔʵϖԮߚʽޝԦ-
								//עҢ:ȧٻҪӃruntimeͳ܆٦Ĝ,јЫɨփUSMART_ENTIMX_SCANΪ1!!!!			
#define USMART_USE_HELP		1	//ʹӃЯֺì؃ֵɨΪ0ì߉Ҕݚʡݼ700ضזݚìիʇݫռւΞרϔʾЯֺЅϢc
#define USMART_USE_WRFUNS	1	//ʹӃցдگʽ,ʹĜբ/,߉ҔցȡȎڎ՘ַՄֵ,۹߉Ҕд܄ԦƷՄֵ.
 
  
#define MAX_FNAME_LEN 		30	//گʽû׮ԳӤֈìӦ؃ɨփΪһСӚ׮ӤگʽûՄӤֈc											   
#define MAX_PARM 			10	//׮ԳΪ10ضҎʽ ,О؄ԋҎʽ,јЫО؄usmart_exeӫ֮֔Ӧ.
#define PARM_LEN 			200	//˹ӐҎʽ֮ڍՄӤֈһӬٽPARM_LENضזݚ,עҢԮߚݓʕҿזҪӫ֮֔Ӧ(һСӚPARM_LEN)


#define USMART_OK 			0  //Ξԭγ
#define USMART_FUNCERR 		1  //گʽԭγ
#define USMART_PARMERR 		2  //Ҏʽԭγ
#define USMART_PARMOVER 	3  //ҎʽҧӶ
#define USMART_NOFUNCFIND 	4  //δՒսƥŤگʽ

#define SP_TYPE_DEC      	0  //10ݸֆҎʽϔʾ
#define SP_TYPE_HEX       	1  //16ݸֆҎʽϔʾ

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
  * @brief  USER_DEBUG گʽûPѭ
  */
  
struct _m_usmart_nametab
{
	void* func;			//گʽָի
	const uint8_t* name;		//گʽû(ҩՒԮ)	 
};

/**
  * @brief  usmartߘֆٜ-Ʒ
  */

struct _m_usmart_dev
{
	struct _m_usmart_nametab *funs;	//گʽûָի

	void (*init)(void);				//ӵʼۯ
	uint8_t (*cmd_rec)(uint8_t*str);	//ʶѰگʽûܰҎʽ
	void (*exe)(void); 					//ִА 
	void (*scan)(void);             	//ɨè
	uint8_t fnum; 				  		//گʽʽ
	uint8_t pnum;                       //Ҏʽʽ
	uint8_t id;							//گʽid
	uint8_t sptype;						//Ҏʽϔʾ Ѝ(ׇז׻ԮҎʽ):0,10ݸֆ;1,16ݸֆ;
	uint16_t parmtype;					//ҎʽՄ Ѝ
	uint8_t  plentbl[MAX_PARM];  		//ÿضҎʽՄӤֈԝԦѭ
	uint8_t  parm[PARM_LEN];  			//گʽՄҎʽ
	uint8_t runtimeflag;				//0,һͳ܆گʽִАʱܤ;1,ͳ܆گʽִАʱܤ,עҢ:ԋ٦ĜјЫԚUSMART_ENTIMX_SCANʹĜՄʱڲ,҅ӐӃ
	uint32_t runtime;					//ԋАʱܤ,եλ:0.1ms,׮ԳѓʱʱܤΪ֨ʱƷCNTֵՄ2Ѷ*0.1ms
};

/** @}
*/
/*----------------------------------------USER_DEBUG Exported types---------------------------------------*/

/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Exported_macro USER_DEBUG Exported macro
  * @{
  */
  
extern struct _m_usmart_nametab usmart_nametab[];	//Ԛusmart_config.c/æ֨ҥ
extern struct _m_usmart_dev usmart_dev;				//Ԛusmart_config.c/æ֨ҥ

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
void usmart_init(void);//ӵʼۯ
/** @}
*/
/*********************USER_DEBUG Exported Functions Group1***************************/
/** @defgroup USER_DEBUG_Exported_Functions_Group2 Operation Functions
  * @{
  */
uint8_t usmart_cmd_rec(uint8_t*str);	//ʶѰ
void usmart_exe(void);					//ִА
void usmart_scan(void);    				//ɨè
uint32_t read_addr(uint32_t addr);		//ցȡָ֨՘ַՄֵ
void write_addr(uint32_t addr,uint32_t val);//Ԛָ֨՘ַдȫָ֨Մֵ
uint32_t usmart_get_runtime(void);			//۱ȡԋАʱܤ
void usmart_reset_runtime(void);			//شλԋАʱܤ
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
