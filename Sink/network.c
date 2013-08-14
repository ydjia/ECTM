#include "network.h"
#include "sht11.h"
#include "system_stm32f10x.h"
#include "global.h"
#include "at86rf212.h"
extern unsigned char Real_TEMP[5];
extern unsigned char Real_HUMID[4];
TheNodeType thenode;
u8 networkED;
void initTheNode()
{
	#if TheAddress==0x00
		//thenode.attri=1;;//0=²»¿ÉÔÙ¹ÒÂ·ÓÉ½Úµã,1=¿ÉÒÔÔÙ¹ÒÂ·ÓÉ½Úµã
		thenode.net_depth=0x00;//½ÚµãÍøÂçÉî¶È
	#else
		thenode.net_depth=0xFF;//½ÚµãÍøÂçÉî¶È
	#endif
		thenode.attri=1;;//0=²»¿ÉÔÙ¹ÒÂ·ÓÉ½Úµã,1=¿ÉÒÔÔÙ¹ÒÂ·ÓÉ½Úµ
		thenode.nodeid=TheAddress;
}

void	commandPackage(uint8_t command,uint8_t destaddr)//ÃüÁî£¬¸¸½ÚµãµØÖ·£¬Ä¿µÄ½ÚµãµØÖ·
{			
		ReadyDataTo(destaddr);
		AT86RF212SendData.Data[0]=0xFF;
		AT86RF212SendData.Data[1]=command;
		AT86RF212SendData.Data[2]=thenode.net_depth;	//ÏÈ²»¿¼ÂÇË¯Ãß¼°RTCÍ¬²½
		AT86RF212SendData.Data[3]=thenode.attri;
}
void	dataPackage()//·â×°×ÔÉíÊı¾İ°üÓÃÓÚÉÏ´«ÎÂÊª¶ÈÊı¾İµ½¸¸½Úµã
{
		ReadyDataTo(thenode.fanode);	
		AT86RF212SendData.Data[0]=0xAB;
		AT86RF212SendData.Data[1]='#';
		AT86RF212SendData.Data[2]=Real_TEMP[3];//Êı¾İ4-12Îª²»±äÊı¾İ
		AT86RF212SendData.Data[3]=Real_TEMP[2];
		AT86RF212SendData.Data[4]=Real_TEMP[1];
		AT86RF212SendData.Data[5]=Real_TEMP[0];
		AT86RF212SendData.Data[6]=Real_HUMID[2];
		AT86RF212SendData.Data[7]=Real_HUMID[1];
		AT86RF212SendData.Data[8]=Real_HUMID[0];
		AT86RF212SendData.Data[9]=thenode.nodeid;//Êı¾İ°üÔ´µØÖ·
		AT86RF212SendData.Data[10]=thenode.fanode;	//Êı¾İ°üÔ´µØÖ·µÄ¸¸½Úµã
		AT86RF212SendData.Data[11]='*';
}



void transferPackage()  //×ª·¢°ü ³ı0x00Íâ¾ùÔÚ×ª·¢
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
		while(icount<30)//ÕâÑùÖ»»áËÑË÷µ½Ò»¸öÁÚ½Ó½Úµã£¬Íâ²ãÔÙÇ¶Ì×while£¿
	 {	
		 AT86RF212_IRQServe();
		 if(RF212RevSuccess == TRUE)
				{
					if(*(AT86RF212ReceiveData.RxBuffer+DestPos)==TheAddress){
					
					break;
					}
				}
		icount++;
		}
		if((*(AT86RF212ReceiveData.RxBuffer+DataStartPos)==0xff)&& (*(AT86RF212ReceiveData.RxBuffer+DataStartPos+1)==I_AM_READY))
		{	neighborTable[i].saddr=*(AT86RF212ReceiveData.RxBuffer+SourcePos);
			neighborTable[i].ed=networkED;
			neighborTable[i].net_deapth=*(AT86RF212ReceiveData.RxBuffer+DataStartPos+2);
			i++;
		}//ËûÃÇĞ´µÄÓÃdelayÀ´ÑÓÊ±£¬ÎÒÃÇÒÑ¾­Í¨¹ıÖĞ¶Ï½øĞĞÁË£¬²¢ÇÒÔÚÖĞ¶ÏÍâ²¿Ç¶Ì×ÁËÑ­»·£¬Ó¦¸Ã²»ÓÃdelayÁË°É
		search_count++;
	}
	return i;
}
