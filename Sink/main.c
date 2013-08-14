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
//���⣺1.��֪��Ϊʲô�Ѵ��ڵ���9600ʱ�ͻ��������
//      2.printfʹ��%s����ַ���ʱ�����һλ�����'\0'�����п��ܻᵼ������������printf����ַ������Ǵӵ�0����ʼ����ġ�
//�������I2C���߻���SPI����������ȷ���޷�����ʱӦ�ÿ���һ���ǲ���ϵͳʱ�ӳ�ʼ�������⡣��F101��F103֧�ֵ����Ƶ�ʲ�һ������

//��д���ն˵Ļ���ʱ����ǵø����Լ��Ķ̵�ַ��

//�����촴ģ������ͬһ�������� ��������20130513 OKIMBIN
//��۽ڵ����  �ڵ���0x00
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
	 SystemInit();//��ʼ��Ϊ24M
	 delay_init(24);
	 NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	 IO_Init();//�˿ڼ�����ʱ��ʹ�ܡ�
 	 uart_init(4800);//��֪Ϊ�β����ʾ�ȻΪ9600.
	 
	 initTheNode();
	 delay_ms(20);//SHT11�ϵ����Ҫ��ʱ����11ms����ܶ���
	 SPI1_Init(); //��ʼ��SPI����
	 AT86RF212_Init();	//��ʼ��RF212оƬ
	 delay_ms(200);
	 
	 Green_LED=1;
	 OLED_Init();
 	 Welcome();
	 OLED_Refresh_Gram();
 	 delay_ms(1500);
	 	
	while(1)
	{	
	
		AT86RF212_IRQServe();//�鿴��û�лظ���ע��˳���û��д�ն˽ڵ�Ĵ���ʽ
		//CoreRed_LED=0;//�ʹ��нڵ��෴����
		
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
									AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//�ظ��������
									delay_ms(500);
									while(RF212SendFinish==TRUE)
									{	
										RF212SendFinish=0;		
										AT86RF212_IRQServe();
										delay_ms(100);
										CoreRed_LED=0;//�ʹ��нڵ��෴����
										delay_ms(500);
										CoreRed_LED=1;//�ʹ��нڵ��෴����
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
							commandPackage(I_AM_READY,*(AT86RF212ReceiveData.RxBuffer+SourcePos));//�㲥���ͷ���ready�ظ�
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
							//�����̵�ַΪ0x00
							//printf("ID:%d ",*(AT86RF212ReceiveData.RxBuffer+SourcePos));
							TempBuf[*(AT86RF212ReceiveData.RxBuffer+SourcePos)]//����λ��0����ַλ�����0
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
		}//������OLED ��ʾ
		OLED_Display();//��ɫ��ť����OLED��������ʾ
	}
}