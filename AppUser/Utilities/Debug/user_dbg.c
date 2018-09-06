/**
  ******************************************************************************
  * @file    STM32H7_CoreBoard/AppUser/Utilities/Debug/user_dbg.c
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
  * @brief The USER_DEBUG allows users to modify function parameters without reflash
  * @{
  */

/** @defgroup USER_DEBUG_Private_Variables USER_DEBUG Private Variables
  * @{
  */
/**
  * @brief	USER_DEBUG 系统命令
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
  * @brief 处理系统指令
  * @param *str: 函数名.
  * @retval 0,成功处理;其他,错误代码
  */

uint8_t usmart_sys_cmd_exe(uint8_t *str)
{
	uint8_t i;
	uint8_t sfname[MAX_FNAME_LEN];//存放本地函数名
	uint8_t pnum;
	uint8_t rval;
	uint32_t res;  
	res=usmart_get_cmdname(str,sfname,&i,MAX_FNAME_LEN);//得到指令及指令长度
	if(res)return USMART_FUNCERR;//错误的指令 
	str+=i;	 	 			    
	for(i=0;i<sizeof(sys_cmd_tab)/4;i++)//支持的系统指令
	{
		if(usmart_strcmp(sfname,sys_cmd_tab[i])==0)break;
	}
	switch(i)
	{					   
		case 0:
		case 1://帮助指令
			Bsp_Printf("\r\n");
#if USMART_USE_HELP 
			Bsp_Printf("------------------------USER_DEBUG V1.0------------------------ \r\n");
			Bsp_Printf("USER_DEBUG is transplated from ALIENTEK. It can provide users to manage some basis functions on-chip-fly.\r\n");
			Bsp_Printf("There are six instructs for USER_DEBUG modules:\r\n\r\n");
			Bsp_Printf("help:   	Get the help information.\r\n");
			Bsp_Printf("list:   	Avaiable functions.\r\n");
			Bsp_Printf("id:     	The ID of the avaiable functions.\r\n");
			Bsp_Printf("hex:    	Display as HEX.\r\n");
			Bsp_Printf("dec:    	Display as DEC.\r\n");
			Bsp_Printf("runtime:	1,Start time keeping.;0,Close time keeping.\r\n");
#else
			Bsp_Printf("Please enable the USMART_USE_HELP .\r\n");
#endif
			break;
		case 2://查询指令
			Bsp_Printf("\r\n");
			Bsp_Printf("-------------------------Function List--------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)Bsp_Printf("%s\r\n",usmart_dev.funs[i].name);
			Bsp_Printf("\r\n");
			break;	 
		case 3://查询ID
			Bsp_Printf("\r\n");
			Bsp_Printf("-------------------------Function ID --------------------------- \r\n");
			for(i=0;i<usmart_dev.fnum;i++)
			{
				usmart_get_fname((uint8_t*)usmart_dev.funs[i].name,sfname,&pnum,&rval);//得到本地函数名 
				Bsp_Printf("%s id is:\r\n0X%08X\r\n",sfname,usmart_dev.funs[i].func); //显示ID
			}
			Bsp_Printf("\r\n");
			break;
		case 4://hex指令
			Bsp_Printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//参数正常
			{
				i=usmart_str2num(sfname,&res);	   	//记录该参数	
				if(i==0)						  	//进制转换功能
				{
					Bsp_Printf("HEX:0X%X\r\n",res);	   	//转为16进制
				}else if(i!=4)return USMART_PARMERR;//参数错误.
				else 				   				//参数显示设定功能
				{
					Bsp_Printf("Hex display!\r\n");
					usmart_dev.sptype=SP_TYPE_HEX;  
				}

			}else return USMART_PARMERR;			//参数错误.
			Bsp_Printf("\r\n"); 
			break;
		case 5://dec指令
			Bsp_Printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//参数正常
			{
				i=usmart_str2num(sfname,&res);	   	//记录该参数	
				if(i==0)						   	//进制转换功能
				{
					Bsp_Printf("DEC:%lu\r\n",res);	   	//转为10进制
				}else if(i!=4)return USMART_PARMERR;//参数错误.
				else 				   				//参数显示设定功能
				{
					Bsp_Printf("Decimal display!\r\n");
					usmart_dev.sptype=SP_TYPE_DEC;  
				}

			}else return USMART_PARMERR;			//参数错误. 
			Bsp_Printf("\r\n"); 
			break;	 
		case 6://runtime指令,设置是否显示函数执行时间
			Bsp_Printf("\r\n");
			usmart_get_aparm(str,sfname,&i);
			if(i==0)//参数正常
			{
				i=usmart_str2num(sfname,&res);	   		//记录该参数	
				if(i==0)						   		//读取指定地址数据功能
				{
					if(USMART_ENTIMX_SCAN==0)Bsp_Printf("\r\nError! \r\nTo EN RunTime function,Please set USMART_ENTIMX_SCAN = 1 first!\r\n");//报错
					else
					{
						usmart_dev.runtimeflag=res;
						if(usmart_dev.runtimeflag)Bsp_Printf("Run Time Calculation ON\r\n");
						else Bsp_Printf("Run Time Calculation OFF\r\n"); 
					}
				}else return USMART_PARMERR;   			//未带参数,或者参数错误	 
 			}else return USMART_PARMERR;				//参数错误. 
			Bsp_Printf("\r\n"); 
			break;	    
		default://非法指令
			return USMART_FUNCERR;
	}
	return 0;
}

#if USMART_ENTIMX_SCAN == 1

//移植注意:本例是以stm32为例,如果要移植到其他mcu,请做相应修改.
//usmart_reset_runtime,清除函数运行时间,连同定时器的计数寄存器以及标志位一起清零.并设置重装载值为最大,以最大限度的延长计时时间.
//usmart_get_runtime,获取函数运行时间,通过读取CNT值获取,由于usmart是通过中断调用的函数,所以定时器中断不再有效,此时最大限度
//只能统计2次CNT的值,也就是清零后+溢出一次,当溢出超过2次,没法处理,所以最大延时,控制在:2*计数器CNT*0.1ms.对STM32来说,是:13.1s左右

/**
  * @brief 复位runtime,需要根据所移植到的MCU的定时器参数进行修改
  * 	   在每次USMART调用函数之前执行，清除计数器，然后在函数执行完后调用usmart_get_runtime()来获取整个函数运行时间
  */

void usmart_reset_runtime(void)
{
    __HAL_TIM_CLEAR_FLAG(&TIM4_Handler,TIM_FLAG_UPDATE);//清除中断标志位 
    __HAL_TIM_SET_AUTORELOAD(&TIM4_Handler,0XFFFF);     //将重装载值设置到最大
    __HAL_TIM_SET_COUNTER(&TIM4_Handler,0);             //清空定时器的CNT
	usmart_dev.runtime=0;	
}

/**
  * @brief 获得runtime时间, 需要根据所移植到的MCU的定时器参数进行修改
  * @retval 执行时间,单位:0.1ms,最大延时时间为定时器CNT值的2倍*0.1ms
  *			由于调用的函数都是在中断中执行的，所以执行时间统计功能最多可以统计定时器溢出1次的时间
  *			定时器4为16位最大计数65535，计时周期为10KHz(0.1ms)，所以最长计时时间为13.1s
  */

uint32_t usmart_get_runtime(void)
{
	if(__HAL_TIM_GET_FLAG(&TIM4_Handler,TIM_FLAG_UPDATE)==SET)//在运行期间,产生了定时器溢出
	{
		usmart_dev.runtime+=0XFFFF;
	}
	usmart_dev.runtime+=__HAL_TIM_GET_COUNTER(&TIM4_Handler);
	return usmart_dev.runtime;		//返回计数值
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
  * @brief 初始化串口控制器
  * @param sysclk:系统时钟（Mhz）
  */

void usmart_init(void)
{
#if USMART_ENTIMX_SCAN==1
	Bsp_InitHardTimer_TIM4(1000,(uint32_t)200*100-1);//分频,时钟为10K ,100ms中断一次,注意,计数频率必须为10Khz,以和runtime单位(0.1ms)同步.
#endif
	usmart_dev.sptype=1;	//十六进制显示参数
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
  * @brief 从str中获取函数名,id,及参数信息
  * @param *str:字符串指针.
  * @retval 返回值:0,识别成功;其他,错误代码.
  */

uint8_t usmart_cmd_rec(uint8_t*str) 
{
	uint8_t sta,i,rval;//状态	 
	uint8_t rpnum,spnum;
	uint8_t rfname[MAX_FNAME_LEN];//暂存空间,用于存放接收到的函数名  
	uint8_t sfname[MAX_FNAME_LEN];//存放本地函数名
	sta=usmart_get_fname(str,rfname,&rpnum,&rval);//得到接收到的数据的函数名及参数个数	  
	if(sta)return sta;//错误
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((uint8_t*)usmart_dev.funs[i].name,sfname,&spnum,&rval);//得到本地函数名及参数个数
		if(sta)return sta;//本地解析有误	  
		if(usmart_strcmp(sfname,rfname)==0)//相等
		{
			if(spnum>rpnum)return USMART_PARMERR;//参数错误(输入参数比源函数参数少)
			usmart_dev.id=i;//记录函数ID.
			break;//跳出.
		}	
	}
	if(i==usmart_dev.fnum)return USMART_NOFUNCFIND;	//未找到匹配的函数
 	sta=usmart_get_fparam(str,&i);					//得到函数参数个数	
	if(sta)return sta;								//返回错误
	usmart_dev.pnum=i;								//参数个数记录
    return USMART_OK;
}

/**
  * @brief 	usamrt执行函数,该函数用于最终执行从串口收到的有效函数.
  * 		最多支持10个参数的函数,更多的参数支持也很容易实现.不过用的很少.一般5个左右的参数的函数已经很少见了.
  *			该函数会在串口打印执行情况.以:"函数名(参数1，参数2...参数N)=返回值".的形式打印.
  *			当所执行的函数没有返回值的时候,所打印的返回值是一个无意义的数据.
  */

void usmart_exe(void)
{
	uint8_t id,i;
	uint32_t res;		   
	uint32_t temp[MAX_PARM];//参数转换,使之支持了字符串 
	uint8_t sfname[MAX_FNAME_LEN];//存放本地函数名
	uint8_t pnum,rval;
	id=usmart_dev.id;
	if(id>=usmart_dev.fnum)return;//不执行.
	usmart_get_fname((uint8_t*)usmart_dev.funs[id].name,sfname,&pnum,&rval);//得到本地函数名,及参数个数 
	Bsp_Printf("\r\n%s(",sfname);//输出正要执行的函数名
	for(i=0;i<pnum;i++)//输出参数
	{
		if(usmart_dev.parmtype&(1<<i))//参数是字符串
		{
			Bsp_Printf("%c",'"');			 
			Bsp_Printf("%s",usmart_dev.parm+usmart_get_parmpos(i));
			Bsp_Printf("%c",'"');
			temp[i]=(uint32_t)&(usmart_dev.parm[usmart_get_parmpos(i)]);
		}else						  //参数是数字
		{
			temp[i]=*(uint32_t*)(usmart_dev.parm+usmart_get_parmpos(i));
			if(usmart_dev.sptype==SP_TYPE_DEC)Bsp_Printf("%ld",temp[i]);//10进制参数显示
			else Bsp_Printf("0X%X",temp[i]);//16进制参数显示 	   
		}
		if(i!=pnum-1)Bsp_Printf(",");
	}
	Bsp_Printf(")");
#if USMART_ENTIMX_SCAN==1
	usmart_reset_runtime();	//计时器清零,开始计时
#endif
	switch(usmart_dev.pnum)
	{
		case 0://无参数(void类型)											  
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)();
			break;
	    case 1://有1个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0]);
			break;
	    case 2://有2个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1]);
			break;
	    case 3://有3个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2]);
			break;
	    case 4://有4个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3]);
			break;
	    case 5://有5个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4]);
			break;
	    case 6://有6个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5]);
			break;
	    case 7://有7个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6]);
			break;
	    case 8://有8个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7]);
			break;
	    case 9://有9个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8]);
			break;
	    case 10://有10个参数
			res=(*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0],temp[1],temp[2],temp[3],temp[4],\
			temp[5],temp[6],temp[7],temp[8],temp[9]);
			break;
	}
#if USMART_ENTIMX_SCAN==1
	usmart_get_runtime();//获取函数执行时间
#endif
	if(rval==1)//需要返回值.
	{
		if(usmart_dev.sptype==SP_TYPE_DEC)Bsp_Printf("=%lu;\r\n",res);//输出执行结果(10进制参数显示)
		else Bsp_Printf("=0X%X;\r\n",res);//输出执行结果(16进制参数显示)	   
	}else Bsp_Printf(";\r\n");		//不需要返回值,直接输出结束
	if(usmart_dev.runtimeflag)	//需要显示函数执行时间
	{ 
		Bsp_Printf("Function Run Time:%d.%1dms\r\n",usmart_dev.runtime/10,usmart_dev.runtime%10);//打印函数执行时间 
	}	
}

/**
  * @brief 	usmart扫描函数,通过调用该函数,实现usmart的各个控制.该函数需要每隔一定时间被调用一次,以及时执行从串口发过来的各个函数.
  * 		本函数可以在中断里面调用,从而实现自动管理.
  */

void usmart_scan(void)
{
	uint8_t sta,len;  
	if(USART_RX_STA&0x8000)//串口接收完成？
	{					   
		len=USART_RX_STA&0x3fff;	//得到此次接收到的数据长度
		USART_RX_BUF[len]='\0';	//在末尾加入结束符. 
		sta=usmart_dev.cmd_rec(USART_RX_BUF);//得到函数各个信息
		if(sta==0)usmart_dev.exe();	//执行函数 
		else 
		{  
			len=usmart_sys_cmd_exe(USART_RX_BUF);
			if(len!=USMART_FUNCERR)sta=len;
			if(sta)
			{   
				switch(sta)
				{
					case USMART_FUNCERR:
						Bsp_Printf("函数错误!\r\n");
                      				
						break;	
					case USMART_PARMERR:
						Bsp_Printf("参数错误!\r\n");   			
						break;				
					case USMART_PARMOVER:
						Bsp_Printf("参数太多!\r\n");   			
						break;		
					case USMART_NOFUNCFIND:
						Bsp_Printf("未找到匹配的函数!\r\n");   			
						break;		
				}
			}
		}
		USART_RX_STA=0;//状态寄存器清空	    
	}
}

#if USMART_USE_WRFUNS==1 	//如果使能了读写操作

/**
  * @brief 取指定地址的值		 
  */

uint32_t read_addr(uint32_t addr)
{
	return *(uint32_t*)addr;//	
}

/**
  * @brief 在指定地址写入指定的值		 
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
