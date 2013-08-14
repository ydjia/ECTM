#include "sht11.h"

void SHT_GPIO_Configuration(void)//SHT基本IO口配置，设置SCL为一直输出
{
		GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );

		GPIO_InitStructure.GPIO_Pin =SHT_SCL_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(SHT_SCL_PORT, &GPIO_InitStructure);
		GPIO_ResetBits(SHT_SCL_PORT,SHT_SCL_PIN);
}

void SHT_SDA_IN(void)//设置SDA为输入
{
		GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin =SHT_SDA_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(SHT_SDA_PORT, &GPIO_InitStructure);	
}

void SHT_SDA_OUT(void)//设置SDA为输出
{
		  GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin =SHT_SDA_PIN;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	    GPIO_Init(SHT_SDA_PORT, &GPIO_InitStructure);
}

void StartSHT(void)//启动SHT传输总线
{
		SHT_SDA_OUT();//设置SDA为输出
    SCL_L;//初始状态
		SDA_H;
		__nop();

		SCL_H;//时钟线为高时数据线由高变低
		__nop();
		SDA_L;
    __nop();

		SCL_L;
		__nop();
		__nop();
		__nop();

		SCL_H;//时钟线为高时数据线由低变高
		__nop();
		SDA_H;
		__nop();

    SCL_L;//时钟线拉低为数据传输做准备
}

u8 SHT_Reset(void)//SHT软复位
{
        u8 i;
        SDA_H;//初始状态
		SCL_L;
		for(i=0;i<9;i++)//9 SCL CYCLES
		{
				SCL_H;
				SCL_L;
		}
        StartSHT();	
        
		StartSHT();
		if(!SHT_SendByte(SHT_SOFT_RESET)) //软复位 0x1e
		{	
		    return (0);   
		}  
        delay_ms(11);//软复位命令后至少等待11ms

		return (1);
}

u8 SHT_SendByte(u8 cmd)//发送一个字节的数据到SHT数据线 返回1成功 0失败！
{
		u8 i;
		u8 j=0;
		for(i=0x80;i>0;i/=2)
		{
				if(i&cmd)
				{
					SDA_H;	
				}
				else
				{
					SDA_L;	
				}

				__nop();
				SCL_H;	
				__nop();
				__nop();
				__nop();
				SCL_L;	
				__nop();
		}

		SDA_H;//释放SDA
		SHT_SDA_IN();//设置SDA为输入
		__nop();
		
		SCL_H;
		if(SDA_STATE)
		{
			j=0;//发送失败		
		}
		else
		{
			j=1;//发送成功
		}
		SCL_L;
		
		return (j);
}

u8 SHT_ReadByte(u8 ack)//返回所读取的测量值
{
		u8 i;
		u8 value=0x00;
		SHT_SDA_OUT();
		SDA_H;
		SHT_SDA_IN();
		for(i=0x80;i>0;i/=2)
		{
				SCL_H;
				if(SDA_STATE)
				{
						value=(value|i);
				}
				SCL_L;
				delay_us(4);
		}
		SHT_SDA_OUT();
		if(ack)
		{  SDA_L; }
		else
		{  SDA_H; }
		__nop();
		SCL_H;
		__nop();
		__nop();
		__nop();
		SCL_L;
		__nop();

		SDA_H;
		return (value);		
}

u8 SHT_Measure(u8 *value,u8 mode)
{
	if(!mode)
	    {		
		    StartSHT();
				if(!SHT_SendByte(SHT_READ_TEMP))//测温度
				{
					//printf("sendwrong ");
					return (0);	
				}
			}
		else
		{   	
				StartSHT();
				if(!SHT_SendByte(SHT_READ_HUMI))//测湿度
				{
				  //printf("sendwrong ");
					return (0);	
				}
		}
		
	  while(SDA_STATE);
		
		*(value)  =SHT_ReadByte(ACK);    //read the first byte (MSB)
    *(value+1)=SHT_ReadByte(noACK);  //read the second byte (LSB) and skip acknowledge to end trans
		return (1);
}

void Calculate_SHT(u8 *realtemp,u8 *realhumid) //将实际测得的温湿度存入realtemp和realhumid两个数组中，温度为xx.xx形式，湿度为xx.x形式
{	  		
		u16 temp1=0,temp2=0;
		const float C1=-4;
	  const float C2=0.0405;
	  const float C3=-0.0000028;
		const float d1=-40;
	  const float d2=0.01;
	  const float T1=0.01;
	  const float T2=0.00008;

	  float rh_lin;
	  float rh_true;
	  float t_C;
	
		SHT_Measure(SHT_TEMP,TEMP);
		SHT_Measure(SHT_HUMID,HUMI);
	
    temp1=((((u16)*(SHT_TEMP))<<8)|((u16)*(SHT_TEMP+1)));
		temp2=((((u16)*(SHT_HUMID))<<8)|((u16)*(SHT_HUMID+1)));
	  t_C=temp1*d2+d1;
	  rh_lin=C1+C2*temp2+C3*temp2*temp2;
	  rh_true=(t_C-25)*(T1+T2*temp2)+rh_lin;
	  if(rh_true>100)rh_true=100;       
    if(rh_true<0.1)rh_true=0.1;
		
		*(realtemp+3)=((temp1-4000)/100)/10+'0';
		*(realtemp+2)=((temp1-4000)/100)%10+'0';
		*(realtemp+1)=((temp1-4000)%100)/10+'0';
		*(realtemp)=((temp1-4000)%100)%10+'0';
		
		//printf("%d.%d ",(temp1-4000)/100,(temp1-4000)%100);
		//printf("%d \r\n",(u8)rh_true);
		rh_true=(int)(rh_true*10);
		*(realhumid+2)=rh_true/100+'0';
		*(realhumid+1)=(rh_true-100*(*(realhumid+2)-'0'))/10+'0';
		*(realhumid)=rh_true-(*(realhumid+2)-'0')*100-(*(realhumid+1)-'0')*10+'0';
		
}

