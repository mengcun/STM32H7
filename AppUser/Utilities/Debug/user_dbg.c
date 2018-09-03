/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/Utilities/Debug/user_dbg.c
  * @author  MCD Application Team
  * @version SW:V1.0.0 HW:V1.0
  * @date    31-August-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The USER_DEBUG canbe used as follows:
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
/** @defgroup USER_DEBUG USER_DEBUG
  * @brief The USER_DEBUG allows users to modify function parameters without reflash
  * @{
  */

/** @defgroup USER_DEBUG_Private_Variables USER_DEBUG Private Variables
  * @{
  */
/**
  * @brief	USER_DEBUG ϵͳ����
  */
  
uint8_t *sys_cmd_tab[] =
{
	"?",
	"help",
	"list",
	"id",
	"hex",
	"dec",
	"runtime",	   
};	   
/** @}
*/
/*--------------------------------------USER_DEBUG Private Variables---------------------------------*/

/* Private functions ------------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Private_Functions USER_DEBUG Private Functions
  * @{
  */
/**
  * @brief ����ϵͳָ��
  * @param *str: ������.
  * @retval 0,�ɹ�����;����,�������
  */

uint8_t usmart_sys_cmd_exe(uint8_t *str)
{
	uint8_t i;
	uint8_t sfname[MAX_FNAME_LEN];//��ű��غ�����
	uint8_t pnum;
	uint8_t rval;
	uint32_t res;  
	res=usmart_get_cmdname(str,sfname,&i,MAX_FNAME_LEN);//�õ�ָ�ָ���
	if(res)return USMART_FUNCERR;//�����ָ�� 
	str+=i;	 	 			    
	for(i=0;i<sizeof(sys_cmd_tab)/4;i++)//֧�ֵ�ϵͳָ��
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i])==0)break;
	}
	switch(i)
	{					   
		case 0:
		case 1://����ָ��
			Bsp_Printf("\r\n");
#if USMART_USE_HELP 
			Bsp_Printf("------------------------USER_DEBUG V1.0------------------------ \r\n");
			Bsp_Printf("USER_DEBUG�Ǵ�ALIENTEK��ֲ������һ�����ɵĴ��ڵ��Ի������,ͨ�� \r\n");
			Bsp_Printf("��,�����ͨ���������ֵ��ó���������κκ���,��ִ��.���,���\r\n");
			Bsp_Printf("��������ĺ������������(֧������(10/16����,֧�ָ���)���ַ���\r\n"),
			Bsp_Printf("��������ڵ�ַ����Ϊ����),�����������֧��10���������,��֧��\r\n"),  
			Bsp_Printf("��������ֵ��ʾ.֧�ֲ�����ʾ�������ù���,֧�ֽ���ת������.\r\n");
			Bsp_Printf("USMART��7��ϵͳ����(����Сд):\r\n");
			Bsp_Printf("?:      ��ȡ������Ϣ\r\n");
			Bsp_Printf("help:   ��ȡ������Ϣ\r\n");
			Bsp_Printf("list:   ���õĺ����б�\r\n\n");
			Bsp_Printf("id:     ���ú�����ID�б�\r\n\n");
			Bsp_Printf("hex:    ����16������ʾ,����ո�+���ּ�ִ�н���ת��\r\n\n");
			Bsp_Printf("dec:    ����10������ʾ,����ո�+���ּ�ִ�н���ת��\r\n\n");
			Bsp_Printf("runtime:1,�����������м�ʱ;0,�رպ������м�ʱ;\r\n\n");
			Bsp_Printf("�밴�ճ����д��ʽ���뺯�������������Իس�������.\r\n");    
#else
			Bsp_Printf("ָ��ʧЧ\r\n");
#endif
			break;
		case 2://��ѯָ��
			Bsp_Printf("\r\n");
			Bsp_Printf("-------------------------Function List--------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)Bsp_Printf("%s\r\n",usmart_dev.funs[i].name);
			Bsp_Printf("\r\n");
			break;	 
		case 3://��ѯID
			Bsp_Printf("\r\n");
			Bsp_Printf("-------------------------Function ID --------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)
			{
				usmart_get_fname((uint8_t*)usmart_dev.funs[i].name,sfname,&pnum,&rval);//�õ����غ����� 
				Bsp_Printf("%s id is:\r\n0X%08X\r\n",sfname,usmart_dev.funs[i].func); //��ʾID
			}
			Bsp_Printf("\r\n");
			break;
		case 4://hexָ��
			Bsp_Printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//��������
			{
				i=usmart_str2num(sfname,&res);	   	//��¼�ò���	
				if(i==0)						  	//����ת������
				{
					Bsp_Printf("HEX:0X%X\r\n",res);	   	//תΪ16����
				}else if(i!=4)return USMART_PARMERR;//��������.
				else 				   				//������ʾ�趨����
				{
					Bsp_Printf("Hex display!\r\n");
					usmart_dev.sptype=SP_TYPE_HEX;  
				}

			}else return USMART_PARMERR;			//��������.
			Bsp_Printf("\r\n"); 
			break;
		case 5://decָ��
			Bsp_Printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//��������
			{
				i=usmart_str2num(sfname,&res);	   	//��¼�ò���	
				if(i==0)						   	//����ת������
				{
					Bsp_Printf("DEC:%lu\r\n",res);	   	//תΪ10����
				}else if(i!=4)return USMART_PARMERR;//��������.
				else 				   				//������ʾ�趨����
				{
					Bsp_Printf("Decimal display!\r\n");
					usmart_dev.sptype=SP_TYPE_DEC;  
				}

			}else return USMART_PARMERR;			//��������. 
			Bsp_Printf("\r\n"); 
			break;	 
		case 6://runtimeָ��,�����Ƿ���ʾ����ִ��ʱ��
			Bsp_Printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//��������
			{
				i=usmart_str2num(sfname,&res);	   		//��¼�ò���	
				if(i==0)						   		//��ȡָ����ַ���ݹ���
				{
					if(USMART_ENTIMX_SCAN==0)Bsp_Printf("\r\nError! \r\nTo EN RunTime function,Please set USMART_ENTIMX_SCAN = 1 first!\r\n");//����
					else
					{
						usmart_dev.runtimeflag=res;
						if(usmart_dev.runtimeflag)Bsp_Printf("Run Time Calculation ON\r\n");
						else Bsp_Printf("Run Time Calculation OFF\r\n"); 
					}
				}else return USMART_PARMERR;   			//δ������,���߲�������	 
 			}else return USMART_PARMERR;				//��������. 
			Bsp_Printf("\r\n"); 
			break;	    
		default://�Ƿ�ָ��
			return USMART_FUNCERR;
	}
	return 0;
}

#if USMART_ENTIMX_SCAN == 1

//��ֲע��:��������stm32Ϊ��,���Ҫ��ֲ������mcu,������Ӧ�޸�.
//usmart_reset_runtime,�����������ʱ��,��ͬ��ʱ���ļ����Ĵ����Լ���־λһ������.��������װ��ֵΪ���,������޶ȵ��ӳ���ʱʱ��.
//usmart_get_runtime,��ȡ��������ʱ��,ͨ����ȡCNTֵ��ȡ,����usmart��ͨ���жϵ��õĺ���,���Զ�ʱ���жϲ�����Ч,��ʱ����޶�
//ֻ��ͳ��2��CNT��ֵ,Ҳ���������+���һ��,���������2��,û������,���������ʱ,������:2*������CNT*0.1ms.��STM32��˵,��:13.1s����

/**
  * @brief ��λruntime,��Ҫ��������ֲ����MCU�Ķ�ʱ�����������޸�
  * 	   ��ÿ��USMART���ú���֮ǰִ�У������������Ȼ���ں���ִ��������usmart_get_runtime()����ȡ������������ʱ��
  */

void usmart_reset_runtime(void)
{
    __HAL_TIM_CLEAR_FLAG(&TIM4_Handler,TIM_FLAG_UPDATE);//����жϱ�־λ 
    __HAL_TIM_SET_AUTORELOAD(&TIM4_Handler,0XFFFF);     //����װ��ֵ���õ����
    __HAL_TIM_SET_COUNTER(&TIM4_Handler,0);             //��ն�ʱ����CNT
	usmart_dev.runtime=0;	
}

/**
  * @brief ���runtimeʱ��, ��Ҫ��������ֲ����MCU�Ķ�ʱ�����������޸�
  * @retval ִ��ʱ��,��λ:0.1ms,�����ʱʱ��Ϊ��ʱ��CNTֵ��2��*0.1ms
  *			���ڵ��õĺ����������ж���ִ�еģ�����ִ��ʱ��ͳ�ƹ���������ͳ�ƶ�ʱ�����1�ε�ʱ��
  *			��ʱ��4Ϊ16λ������65535����ʱ����Ϊ10KHz(0.1ms)���������ʱʱ��Ϊ13.1s
  */

uint32_t usmart_get_runtime(void)
{
	if(__HAL_TIM_GET_FLAG(&TIM4_Handler,TIM_FLAG_UPDATE)==SET)//�������ڼ�,�����˶�ʱ�����
	{
		usmart_dev.runtime+=0XFFFF;
	}
	usmart_dev.runtime+=__HAL_TIM_GET_COUNTER(&TIM4_Handler);
	return usmart_dev.runtime;		//���ؼ���ֵ
}  
#endif

/** @}
*/
/*--------------------------------------USER_DEBUG Private Functions---------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup USER_DEBUG_Exported_Functions USER_DEBUG Exported Functions
  * @{
  */
/** @defgroup USER_DEBUG_Exported_Functions_Group1 Initialization and de-initialization functions
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
  * @brief ��ʼ�����ڿ�����
  * @param sysclk:ϵͳʱ�ӣ�Mhz��
  */

void usmart_init(void)
{
#if USMART_ENTIMX_SCAN==1
	Bsp_InitHardTimer_TIM4(1000,(uint32_t)200*100-1);//��Ƶ,ʱ��Ϊ10K ,100ms�ж�һ��,ע��,����Ƶ�ʱ���Ϊ10Khz,�Ժ�runtime��λ(0.1ms)ͬ��.
#endif
	usmart_dev.sptype=1;	//ʮ��������ʾ����
}		
/** @}
*/
/*********************USER_DEBUG Exported Functions Group1**************************/

/** @defgroup USER_DEBUG_Exported_Functions_Group2 Operation Functions
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
  * @brief ��str�л�ȡ������,id,��������Ϣ
  * @param *str:�ַ���ָ��.
  * @retval ����ֵ:0,ʶ��ɹ�;����,�������.
  */

uint8_t usmart_cmd_rec(uint8_t*str) 
{
	uint8_t sta,i,rval;//״̬	 
	uint8_t rpnum,spnum;
	uint8_t rfname[MAX_FNAME_LEN];//�ݴ�ռ�,���ڴ�Ž��յ��ĺ�����  
	uint8_t sfname[MAX_FNAME_LEN];//��ű��غ�����
	sta=usmart_get_fname(str,rfname,&rpnum,&rval);//�õ����յ������ݵĺ���������������	  
	if(sta)return sta;//����
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((uint8_t*)usmart_dev.funs[i].name,sfname,&spnum,&rval);//�õ����غ���������������
		if(sta)return sta;//���ؽ�������	  
		if(usmart_strcmp(sfname,rfname)==0)//���
		{
			if(spnum>rpnum)return USMART_PARMERR;//��������(���������Դ����������)
			usmart_dev.id=i;//��¼����ID.
			break;//����.
		}	
	}
	if(i==usmart_dev.fnum)return USMART_NOFUNCFIND;	//δ�ҵ�ƥ��ĺ���
 	sta=usmart_get_fparam(str,&i);					//�õ�������������	
	if(sta)return sta;								//���ش���
	usmart_dev.pnum=i;								//����������¼
    return USMART_OK;
}

/**
  * @brief 	usamrtִ�к���,�ú�����������ִ�дӴ����յ�����Ч����.
  * 		���֧��10�������ĺ���,����Ĳ���֧��Ҳ������ʵ��.�����õĺ���.һ��5�����ҵĲ����ĺ����Ѿ����ټ���.
  *			�ú������ڴ��ڴ�ӡִ�����.��:"������(����1������2...����N)=����ֵ".����ʽ��ӡ.
  *			����ִ�еĺ���û�з���ֵ��ʱ��,����ӡ�ķ���ֵ��һ�������������.
  */

void usmart_exe(void)
{
	uint8_t id,i;
	uint32_t res;		   
	uint32_t temp[MAX_PARM];//����ת��,ʹ֧֮�����ַ��� 
	uint8_t sfname[MAX_FNAME_LEN];//��ű��غ�����
	uint8_t pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum)return;//��ִ��.
	usmart_get_fname((uint8_t*)usmart_dev.funs[id].name,sfname,&pnum,&rval);//�õ����غ�����,���������� 
	Bsp_Printf("\r\n%s(",sfname);//�����Ҫִ�еĺ�����
	for(i=0;i<pnum;i++)//�������
	{
		if(usmart_dev.parmtype&(1<<i))//�������ַ���
		{
			Bsp_Printf("%c",'"');			 
			Bsp_Printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			Bsp_Printf("%c",'"');
			temp[i]=(uint32_t)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else						  //����������
		{
			temp[i]=*(uint32_t*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC)Bsp_Printf("%ld",temp[i]);//10���Ʋ�����ʾ
			else Bsp_Printf("0X%X",temp[i]);//16���Ʋ�����ʾ 	   
		}
		if(i!=pnum-1)Bsp_Printf(",");
	}
	Bsp_Printf(")");
#if USMART_ENTIMX_SCAN==1
	usmart_reset_runtime();	//��ʱ������,��ʼ��ʱ
#endif
	switch(usmart_dev.pnum)
	{
		case 0://�޲���(void����)											  
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)();
			break;
	    case 1://��1������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0]);
			break;
	    case 2://��2������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1]);
			break;
	    case 3://��3������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2]);
			break;
	    case 4://��4������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3]);
			break;
	    case 5://��5������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4]);
			break;
	    case 6://��6������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5]);
			break;
	    case 7://��7������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6]);
			break;
	    case 8://��8������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7]);
			break;
	    case 9://��9������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8]);
			break;
	    case 10://��10������
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8],temp[9]);
			break;
	}
#if USMART_ENTIMX_SCAN==1
	usmart_get_runtime();//��ȡ����ִ��ʱ��
#endif
	if(rval==1)//��Ҫ����ֵ.
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)Bsp_Printf("=%lu;\r\n",res);//���ִ�н��(10���Ʋ�����ʾ)
		else Bsp_Printf("=0X%X;\r\n",res);//���ִ�н��(16���Ʋ�����ʾ)	   
	}else Bsp_Printf(";\r\n");		//����Ҫ����ֵ,ֱ���������
	if(usmart_dev.runtimeflag)	//��Ҫ��ʾ����ִ��ʱ��
	{ 
		Bsp_Printf("Function Run Time:%d.%1dms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);//��ӡ����ִ��ʱ�� 
	}	
}

/**
  * @brief 	usmartɨ�躯��,ͨ�����øú���,ʵ��usmart�ĸ�������.�ú�����Ҫÿ��һ��ʱ�䱻����һ��,�Լ�ʱִ�дӴ��ڷ������ĸ�������.
  * 		�������������ж��������,�Ӷ�ʵ���Զ�����.
  */

void usmart_scan(void)
{
	uint8_t sta,len;  
	if(USART_RX_STA&0x8000)//���ڽ�����ɣ�
	{					   
		len=USART_RX_STA&0x3fff;	//�õ��˴ν��յ������ݳ���
		USART_RX_BUF[len]='\0';	//��ĩβ���������. 
		sta=usmart_dev.cmd_rec(USART_RX_BUF);//�õ�����������Ϣ
		if(sta==0)usmart_dev.exe();	//ִ�к��� 
		else 
		{  
			len=usmart_sys_cmd_exe(USART_RX_BUF);
			if(len!=USMART_FUNCERR)sta=len;
			if(sta)
			{   
				switch(sta)
				{
					case USMART_FUNCERR:
						Bsp_Printf("��������!\r\n");
                      				
						break;	
					case USMART_PARMERR:
						Bsp_Printf("��������!\r\n");   			
						break;				
					case USMART_PARMOVER:
						Bsp_Printf("����̫��!\r\n");   			
						break;		
					case USMART_NOFUNCFIND:
						Bsp_Printf("δ�ҵ�ƥ��ĺ���!\r\n");   			
						break;		
				}
			}
		}
		USART_RX_STA=0;//״̬�Ĵ������	    
	}
}

#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����

/**
  * @brief ȡָ����ַ��ֵ		 
  */

uint32_t read_addr(uint32_t addr)
{
	return *(uint32_t*)addr;//	
}

/**
  * @brief ��ָ����ַд��ָ����ֵ		 
  */

void write_addr(uint32_t addr,uint32_t val)
{
	*(uint32_t*)addr=val; 	
}

#endif

/** @}
*/
/****************************XXXX Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------USER_DEBUG Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------USER_DEBUG-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
