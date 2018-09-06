/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/Utilities/Debug/user_dbg_str.c
  * @author  CME
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
  * @brief �ַ�������ش������
  * @{
  */

/* Exported types ---------------------------------------------------------------------------------*/
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
	    This subsection provides a set of functions allowing to initialize USER_DEBUG
  @endverbatim
  * @{
  */
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
			This subsection provides a set of functions allowing to manage the USER_DEBUG
  @endverbatim
  * @{
  */
/**
  * @brief 	�Ա��ַ���str1��str2
  *	@param	str1:�ַ���1ָ��
  *	 		str2:�ַ���2ָ��
  *	@retval	����ֵ:0�����;1�������;
  */

uint8_t usmart_strcmp(uint8_t *str1,uint8_t *str2)
{
	while(1)
	{
		if(*str1!=*str2)return 1;//�����
		if(*str1=='\0')break;//�Ա������.
		str1++;
		str2++;
	}
	return 0;//�����ַ������
}

/**
  * @brief 	��str1������copy��str2
  * @param 	*str1:�ַ���1ָ��
  *			str2:�ַ���2ָ��
  */

void usmart_strcopy(uint8_t*str1,uint8_t *str2)
{
	while(1)
	{										   
		*str2=*str1;	//����
		if(*str1=='\0')break;//���������.
		str1++;
		str2++;
	}
}

/**
  * @brief �õ��ַ����ĳ���(�ֽ�)
  * @param *str:�ַ���ָ��
  * @retval ����ֵ:�ַ����ĳ���	
  */

uint8_t usmart_strlen(uint8_t*str)
{
	uint8_t len=0;
	while(1)
	{							 
		if(*str=='\0')break;//���������.
		len++;
		str++;
	}
	return len;
}

/**
  * @brief m^n����
* @param m: ��,n: ��.
  * @retval m^n�η�
  */

uint32_t usmart_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	

/**
  * @brief ���ַ���תΪ���� ֧��16����ת��,����16������ĸ�����Ǵ�д��,�Ҹ�ʽΪ��0X��ͷ��.֧�ָ���
  * @param *str:�����ַ���ָ�� *res:ת����Ľ����ŵ�ַ.
  * @retval 0���ɹ�ת�����.����,������� 1,���ݸ�ʽ����.2,16����λ��Ϊ0.3,��ʼ��ʽ����.4,ʮ����λ��Ϊ0.
  */

uint8_t usmart_str2num(uint8_t*str,uint32_t *res)
{
	uint32_t t;
	int tnum;
	uint8_t bnum=0;		//���ֵ�λ��
	uint8_t *p;		  
	uint8_t hexdec=10;	//Ĭ��Ϊʮ��������
	uint8_t flag=0;		//0,û�з��ű��;1,��ʾ����;2,��ʾ����.
	p=str;
	*res=0;//����.
	while(1)
	{
		if((*p<='9'&&*p>='0')||((*str=='-'||*str=='+')&&bnum==0)||(*p<='F'&&*p>='A')||(*p=='X'&&bnum==1))//�����Ϸ�
		{
			if(*p>='A')hexdec=16;	//�ַ����д�����ĸ,Ϊ16���Ƹ�ʽ.
			if(*str=='-'){flag=2;str+=1;}//ƫ�Ƶ�����
			else if(*str=='+'){flag=1;str+=1;}//ƫ�Ƶ�����
			else bnum++;			//λ������.
		}else if(*p=='\0')break;	//����������,�˳�.
		else return 1;				//��ȫ��ʮ���ƻ���16��������.
		p++; 
	} 
	p=str;			    //���¶�λ���ַ�����ʼ�ĵ�ַ.
	if(hexdec==16)		//16��������
	{
		if(bnum<3)return 2;			//λ��С��3��ֱ���˳�.��Ϊ0X��ռ��2��,���0X���治������,������ݷǷ�.
		if(*p=='0' && (*(p+1)=='X'))//������'0X'��ͷ.
		{
			p+=2;	//ƫ�Ƶ�������ʼ��ַ.
			bnum-=2;//��ȥƫ����	 
		}else return 3;//��ʼͷ�ĸ�ʽ����
	}else if(bnum==0)return 4;//λ��Ϊ0��ֱ���˳�.	  
	while(1)
	{
		if(bnum)bnum--;
		if(*p<='9'&&*p>='0')t=*p-'0';	//�õ����ֵ�ֵ
		else t=*p-'A'+10;				//�õ�A~F��Ӧ��ֵ	    
		*res+=t*usmart_pow(hexdec,bnum);		   
		p++;
		if(*p=='\0')break;//���ݶ�������.	
	}
	if(flag==2)//�Ǹ���?
	{	
		tnum=-*res; 
		*res=tnum;
	}
	return 0;//�ɹ�ת��
}

/**
  * @brief �õ�ָ����
  * @param *str:Դ�ַ��� *cmdname:ָ���� *nlen:ָ�������� maxlen:��󳤶�(������,ָ�����̫����)	
  * @retval 0,�ɹ�;����,ʧ��
  */
  
uint8_t usmart_get_cmdname(uint8_t*str,uint8_t*cmdname,uint8_t *nlen,uint8_t maxlen)
{
	*nlen=0;
 	while(*str!=' '&&*str!='\0') //�ҵ��ո���߽���������Ϊ������
	{
		*cmdname=*str;
		str++;
		cmdname++;
		(*nlen)++;//ͳ�������
		if(*nlen>=maxlen)return 1;//�����ָ��
	}
	*cmdname='\0';//���������
	return 0;//��������
}

/**
  * @brief ��ȡ��һ���ַ������м��кܶ�ո��ʱ�򣬴˺���ֱ�Ӻ��Կո��ҵ��ո�֮��ĵ�һ���ַ���
  * @param str:�ַ���ָ��
  * @retval ��һ���ַ�
  */

uint8_t usmart_search_nextc(uint8_t* str)
{		   	 	
	str++;
	while(*str==' '&&str!='\0')str++;
	return *str;
} 

/**
  * @brief ��str�еõ�������
  * @param *str:Դ�ַ���ָ�� *fname:��ȡ���ĺ�������ָ�� *pnum:�����Ĳ������� *rval:�Ƿ���Ҫ��ʾ����ֵ(0,����Ҫ;1,��Ҫ)
  * @retval 0,�ɹ�;����,�������.
  */

uint8_t usmart_get_fname(uint8_t*str,uint8_t*fname,uint8_t *pnum,uint8_t *rval)
{
	uint8_t res;
	uint8_t fover=0;	  //�������
	uint8_t *strtemp;
	uint8_t offset=0;  
	uint8_t parmnum=0;
	uint8_t temp=1;
	uint8_t fpname[6];//void+X+'/0'
	uint8_t fplcnt=0; //��һ�������ĳ��ȼ�����
	uint8_t pcnt=0;	 //����������
	uint8_t nchar;
	//�жϺ����Ƿ��з���ֵ
	strtemp=str;
	while(*strtemp!='\0')//û�н���
	{
		if(*strtemp!=' '&&(pcnt&0X7F)<5)//����¼5���ַ�
		{	
			if(pcnt==0)pcnt|=0X80;//��λ���λ,��ǿ�ʼ���շ���ֵ����
			if(((pcnt&0x7f)==4)&&(*strtemp!='*'))break;//���һ���ַ�,������*
			fpname[pcnt&0x7f]=*strtemp;//��¼�����ķ���ֵ����
			pcnt++;
		}else if(pcnt==0X85)break;
		strtemp++; 
	} 
	if(pcnt)//��������
	{
		fpname[pcnt&0x7f]='\0';//���������
		if(usmart_strcmp(fpname,"void")==0)*rval=0;//����Ҫ����ֵ
		else *rval=1;							   //��Ҫ����ֵ
		pcnt=0;
	} 
	res=0;
	strtemp=str;
	while(*strtemp!='('&&*strtemp!='\0') //�˴����ҵ���������������ʼλ��
	{  
		strtemp++;
		res++;
		if(*strtemp==' '||*strtemp=='*')
		{
			nchar=usmart_search_nextc(strtemp);		//��ȡ��һ���ַ�
			if(nchar!='('&&nchar!='*')offset=res;	//�����ո��*��
		}
	}	 
	strtemp=str;
	if(offset)strtemp+=offset+1;//������������ʼ�ĵط�	   
	res=0;
	nchar=0;//�Ƿ������ַ�������ı�־,0�������ַ���;1�����ַ���;
	while(1)
	{
		if(*strtemp==0)
		{
			res=USMART_FUNCERR;//��������
			break;
		}else if(*strtemp=='('&&nchar==0)fover++;//�����������һ��	 
		else if(*strtemp==')'&&nchar==0)
		{
			if(fover)fover--;
			else res=USMART_FUNCERR;//�������,û�յ�'('
			if(fover==0)break;//��ĩβ��,�˳�	    
		}else if(*strtemp=='"')nchar=!nchar;

		if(fover==0)//��������û������
		{
			if(*strtemp!=' ')//�ո����ں�����
			{
				*fname=*strtemp;//�õ�������
				fname++;
			}
		}else //�Ѿ��������˺�������.
		{
			if(*strtemp==',')
			{
				temp=1;		//ʹ������һ������
				pcnt++;	
			}else if(*strtemp!=' '&&*strtemp!='(')
			{
				if(pcnt==0&&fplcnt<5)		//����һ��������ʱ,Ϊ�˱���ͳ��void���͵Ĳ���,�������ж�.
				{
					fpname[fplcnt]=*strtemp;//��¼��������.
					fplcnt++;
				}
				temp++;	//�õ���Ч����(�ǿո�)
			}
			if(fover==1&&temp==2)
			{
				temp++;		//��ֹ�ظ�����
				parmnum++; 	//��������һ��
			}
		}
		strtemp++; 			
	}   
	if(parmnum==1)//ֻ��1������.
	{
		fpname[fplcnt]='\0';//���������
		if(usmart_strcmp(fpname,"void")==0)parmnum=0;//����Ϊvoid,��ʾû�в���.
	}
	*pnum=parmnum;	//��¼��������
	*fname='\0';	//���������
	return res;		//����ִ�н��
}

/**
  * @brief ��str�еõ�һ�������Ĳ���
  * @param *str:Դ�ַ���ָ�� *fparm:�����ַ���ָ�� *ptype:�������� 0������;1���ַ���;0XFF����������
  * @retval 0,�Ѿ��޲�����;����,��һ��������ƫ����.
  */

uint8_t usmart_get_aparm(uint8_t *str,uint8_t *fparm,uint8_t *ptype)
{
	uint8_t i=0;
	uint8_t enout=0;
	uint8_t type=0;//Ĭ��������
	uint8_t string=0; //���str�Ƿ����ڶ�
	while(1)
	{		    
		if(*str==','&& string==0)enout=1;			//�ݻ������˳�,Ŀ����Ѱ����һ����������ʼ��ַ
		if((*str==')'||*str=='\0')&&string==0)break;//�����˳���ʶ��
		if(type==0)//Ĭ�������ֵ�
		{
			if((*str>='0' && *str<='9')||*str=='-'||*str=='+'||(*str>='a' && *str<='f')||(*str>='A' && *str<='F')||*str=='X'||*str=='x')//���ִ����
			{
				if(enout)break;					//�ҵ�����һ������,ֱ���˳�.
				if(*str>='a')*fparm=*str-0X20;	//Сдת��Ϊ��д
				else *fparm=*str;		   		//Сд�������ֱ��ֲ���
				fparm++;
			}else if(*str=='"')//�ҵ��ַ����Ŀ�ʼ��־
			{
				if(enout)break;//�ҵ�,����ҵ�",��Ϊ������.
				type=1;
				string=1;//�Ǽ�STRING ���ڶ���
			}else if(*str!=' '&&*str!=',')//���ַǷ��ַ�,��������
			{
				type=0XFF;
				break;
			}
		}else//string��
		{ 
			if(*str=='"')string=0;
			if(enout)break;			//�ҵ�����һ������,ֱ���˳�.
			if(string)				//�ַ������ڶ�
			{	
				if(*str=='\\')		//����ת���(������ת���)
				{ 
					str++;			//ƫ�Ƶ�ת���������ַ�,����ʲô�ַ�,ֱ��COPY
					i++;
				}					
				*fparm=*str;		//Сд�������ֱ��ֲ���
				fparm++;
			}	
		}
		i++;//ƫ��������
		str++;
	}
	*fparm='\0';	//���������
	*ptype=type;	//���ز�������
	return i;		//���ز�������
}

/**
  * @brief �õ�ָ����������ʼ��ַ
  * @param num:��num������,��Χ0~9.
  * @retval �ò�������ʼ��ַ
  */

uint8_t usmart_get_parmpos(uint8_t num)
{
	uint8_t temp=0;
	uint8_t i;
	for(i=0;i<num;i++)temp+=usmart_dev.plentbl[i];
	return temp;
}

/**
  * @brief ��str�еõ���������
  * @param str:Դ�ַ���; parn:�����Ķ���.0��ʾ�޲��� void����
  * @retval 0,�ɹ�;����,�������.
  */

uint8_t usmart_get_fparam(uint8_t*str,uint8_t *parn)
{	
	uint8_t i,type;  
	uint32_t res;
	uint8_t n=0;
	uint8_t len;
	uint8_t tstr[PARM_LEN+1];//�ֽڳ��ȵĻ���,�����Դ��PARM_LEN���ַ����ַ���
	for(i=0;i<MAX_PARM;i++)usmart_dev.plentbl[i]=0;//��ղ������ȱ�
	while(*str!='(')//ƫ�Ƶ�������ʼ�ĵط�
	{
		str++;											    
		if(*str=='\0')return USMART_FUNCERR;//������������
	}
	str++;//ƫ�Ƶ�"("֮��ĵ�һ���ֽ�
	while(1)
	{
		i=usmart_get_aparm(str,tstr,&type);	//�õ���һ������  
		str+=i;								//ƫ��
		switch(type)
		{
			case 0:	//����
				if(tstr[0]!='\0')				//���յ��Ĳ�����Ч
				{					    
					i=usmart_str2num(tstr,&res);	//��¼�ò���	 
					if(i)return USMART_PARMERR;		//��������.
					*(uint32_t*)(usmart_dev.parm+usmart_get_parmpos(n))=res;//��¼ת���ɹ��Ľ��.
					usmart_dev.parmtype&=~(1<<n);	//�������
					usmart_dev.plentbl[n]=4;		//�ò����ĳ���Ϊ4  
					n++;							//��������  
					if(n>MAX_PARM)return USMART_PARMOVER;//����̫��
				}
				break;
			case 1://�ַ���	 	
				len=usmart_strlen(tstr)+1;	//�����˽�����'\0'
				usmart_strcopy(tstr,&usmart_dev.parm[usmart_get_parmpos(n)]);//����tstr���ݵ�usmart_dev.parm[n]
				usmart_dev.parmtype|=1<<n;	//����ַ��� 
				usmart_dev.plentbl[n]=len;	//�ò����ĳ���Ϊlen  
				n++;
				if(n>MAX_PARM)return USMART_PARMOVER;//����̫��
				break;
			case 0XFF://����
				return USMART_PARMERR;//��������	  
		}
		if(*str==')'||*str=='\0')break;//�鵽������־��.
	}
	*parn=n;	//��¼�����ĸ���
	return USMART_OK;//��ȷ�õ��˲���
}

/** @}
*/
/****************************USER_DEBUG Exported Functions Group2*********************/
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
