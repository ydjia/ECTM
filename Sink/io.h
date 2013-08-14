#ifndef __io_H
#define __io_H	 
#include "stm32f10x.h"
//Mini STM32开发板
//LED驱动代码			 
//正点原子@ALIENTEK
//2012/2/27

//LED端口定义
#define CoreGreen_LED PBout(8)
#define CoreRed_LED PBout(9)
#define Choose PBout(3)
#define Green_LED PAout(13)
#define BlueKey PBin(5)
#define RedKey PBin(4)
void IO_Init(void);//初始化

		 				    
#endif
