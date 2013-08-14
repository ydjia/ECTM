#include "stm32f10x.h"
#include "io.h"

void IO_Init(void)
{	
 GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PA,PB端口时钟	
 
	//CLKM --PA0    SEL --PA4   Green_LED --PA13
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_13;	    		 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					
 
 GPIO_SetBits(GPIOA,GPIO_Pin_4); //SEL置1

 // RST --PB11    SLP--PB0      CoreGreen_LED--PB8     CoreRed_Led--PB9  		
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_0|GPIO_Pin_8|GPIO_Pin_9;	 //推挽输出   		
 GPIO_Init(GPIOB, &GPIO_InitStructure);
//	Choose--PB3
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);//高电平熄灭

 	//DIG2--PB1   	    		
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //悬空输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 
 //IRQ --PA1
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 
 //BlueKey --PB5    RedKey--PB4
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 
}
 
