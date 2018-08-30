/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/RDB_LCD/Coreboard_Lcd_Bsp.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 20-Junly-2018
  * @brief   The template of header file.
  *					 Please copy this file and rename the file name if needed.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __LCD_BSP_H
#define __LCD_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h7xx_hal.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup LCD
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup LCD_Private_Types LCD Private Define
  * @{
  */
/**
  * @brief  RGB_LCD BL Define
	* @note	  LCD背光PD13
  */
#define LCD_LED(n)	(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET))   
/**
  * @brief  MPU_LCD BL Define
	* @note	  LCD背光PB5
  */
	//LCD背光	PB5	
#define LCD_LED(n) (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)) 	    
/**
  * @brief  RGB_LCD Color Define
  */
#define LCD_PIXEL_FORMAT_ARGB8888       0X00    
#define LCD_PIXEL_FORMAT_RGB888         0X01    
#define LCD_PIXEL_FORMAT_RGB565         0X02       
#define LCD_PIXEL_FORMAT_ARGB1555       0X03      
#define LCD_PIXEL_FORMAT_ARGB4444       0X04     
#define LCD_PIXEL_FORMAT_L8             0X05     
#define LCD_PIXEL_FORMAT_AL44           0X06     
#define LCD_PIXEL_FORMAT_AL88           0X07 
/**
  * @brief  RGB_LCD Color Define
  */
//定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0X00000000	
/**
  * @brief  RGB_LCD Frame buffer address
  */
//LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR			0XC0000000  
/**
  * @brief LCD MPU protect parameters
  */
//LCD MPU保护参数
#define LCD_REGION_NUMBER		MPU_REGION_NUMBER0		//LCD使用region0
#define LCD_ADDRESS_START		(0X60000000)			//LCD区的首地址
#define LCD_REGION_SIZE			MPU_REGION_SIZE_256MB   //LCD区大小
/**
  * @brief MPU LCD address parameters
  */
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A18作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 			    
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
/**
  * @brief  LCD MPU LCD scan direction
  */	 
//扫描方向定义
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向
/**
  * @brief MPU LCD painter color
  */
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊
//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

/** @}
*/
/*----------------------------------------RGB_LCD Private Types---------------------------------------*/

/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/** @defgroup LCD_Private_Types LCD Private Types
  * @{
  */
/**
  * @brief  LCD Parameter definition
	* @note		LCD 重要参数集
  */
//LCD重要参数集
typedef struct  
{		 	 
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				//LCD ID
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t setxcmd;		//设置x坐标指令
	uint16_t setycmd;		//设置y坐标指令 
}_lcd_dev; 	  
/**
  * @brief  RGB_LCD LCD Parameter definition
	* @note		LCD 地址结构体
  */
//LCD地址结构体
typedef struct
{
	__IO uint16_t LCD_REG;
	__IO uint16_t LCD_RAM;
} LCD_TypeDef;
/**
  * @brief  RGB_LCD LTDC Parameter definition
	* @note		LCD LTDC重要参数集
  */
typedef struct  
{							 
	uint32_t pwidth;			//LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
	uint32_t pheight;		//LCD面板的高度,固定参数,不随显示方向改变
	uint16_t hsw;			//水平同步宽度
	uint16_t vsw;			//垂直同步宽度
	uint16_t hbp;			//水平后廊
	uint16_t vbp;			//垂直后廊
	uint16_t hfp;			//水平前廊
	uint16_t vfp;			//垂直前廊 
	uint8_t activelayer;		//当前层编号:0/1	
	uint8_t dir;				//0,竖屏;1,横屏;
	uint16_t width;			//LCD宽度
	uint16_t height;			//LCD高度
	uint32_t pixsize;		//每个像素所占字节数
}_ltdc_dev; 
/** @}
*/
/*----------------------------------------LCD Private Types---------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup LCD_Exported_Functions LCD Exported Functions
  * @{
  */
/** @defgroup LCD_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
void Bsp_LCD_Init(void);													  //初始化MPU_LCD
void Bsp_RGB_LCD_Init(void);                     		//初始化RGB_LCD
extern _ltdc_dev lcdltdc;		            				//管理LCD LTDC参数
extern LTDC_HandleTypeDef LTDC_Handler;	    		//LTDC句柄
extern DMA2D_HandleTypeDef DMA2D_Handler;   		//DMA2D句柄
extern uint32_t *ltdc_framebuf[2];							//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern uint32_t  POINT_COLOR;//默认红色    
extern uint32_t  BACK_COLOR; //背景颜色.默认为白色
/** @}
*/
/*********************LCD Exported Functions Group1***************************/
/** @defgroup LCD_Exported_Functions_Group2 Operation Functions
  * @{
  */
	/******MPU_LCD****/
void Bsp_Lcd_WR_REG(__IO uint16_t regval);
void Bsp_Lcd_WR_DATA(__IO uint16_t data);
uint16_t Bsp_Lcd_RD_DATA(void);
void Bsp_Lcd_Mpu_Config(void);
void Bsp_LCD_DisplayOn(void);													//开显示
void Bsp_LCD_DisplayOff(void);													//关显示
void Bsp_LCD_Clear(uint32_t Color);	 												//清屏
void Bsp_LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);										//设置光标
void Bsp_LCD_DrawPoint(uint16_t x,uint16_t y);											//画点
void Bsp_LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint32_t color);								//快速画点
uint32_t  Bsp_LCD_ReadPoint(uint16_t x,uint16_t y); 											//读点 
void Bsp_LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);						 			//画圆
void Bsp_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);							//画线
void Bsp_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   				//画矩形
void Bsp_LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);		   				//填充单色
void Bsp_LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);				//填充指定颜色
void Bsp_LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//显示一个字符
void Bsp_LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  						//显示一个数字
void Bsp_LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				//显示 数字
void Bsp_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//显示一个字符串,12/16字体

void Bsp_LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t Bsp_LCD_ReadReg(uint16_t LCD_Reg);
void Bsp_LCD_WriteRAM_Prepare(void);
void Bsp_LCD_WriteRAM(uint16_t RGB_Code);
void Bsp_LCD_SSD_BackLightSet(uint8_t pwm);							//SSD1963 背光控制
void Bsp_LCD_Scan_Dir(uint8_t dir);									//设置屏扫描方向
void Bsp_LCD_Display_Dir(uint8_t dir);								//设置屏幕显示方向
void Bsp_LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);	//设置窗口		

/******RGB_LCD*****/
void Bsp_LTDC_Switch(uint8_t sw);					//LTDC开关
void Bsp_LTDC_Layer_Switch(uint8_t layerx,uint8_t sw);	//层开关
void Bsp_LTDC_Select_Layer(uint8_t layerx);			//层选择
void Bsp_LTDC_Display_Dir(uint8_t dir);				//显示方向控制
void Bsp_LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color);//画点函数
uint32_t Bsp_LTDC_Read_Point(uint16_t x,uint16_t y);			//读点函数
void Bsp_LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);			//矩形单色填充函数
void Bsp_LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);	//矩形彩色填充函数
void Bsp_LTDC_Clear(uint32_t color);					//清屏函数
uint8_t Bsp_LTDC_Clk_Set(uint32_t pll3m,uint32_t pll3n,uint32_t pll3r);//LTDC时钟配置
void Bsp_LTDC_Layer_Window_Config(uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//LTDC层窗口设置
void Bsp_LTDC_Layer_Parameter_Config(uint8_t layerx,uint32_t bufaddr,uint8_t pixformat,uint8_t alpha,uint8_t alpha0,uint8_t bfac1,uint8_t bfac2,uint32_t bkcolor);//LTDC基本参数设置
uint16_t Bsp_LTDC_PanelID_Read(void);				//LCD ID读取函数
/** @}
*/
/**********************LCD Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------LCD Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------LCD------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __LCD_BSP_H */

/**********************************************END OF FILE******************************************/
