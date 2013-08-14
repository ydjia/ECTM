#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "at86rf212.h"
#include "stm32f10x.h"
#include  "sourcetype.h"


//节点属性
typedef struct
{
	//uint8_t signaddr;//检测到的信号地址
	uint8_t attri;//0=不可再挂路由节点,1=可以再挂路由节点
	uint8_t net_depth;//节点网络深度
	uint8_t fanode; //父节点ID
	uint8_t nodeid; //自身的ID
}TheNodeType;

extern TheNodeType thenode;


extern u8 networkED;
extern unsigned char Real_TEMP[5];//四个数，最后一位用来放’\0'防止printf输出出错
extern unsigned char Real_HUMID[4];//三个数，最后一位用来放'\0'防止printf输出出错
//extern static u8 SHT_TEMP[2];
//extern static u8 SHT_HUMID[2];

extern u8 SHT_TEMP[2];
extern u8 SHT_HUMID[2];
extern u32 HumidBuf[6];
extern u32 TempBuf[6];

#endif
