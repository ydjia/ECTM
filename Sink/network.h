#ifndef __NETWORK_H
#define __NETWORK_H
#include "sys.h"
#include "delay.h"
#include "at86rf212.h"
#include "stdio.h"
#include "global.h"

//定义各种信令
#define I_WANT_JOIN_BROADCAST        (0x01)//请求入网广播信令
#define I_WANT_JOIN_YOU              (0x02)//请求成为我的父节点
#define YOU_CAN_JOIN                 (0x03)//入网确认及一些节点参数设置
#define I_AM_READY                   (0x04)//可以加节点广播

//邻居表结构体
typedef struct
{
	uint8_t saddr;//检测到的信号地址
	uint8_t ed;//ed检测值
	uint8_t net_deapth;//节点网络深度
}NeighborNode;



uint8_t scanNeighborNodes(NeighborNode* neighborTable);
void initTheNode(void);
void	commandPackage(uint8_t command,uint8_t destaddr);
void	dataPackage(void);
void transferPackage(void);
#endif
