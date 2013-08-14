#ifndef __NETWORK_H
#define __NETWORK_H
#include "sys.h"
#include "delay.h"
#include "at86rf212.h"
#include "stdio.h"
#include "global.h"

//�����������
#define I_WANT_JOIN_BROADCAST        (0x01)//���������㲥����
#define I_WANT_JOIN_YOU              (0x02)//�����Ϊ�ҵĸ��ڵ�
#define YOU_CAN_JOIN                 (0x03)//����ȷ�ϼ�һЩ�ڵ��������
#define I_AM_READY                   (0x04)//���Լӽڵ�㲥

//�ھӱ�ṹ��
typedef struct
{
	uint8_t saddr;//��⵽���źŵ�ַ
	uint8_t ed;//ed���ֵ
	uint8_t net_deapth;//�ڵ��������
}NeighborNode;



uint8_t scanNeighborNodes(NeighborNode* neighborTable);
void initTheNode(void);
void	commandPackage(uint8_t command,uint8_t destaddr);
void	dataPackage(void);
void transferPackage(void);
#endif
