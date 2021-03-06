#include "network.h"
//#include "sht11.h"
#include "system_stm32f10x.h"
#include "global.h"
extern unsigned char Real_TEMP[5];
extern unsigned char Real_HUMID[4];
void initTheNode()
{
// 	#if TheAddress==0x00
// 		//thenode.attri=1;;//0=不可再挂路由节点,1=可以再挂路由节点
// 		thenode.net_depth=0x00;//节点网络深度
// 	#else
		thenode.net_depth=0xFF;//节点网络深度
		thenode.attri=1;//0=不可再挂路由节点,1=可以再挂路由节�
		thenode.nodeid=TheAddress;
}

void	commandPackage(uint8_t command,uint8_t destaddr)//命令，父节点地址，目的节点地址
{			
		ReadyDataTo(destaddr);
		AT86RF212SendData.Data[0]=0xFF;
		AT86RF212SendData.Data[1]=command;
		AT86RF212SendData.Data[2]=thenode.net_depth;	//先不考虑睡眠及RTC同步
		AT86RF212SendData.Data[3]=thenode.attri;
		AT86RF212SendData.Data[4]='\n';
}
void	dataPackage()//封装自身数据包用于上传温湿度数据到父节点
{
		ReadyDataTo(thenode.fanode);	
		AT86RF212SendData.Data[0]=0xAB;
		AT86RF212SendData.Data[1]='#';
		AT86RF212SendData.Data[2]=Real_TEMP[3];//数据4-12为不变数据
		AT86RF212SendData.Data[3]=Real_TEMP[2];
		AT86RF212SendData.Data[4]=Real_TEMP[1];
		AT86RF212SendData.Data[5]=Real_TEMP[0];
		AT86RF212SendData.Data[6]=Real_HUMID[2];
		AT86RF212SendData.Data[7]=Real_HUMID[1];
		AT86RF212SendData.Data[8]=Real_HUMID[0];
		AT86RF212SendData.Data[9]=thenode.nodeid+'0';//数据包源地址
		AT86RF212SendData.Data[10]=thenode.fanode+'0';	//数据包源地址的父节点
		AT86RF212SendData.Data[11]='*';
		AT86RF212SendData.Data[12]='\n';
}



void transferPackage()  //转发包 除0x00外均在转发
{		
		uint8_t count;
		ReadyDataTo(thenode.fanode);	
		for(count=0;count<12;count++)
		AT86RF212SendData.Data[count]=*(AT86RF212ReceiveData.RxBuffer+DataStartPos+count);
}

uint8_t scanNeighborNodes(NeighborNode* neighborTable)
{ 
	uint8_t icount=0,i=0,search_count=0;
	while(search_count<5)
	{
		while(icount<30)//这样只会搜索到一个邻接节点，外层再嵌套while？
		{	
			AT86RF212_IRQServe();
			if(RF212RevSuccess == TRUE)
			{
					if(*(AT86RF212ReceiveData.RxBuffer+DestPos)==TheAddress)
					{
						break;
					}
			}
			icount++;
		}
		if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==I_AM_READY))
		{
			neighborTable[i].ed=networkED;
			neighborTable[i].net_deapth=*(AT86RF212ReceiveData.RxBuffer+DataStartPos+2);
			neighborTable[i].attri=*(AT86RF212ReceiveData.RxBuffer+DataStartPos+3);
			i++;
		}//他们写的用delay来延时，我们已经通过中断进行了，并且在中断外部嵌套了循环，应该不用delay了吧
		search_count++;
	}
	return i;
}
