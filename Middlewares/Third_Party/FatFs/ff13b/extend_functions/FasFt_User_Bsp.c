/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/Middlewares/Third_Party/FatFs/ff13b/extend_functions/FasFt_User_Bsp.c
  * @author  CME
  * @version SW:V1.0.0 HW:V1.0
  * @date    28-September-2018
  * @brief   This file provides set of firmware functions to manage:
  *					 -
  *
  @verbatim
 ===============================================================================
                        ##### How to use this file #####
 ===============================================================================
  [..]
	The FasFt_User canbe used as follows:
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
/** @defgroup 	FasFt_User FasFt_User
  * @brief 		FasFt_User
  * @note		FasFt_User
  * @{
  */

/* Exported Variable------------------------------------------------------------------------------*/
/** @defgroup FasFt_User_Exported Variable FasFt_User Exported Variable
  * @{
  */
/**
  * @brief  The define of the variable
  */
BYTE Buff[4096] __attribute__ ((aligned (4))) ;	/* Working buffer */  

char ReadDataBuff[BUF_SIZE];
char WriteDataBuff[BUF_SIZE] = {"This is the test for the write file."};

/**
  * @brief  The types of files that FasFt support
  */
uint8_t*const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM]=
{
{"BIN"},						//BIN�ļ�
{"LRC"},						//LRC�ļ�
{"NES","SMS"},					//NES/SMS�ļ�
{"TXT","C","H"},				//�ı��ļ�
{"WAV","MP3","APE","FLAC"},		//֧�ֵ������ļ�
{"BMP","JPG","JPEG","GIF"},		//ͼƬ�ļ�
{"AVI"},						//��Ƶ�ļ�
};

/**
  * @brief  Public file
  */
FATFS *fs[FF_VOLUMES];			//�߼����̹�����.	 
FIL *file;	  					//�ļ�1
FIL *ftemp;	  					//�ļ�2.
UINT br,bw;						//��д����
FILINFO fileinfo;				//�ļ���Ϣ
DIR dir;  						//Ŀ¼

uint8_t *fatbuf;				//SD�����ݻ�����

/** @}
*/
/*--------------------------------------FasFt_User Exported Variable---------------------------------*/

/* Exported types ---------------------------------------------------------------------------------*/

/* Exported functions -----------------------------------------------------------------------------*/
/** @defgroup FasFt_User_Exported_Functions FasFt_User Exported Functions
  * @{
  */
/** @defgroup FasFt_User_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief   Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
===============================================================================
  [..]
	    This subsection provides a set of functions allowing to initialize FasFt_User
  @endverbatim
  * @{
  */
/**
  * @brief Initialize the FasFt_User according to the specified, mainly Memory allocation
  * @retval 0: success, 1: failure
  */
uint8_t Bsp_FatFs_Init(void)
{
	uint8_t i;
	for(i=0;i<FF_VOLUMES;i++)
	{
		fs[i]=(FATFS*)Bsp_mymalloc(SRAMIN,sizeof(FATFS));	//Ϊ����i�����������ڴ�	
		if(!fs[i])
		{
			break;
		}
	}
	file=(FIL*)Bsp_mymalloc(SRAMIN,sizeof(FIL));			//Ϊfile�����ڴ�
	ftemp=(FIL*)Bsp_mymalloc(SRAMIN,sizeof(FIL));			//Ϊftemp�����ڴ�
	fatbuf=(uint8_t*)Bsp_mymalloc(SRAMIN,512);				//Ϊfatbuf�����ڴ�
	if(i==FF_VOLUMES&&file&&ftemp&&fatbuf)
	{
		return 0;  											//������һ��ʧ��,��ʧ��.
	}
	else 
	{
		return 1;
	}		
}
/** @}
*/
/*********************FasFt_User Exported Functions Group1**************************/

/** @defgroup FasFt_User_Exported_Functions_Group2 Operation Functions
  *  @brief   Operation Functions
  *
@verbatim
===============================================================================
            ##### 					Operation Functions 						#####
===============================================================================
  [..]
			This subsection provides a set of functions allowing to manage the FasFt_User
  @endverbatim
  * @{
  */
/**
  * @brief Tell the type of file
  * @param fname: The name of file.
  * @retval 0XFF,Can not tell the file type(The High 4 bit is the main type, the Low 4 bit is the sub_main type. )
  */
uint8_t Bsp_FatFs_File_Tell(uint8_t *fname)
{
	uint8_t tbuf[5];
	uint8_t *attr='\0';		//��׺��
	uint8_t i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')
		{
			break;			//ƫ�Ƶ��������.
		}
		fname++;
	}
	if(i==250)
	{
		return 0XFF;		//������ַ���.
	}
 	for(i=0;i<5;i++)		//�õ���׺��
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  	}
	strcpy((char *)tbuf,(const char*)attr);		//copy
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//ȫ����Ϊ��д 
	for(i=0;i<FILE_MAX_TYPE_NUM;i++)			//����Ա�
	{
		for(j=0;j<FILE_MAX_SUBT_NUM;j++)		//����Ա�
		{
			if(*FILE_TYPE_TBL[i][j]==0)
			{
				break;							//�����Ѿ�û�пɶԱȵĳ�Ա��.
			}
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;								//û�ҵ�		 			   
}	 

/**
  * @brief  Get the Volum of the disk. Total and Free
  * @param  drv:���̱��("0:"/"1:")
  * @param  total:������	 ����λKB��
  * @param  free:ʣ������	 ����λKB��
  * @retval  0,����.����,�������
*/

uint8_t Bsp_FatFs_GetDisk_Volume(uint8_t *drv,uint32_t *total,uint32_t *free)
{
	FATFS *fs1;
	uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res =(uint32_t)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//�õ���������
	    fre_sect=fre_clust*fs1->csize;			//�õ�����������	   
#if FF_MAX_SS!=512				  				//������С����512�ֽ�,��ת��Ϊ512�ֽ�
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//��λΪKB
		*free=fre_sect>>1;	//��λΪKB 
 	}
	return res;
}	

/**
  * @brief File Copy,��psrc�ļ�,copy��pdst. make sure the size is less 4GB
  * @param fcpymsg,����ָ��,����ʵ�ֿ���ʱ����Ϣ��ʾ
  * @param pname:�ļ���
  * @param pct:�ٷֱ�
  * @param mode:[0]:�����ļ���; [1]:���°ٷֱ�pct; [2]:�����ļ���; [3~7]:����
  * @param psrc,pdst:Դ�ļ���Ŀ���ļ�
  * @param totsize:�ܴ�С(��totsizeΪ0��ʱ��,��ʾ����Ϊ�����ļ�����)
  * @param cpdsize:�Ѹ����˵Ĵ�С.
  * @param fwmode:�ļ�д��ģʽ 0:������ԭ�е��ļ�;1:����ԭ�е��ļ�
  * @retval 0:����;0XFF:ǿ���˳�;����:�˳�
  */
uint8_t Bsp_FatFs_CopyFile(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t totsize,uint32_t cpdsize,uint8_t fwmode)
{
	uint8_t res;
    uint16_t br=0;
	uint16_t bw=0;
	FIL *fsrc=0;
	FIL *fdst=0;
	uint8_t *fbuf=0;
	uint8_t curpct=0;
	unsigned long long lcpdsize=cpdsize; 
 	fsrc=(FIL*)Bsp_mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
 	fdst=(FIL*)Bsp_mymalloc(SRAMIN,sizeof(FIL));
	fbuf=(uint8_t*)Bsp_mymalloc(SRAMIN,8192);
  	if(fsrc==NULL||fdst==NULL||fbuf==NULL)
	{
		res=100;								//ǰ���ֵ����fatfs
	}
	else
	{   
		if(fwmode==0)
		{
			fwmode=FA_CREATE_NEW;				//������
		}
		else 
		{
			fwmode=FA_CREATE_ALWAYS;	  		//���Ǵ��ڵ��ļ�
		}
	 	res=f_open(fsrc,(const TCHAR*)psrc,FA_READ|FA_OPEN_EXISTING);	//��ֻ���ļ�
	 	if(res==0)
		{
			res=f_open(fdst,(const TCHAR*)pdst,FA_WRITE|fwmode); 		//��һ���򿪳ɹ�,�ſ�ʼ�򿪵ڶ���
		}
		if(res==0)//�������򿪳ɹ���
		{
			if(totsize==0)//�����ǵ����ļ�����
			{
				totsize=fsrc->obj.objsize;
				lcpdsize=0;
				curpct=0;
		 	}
			else 
			{
				curpct=(lcpdsize*100)/totsize;	//�õ��°ٷֱ�
			}
			fcpymsg(psrc,curpct,0X02);			//���°ٷֱ�
			while(res==0)//��ʼ����
			{
				res=f_read(fsrc,fbuf,8192,(UINT*)&br);	//Դͷ����512�ֽ�
				if(res||br==0)
				{
					break;
				}
				res=f_write(fdst,fbuf,(UINT)br,(UINT*)&bw);	//д��Ŀ���ļ�
				lcpdsize+=bw;
				if(curpct!=(lcpdsize*100)/totsize)//�Ƿ���Ҫ���°ٷֱ�
				{
					curpct=(lcpdsize*100)/totsize;
					if(fcpymsg(psrc,curpct,0X02))//���°ٷֱ�
					{
						res=0XFF;//ǿ���˳�
						break;
					}
				}			     
				if(res||bw<br)
				{
					break;       
				}
			}
		    f_close(fsrc);
		    f_close(fdst);
		}
	}
	Bsp_myfree(SRAMIN,fsrc);//�ͷ��ڴ�
	Bsp_myfree(SRAMIN,fdst);
	Bsp_myfree(SRAMIN,fbuf);
	return res;
}

/**
  * @brief  �õ�·���µ��ļ���
  * @param  dpfn:���̱��("0:"/"1:")
  * @retval  0,·�����Ǹ�����;����,�ļ��������׵�ַ
*/
uint8_t* Bsp_FatFs_GetSrc_Dname(uint8_t* dpfn)
{
	uint16_t temp=0;
 	while(*dpfn!=0)
	{
		dpfn++;
		temp++;	
	}
	if(temp<4)return 0; 
	while((*dpfn!=0x5c)&&(*dpfn!=0x2f))dpfn--;	//׷����������һ��"\"����"/"�� 
	return ++dpfn;
}

/**
  * @brief  �õ��ļ��д�С,ע���ļ��д�С��Ҫ����4GB.
  * @param  fdname:�ļ�������
  * @retval  0,�ļ��д�СΪ0,���߶�ȡ�����з����˴���; ����,�ļ��д�С.
*/

uint32_t Bsp_FatFs_GetFile_Size(uint8_t *fdname)
 {
#define MAX_PATHNAME_DEPTH	512+1	//���Ŀ���ļ�·��+�ļ������
	uint8_t res=0;	  
    DIR *fddir=0;					//Ŀ¼
	FILINFO *finfo=0;				//�ļ���Ϣ
	uint8_t * pathname=0;			//Ŀ���ļ���·��+�ļ���
 	uint16_t pathlen=0;				//Ŀ��·������
	uint32_t fdsize=0;

	fddir=(DIR*)Bsp_mymalloc(SRAMIN,sizeof(DIR));//�����ڴ�
 	finfo=(FILINFO*)Bsp_mymalloc(SRAMIN,sizeof(FILINFO));
   	if(fddir==NULL||finfo==NULL)res=100;
	if(res==0)
	{ 
 		pathname=Bsp_mymalloc(SRAMIN,MAX_PATHNAME_DEPTH);	    
 		if(pathname==NULL)res=101;	   
 		if(res==0)
		{
			pathname[0]=0;	    
			strcat((char*)pathname,(const char*)fdname); 	//����·��	
		    res=f_opendir(fddir,(const TCHAR*)fdname); 		//��ԴĿ¼
		    if(res==0)										//��Ŀ¼�ɹ� 
			{														   
				while(res==0)								//��ʼ�����ļ�������Ķ���
				{
			        res=f_readdir(fddir,finfo);				//��ȡĿ¼�µ�һ���ļ�
			        if(res!=FR_OK||finfo->fname[0]==0)
					{
						break;								//������/��ĩβ��,�˳�
					}
			        if(finfo->fname[0]=='.')
					{
						continue;     						//�����ϼ�Ŀ¼
					}
					if(finfo->fattrib&0X10)					//����Ŀ¼(�ļ�����,0X20,�鵵�ļ�;0X10,��Ŀ¼;)
					{
 						pathlen=strlen((const char*)pathname);				//�õ���ǰ·���ĳ���
						strcat((char*)pathname,(const char*)"/");			//��б��
						strcat((char*)pathname,(const char*)finfo->fname);	//Դ·��������Ŀ¼����
 						Bsp_Printf("\r\nsub folder:%s\r\n",pathname);		//��ӡ��Ŀ¼��
						fdsize+=Bsp_FatFs_GetFile_Size(pathname);			//�õ���Ŀ¼��С,�ݹ����
						pathname[pathlen]=0;								//���������
					}
					else 
					{
						fdsize+=finfo->fsize;								//��Ŀ¼,ֱ�Ӽ����ļ��Ĵ�С
					}
				} 
		    }	  
  			Bsp_myfree(SRAMIN,pathname);	     
		}
 	}
	Bsp_myfree(SRAMIN,fddir);    
	Bsp_myfree(SRAMIN,finfo);
	if(res)return 0;
	else return fdsize;
}

/**
  * @brief �ļ��и�ֵ,��psrc�ļ���,copy��pdst�ļ���. make sure the size is less 4GB��pdst:��������"X:"/"X:XX"/"X:XX/XX"֮���.����Ҫʵ��ȷ����һ���ļ��д���
  * @param fcpymsg,����ָ��,����ʵ�ֿ���ʱ����Ϣ��ʾ
  * @param pname:�ļ�����
  * @param pct:�ٷֱ�
  * @param mode:[0]:�����ļ���; [1]:���°ٷֱ�pct; [2]:�����ļ���; [3~7]:����
  * @param psrc,pdst:Դ�ļ��к�Ŀ���ļ���
  * @param totsize:�ܴ�С(��totsizeΪ0��ʱ��,��ʾ����Ϊ�����ļ�����)
  * @param cpdsize:�Ѹ����˵Ĵ�С.
  * @param fwmode:�ļ�д��ģʽ 0:������ԭ�е��ļ�;1:����ԭ�е��ļ�
  * @retval 0:����;0XFF:ǿ���˳�;����:�˳�
  */

uint8_t Bsp_FatFs_CopyFolder(uint8_t(*fcpymsg)(uint8_t*pname,uint8_t pct,uint8_t mode),uint8_t *psrc,uint8_t *pdst,uint32_t *totsize,uint32_t *cpdsize,uint8_t fwmode)
{
#define MAX_PATHNAME_DEPTH	512+1	//���Ŀ���ļ�·��+�ļ������
	uint8_t res=0;	  
    DIR *srcdir=0;		//ԴĿ¼
	DIR *dstdir=0;		//ԴĿ¼
	FILINFO *finfo=0;	//�ļ���Ϣ
	uint8_t *fn=0;   		//���ļ���

	uint8_t * dstpathname=0;	//Ŀ���ļ���·��+�ļ���
	uint8_t * srcpathname=0;	//Դ�ļ���·��+�ļ���
	
 	uint16_t dstpathlen=0;	//Ŀ��·������
 	uint16_t srcpathlen=0;	//Դ·������

  
	srcdir=(DIR*)Bsp_mymalloc(SRAMIN,sizeof(DIR));//�����ڴ�
 	dstdir=(DIR*)Bsp_mymalloc(SRAMIN,sizeof(DIR));
	finfo=(FILINFO*)Bsp_mymalloc(SRAMIN,sizeof(FILINFO));

   	if(srcdir==NULL||dstdir==NULL||finfo==NULL)res=100;
	if(res==0)
	{ 
 		dstpathname=Bsp_mymalloc(SRAMIN,MAX_PATHNAME_DEPTH);
		srcpathname=Bsp_mymalloc(SRAMIN,MAX_PATHNAME_DEPTH);
 		if(dstpathname==NULL||srcpathname==NULL)res=101;	   
 		if(res==0)
		{
			dstpathname[0]=0;
			srcpathname[0]=0;
			strcat((char*)srcpathname,(const char*)psrc); 	//����ԭʼԴ�ļ�·��	
			strcat((char*)dstpathname,(const char*)pdst); 	//����ԭʼĿ���ļ�·��	
		    res=f_opendir(srcdir,(const TCHAR*)psrc); 		//��ԴĿ¼
		    if(res==0)//��Ŀ¼�ɹ� 
			{
  				strcat((char*)dstpathname,(const char*)"/");//����б��
 				fn=Bsp_FatFs_GetSrc_Dname(psrc);
				if(fn==0)//��꿽��
				{
					dstpathlen=strlen((const char*)dstpathname);
					dstpathname[dstpathlen]=psrc[0];	//��¼���
					dstpathname[dstpathlen+1]=0;		//������ 
				}
				else 
				{
					strcat((char*)dstpathname,(const char*)fn);//���ļ���	
				}					
 				fcpymsg(fn,0,0X04);//�����ļ�����
				res=f_mkdir((const TCHAR*)dstpathname);//����ļ����Ѿ�����,�Ͳ�����.��������ھʹ����µ��ļ���.
				if(res==FR_EXIST)
				{
					res=0;
				}
				while(res==0)//��ʼ�����ļ�������Ķ���
				{
			        res=f_readdir(srcdir,finfo);					//��ȡĿ¼�µ�һ���ļ�
			        if(res!=FR_OK||finfo->fname[0]==0)
					{
						break;		//������/��ĩβ��,�˳�
					}
			        if(finfo->fname[0]=='.')
					{
						continue;     			//�����ϼ�Ŀ¼
					}
					fn=(uint8_t*)finfo->fname; 							//�õ��ļ���
					dstpathlen=strlen((const char*)dstpathname);	//�õ���ǰĿ��·���ĳ���
					srcpathlen=strlen((const char*)srcpathname);	//�õ�Դ·������

					strcat((char*)srcpathname,(const char*)"/");//Դ·����б��
 					if(finfo->fattrib&0X10)//����Ŀ¼(�ļ�����,0X20,�鵵�ļ�;0X10,��Ŀ¼;)
					{
						strcat((char*)srcpathname,(const char*)fn);		//Դ·��������Ŀ¼����
						res=Bsp_FatFs_CopyFolder(fcpymsg,srcpathname,dstpathname,totsize,cpdsize,fwmode);	//�����ļ���
					}
					else //��Ŀ¼
					{
						strcat((char*)dstpathname,(const char*)"/");//Ŀ��·����б��
						strcat((char*)dstpathname,(const char*)fn);	//Ŀ��·�����ļ���
						strcat((char*)srcpathname,(const char*)fn);	//Դ·�����ļ���
 						fcpymsg(fn,0,0X01);//�����ļ���
						res=Bsp_FatFs_CopyFile(fcpymsg,srcpathname,dstpathname,*totsize,*cpdsize,fwmode);//�����ļ�
						*cpdsize+=finfo->fsize;//����һ���ļ���С
					}
					srcpathname[srcpathlen]=0;//���������
					dstpathname[dstpathlen]=0;//���������	    
				} 
		    }	  
  			Bsp_myfree(SRAMIN,dstpathname);
 			Bsp_myfree(SRAMIN,srcpathname); 
		}
 	}
	Bsp_myfree(SRAMIN,srcdir);
	Bsp_myfree(SRAMIN,dstdir);
	Bsp_myfree(SRAMIN,finfo);
    return res;	  
}

/**
  * @brief ��ʾĿ¼�µ��ļ�
  * @param path,Ҫ��ʾ��Ŀ¼
  * @retval 0:����;����:�˳�
  */
uint8_t Bsp_FatFs_ViewDir(const TCHAR* path)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	DIR DirInf;
	FILINFO FileInf;
	uint32_t cnt = 0;

	result = f_opendir(&DirInf, path); 
	if (result != FR_OK)
	{
		Bsp_Printf("Open the directory %s failed (%d).\r\n", path, result);
		return 1;
	}
	Bsp_Printf("Attribute		|	size	|	Primary file name	|	Altenative file name\r\n");
	for (cnt = 0; ;cnt++)
	{
		result = f_readdir(&DirInf,&FileInf); 		/* ��ȡĿ¼��������Զ����� */
		if (result != FR_OK || FileInf.fname[0] == 0)
		{
			return 1;
		}
		if (FileInf.fname[0] == '.')
		{
			continue;
		}
		if (FileInf.fattrib & AM_DIR)
		{
			Bsp_Printf("(0x%02d)contents	|	%llud	|	%s	|	%s\r\n", FileInf.fattrib, FileInf.fsize, FileInf.fname, FileInf.altname);
			return 0;
		}
		else
		{
			Bsp_Printf("(0x%02d)files		|	%llud	|	%s	|	%s\r\n", FileInf.fattrib, FileInf.fsize, FileInf.fname, FileInf.altname);
			return 0;
		}
	}
}

/**
  * @brief ��SD������һ�����ļ�
  * @param path,Ҫ�����ļ����ڵ�Ŀ¼
  * @param fname,Ҫ�������ļ��������������·��
  * @retval 0:����;����:�˳�
  */
uint8_t Bsp_FatFs_CreateNewFile(const TCHAR* path, const TCHAR* fname)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FIL file;
	DIR DirInf;
	uint32_t bw;
	
	result = f_opendir(&DirInf, path);
	if (result != FR_OK)
	{
		Bsp_Printf("Open the directory %s failed (%d).\r\n", path, result);
		return 1;
	}
	result = f_open(&file, fname, FA_CREATE_ALWAYS | FA_WRITE);
	if (result !=  FR_OK)
	{
		Bsp_Printf("The File %s is opened failed (%d).\r\n",fname, result);
		return 1;
	}
	result = f_write(&file, "FatFS Write Test \r\n www.IEECHN.com \r\n", 34, &bw);
	f_close(&file);
	if (result == FR_OK)
	{
		Bsp_Printf("Creat file %s and write data success (%d).\r\n", fname, result);
		return 0;
	}
	else
	{
		Bsp_Printf("Creat file %s and write data failure (%d).\r\n", fname, result);
		return 0;
	}		
}

/**
  * @brief ��SD������һ����Ŀ¼
  * @param path,Ҫ�����ļ����ڵ�Ŀ¼
  * @retval 0:����;����:�˳�
  */
uint8_t Bsp_FatFs_CreateDir(const TCHAR* path)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;

	result = f_mkdir(path);
	if (result == FR_OK)
	{
		Bsp_Printf("Creat Directoy %s Success (%d).\r\n", path, result);
		return 0;
	}
	else if (result == FR_EXIST)
	{
		Bsp_Printf("The directory %s already existt (%d).\r\n", path, result);
		return 2;
	}
	else
	{
		Bsp_Printf("Creat Directoy %s Fail (%d).\r\n", path, result);
		return 1;
	}
}

/**
  * @brief ��SD��ɾ��һ���ļ����ļ���
  * @param path,Ҫ�������ļ����ļ�������Ŀ¼
  * @param fname,Ҫɾ�����ļ����ļ��У������Ǵ�����·��
  * @retval 0:����;����:�˳�
  */

uint8_t Bsp_FatFs_DeleteDirFile(const TCHAR* path, const TCHAR* fname)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	DIR DirInf;

	result = f_opendir(&DirInf, path);
	if (result != FR_OK)
	{
		Bsp_Printf("Open the directory %s failed (%d).\r\n", path, result);
		return 1;
	}
	result = f_unlink(fname);
	if (result == FR_OK)
	{
		Bsp_Printf("Delte The Directory %s success (%d).\r\n", fname, result);
		return 0;
	}
	else if (result == FR_NO_FILE)
	{
		Bsp_Printf("The Directory %s don't exist (%d). \r\n", fname, result);
		return 2;
	}
	else
	{
		Bsp_Printf("Delete The File Fail, The File Ready Only or the Directory isn't Empty (%d).\r\n", result);
		return 1;
	}
}

/**
  * @brief ��SD���е��ļ�������
  * @param path,Ҫ��ȡ���ļ�����Ŀ¼
  * @param fname,Ҫ��ȡ���ļ��������Ǵ�����·��
  * @param ReadDataBuff,�洢��ȡ�����ļ��ڲ�����
  * @retval 0:����;����:�˳�
  */

uint8_t Bsp_FatFs_ReadFileData(const TCHAR* path, const TCHAR* fname, char* ReadDataBuff)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FIL file;
	DIR DirInf;
	uint32_t bw;

	result = f_opendir(&DirInf, path);
	if (result != FR_OK)
	{
		Bsp_Printf("Open the directory %s failed (%d).\r\n", path, result);
		return 1;
	}
	result = f_open(&file, fname, FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
		Bsp_Printf("The File %s is opened failed (%d).\r\n",fname, result);
		return 1;
	}
	result = f_read(&file, ReadDataBuff, strlen(WriteDataBuff), &bw);
	f_close(&file);
	if (bw > 0)
	{
		ReadDataBuff[bw] = 0;
		Bsp_Printf("The contents in the file %s are : \r\n%s\r\n", fname, ReadDataBuff);
		return 0;
	}
	else
	{
		Bsp_Printf("The file %s is empty. \r\n", fname);
		return 0;
	}

}

/**
  * @brief дSD���е��ļ�������
  * @param path,Ҫд���ļ�����Ŀ¼
  * @param fname,Ҫд���ļ��������Ǵ�����·��
  * @param WriteDataBuff,�洢Ҫд���ļ��ڲ�������
  * @retval 0:����;����:�˳�
  */

uint8_t Bsp_FatFs_WriteFileData(const TCHAR* path, const TCHAR* fname, char* WriteDataBuff)
{
	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FIL file;
	DIR DirInf;
	uint32_t bw;

	result = f_opendir(&DirInf, path);
	if (result != FR_OK)
	{
		Bsp_Printf("Open the directory %s failed (%d).\r\n", path, result);
		return 1;
	}
	result = f_open(&file, fname, FA_CREATE_ALWAYS | FA_WRITE);
	if (result !=  FR_OK)
	{
		Bsp_Printf("The File %s is opened failed (%d).\r\n",fname, result);
		return 1;
	}	
	result = f_write(&file, WriteDataBuff, strlen(WriteDataBuff), &bw);
	f_close(&file);
	if (bw > 0)
	{
		Bsp_Printf("Write The Contents to the file %s sucess (%d).\r\n", fname, result);
		return 0;
	}
	else
	{
		Bsp_Printf("The file %s is empty. \r\n", fname);
		return 0;
	}

}

/** @}
*/
/****************************FasFt_User Exported Functions Group2*********************/
/** @}
*/
/*----------------------------------FasFt_User Exported Functions------------------------------------*/
/** @}
*/
/*----------------------------------------------FasFt_User-------------------------------------------*/
/** @}
*/
/*--------------------------------------STM32H743II_CoreBoard--------------------------------------*/
/*************************************************END OF FILE***************************************/
