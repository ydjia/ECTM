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
//ÔÆÌì´´F5Ä£¿é´«¸ĞÆ÷½Úµã´úÂë
//
//ÈÕÆÚ£º2013.05.08
//Íâ²¿¾§Õñ£º16MHz

//MCU:STM32F101CB  WirelessChip:AT86RF212
//ÎÊÌâ£º1.²»ÖªµÀÎªÊ²Ã´°Ñ´®¿Úµ÷µ½9600Ê±¾Í»á³öÏÖÂÒÂë£
//      2.printfÊ¹ÓÃ%sÊä³ö×Ö·û´®Ê±£¬×îºóÒ»Î»Îñ±ØÊÇ'\0'·ñÔòÓĞ¿ÉÄÜ»áµ¼ÖÂÊä³ö½á¹û³ö´í¡£printfÊä³ö×Ö·ûÊı×éÊÇ´ÓµÚ0¸ö¿ªÊ¼Êä³öµÄ¡£
//Èç¹û·¢ÏÖI2C×ÜÏß»òÕßSPI×ÜÏßÉèÖÃÕıÈ·µ«ÎŞ·¨²Ù×÷Ê±Ó¦¸Ã¿¼ÂÇÒ»ÏÂÊÇ²»ÊÇÏµÍ³Ê±ÖÓ³õÊ¼»¯ÓĞÎÊÌâ¡££¨F101ºÍF103Ö§³ÖµÄ×î¸ßÆµÂÊ²»Ò»Ñù£¡£©
//			3.Èç¹ûÊ±ÖÓ²»ÊÇÄ¬ÈÏÉèÖÃµÄ72MµÄ»°£¬¿ÉÄÜ»áµ¼ÖÂdelay_initº¯ÊıÀïÑÓÊ±Ê±¼ä²»×¼È·¡£
//
//
//ÔÚĞ´½ÓÊÕ¶ËµÄ»ú×ÓÊ±±ØĞë¼ÇµÃ¸ü¸Ä×Ô¼ºµÄ¶ÌµØÖ·¡£

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
	 SystemInit();//³õÊ¼»¯Îª24M
	 delay_init(24);
	 NVIC_Configuration(); 	 //ÉèÖÃNVICÖĞ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
 	 IO_Init();//¶Ë¿Ú¼°¸÷ÖÖÊ±ÖÓÊ¹ÄÜ¡£
 	 uart_init(4800);//²»ÖªÎªºÎ²¨ÌØÂÊ¾ÓÈ»Îª9600.
	 
	 initTheNode();
	 SHT_EN=1;
	 delay_ms(20);//SHT11ÉÏµçºóĞèÒªÑÓÊ±³¬¹ı11msºó²ÅÄÜ¶ÁÊı
	 SHT_GPIO_Configuration();//³õÊ¼»¯SHT11Ê±ÖÓ
	 SPI1_Init(); //³õÊ¼»¯SPI×ÜÏß
	 AT86RF212_Init();	//³õÊ¼»¯RF212Ğ¾Æ¬
	 delay_ms(200);
	 
	 Green_LED=0;
	 Red_LED=0;
	 commandPackage(I_WANT_JOIN_BROADCAST,0xff);	
	 AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//·¢ËÍÈëÍø¹ã²¥
	 AT86RF212_IRQServe();//ÍâÃæÒ²Ó¦¸Ã¶ÁÒ»±é°¡
	 while(RF212SendFinish==TRUE)
	 {	
		 RF212SendFinish=0;
		 printf("GoodJob\n");			
		 Green_LED=1;//ÁÁ
		 delay_ms(500);
		 Green_LED=0;
		 delay_ms(500);
		 AT86RF212_IRQServe();
		 delay_ms(100);
	 }	
	 tlength=scanNeighborNodes(neighbortable);
	 while(scan_flag)
	 {
			for(i=0;i<tlength;i++)//Ñ°ÕÒED×î´óÖµ
			{
			//°´ÍøÂçÉî¶È¼ìË÷ÁÚ¾Ó±í
				if(neighbortable[i].net_deapth == net_deapth_temp&&neighbortable[i].ed>=ED_LEVEL&&neighbortable[i].ed>ed_m
					&&neighbortable[i].attri==1)
				{
				//µ±Ç°ÍøÂçÉî¶ÈÈôÓĞ½ÚµãÈ¡³öÇ¿¶È×î´óµÄ
					ed_m=neighbortable[i].ed;
					saddr_m=neighbortable[i].saddr;
				}
			}
			if(net_deapth_temp>3)
				scan_flag=0;
			if(ed_m<=0||saddr_m==0xFE)	
				net_deapth_temp++;//¸Ã¼ìË÷ÍøÂçÉî¶ÈÖĞÃ»ÓĞ½ÚµãÔò¼ìË÷ÍøÂçÉî¶È¼Ó1
			else
				scan_flag=0;
	}
  if(ed_m<=0 || tlength==0)//Î´ÕÒµ½ÍøÂç
	{
			goto START;
	}
  commandPackage(I_WANT_JOIN_YOU,saddr_m);
	AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//·¢ËÍµ±¶ù×ÓĞÅÁî
	AT86RF212_IRQServe();//ÍâÃæÒ²Ó¦¸Ã¶ÁÒ»±é°¡
		while(RF212SendFinish==TRUE)
		{	
			RF212SendFinish=0;
			AT86RF212_IRQServe();
			delay_ms(100);
		}	
	while(k<20)
	{
		 AT86RF212_IRQServe();//²é¿´ÓĞÃ»ÓĞ»Ø¸´
		 if(RF212RevSuccess == TRUE)
		 { 	
				RF212RevSuccess=0;
				if(*(AT86RF212ReceiveData.RxBuffer+DestPos)==TheAddress)
				{
					if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==YOU_CAN_JOIN))//ÊÕµ½»Ø¸´£¬×ö¸¸½Úµã
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
	if(k>=20)//³¬¹ıÑ­»·´ÎÊıÎ´ÊÕµ½»Ø¸´
			goto START;
	
	while(1)
	{
			Calculate_SHT(Real_TEMP,Real_HUMID);
			dataPackage();
			AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	
			AT86RF212_IRQServe();//²é¿´ÓĞÃ»ÓĞ»Ø¸´£¬×¢Òâ´Ë³ÌĞòÃ»ÓĞĞ´ÖÕ¶Ë½ÚµãµÄ´¦Àí·½Ê½
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
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//»Ø¸´ÎÒÊÇÄã°Ö
								AT86RF212_IRQServe();//ÍâÃæÒ²Ó¦¸Ã¶ÁÒ»±é°¡
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
								commandPackage(I_AM_READY,*(AT86RF212ReceiveData.RxBuffer+SourcePos));//¹ã²¥·¢ËÍ·¢ËÍready»Ø¸´
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));
								AT86RF212_IRQServe();//ÍâÃæÒ²Ó¦¸Ã¶ÁÒ»±é°¡
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
								AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));//Èç¹ûÊÇÊı¾İ°ü×ª·¢
								AT86RF212_IRQServe();//ÍâÃæÒ²Ó¦¸Ã¶ÁÒ»±é°¡
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