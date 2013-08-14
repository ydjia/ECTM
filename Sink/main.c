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
//ÎÊÌâ£º1.²»ÖªµÀÎªÊ²Ã´°Ñ´®¿Úµ÷µ½9600Ê±¾Í»á³öÏÖÂÒÂë£
//      2.printfÊ¹ÓÃ%sÊä³ö×Ö·û´®Ê±£¬×îºóÒ»Î»Îñ±ØÊÇ'\0'·ñÔòÓĞ¿ÉÄÜ»áµ¼ÖÂÊä³ö½á¹û³ö´í¡£printfÊä³ö×Ö·ûÊı×éÊÇ´ÓµÚ0¸ö¿ªÊ¼Êä³öµÄ¡£
//Èç¹û·¢ÏÖI2C×ÜÏß»òÕßSPI×ÜÏßÉèÖÃÕıÈ·µ«ÎŞ·¨²Ù×÷Ê±Ó¦¸Ã¿¼ÂÇÒ»ÏÂÊÇ²»ÊÇÏµÍ³Ê±ÖÓ³õÊ¼»¯ÓĞÎÊÌâ¡££¨F101ºÍF103Ö§³ÖµÄ×î¸ßÆµÂÊ²»Ò»Ñù£¡£©

//ÔÚĞ´½ÓÊÕ¶ËµÄ»ú×ÓÊ±±ØĞë¼ÇµÃ¸ü¸Ä×Ô¼ºµÄ¶ÌµØÖ·¡£

//ÓëÔÆÌì´´Ä£¿é»ìºÏÔÚÍ¬Ò»¸öÍøÂçÖĞ ¡ª¡ª¡ª¡ª20130513 OKIMBIN
//»ã¾Û½Úµã´úÂë  ½Úµã±àºÅ0x00
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
	 SystemInit();//³õÊ¼»¯Îª24M
	 delay_init(24);
	 NVIC_Configuration(); 	 //ÉèÖÃNVICÖĞ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
 	 IO_Init();//¶Ë¿Ú¼°¸÷ÖÖÊ±ÖÓÊ¹ÄÜ¡£
 	 uart_init(4800);//²»ÖªÎªºÎ²¨ÌØÂÊ¾ÓÈ»Îª9600.
	 
	 initTheNode();
	 delay_ms(20);//SHT11ÉÏµçºóĞèÒªÑÓÊ±³¬¹ı11msºó²ÅÄÜ¶ÁÊı
	 SPI1_Init(); //³õÊ¼»¯SPI×ÜÏß
	 AT86RF212_Init();	//³õÊ¼»¯RF212Ğ¾Æ¬
	 delay_ms(200);
	 
	 Green_LED=1;
	 OLED_Init();
 	 Welcome();
	 OLED_Refresh_Gram();
 	 delay_ms(1500);
	 	
	while(1)
	{	
	
		AT86RF212_IRQServe();//²é¿´ÓĞÃ»ÓĞ»Ø¸´£¬×¢Òâ´Ë³ÌĞòÃ»ÓĞĞ´ÖÕ¶Ë½ÚµãµÄ´¦Àí·½Ê½
		//CoreRed_LED=0;//ºÍ´«¸Ğ½ÚµãÏà·´£¬ÁÁ
		
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
									AT86RF212_EnterTXModeSendData((uint8_t*)(&AT86RF212SendData),sizeof(AT86RF212SendData));	//»Ø¸´ÎÒÊÇÄã°Ö
									delay_ms(500);
									while(RF212SendFinish==TRUE)
									{	
										RF212SendFinish=0;		
										AT86RF212_IRQServe();
										delay_ms(100);
										CoreRed_LED=0;//ºÍ´«¸Ğ½ÚµãÏà·´£¬ÁÁ
										delay_ms(500);
										CoreRed_LED=1;//ºÍ´«¸Ğ½ÚµãÏà·´£¬°µ
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
							commandPackage(I_AM_READY,*(AT86RF212ReceiveData.RxBuffer+SourcePos));//¹ã²¥·¢ËÍ·¢ËÍready»Ø¸´
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
							//±¾»ú¶ÌµØÖ·Îª0x00
							//printf("ID:%d ",*(AT86RF212ReceiveData.RxBuffer+SourcePos));
							TempBuf[*(AT86RF212ReceiveData.RxBuffer+SourcePos)]//Êı¾İÎ»¼õ0£¬µØÖ·Î»ÎŞĞè¼õ0
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
		}//°´¼üºóOLED ÏÔÊ¾
		OLED_Display();//À¶É«°´Å¥¿ØÖÆOLEDµÄÊı¾İÏÔÊ¾
	}
}