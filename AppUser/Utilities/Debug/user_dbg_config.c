/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/Utilities/Debug/user_dbg_config.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    31-August-2018
  * @brief   The USER_DEBUG of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __TEMPLATE_H
#define __TEMPLATE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "Coreboard_Bsp.h"
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
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Exported_types USER_DEBUG Exported types
  * @{
  */
/**
  * @brief  函数名列表初始化(用户自己添加)
  *			用户直接在这里输入要执行的函数名及其查找串
  */

struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"uint32_t read_addr(uint32_t addr)",
	(void*)write_addr,"void write_addr(uint32_t addr,uint32_t val)",	 
#endif		   
	(void*)Bsp_Delay_ms,"void Bsp_Delay_ms(uint16_t nms)",
 	(void*)Bsp_Delay_us,"void Bsp_Delay_us(uint32_t nus)",	 
	(void*)Bsp_LCD_Clear,"void Bsp_LCD_Clear(uint16_t Color)",
	(void*)Bsp_LCD_Fill,"void Bsp_LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)",
	(void*)Bsp_LCD_DrawLine,"void Bsp_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)",
	(void*)Bsp_LCD_DrawRectangle,"void Bsp_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)",
	(void*)Bsp_LCD_Draw_Circle,"void Bsp_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)",
	(void*)Bsp_LCD_ShowNum,"void Bsp_LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)",
	(void*)Bsp_LCD_ShowString,"void Bsp_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)",
	(void*)Bsp_LCD_Fast_DrawPoint,"void Bsp_LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)",
	(void*)Bsp_LCD_ReadPoint,"uint16_t Bsp_LCD_ReadPoint(uint16_t x,uint16_t y)",							 
	(void*)Bsp_LCD_Display_Dir,"void Bsp_LCD_Display_Dir(uint8_t dir)",
	(void*)Bsp_LCD_ShowxNum,"void Bsp_LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)", 

	(void*)Bsp_LTDC_Switch,"void Bsp_LTDC_Switch(uint8_t sw)",
	(void*)Bsp_LTDC_Layer_Switch,"void Bsp_LTDC_Layer_Switch(uint8_t layerx,uint8_t sw)", 
	(void*)Bsp_LTDC_Select_Layer,"void Bsp_LTDC_Select_Layer(uint8_t layerx)", 		
	(void*)Bsp_LTDC_Display_Dir,"void Bsp_LTDC_Display_Dir(uint8_t dir)", 		
	(void*)Bsp_LTDC_Draw_Point,"void Bsp_LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color)", 		
	(void*)Bsp_LTDC_Read_Point,"uint32_t Bsp_LTDC_Read_Point(uint16_t x,uint16_t y)", 		
	(void*)Bsp_LTDC_Fill,"void Bsp_LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)", 	
	(void*)Bsp_LTDC_Layer_Window_Config,"void Bsp_LTDC_Layer_Window_Config(uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)", 				
	(void*)Bsp_LTDC_Clear,"void Bsp_LTDC_Clear(uint32_t color)", 
		
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};

/** @}
*/
/*----------------------------------------USER_DEBUG Exported types---------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Exported_Functions USER_DEBUG Exported Functions
  * @{
  */
/** @defgroup USER_DEBUG_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
//CODE HERE
/** @}
*/
/*********************USER_DEBUG Exported Functions Group1***************************/
/** @defgroup USER_DEBUG_Exported_Functions_Group2 Operation Functions
  * @{
  */
//CODE HERE
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
