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
//���촴F5ģ�鴫�����ڵ����
//
//���ڣ�2013.05.08
//�ⲿ����16MHz

//MCU:STM32F101CB  WirelessChip:AT86RF212
//���⣺1.��֪��Ϊʲô�Ѵ��ڵ���9600ʱ�ͻ��������
//      2.printfʹ��%s����ַ���ʱ�����һλ�����'\0'�����п��ܻᵼ������������printf����ַ������Ǵӵ�0����ʼ����ġ�
//�������I2C���߻���SPI����������ȷ���޷�����ʱӦ�ÿ���һ���ǲ���ϵͳʱ�ӳ�ʼ�������⡣��F101��F103֧�ֵ����Ƶ�ʲ�һ������
//			3.���ʱ�Ӳ���Ĭ�����õ�72M�Ļ������ܻᵼ��delay_init��������ʱʱ�䲻׼ȷ��
//
//
//��д���ն˵Ļ���ʱ����ǵø����Լ��Ķ̵�ַ��

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
	 SystemInit();//��ʼ��Ϊ24M
	 delay_init(24);
	 NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
 	 IO_Init();//�˿ڼ�����ʱ��ʹ�ܡ�
 	 uart_init(4800);//��֪Ϊ�β����ʾ�ȻΪ9600.
	 
	 initTheNode();
	 SHT_EN=1;
	 delay_ms(20);//SHT11�ϵ����Ҫ��ʱ����11ms����ܶ���
	 SHT_GPIO_Configuration();//��ʼ��SHT11ʱ��
	 SPI1_Init(); //��ʼ��SPI����
	 AT86RF212_Init();	//��ʼ��RF212оƬ
	 delay_ms(200);
	 
	 Green_LED=0;
	 Red_LED=0;
	 commandPackage(I_WANT_JOIN_BROADCAST,0xff);	
	 AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//���������㲥
	 AT86RF212_IRQServe();//����ҲӦ�ö�һ�鰡
	 while(RF212SendFinish==TRUE)
	 {	
		 RF212SendFinish=0;
		 printf("GoodJob\n");			
		 Green_LED=1;//��
		 delay_ms(500);
		 Green_LED=0;
		 delay_ms(500);
		 AT86RF212_IRQServe();
		 delay_ms(100);
	 }	
	 tlength=scanNeighborNodes(neighbortable);
	 while(scan_flag)
	 {
			for(i=0;i<tlength;i++)//Ѱ��ED���ֵ
			{
			//��������ȼ����ھӱ�
				if(neighbortable[i].net_deapth == net_deapth_temp&&neighbortable[i].ed>=ED_LEVEL&&neighbortable[i].ed>ed_m
					&&neighbortable[i].attri==1)
				{
				//��ǰ����������нڵ�ȡ��ǿ������
					ed_m=neighbortable[i].ed;
					saddr_m=neighbortable[i].saddr;
				}
			}
			if(net_deapth_temp>3)
				scan_flag=0;
			if(ed_m<=0||saddr_m==0xFE)	
				net_deapth_temp++;//�ü������������û�нڵ������������ȼ�1
			else
				scan_flag=0;
	}
  if(ed_m<=0 || tlength==0)//δ�ҵ�����
	{
			goto START;
	}
  commandPackage(I_WANT_JOIN_YOU,saddr_m);
	AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//���͵���������
	AT86RF212_IRQServe();//����ҲӦ�ö�һ�鰡
		while(RF212SendFinish==TRUE)
		{	
			RF212SendFinish=0;
			AT86RF212_IRQServe();
			delay_ms(100);
		}	
	while(k<20)
	{
		 AT86RF212_IRQServe();//�鿴��û�лظ�
		 if(RF212RevSuccess == TRUE)
		 { 	
				RF212RevSuccess=0;
				if(*(AT86RF212ReceiveData.RxBuffer+DestPos)==TheAddress)
				{
					if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==YOU_CAN_JOIN))//�յ��ظ��������ڵ�
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
	if(k>=20)//����ѭ������δ�յ��ظ�
			goto START;
	
	while(1)
	{
			Calculate_SHT(Real_TEMP,Real_HUMID);
			dataPackage();
			AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	
			AT86RF212_IRQServe();//�鿴��û�лظ���ע��˳���û��д�ն˽ڵ�Ĵ���ʽ
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
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//�ظ��������
								AT86RF212_IRQServe();//����ҲӦ�ö�һ�鰡
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
								commandPackage(I_AM_READY,*(AT86RF212ReceiveData.RxBuffer+SourcePos));//�㲥���ͷ���ready�ظ�
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));
								AT86RF212_IRQServe();//����ҲӦ�ö�һ�鰡
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
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));//��������ݰ�ת��
								AT86RF212_IRQServe();//����ҲӦ�ö�һ�鰡
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