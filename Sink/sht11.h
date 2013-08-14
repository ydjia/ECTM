#ifndef _SHT11_
#define _SHT11_
#include "sys.h"
#include "stdio.h"
#include "delay.h"

static u8 SHT_TEMP[2]={0};
static u8 SHT_HUMID[2]={0};
//-----------------�Ĵ��������������޸�-----------2013.03.30 OKIMBIN--//
#define SHT_SOFT_RESET     0x1e  //��λ����
#define SHT_READ_TEMP      0x03  //�¶Ȳ��������΢����������
#define SHT_READ_HUMI      0x05  //ʪ�Ȳ�������
#define SHT_WRITE_STATUS   0x06  //д״̬�Ĵ���
#define SHT_READ_STATUS    0x07  //��״̬�Ĵ���
#define ACK 1
#define noACK 0
#define TEMP 0
#define HUMI 1
//-----------------�������˿ڲ����������޸�-------2013.03.30 OKIMBIN--//
//------------�ǵ��޸�SHT11�˿ڳ�ʼ�������������Ӧ�˿�ʱ�ӵĳ�ʼ��---//
#define SHT_SCL_PORT  GPIOB
#define SHT_SCL_PIN   GPIO_Pin_6
#define SHT_SDA_PORT  GPIOB
#define SHT_SDA_PIN   GPIO_Pin_7

#define SCL_H      GPIO_SetBits(SHT_SCL_PORT,SHT_SCL_PIN)
#define SCL_L      GPIO_ResetBits(SHT_SCL_PORT,SHT_SCL_PIN)
#define SDA_H      GPIO_SetBits(SHT_SDA_PORT,SHT_SDA_PIN)
#define SDA_L      GPIO_ResetBits(SHT_SDA_PORT,SHT_SDA_PIN)
#define SDA_STATE  GPIO_ReadInputDataBit(SHT_SDA_PORT,SHT_SDA_PIN)



//----------------------SHT����------------------2013.03.30 OKIMBIN--//
void SHT_GPIO_Configuration(void);
void SHT_SDA_IN(void);
void SHT_SDA_OUT(void);
void StartSHT(void);
u8 SHT_Reset(void);
u8 SHT_SendByte(u8 cmd);
u8 SHT_ReadByte(u8 ack);
u8 SHT_Measure(u8 *value,u8 mode);
void Calculate_SHT(u8 *ptemp,u8 *phumi);


#endif
