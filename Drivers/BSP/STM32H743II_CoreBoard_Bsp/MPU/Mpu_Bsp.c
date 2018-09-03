/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Drivers/BSP/STM32H743II_CoreBoard_Bsp/MPU/Mpu_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    14-August-2018
  * @brief   This file provides set of firmware functions to protect mpu
  *					 
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The Template canbe used as follows:
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
/** @defgroup MPU MPU
  * @brief MPU
	* @note	MPU
  * @{
  */
/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup MPU_Exported_Functions MPU Exported Functions
  * @{
  */
/** @defgroup MPU_Exported_Functions_Group2 Operation Functions
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
  * @brief	Set the MPU protect zone
  * @param 	baseaddr: The base address of MPU protect 
  * @param 	size: The size of MPU protect
  * @param 	rnum: The number of MPU protect zone
  * @param 	ap: CORTEX_MPU_Region_Permission_Attributes
  * @retval 0: sucess.
  */

//	baseaddr:MPU��������Ļ�ַ(�׵�ַ)
//  size:MPU��������Ĵ�С(������32�ı���,��λΪ�ֽ�),�����õ�ֵ�ο�:CORTEX_MPU_Region_Size
//  rnum:MPU���������,��Χ:0~15,���֧��16����������,�����õ�ֵ�ο���CORTEX_MPU_Region_Number
//  ap:����Ȩ��,���ʹ�ϵ����:�����õ�ֵ�ο���CORTEX_MPU_Region_Permission_Attributes
// 
//MPU_REGION_NO_ACCESS,�޷��ʣ���Ȩ&�û������ɷ��ʣ�
//MPU_REGION_PRIV_RW,��֧����Ȩ��д����
//MPU_REGION_PRIV_RW_URO,��ֹ�û�д���ʣ���Ȩ�ɶ�д���ʣ�
//MPU_REGION_FULL_ACCESS,ȫ���ʣ���Ȩ&�û����ɷ��ʣ�
//MPU_REGION_PRIV_RO,��֧����Ȩ������
//MPU_REGION_PRIV_RO_URO,ֻ������Ȩ&�û���������д��
//���:STM32F7 Series Cortex-M7 processor programming manual.pdf,4.6��,Table 89.
uint8_t Bsp_Mpu_Set_Protection(uint32_t baseaddr,uint32_t size,uint32_t rnum,uint32_t ap)
{
	MPU_Region_InitTypeDef MPU_Initure;
	
	HAL_MPU_Disable();								        //����MPU֮ǰ�ȹر�MPU,��������Ժ���ʹ��MPU

	MPU_Initure.Enable=MPU_REGION_ENABLE;			        //ʹ�ܸñ������� 
	MPU_Initure.Number=rnum;			                    //���ñ�������
	MPU_Initure.BaseAddress=baseaddr;	                    //���û�ַ
	MPU_Initure.Size=size;				                    //���ñ��������С
	MPU_Initure.SubRegionDisable=0X00;                      //��ֹ������
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;                //����������չ��Ϊlevel0
	MPU_Initure.AccessPermission=(uint8_t)ap;		        //���÷���Ȩ��,
	MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;	//����ָ�����(�����ȡָ��)
	MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;       //��ֹ����
	MPU_Initure.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;       //��ֹcache  
	MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;         //������
	HAL_MPU_ConfigRegion(&MPU_Initure);                     //����MPU
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);			        //����MPU
    return 0;
}

/**
  * @brief	Set the MPU protect for Memory
  */

//������Ҫ�����Ĵ洢��
//����Բ��ִ洢�������MPU����,������ܵ��³��������쳣
//����MCU������ʾ,����ͷ�ɼ����ݳ���ȵ�����...
void Bsp_MPU_Memory_Protection(void)
{
	Bsp_Mpu_Set_Protection(0x60000000,MPU_REGION_SIZE_64MB,MPU_REGION_NUMBER0,MPU_REGION_FULL_ACCESS);		//����MCU LCD�����ڵ�FMC����,,��64M�ֽ�
	Bsp_Mpu_Set_Protection(0x20000000,MPU_REGION_SIZE_512KB,MPU_REGION_NUMBER1,MPU_REGION_FULL_ACCESS);		//���������ڲ�SRAM,����SRAM1,SRAM2��DTCM,��512K�ֽ�
	Bsp_Mpu_Set_Protection(0XC0000000,MPU_REGION_SIZE_32MB,MPU_REGION_NUMBER2,MPU_REGION_FULL_ACCESS);		//����SDRAM����,��32M�ֽ�
	Bsp_Mpu_Set_Protection(0X80000000,MPU_REGION_SIZE_256MB,MPU_REGION_NUMBER3,MPU_REGION_FULL_ACCESS);		//��������NAND FLASH����,��256M�ֽ�
}
/** @}
*/
/****************************MPU Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------MPU Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------MPU-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
