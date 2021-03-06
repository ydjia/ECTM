#include "io.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "at86rf212.h"
#include "spi.h"
#include "sht11.h"
#include "network.h"
#include "sourcetype.h"
#include "global.h"
//云天创F5模块传感器节点代码
//
//日期：2013.05.08
//外部晶振：16MHz

//MCU:STM32F101CB  WirelessChip:AT86RF212
//问题：1.不知道为什么把串口调到9600时就会出现乱码�
//      2.printf使用%s输出字符串时，最后一位务必是'\0'否则有可能会导致输出结果出错。printf输出字符数组是从第0个开始输出的。
//如果发现I2C总线或者SPI总线设置正确但无法操作时应该考虑一下是不是系统时钟初始化有问题。（F101和F103支持的最高频率不一样！）
//			3.如果时钟不是默认设置的72M的话，可能会导致delay_init函数里延时时间不准确。
//
//
//在写接收端的机子时必须记得更改自己的短地址。

TheNodeType thenode;

 int main(void) 
 {
	 uint8_t tempi=0;
	 NeighborNode neighbortable[5]={'\0'};
	 char scan_flag=1;
   uint8_t tlength;
	 uint8_t net_deapth_temp=0;
	 uint8_t ed_m=0;
	 uint8_t saddr_m=0xFE;
	 uint8_t ED_LEVEL=0x15;
	 uint8_t t,k,i;
 START:
	 tempi=0;
	 scan_flag=1;
	 net_deapth_temp=0;
	 ed_m=0;
	 saddr_m=0xFE;
	 k=0;
	 
	 for(t=0;t<5;t++)
	 {
		 neighbortable[t].ed=0;
		 neighbortable[t].saddr=0xFF;
		 neighbortable[t].net_deapth=0xFF;
		}
	 SystemInit();//初始化为24M
	 delay_init(24);
	 NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	 IO_Init();//端口及各种时钟使能。
 	 uart_init(4800);//不知为何波特率居然为9600.
	 
	 initTheNode();
	 SHT_EN=1;
	 delay_ms(20);//SHT11上电后需要延时超过11ms后才能读数
	 SHT_GPIO_Configuration();//初始化SHT11时钟
	 SPI1_Init(); //初始化SPI总线
	 AT86RF212_Init();	//初始化RF212芯片
	 delay_ms(200);
	 
	 Green_LED=0;
	 Red_LED=0;
	 commandPackage(I_WANT_JOIN_BROADCAST,0xff);	
	 AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//发送入网广播
	 AT86RF212_IRQServe();//外面也应该读一遍啊
	 while(RF212SendFinish==TRUE)
	 {	
		 RF212SendFinish=0;
		 printf("GoodJob\n");			
		 Green_LED=1;//亮
		 delay_ms(500);
		 Green_LED=0;
		 delay_ms(500);
		 AT86RF212_IRQServe();
		 delay_ms(100);
	 }	
	 tlength=scanNeighborNodes(neighbortable);
	 while(scan_flag)
	 {
			for(i=0;i<tlength;i++)//寻找ED最大值
			{
			//按网络深度检索邻居表
				if(neighbortable[i].net_deapth == net_deapth_temp&&neighbortable[i].ed>=ED_LEVEL&&neighbortable[i].ed>ed_m
					&&neighbortable[i].attri==1)
				{
				//当前网络深度若有节点取出强度最大的
					ed_m=neighbortable[i].ed;
					saddr_m=neighbortable[i].saddr;
				}
			}
			if(net_deapth_temp>3)
				scan_flag=0;
			if(ed_m<=0||saddr_m==0xFE)	
				net_deapth_temp++;//该检索网络深度中没有节点则检索网络深度加1
			else
				scan_flag=0;
	}
  if(ed_m<=0 || tlength==0)//未找到网络
	{
			goto START;
	}
  commandPackage(I_WANT_JOIN_YOU,saddr_m);
	AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//发送当儿子信令
	AT86RF212_IRQServe();//外面也应该读一遍啊
		while(RF212SendFinish==TRUE)
		{	
			RF212SendFinish=0;
			AT86RF212_IRQServe();
			delay_ms(100);
		}	
	while(k<20)
	{
		 AT86RF212_IRQServe();//查看有没有回复
		 if(RF212RevSuccess == TRUE)
		 { 	
				RF212RevSuccess=0;
				if(*(AT86RF212ReceiveData.RxBuffer+DestPos)==TheAddress)
				{
					if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==YOU_CAN_JOIN))//收到回复，做父节点
					{
						thenode.fanode=*(AT86RF212ReceiveData.RxBuffer+SourcePos);
						thenode.net_depth=*(AT86RF212ReceiveData.RxBuffer+DataStartPos+2)+1;
						Red_LED=1;
						break;
					}
				}
		 }
		 else
		 {
				delay_ms(100);
				k++;
		 }
	}
	if(k>=20)//超过循环次数未收到回复
			goto START;
	
	while(1)
	{
			Calculate_SHT(Real_TEMP,Real_HUMID);
			dataPackage();
			AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	
			AT86RF212_IRQServe();//查看有没有回复，注意此程序没有写终端节点的处理方式
			while(RF212SendFinish==TRUE)
			{
				RF212SendFinish=0;
				AT86RF212_IRQServe();
				delay_ms(100);
			}	
			delay_ms(500);
			AT86RF212_IRQServe();
			while(RF212RevSuccess == TRUE)
			{		
					RF212RevSuccess=0;
					Green_LED=1;
					if(*(AT86RF212ReceiveData.RxBuffer+DestPos)==TheAddress)
					{
						if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)
							&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==I_WANT_JOIN_YOU))
							{
								commandPackage(YOU_CAN_JOIN,*(AT86RF212ReceiveData.RxBuffer+SourcePos));
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//回复我是你爸
								AT86RF212_IRQServe();//外面也应该读一遍啊
								while(RF212SendFinish==TRUE)
								{	
									RF212SendFinish=0;
									AT86RF212_IRQServe();
									delay_ms(100);
								}	
							}
						else if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)
							&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==I_WANT_JOIN_BROADCAST))
							{
								commandPackage(I_AM_READY,*(AT86RF212ReceiveData.RxBuffer+SourcePos));//广播发送发送ready回复
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));
								AT86RF212_IRQServe();//外面也应该读一遍啊
								while(RF212SendFinish==TRUE)
								{	
									RF212SendFinish=0;
									AT86RF212_IRQServe();
									delay_ms(100);
								}	
							}
						else if(*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xAB)
							{
								transferPackage();
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));//如果是数据包转发
								AT86RF212_IRQServe();//外面也应该读一遍啊
								while(RF212SendFinish==TRUE)
								{	
									RF212SendFinish=0;
									AT86RF212_IRQServe();
									delay_ms(100);
								}	
							}
					}
					AT86RF212_IRQServe();
			}
	}
}