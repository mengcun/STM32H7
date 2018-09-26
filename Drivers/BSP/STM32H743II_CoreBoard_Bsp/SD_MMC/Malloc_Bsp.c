/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/MALLOC/Adc_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date  	 26-September-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The MALLOC canbe used as follows:
	(#)...
		(++)...
				(+++)...
  @endverbatim
  */

/* Includes ---------------------------------------------------------------------------------------*/
/***************************************Include StdLib**********************************************/
#include "stdint.h"
/*******************************************APP/BSP*************************************************/
#include "Coreboard_Bsp.h"
/********************************************Macro**************************************************/
/**********************************************OS***************************************************/
/********************************************STwin**************************************************/
/********************************************FatFS**************************************************/

/** @addtogroup STM32H743II_CoreBoard
  * @{
  */
/** @defgroup MALLOC MALLOC
  * @brief MALLOC
	* @note	MALLOC
  * @{
  */
/* Private variables -------------------------------------------------------------------*/
/** @defgroup DAC_LFSR_NOISE_Private_c DAC_LFSR_NOISE Private variables
  * @{
  */
/**
  * @brief �ڴ��(64�ֽڶ���)
  */  
__align(64) uint8_t mem1base[MEM1_MAX_SIZE];													//�ڲ�SRAM�ڴ��
__align(64) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((at(0XC01F4000)));					//�ⲿSDRAM�ڴ��,ǰ��2M��LTDC����(1280*800*2)
__align(64) uint8_t mem3base[MEM3_MAX_SIZE] __attribute__((at(0x30000000)));					//�ڲ�SRAM1+SRAM2�ڴ��
__align(64) uint8_t mem4base[MEM4_MAX_SIZE] __attribute__((at(0x38000000)));					//�ڲ�SRAM4�ڴ��
__align(64) uint8_t mem5base[MEM5_MAX_SIZE] __attribute__((at(0x20000000)));					//�ڲ�DTCM�ڴ��
__align(64) uint8_t mem6base[MEM6_MAX_SIZE] __attribute__((at(0x00000000)));					//�ڲ�ITCM�ڴ�� 

/**
  * @brief �ڴ�����
  */
uint32_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//�ڲ�SRAM�ڴ��MAP
uint32_t mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0XC01F4000+MEM2_MAX_SIZE)));		//�ⲿSDRAM�ڴ��MAP
uint32_t mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0x30000000+MEM3_MAX_SIZE)));		//�ڲ�SRAM1+SRAM2�ڴ��MAP
uint32_t mem4mapbase[MEM4_ALLOC_TABLE_SIZE] __attribute__((at(0x38000000+MEM4_MAX_SIZE)));		//�ڲ�SRAM4�ڴ��MAP
uint32_t mem5mapbase[MEM5_ALLOC_TABLE_SIZE] __attribute__((at(0x20000000+MEM5_MAX_SIZE)));		//�ڲ�DTCM�ڴ��MAP
uint32_t mem6mapbase[MEM6_ALLOC_TABLE_SIZE] __attribute__((at(0x00000000+MEM6_MAX_SIZE)));		//�ڲ�ITCM�ڴ��MAP 

/**
  * @brief �ڴ�������
  */	   
const uint32_t memtblsize[SRAMBANK]={	MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE,
										MEM4_ALLOC_TABLE_SIZE,MEM5_ALLOC_TABLE_SIZE,MEM6_ALLOC_TABLE_SIZE};	//�ڴ���С
const uint32_t memblksize[SRAMBANK]={	MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE,
										MEM4_BLOCK_SIZE,MEM5_BLOCK_SIZE,MEM6_BLOCK_SIZE};					//�ڴ�ֿ��С
const uint32_t memsize[SRAMBANK]={		MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE,
										MEM4_MAX_SIZE,MEM5_MAX_SIZE,MEM6_MAX_SIZE};							//�ڴ��ܴ�С

/**
  * @brief �ڴ���������
  */
struct _m_mallco_dev mallco_dev=
{
	Bsp_my_mem_init,															//�ڴ��ʼ��
	Bsp_my_mem_perused,															//�ڴ�ʹ����
	mem1base,mem2base,mem3base,mem4base,mem5base,mem6base,						//�ڴ��
	mem1mapbase,mem2mapbase,mem3mapbase,mem4mapbase,mem5mapbase,mem6mapbase,	//�ڴ����״̬��
	0,0,0,0,0,0,								  		 						//�ڴ����δ����
};
										
/** @}
*/		
/*----------------------------ADC Private variables--------------------------------*/ 
  
/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup MALLOC_Exported_Functions MALLOC Exported Functions
  * @{
  */
/** @defgroup MALLOC_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize MALLOC
  @endverbatim
  * @{
  */
/** @}
*/
/*********************MALLOC Exported Functions Group1**************************/

/** @defgroup MALLOC_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the MALLOC
  @endverbatim
  * @{
  */
/**
  * @brief 	�����ڴ���Ϣ
  * @param 	*des:Ŀ�ĵ�ַ
  * @param 	*src:Դ��ַ
  * @param 	n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
  */

void Bsp_mymemcpy(void *des,void *src,uint32_t n)  
{  
    uint8_t *xdes=des;
	uint8_t *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  

/**
  * @brief 	�����ڴ�
  * @param 	*s:�ڴ��׵�ַ
  * @param 	c :Ҫ���õ�ֵ
  * @param 	count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
  */

void Bsp_mymemset(void *s,uint8_t c,uint32_t count)  
{  
    uint8_t *xs = s;  
    while(count--)*xs++=c;  
}	

/**
  * @brief 	�ڴ�����ʼ��
  * @param 	memx:�����ڴ��
  */

void Bsp_my_mem_init(uint8_t memx)  
{  
    Bsp_mymemset(mallco_dev.memmap[memx],0,memtblsize[memx]*4);	//�ڴ�״̬����������  
 	mallco_dev.memrdy[memx]=1;									//�ڴ�����ʼ��OK  
}  

/**
  * @brief 	��ȡ�ڴ�ʹ����
  * @param 	memx:�����ڴ��
  * @return ʹ����(������10��,0~1000,����0.0%~100.0%)
  */

uint16_t Bsp_my_mem_perused(uint8_t memx)  
{  
    uint32_t used=0;  
    uint32_t i;  
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(mallco_dev.memmap[memx][i])used++; 
    } 
    return (used*1000)/(memtblsize[memx]);  
} 

/**
  * @brief 	�ڴ����(�ڲ�����)
  * @param 	memx:�����ڴ��
  * @param 	size:Ҫ������ڴ��С(�ֽ�)
  * @return 0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ
  */
 
uint32_t Bsp_my_mem_malloc(uint8_t memx,uint32_t size)  
{  
    signed long offset=0;  
    uint32_t nmemb;	//��Ҫ���ڴ����  
	uint32_t cmemb=0;//�������ڴ����
    uint32_t i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//δ��ʼ��,��ִ�г�ʼ�� 
    if(size==0)return 0XFFFFFFFF;//����Ҫ����
    nmemb=size/memblksize[memx];  	//��ȡ��Ҫ����������ڴ����
    if(size%memblksize[memx])nmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//���������ڴ������  
    {     
		if(!mallco_dev.memmap[memx][offset])cmemb++;//�������ڴ��������
		else cmemb=0;								//�����ڴ������
		if(cmemb==nmemb)							//�ҵ�������nmemb�����ڴ��
		{
            for(i=0;i<nmemb;i++)  					//��ע�ڴ��ǿ� 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*memblksize[memx]);//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  

/**
  * @brief 	�ͷ��ڴ�(�ڲ�����) 
  * @param 	memx:�����ڴ��
  * @param 	ffset:�ڴ��ַƫ��
  * @return 0,�ͷųɹ�;1,�ͷ�ʧ��;
  */
 
uint8_t Bsp_my_mem_free(uint8_t memx,uint32_t offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
	{
		mallco_dev.init(memx);    
        return 1;//δ��ʼ��  
    }  
    if(offset<memsize[memx])//ƫ�����ڴ����. 
    {  
        int index=offset/memblksize[memx];			//ƫ�������ڴ�����  
        int nmemb=mallco_dev.memmap[memx][index];	//�ڴ������
        for(i=0;i<nmemb;i++)  						//�ڴ������
        {  
            mallco_dev.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}  

/**
  * @brief 	�ͷ��ڴ�(�ⲿ����) 
  * @param 	memx:�����ڴ��
  * @param 	ptr:�ڴ��׵�ַ 
  */
 
void Bsp_myfree(uint8_t memx,void *ptr)  
{  
	uint32_t offset;   
	if(ptr==NULL)return;//��ַΪ0.  
 	offset=(uint32_t)ptr-(uint32_t)mallco_dev.membase[memx];     
    Bsp_my_mem_free(memx,offset);	//�ͷ��ڴ�      
}  

/**
  * @brief  �����ڴ�(�ⲿ����) 
  * @param 	size:�ڴ��С(�ֽ�)
  * @param 	���䵽���ڴ��׵�ַ.
  */
 
void *Bsp_mymalloc(uint8_t memx,uint32_t size)  
{  
    uint32_t offset;   
	offset=Bsp_my_mem_malloc(memx,size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  
}  

/**
  * @brief  ���·����ڴ�(�ⲿ����)
  * @param 	memx:�����ڴ��
  * @param 	*ptr:���ڴ��׵�ַ
  * @param 	size:Ҫ������ڴ��С(�ֽ�)
  * @return �·��䵽���ڴ��׵�ַ.
  */
 
void *Bsp_myrealloc(uint8_t memx,void *ptr,uint32_t size)  
{  
    uint32_t offset;    
    offset=Bsp_my_mem_malloc(memx,size);   	
    if(offset==0XFFFFFFFF)
	{
		return NULL;     
	}
    else  
    {  									   
	    Bsp_mymemcpy((void*)((uint32_t)mallco_dev.membase[memx]+offset),ptr,size);	//�������ڴ����ݵ����ڴ�   
        Bsp_myfree(memx,ptr);  											  		//�ͷž��ڴ�
        return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  				//�������ڴ��׵�ַ
    }  
}
/** @}
*/
/****************************MALLOC Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------MALLOC Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------MALLOC-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
