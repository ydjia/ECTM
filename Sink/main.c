#include "io.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "at86rf212.h"
#include "spi.h"
#include "oled.h"
#include "network.h"
#include "global.h"
#include "sourcetype.h"

//MCU:STM32F101CB  WirelessChip:AT86RF212
//问题：1.不知道为什么把串口调到9600时就会出现乱码�
//      2.printf使用%s输出字符串时，最后一位务必是'\0'否则有可能会导致输出结果出错。printf输出字符数组是从第0个开始输出的。
//如果发现I2C总线或者SPI总线设置正确但无法操作时应该考虑一下是不是系统时钟初始化有问题。（F101和F103支持的最高频率不一样！）

//在写接收端的机子时必须记得更改自己的短地址。

//与云天创模块混合在同一个网络中 ————20130513 OKIMBIN
//汇聚节点代码  节点编号0x00
int main(void) {
	 uint8_t tempi=0,k=0;
	 char scan_flag=1;
   uint8_t tlength;
	 uint8_t net_deapth_temp=0;
	 uint8_t t,i=0;
	 uint8_t attri_num=0;
	 tempi=0;
	 scan_flag=1;
	 net_deapth_temp=0;
	 SystemInit();//初始化为24M
	 delay_init(24);
	 NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
 	 IO_Init();//端口及各种时钟使能。
 	 uart_init(4800);//不知为何波特率居然为9600.
	 
	 initTheNode();
	 delay_ms(20);//SHT11上电后需要延时超过11ms后才能读数
	 SPI1_Init(); //初始化SPI总线
	 AT86RF212_Init();	//初始化RF212芯片
	 delay_ms(200);
	 
	 Green_LED=1;
	 OLED_Init();
 	 Welcome();
	 OLED_Refresh_Gram();
 	 delay_ms(1500);
	 	
	while(1)
	{	
	
		AT86RF212_IRQServe();//查看有没有回复，注意此程序没有写终端节点的处理方式
		//CoreRed_LED=0;//和传感节点相反，亮
		
		if(RF212RevSuccess == TRUE)
		{			
					RF212RevSuccess=0;
					if((*(AT86RF212ReceiveData.RxBuffer+DestPos)==TheAddress)||(*(AT86RF212ReceiveData.RxBuffer+DestPos)==0xff))
					{
						if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)
							&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==I_WANT_JOIN_YOU))
						{
							printf("Receive %c JOIN_ME\n",*(AT86RF212ReceiveData.RxBuffer+SourcePos)+'0');
							if (attri_num<2)
							{	
								for(k=0;k<9;k++)
								{
									commandPackage(YOU_CAN_JOIN,*(AT86RF212ReceiveData.RxBuffer+SourcePos));
									AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//回复我是你爸
									delay_ms(500);
									while(RF212SendFinish==TRUE)
									{	
										RF212SendFinish=0;		
										AT86RF212_IRQServe();
										delay_ms(100);
										CoreRed_LED=0;//和传感节点相反，亮
										delay_ms(500);
										CoreRed_LED=1;//和传感节点相反，暗
										delay_ms(500);
									}
								}
								attri_num++;
								printf("%d",attri_num);								
							}
							else
								thenode.attri=0;
						}
						else if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)
							&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==I_WANT_JOIN_BROADCAST))
						{	
							printf("Receive %c BROADCAST\n",*(AT86RF212ReceiveData.RxBuffer+SourcePos)+'0');
							commandPackage(I_AM_READY,*(AT86RF212ReceiveData.RxBuffer+SourcePos));//广播发送发送ready回复
							AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));
							delay_ms(500);
							while(RF212SendFinish==TRUE)
							{	
								RF212SendFinish=0;		
								AT86RF212_IRQServe();
							}	
						}
						else if(*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xAB)
						{
							//本机短地址为0x00
							//printf("ID:%d ",*(AT86RF212ReceiveData.RxBuffer+SourcePos));
							TempBuf[*(AT86RF212ReceiveData.RxBuffer+SourcePos)]//数据位减0，地址位无需减0
								=(*(AT86RF212ReceiveData.RxBuffer+TempStartPos)-'0')*1000+
								 (*(AT86RF212ReceiveData.RxBuffer+TempStartPos+1)-'0')*100+
								 (*(AT86RF212ReceiveData.RxBuffer+TempStartPos+2)-'0')*10+
							   (*(AT86RF212ReceiveData.RxBuffer+TempStartPos+3)-'0')*1;
							HumidBuf[*(AT86RF212ReceiveData.RxBuffer+SourcePos)]
								=(*(AT86RF212ReceiveData.RxBuffer+HumidStartPos)-'0')*100+
								 (*(AT86RF212ReceiveData.RxBuffer+HumidStartPos+1)-'0')*10+
								 (*(AT86RF212ReceiveData.RxBuffer+HumidStartPos+2)-'0')*1;
								
							for(tempi=DataStartPos ;tempi<=DataEndPos;tempi++)
								printf("%c",*(AT86RF212ReceiveData.RxBuffer+tempi));
							//printf("RSSI:%d\n",AT86RF212ReceiveData.RSSI);
							//printf("ENERGY:%d\n",AT86RF212ReceiveData.ENERGY);
							RF212RevSuccess =0;
							CoreGreen_LED=0;
							delay_ms(500);
							CoreGreen_LED=1;
							delay_ms(500);
						}
					}
		}//按键后OLED 显示
		OLED_Display();//蓝色按钮控制OLED的数据显示
	}
}