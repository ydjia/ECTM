#ifndef _SHT11_
#define _SHT11_
#include "sys.h"
#include "stdio.h"
#include "delay.h"

static u8 SHT_TEMP[2]={0};
static u8 SHT_HUMID[2]={0};
//-----------------寄存器参数，不用修改-----------2013.03.30 OKIMBIN--//
#define SHT_SOFT_RESET     0x1e  //软复位命令
#define SHT_READ_TEMP      0x03  //温度测量命令，由微控制器发出
#define SHT_READ_HUMI      0x05  //湿度测量命令
#define SHT_WRITE_STATUS   0x06  //写状态寄存器
#define SHT_READ_STATUS    0x07  //读状态寄存器
#define ACK 1
#define noACK 0
#define TEMP 0
#define HUMI 1
//-----------------传感器端口参数，按需修改-------2013.03.30 OKIMBIN--//
//------------记得修改SHT11端口初始化函数里面对相应端口时钟的初始化---//
#define SHT_SCL_PORT  GPIOB
#define SHT_SCL_PIN   GPIO_Pin_6
#define SHT_SDA_PORT  GPIOB
#define SHT_SDA_PIN   GPIO_Pin_7

#define SCL_H      GPIO_SetBits(SHT_SCL_PORT,SHT_SCL_PIN)
#define SCL_L      GPIO_ResetBits(SHT_SCL_PORT,SHT_SCL_PIN)
#define SDA_H      GPIO_SetBits(SHT_SDA_PORT,SHT_SDA_PIN)
#define SDA_L      GPIO_ResetBits(SHT_SDA_PORT,SHT_SDA_PIN)
#define SDA_STATE  GPIO_ReadInputDataBit(SHT_SDA_PORT,SHT_SDA_PIN)



//----------------------SHT函数------------------2013.03.30 OKIMBIN--//
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
