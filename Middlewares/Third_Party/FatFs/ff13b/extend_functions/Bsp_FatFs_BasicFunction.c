#include "ff.h"
#include "Bsp_FatFs_BasicFunction.h"	 
#include "Bsp_FatFs_ExtendFunctions.h"

BYTE Buff[4096] __attribute__ ((aligned (4))) ;	/* Working buffer */  
//ΪSD��ע�Ṥ����	 
//path:����·��������"0:"��"1:"
//mt:0��������ע�ᣨ�Ժ�ע�ᣩ��1������ע��
//����ֵ:ִ�н��
uint8_t Bsp_fmount_SDCARD(uint8_t* path,uint8_t mt)
{		   
	return f_mount(fs[0],(const TCHAR*)path,mt); 
}

//ΪSD��ע�Ṥ����	 
//path:����·��������"0:"��"1:"
//mt:0��������ע�ᣨ�Ժ�ע�ᣩ��1������ע��
//����ֵ:ִ�н��
uint8_t Bsp_fmount_FLASH(uint8_t* path,uint8_t mt)
{		   
	return f_mount(fs[1],(const TCHAR*)path,mt); 
}

//��·���µ��ļ�
//path:·��+�ļ���
//mode:��ģʽ
//����ֵ:ִ�н��
uint8_t Bsp_fopen(uint8_t*path,uint8_t mode)
{
	uint8_t res;	 
	res=f_open(file,(const TCHAR*)path,mode);//���ļ���
	return res;
} 
//�ر��ļ�
//����ֵ:ִ�н��
uint8_t Bsp_fclose(void)
{
	f_close(file);
	return 0;
}
//��������
//len:�����ĳ���
//����ֵ:ִ�н��
uint8_t Bsp_fread(uint16_t len)
{
	uint16_t i,t;
	uint8_t res=0;
	uint16_t tlen=0;
	Bsp_Printf("\r\nRead file data is:\r\n");
	for(i=0;i<len/512;i++)
	{
		res=f_read(file,fatbuf,512,&br);
		if(res)
		{
			Bsp_Printf("Read Error:%d\r\n",res);
			break;
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)Bsp_Printf("%c",fatbuf[t]); 
		}
	}
	if(len%512)
	{
		res=f_read(file,fatbuf,len%512,&br);
		if(res)	//�����ݳ�����
		{
			Bsp_Printf("\r\nRead Error:%d\r\n",res);   
		}else
		{
			tlen+=br;
			for(t=0;t<br;t++)Bsp_Printf("%c",fatbuf[t]); 
		}	 
	}
	if(tlen)Bsp_Printf("\r\nReaded data len:%d\r\n",tlen);//���������ݳ���
	Bsp_Printf("Read data over\r\n");	 
	return res;
}
//д������
//dat:���ݻ�����
//len:д�볤��
//����ֵ:ִ�н��
uint8_t Bsp_fwrite(uint8_t*dat,uint16_t len)
{			    
	uint8_t res;	   					   

	Bsp_Printf("\r\nBegin Write file...\r\n");
	Bsp_Printf("Write data len:%d\r\n",len);	 
	res=f_write(file,dat,len,&bw);
	if(res)
	{
		Bsp_Printf("Write Error:%d\r\n",res);   
	}else Bsp_Printf("Writed data len:%d\r\n",bw);
	Bsp_Printf("Write data over.\r\n");
	return res;
}

//��Ŀ¼
 //path:·��
//����ֵ:ִ�н��
uint8_t Bsp_fopendir(uint8_t* path)
{
	return f_opendir(&dir,(const TCHAR*)path);	
}
//�ر�Ŀ¼ 
//����ֵ:ִ�н��
uint8_t Bsp_fclosedir(void)
{
	return f_closedir(&dir);	
}
//���ȡ�ļ���
//����ֵ:ִ�н��
uint8_t Bsp_freaddir(void)
{
	uint8_t res;    
	res=f_readdir(&dir,&fileinfo);	//��ȡһ���ļ�����Ϣ
	if(res!=FR_OK)return res;		//������ 
	Bsp_Printf("\r\n DIR info:\r\n");

	Bsp_Printf("dir.dptr:%d\r\n",dir.dptr);
	Bsp_Printf("dir.obj.id:%d\r\n",dir.obj.id);
	Bsp_Printf("dir.obj.sclust:%d\r\n",dir.obj.sclust);
	Bsp_Printf("dir.obj.objsize:%lld\r\n",dir.obj.objsize);
	Bsp_Printf("dir.obj.c_ofs:%d\r\n",dir.obj.c_ofs);
	Bsp_Printf("dir.clust:%d\r\n",dir.clust);
	Bsp_Printf("dir.sect:%d\r\n",dir.sect);	  
	Bsp_Printf("dir.blk_ofs:%d\r\n",dir.blk_ofs);	  

	Bsp_Printf("\r\n");
	Bsp_Printf("File Name is:%s\r\n",fileinfo.fname);
	Bsp_Printf("File Size is:%lld\r\n",fileinfo.fsize);
	Bsp_Printf("File data is:%d\r\n",fileinfo.fdate);
	Bsp_Printf("File time is:%d\r\n",fileinfo.ftime);
	Bsp_Printf("File Attr is:%d\r\n",fileinfo.fattrib);
	Bsp_Printf("\r\n"); 
	return 0;
}			 

 //�����ļ�
 //path:·��
 //����ֵ:ִ�н��
uint8_t Bsp_fscan_files(uint8_t * path)
{
	FRESULT res;	 
    res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
    if (res == FR_OK) 
	{	
		Bsp_Printf("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
	        //if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
 			Bsp_Printf("%s/", path);//��ӡ·��	
			Bsp_Printf("%s\r\n",fileinfo.fname);//��ӡ�ļ���	  
		} 
    }	   
    return res;	  
}
//��ʾʣ������
//drv:�̷�
//����ֵ:ʣ������(�ֽ�)
uint32_t Bsp_fshowfree(uint8_t *drv)
{
	FATFS *fs1;
	uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//�õ���������
	    fre_sect = fre_clust * fs1->csize;			//�õ�����������	   
#if FF_MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		if(tot_sect<20480)//������С��10M
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    Bsp_Printf("\r\n����������:%d KB\r\n"
		           "���ÿռ�:%d KB\r\n",
		           tot_sect>>1,fre_sect>>1);
		}else
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    Bsp_Printf("\r\n����������:%d MB\r\n"
		           "���ÿռ�:%d MB\r\n",
		           tot_sect>>11,fre_sect>>11);
		}
	}
	return fre_sect;
}		    
//�ļ���дָ��ƫ��
//offset:����׵�ַ��ƫ����
//����ֵ:ִ�н��.
uint8_t Bsp_flseek(uint32_t offset)
{
	return f_lseek(file,offset);
}
//��ȡ�ļ���ǰ��дָ���λ��.
//����ֵ:λ��
uint32_t Bsp_ftell(void)
{
	return f_tell(file);
}
//��ȡ�ļ���С
//����ֵ:�ļ���С
uint32_t Bsp_fsize(void)
{
	return f_size(file);
} 
//����Ŀ¼
//pname:Ŀ¼·��+����
//����ֵ:ִ�н��
uint8_t Bsp_fmkdir(uint8_t*pname)
{
	return f_mkdir((const TCHAR *)pname);
}
//��ʽ��
//path:����·��������"0:"��"1:"
//mode:ģʽ
//au:�ش�С
//����ֵ:ִ�н��
uint8_t Bsp_fmkfs(uint8_t* path,uint8_t mode,uint16_t au, void* Buff, uint16_t SizeOfBuff)
{
	return f_mkfs((const TCHAR*)path, mode, au, Buff, SizeOfBuff);
} 
//ɾ���ļ�/Ŀ¼
//pname:�ļ�/Ŀ¼·��+����
//����ֵ:ִ�н��
uint8_t Bsp_funlink(uint8_t *pname)
{
	return  f_unlink((const TCHAR *)pname);
}

//�޸��ļ�/Ŀ¼����(���Ŀ¼��ͬ,�������ƶ��ļ�Ŷ!)
//oldname:֮ǰ������
//newname:������
//����ֵ:ִ�н��
uint8_t Bsp_frename(uint8_t *oldname,uint8_t* newname)
{
	return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}
//��ȡ�̷����������֣�
//path:����·��������"0:"��"1:"  
void Bsp_fgetlabel(uint8_t *path)
{
	uint8_t buf[20];
	uint32_t sn=0;
	uint8_t res;
	res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==FR_OK)
	{
		Bsp_Printf("\r\n����%s ���̷�Ϊ:%s\r\n",path,buf);
		Bsp_Printf("����%s �����к�:%X\r\n\r\n",path,sn); 
	}else Bsp_Printf("\r\n��ȡʧ�ܣ�������:%X\r\n",res);
}
//�����̷����������֣����11���ַ�������֧�����ֺʹ�д��ĸ����Լ����ֵ�
//path:���̺�+���֣�����"0:ALIENTEK"��"1:OPENEDV"  
void Bsp_fsetlabel(uint8_t *path)
{
	uint8_t res;
	res=f_setlabel ((const TCHAR *)path);
	if(res==FR_OK)
	{
		Bsp_Printf("\r\n�����̷����óɹ�:%s\r\n",path);
	}else Bsp_Printf("\r\n�����̷�����ʧ�ܣ�������:%X\r\n",res);
} 

//���ļ������ȡһ���ַ���
//size:Ҫ��ȡ�ĳ���
void Bsp_fgets(uint16_t size)
{
 	TCHAR* rbuf;
	rbuf=f_gets((TCHAR*)fatbuf,size,file);
	if(*rbuf==0)return  ;//û�����ݶ���
	else
	{
		Bsp_Printf("\r\nThe String Readed Is:%s\r\n",rbuf);  	  
	}			    	
}
//��Ҫ_USE_STRFUNC>=1
//дһ���ַ����ļ�
//c:Ҫд����ַ�
//����ֵ:ִ�н��
uint8_t Bsp_fputc(uint8_t c)
{
	return f_putc((TCHAR)c,file);
}
//д�ַ������ļ�
//c:Ҫд����ַ���
//����ֵ:д����ַ�������
uint8_t Bsp_fputs(uint8_t*c)
{
	return f_puts((TCHAR*)c,file);
}













