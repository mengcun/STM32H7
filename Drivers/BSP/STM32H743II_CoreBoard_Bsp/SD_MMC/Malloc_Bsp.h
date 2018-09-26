/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/MALLOC_C_H/MALLOC.h
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 14-Junly-2018
  * @brief   The template of header file.
  ******************************************************************************
**/ 

/* Define to prevent recursive inclusion ----------------------------------------------------------*/
#ifndef __MALLOC_BSP_H
#define __MALLOC_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "stm32h7xx_hal.h"
/*******************************************APP/BSP*************************************************/
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @addtogroup MALLOC
  * @{
  */ 
	 
/* Private constants ------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/** @defgroup MALLOC_Private_define MALLOC Private define
  * @{
  */
/**
  * @brief  MALLOC Macro definition
  */
#ifndef NULL
#define NULL 0
#endif

//���������ڴ��
#define SRAMIN	 	0		//AXI�ڴ��,AXI��512KB 
#define SRAMEX   	1		//�ⲿ�ڴ��(SDRAM),SDRAM��32MB
#define SRAM12		2		//SRAM1/2/3�ڴ��,SRAM1+SRAM2,��256KB
#define SRAM4		3		//SRAM4�ڴ��,SRAM4��64KB
#define SRAMDTCM 	4		//DTCM�ڴ��,DTCM��128KB,�˲����ڴ��CPU��MDMA(ͨ��AHBS)���Է���!!!!
#define SRAMITCM 	5		//ITCM�ڴ��,DTCM��64 KB,�˲����ڴ��CPU��MDMA(ͨ��AHBS)���Է���!!!!


#define SRAMBANK 	6		//����֧�ֵ�SRAM����.	


//mem1�ڴ�����趨.mem1��H7�ڲ���AXI�ڴ�.
#define MEM1_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM1_MAX_SIZE			448*1024  						//�������ڴ� 448K,H7��AXI�ڴ��ܹ�512KB
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С

//mem2�ڴ�����趨.mem2���ⲿ��SDRAM�ڴ�
#define MEM2_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE			28912 *1024  					//�������ڴ�28912K,����SDRAM�ܹ�32MB,LTDCռ��2MB,��ʣ30MB.
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С
		 
//mem3�ڴ�����趨.mem3��H7�ڲ���SRAM1+SRAM2�ڴ�
#define MEM3_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM3_MAX_SIZE			240 *1024  						//�������ڴ�240K,H7��SRAM1+SRAM2��256KB
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//�ڴ���С
		 
//mem4�ڴ�����趨.mem4��H7�ڲ���SRAM4�ڴ�
#define MEM4_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM4_MAX_SIZE			60 *1024  						//�������ڴ�60K,H7��SRAM4��64KB
#define MEM4_ALLOC_TABLE_SIZE	MEM4_MAX_SIZE/MEM4_BLOCK_SIZE 	//�ڴ���С
		 
//mem5�ڴ�����趨.mem5��H7�ڲ���DTCM�ڴ�,�˲����ڴ��CPU��MDMA���Է���!!!!!!
#define MEM5_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM5_MAX_SIZE			120 *1024  						//�������ڴ�120K,H7��DTCM��128KB
#define MEM5_ALLOC_TABLE_SIZE	MEM5_MAX_SIZE/MEM5_BLOCK_SIZE 	//�ڴ���С
		 
//mem6�ڴ�����趨.mem6��H7�ڲ���ITCM�ڴ�,�˲����ڴ��CPU��MDMA���Է���!!!!!!
#define MEM6_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM6_MAX_SIZE			60 *1024  						//�������ڴ�60K,H7��ITCM��64KB
#define MEM6_ALLOC_TABLE_SIZE	MEM6_MAX_SIZE/MEM6_BLOCK_SIZE 	//�ڴ���С
		 
/** @}
*/
/*----------------------------------------MALLOC Private define---------------------------------------*/
/* Private macros ---------------------------------------------------------------------------------*/
/* Private types ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
/** @defgroup MALLOC_Exported_variables MALLOC Exported variables
  * @{
  */
//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(uint8_t);							//��ʼ��
	uint16_t (*perused)(uint8_t);		  	    	//�ڴ�ʹ����
	uint8_t *membase[SRAMBANK];						//�ڴ�� ����SRAMBANK��������ڴ�
	uint32_t *memmap[SRAMBANK]; 					//�ڴ����״̬��
	uint8_t memrdy[SRAMBANK]; 						//�ڴ�����Ƿ����
};

extern struct _m_mallco_dev mallco_dev;	 			//��Mallco_Bsp.c���涨��

/** @}
*/
/*----------------------------------------MALLOC Exported variables---------------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/
/* Exported constants -----------------------------------------------------------------------------*/
/* Exported macro ---------------------------------------------------------------------------------*/
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup MALLOC_Exported_Functions MALLOC Exported Functions
  * @{
  */
/** @defgroup MALLOC_Exported_Functions_Group1 Initialization and de-initialization functions
	* @{
  */
/** @}
*/
/*********************MALLOC Exported Functions Group1***************************/
/** @defgroup MALLOC_Exported_Functions_Group2 Operation Functions
  * @{
  */
uint32_t Bsp_my_mem_malloc(uint8_t memx,uint32_t size);				//�ڴ����(�ڲ�����)
uint8_t Bsp_my_mem_free(uint8_t memx,uint32_t offset);				//�ڴ��ͷ�(�ڲ�����)
uint16_t Bsp_my_mem_perused(uint8_t memx) ;							//����ڴ�ʹ����(��/�ڲ�����) 

void Bsp_mymemset(void *s,uint8_t c,uint32_t count);				//�����ڴ�
void Bsp_mymemcpy(void *des,void *src,uint32_t n);					//�����ڴ�     
void Bsp_my_mem_init(uint8_t memx);									//�ڴ�����ʼ������(��/�ڲ�����)

void Bsp_myfree(uint8_t memx,void *ptr);  							//�ڴ��ͷ�(�ⲿ����)
void *Bsp_mymalloc(uint8_t memx,uint32_t size);						//�ڴ����(�ⲿ����)
void *Bsp_myrealloc(uint8_t memx,void *ptr,uint32_t size);			//���·����ڴ�(�ⲿ����)
/** @}
*/
/**********************MALLOC Exported Functions Group2**************************/
/** @}
*/
/*--------------------------------MALLOC Exported Functions--------------------------------------*/
/** @}
*/
/*-----------------------------------------MALLOC------------------------------------------------*/
/** @}
*/
/*-----------------------------------STM32H743II_CoreBoard-----------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __MALLOC_BSP_H */

/**********************************************END OF FILE******************************************/
