#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "at86rf212.h"
#include "stm32f10x.h"
#include  "sourcetype.h"


//�ڵ�����
typedef struct
{
	//uint8_t signaddr;//��⵽���źŵ�ַ
	uint8_t attri;//0=�����ٹ�·�ɽڵ�,1=�����ٹ�·�ɽڵ�
	uint8_t net_depth;//�ڵ��������
	uint8_t fanode; //���ڵ�ID
	uint8_t nodeid; //�����ID
}TheNodeType;

extern TheNodeType thenode;


extern u8 networkED;
extern unsigned char Real_TEMP[5];//�ĸ��������һλ�����š�\0'��ֹprintf�������
extern unsigned char Real_HUMID[4];//�����������һλ������'\0'��ֹprintf�������
//extern static u8 SHT_TEMP[2];
//extern static u8 SHT_HUMID[2];

extern u8 SHT_TEMP[2];
extern u8 SHT_HUMID[2];
extern u32 HumidBuf[6];
extern u32 TempBuf[6];

#endif
