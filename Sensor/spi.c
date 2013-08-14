					  
//SPI口初始化
//这里针是对SPI1的初始化
#include "spi.h"
SPI_InitTypeDef  SPI_InitStructure;
 void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  // PA5--SCK   PA6--MISO  PA7--MOSI 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1|RCC_APB2Periph_AFIO, ENABLE );	//功能复用时钟也要开启？
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_ResetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);//置0或者置1？

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//时钟线空闲时是低电平！（依据RF212而定）
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//第一个时钟沿采样（根据RF212而定的）
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	//SPI1_ReadWriteByte(0xff);//启动传输		 
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
  
void SPI1_SetSpeed(u8 SpeedSet)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 ReadWriteByte(u8 TxData)
{		
	//u8 retry=0;		 
		 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
	
	//	{
	//	retry++;
	//	if(retry>200)return 0;
	//	}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	//retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//?有问题么？ //检查指定的SPI标志位设置与否:接受缓存非空标志位
		//{
		//retry++;
		//if(retry>200)return 0;
		//}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}


//多字节写入
//输　	 入: Rt：返回数据地址，Data：写入数据地址，Nbyte：写入数据个数
void  ReadWriteBytes(uint8_t* Rt,uint8_t* Data,uint16_t Nbyte)
{
	uint8_t i;	
	//======循环单字发送
	for(i=0;i<Nbyte;i++) {
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  	//循环检测发送缓冲区是否是空
	  	SPI_I2S_SendData(SPI1,*Data++);	 										//通过SPI2外设发出数据
	  	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);  	//等待接收数据，循环检查接收数据缓冲区
	  	*Rt++ = SPI_I2S_ReceiveData(SPI1);	 								//返回读出的数据
	} 	 
	//=====DMA模式发送
//	BSP_SPIDMA_RW(SPI2, Rt, Data, Nbyte);
}































