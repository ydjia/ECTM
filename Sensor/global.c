#include "global.h"


unsigned char Real_TEMP[5]={'\0'};//四个数，最后一位用来放’\0'防止printf输出出错
unsigned char Real_HUMID[4]={'\0'};//三个数，最后一位用来放'\0'防止printf输出出错
u8 SHT_TEMP[2]={0};
u8 SHT_HUMID[2]={0};
extern uint8_t networkED;
extern TheNodeType thenode;
