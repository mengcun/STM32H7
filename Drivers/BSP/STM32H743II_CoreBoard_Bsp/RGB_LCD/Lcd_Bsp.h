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
	* @note	  LCD����PD13
  */
#define LCD_LED(n)	(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET))   
/**
  * @brief  MPU_LCD BL Define
	* @note	  LCD����PB5
  */
	//LCD����	PB5	
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
//������ɫ���ظ�ʽ,һ����RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define LTDC_BACKLAYERCOLOR			0X00000000	
/**
  * @brief  RGB_LCD Frame buffer address
  */
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			0XC0000000  
/**
  * @brief LCD MPU protect parameters
  */
//LCD MPU��������
#define LCD_REGION_NUMBER		MPU_REGION_NUMBER0		//LCDʹ��region0
#define LCD_ADDRESS_START		(0X60000000)			//LCD�����׵�ַ
#define LCD_REGION_SIZE			MPU_REGION_SIZE_256MB   //LCD����С
/**
  * @brief MPU LCD address parameters
  */
//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A18��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 			    
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
/**
  * @brief  LCD MPU LCD scan direction
  */	 
//ɨ�跽����
#define L2R_U2D  0 		//������,���ϵ���
#define L2R_D2U  1 		//������,���µ���
#define R2L_U2D  2 		//���ҵ���,���ϵ���
#define R2L_D2U  3 		//���ҵ���,���µ���

#define U2D_L2R  4 		//���ϵ���,������
#define U2D_R2L  5 		//���ϵ���,���ҵ���
#define D2U_L2R  6 		//���µ���,������
#define D2U_R2L  7		//���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��
/**
  * @brief MPU LCD painter color
  */
//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

//LCD�ֱ�������
#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH		1		//ˮƽ����
#define SSD_HOR_BACK_PORCH		46		//ˮƽǰ��
#define SSD_HOR_FRONT_PORCH		210		//ˮƽ����

#define SSD_VER_PULSE_WIDTH		1		//��ֱ����
#define SSD_VER_BACK_PORCH		23		//��ֱǰ��
#define SSD_VER_FRONT_PORCH		22		//��ֱǰ��
//���¼����������Զ�����
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
	* @note		LCD ��Ҫ������
  */
//LCD��Ҫ������
typedef struct  
{		 	 
	uint16_t width;			//LCD ���
	uint16_t height;			//LCD �߶�
	uint16_t id;				//LCD ID
	uint8_t  dir;			//���������������ƣ�0��������1��������	
	uint16_t	wramcmd;		//��ʼдgramָ��
	uint16_t setxcmd;		//����x����ָ��
	uint16_t setycmd;		//����y����ָ�� 
}_lcd_dev; 	  
/**
  * @brief  RGB_LCD LCD Parameter definition
	* @note		LCD ��ַ�ṹ��
  */
//LCD��ַ�ṹ��
typedef struct
{
	__IO uint16_t LCD_REG;
	__IO uint16_t LCD_RAM;
} LCD_TypeDef;
/**
  * @brief  RGB_LCD LTDC Parameter definition
	* @note		LCD LTDC��Ҫ������
  */
typedef struct  
{							 
	uint32_t pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	uint32_t pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	uint16_t hsw;			//ˮƽͬ�����
	uint16_t vsw;			//��ֱͬ�����
	uint16_t hbp;			//ˮƽ����
	uint16_t vbp;			//��ֱ����
	uint16_t hfp;			//ˮƽǰ��
	uint16_t vfp;			//��ֱǰ�� 
	uint8_t activelayer;		//��ǰ����:0/1	
	uint8_t dir;				//0,����;1,����;
	uint16_t width;			//LCD���
	uint16_t height;			//LCD�߶�
	uint32_t pixsize;		//ÿ��������ռ�ֽ���
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
void Bsp_LCD_Init(void);													  //��ʼ��MPU_LCD
void Bsp_RGB_LCD_Init(void);                     		//��ʼ��RGB_LCD
extern _ltdc_dev lcdltdc;		            				//����LCD LTDC����
extern LTDC_HandleTypeDef LTDC_Handler;	    		//LTDC���
extern DMA2D_HandleTypeDef DMA2D_Handler;   		//DMA2D���
extern uint32_t *ltdc_framebuf[2];							//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint32_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint32_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
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
void Bsp_LCD_DisplayOn(void);													//����ʾ
void Bsp_LCD_DisplayOff(void);													//����ʾ
void Bsp_LCD_Clear(uint32_t Color);	 												//����
void Bsp_LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);										//���ù��
void Bsp_LCD_DrawPoint(uint16_t x,uint16_t y);											//����
void Bsp_LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint32_t color);								//���ٻ���
uint32_t  Bsp_LCD_ReadPoint(uint16_t x,uint16_t y); 											//���� 
void Bsp_LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);						 			//��Բ
void Bsp_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);							//����
void Bsp_LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   				//������
void Bsp_LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);		   				//��䵥ɫ
void Bsp_LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);				//���ָ����ɫ
void Bsp_LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//��ʾһ���ַ�
void Bsp_LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  						//��ʾһ������
void Bsp_LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				//��ʾ ����
void Bsp_LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//��ʾһ���ַ���,12/16����

void Bsp_LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t Bsp_LCD_ReadReg(uint16_t LCD_Reg);
void Bsp_LCD_WriteRAM_Prepare(void);
void Bsp_LCD_WriteRAM(uint16_t RGB_Code);
void Bsp_LCD_SSD_BackLightSet(uint8_t pwm);							//SSD1963 �������
void Bsp_LCD_Scan_Dir(uint8_t dir);									//������ɨ�跽��
void Bsp_LCD_Display_Dir(uint8_t dir);								//������Ļ��ʾ����
void Bsp_LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);	//���ô���		

/******RGB_LCD*****/
void Bsp_LTDC_Switch(uint8_t sw);					//LTDC����
void Bsp_LTDC_Layer_Switch(uint8_t layerx,uint8_t sw);	//�㿪��
void Bsp_LTDC_Select_Layer(uint8_t layerx);			//��ѡ��
void Bsp_LTDC_Display_Dir(uint8_t dir);				//��ʾ�������
void Bsp_LTDC_Draw_Point(uint16_t x,uint16_t y,uint32_t color);//���㺯��
uint32_t Bsp_LTDC_Read_Point(uint16_t x,uint16_t y);			//���㺯��
void Bsp_LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);			//���ε�ɫ��亯��
void Bsp_LTDC_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);	//���β�ɫ��亯��
void Bsp_LTDC_Clear(uint32_t color);					//��������
uint8_t Bsp_LTDC_Clk_Set(uint32_t pll3m,uint32_t pll3n,uint32_t pll3r);//LTDCʱ������
void Bsp_LTDC_Layer_Window_Config(uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);//LTDC�㴰������
void Bsp_LTDC_Layer_Parameter_Config(uint8_t layerx,uint32_t bufaddr,uint8_t pixformat,uint8_t alpha,uint8_t alpha0,uint8_t bfac1,uint8_t bfac2,uint32_t bkcolor);//LTDC������������
uint16_t Bsp_LTDC_PanelID_Read(void);				//LCD ID��ȡ����
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
