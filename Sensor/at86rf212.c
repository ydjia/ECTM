#define	_AT86RF212_GLOBAL_
#include "sys.h"
#include "delay.h"
#include "at86rf212.h"
#include "stdio.h"
#include "spi.h"
#include "network.h"
#include "global.h"

u8 networkED;
void AT86RF212_Dely1us(uint32_t delay);

void AT86RF212_Dely1us(uint32_t delay)
{	
	delay_us(delay);
}

/********************************************************************************************************
** 函数名称: AT86RF212_ReadWriteReg
** 功能描述: AT86RF212读寄存器操作
**					
** 输　  入: cmd：指令，addr：地址，Data：数据
**
** 输　  出: 输出最后一次SPI返回值
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8_t AT86RF212_ReadWriteReg(uint8_t cmd,uint8_t addr,uint8_t Data)
{  
	uint8_t r_data;

	AT86RF212_SELEnable();
  	ReadWriteByte(cmd|addr);			    	//指令
  	r_data = ReadWriteByte(Data);				//写数据
	AT86RF212_SELDisable(); 
	
	return (r_data); 
}

/********************************************************************************************************
** 函数名称: AT86RF212_ReadModifyWriteReg
** 功能描述: AT86RF212对寄存器的读改写操作
**					
** 输　  入: addr：地址，val：数据,mask：要改写位屏蔽字
**
** 输　  出: 输出phy_status状态
*********************************************************************************************************/
uint8_t AT86RF212_ReadModifyWriteReg(uint8_t addr,uint8_t val,uint8_t mask)
{
	uint8_t tmpreg,phy_status;

	AT86RF212_SELEnable();
  	phy_status = ReadWriteByte(SPI_CMD_READ_REGISTER|addr);		//读指令
  	tmpreg = ReadWriteByte(0xff);									//读出数据
	AT86RF212_SELDisable();
	
	val &= mask;                	// mask off stray bits from val
    tmpreg &= ~mask;               // mask off bits in reg val
    tmpreg |= val;                 // copy val into reg val
    
	AT86RF212_SELEnable();
	ReadWriteByte(SPI_CMD_WRITE_REGISTER|addr);		//写指令
  	ReadWriteByte(tmpreg);								//写数据
	AT86RF212_SELDisable();

	return (phy_status);
}

/********************************************************************************************************
** 函数名称: AT86RF212_WriteFrameBuffer
** 功能描述: AT86RF212写帧缓存
**					
** 输　  入: pdata:写入数据指针,Nbyte:写入数据长度
**
** 输　  出: 输出phy_status状态
*********************************************************************************************************/
uint8_t AT86RF212_WriteFrameBuffer(uint8_t* pdata,uint8_t Nbyte)
{
	uint8_t phy_status,buf[127];// = {0xff};

	AT86RF212_SELEnable();
  
  phy_status = ReadWriteByte(SPI_CMD_WRITE_FRAMEBUF);	//指令
	
	ReadWriteByte(Nbyte+2);								//写帧长度 +2字节的CRC
	ReadWriteBytes(buf,pdata,Nbyte);
	AT86RF212_SELDisable(); 
	
	return (phy_status);
}

/********************************************************************************************************
** 函数名称: AT86RF212_ReadFrameBuffer
** 功能描述: AT86RF212读帧缓存
**					
** 输　  入: Rt:接收数据指针
**
** 输　  出: 输出phy_status状态
*********************************************************************************************************/
uint8_t AT86RF212_ReadFrameBuffer(uint8_t* Rt)
{
	uint8_t i,phy_status,buf[127];
	
	AT86RF212_SELEnable();

  	phy_status = ReadWriteByte(SPI_CMD_READ_FRAMEBUF);					//指令
	AT86RF212ReceiveData.RxLength = ReadWriteByte(0xff) - 2;			//返回帧长度-2字节CRC
   	/*
	for(i=0;i<AT86RF212ReceiveData.RxLength;i++) {			 					//
		*Rt++ = ReadWriteByte(0xff);
	} */	  
	ReadWriteBytes(Rt,buf,AT86RF212ReceiveData.RxLength);

	//*RSSI=ReadWriteByte(0xff);				//链路质量指示

	ReadWriteByte(0xff);				//RX_STATUS状态

	AT86RF212_SELDisable(); 

	return (phy_status);
}

/********************************************************************************************************
** 函数名称: AT86RF212_WriteSRAM
** 功能描述: AT86RF212写SRAM
**					
** 输　  入: pdata:写入数据指针,Nbyte:写入数据长度
**
** 输　  出: 输出phy_status状态
*********************************************************************************************************/
uint8_t AT86RF212_WriteSRAM(uint8_t addr,uint8_t* pdata,uint8_t Nbyte)
{
	uint8_t i,phy_status;
	
	AT86RF212_SELEnable();

  	phy_status = ReadWriteByte(SPI_CMD_WRITE_SRAM);			//指令
	ReadWriteByte(addr);													//写地址
	for(i=0;i<(Nbyte+2);i++) {															//写数据,要包含2字节CRC
		ReadWriteByte(*pdata++);
	}
	
	AT86RF212_SELDisable(); 
	
	return (phy_status);
}

/********************************************************************************************************
** 函数名称: AT86RF212_ReadSRAM
** 功能描述: AT86RF212读SRAM
**					
** 输　  入: Rt:接收数据指针，Nbyte：读取数据长度
**
** 输　  出: 输出phy_status状态
*********************************************************************************************************/
uint8_t AT86RF212_ReadSRAM(uint8_t addr,uint8_t* Rt,uint8_t Nbyte)
{
	uint8_t i,phy_status;
	
	AT86RF212_SELEnable();

  	phy_status = ReadWriteByte(SPI_CMD_READ_SRAM);			//指令
	ReadWriteByte(addr);										//写地址
	for(i=0;i<Nbyte;i++) {
		*Rt++ = ReadWriteByte(0xff);
	}
	
	AT86RF212_SELDisable(); 
	return (phy_status);
}

/********************************************************************************************************
** 函数名称: AT86RF212_GetTRXStatus
** 功能描述: 获取AT86RF212的TRXStatus状态寄存器
**					
** 输　  入: 无
**
** 输　  出: 输出TRXStatus状态
*********************************************************************************************************/
uint8_t AT86RF212_GetTRXStatus(void)
{
	return (AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_TRX_STATUS_ADDR,0xff) & 0x1f);
}


/********************************************************************************************************
** 函数名称: AT86RF212_Reset
** 功能描述: AT86RF212复位操作
*********************************************************************************************************/
void AT86RF212_Reset(void)
{	
	uint32_t	i,ltemp;
	uint8_t		temp;

	AT86RF212_SLP_TRReset();	//AT86RF212_SLP_TR = L
	AT86RF212_SELDisable();		//SEL = H	
	AT86RF212_ResetDis();		//Reset = H;
	AT86RF212_Dely1us(400);		//延时400us

	AT86RF212_ResetEn();		//Reset = L;
	AT86RF212_Dely1us(1);		//延时625ns,复位脉冲宽度
	AT86RF212_ResetDis();		//Reset = H;
}

/********************************************************************************************************
** 函数名称: AT86RF212_PhysicalConfig
** 功能描述: AT86RF212无线硬件相关配置
**					
** 输　入: 	bandrate :设置空中码片速率
**						@ BPSK_20	
**						@ BPSK_40						
**						@ OQPSK_SIN_RC_100				
**						@ OQPSK_SIN_RC_200			
**						@ OQPSK_SIN_RC_400_SCR_ON 
**						@ OQPSK_SIN_RC_400_SCR_OFF		
**						@ OQPSK_SIN_250 			
**						@ OQPSK_SIN_500 				
**						@ OQPSK_SIN_1000_SCR_ON 	
**						@ OQPSK_SIN_1000_SCR_OFF 		
**						@ OQPSK_RC_250				
**						@ OQPSK_RC_500 					
**						@ OQPSK_RC_1000_SCR_ON 			
**						@ OQPSK_RC_1000_SCR_OFF			
**			txoutpwr：发送功率
**						@ TX_OUTPWR_P5DBM-----------------+5dBm
**						@ TX_OUTPWR_P4DBM-----------------+4dBm
**						@ TX_OUTPWR_P3DBM-----------------+3dBm
**						@ TX_OUTPWR_P2DBM-----------------+2dBm
**						@ TX_OUTPWR_P1DBM-----------------+1dBm
**						@ TX_OUTPWR_P0DBM-----------------+0dBm
**						@ TX_OUTPWR_N1DBM------------------1dBm
**						@ TX_OUTPWR_N2DBM------------------2dBm
**						@ TX_OUTPWR_N3DBM------------------3dBm
**						@ TX_OUTPWR_N4DBM------------------4dBm
**						@ TX_OUTPWR_N5DBM------------------5dBm
**						@ TX_OUTPWR_N6DBM------------------6dBm
**						@ TX_OUTPWR_N7DBM------------------7dBm
**						@ TX_OUTPWR_N8DBM------------------8dBm
**						@ TX_OUTPWR_N9DBM------------------9dBm
**						@ TX_OUTPWR_N10DBM------------------10dBm
**						@ TX_OUTPWR_N11DBM------------------11dBm
**			pa_stattime：PA提前启动时间
**						@ PA_STAT_2US------------------提前2us启动
**						@ PA_STAT_4US------------------提前4us启动
**						@ PA_STAT_6US------------------提前6us启动
**						@ PA_STAT_8US------------------提前8us启动
**
*********************************************************************************************************/
void AT86RF212_PhysicalConfig(uint8_t bandrate,uint8_t txoutpwr,uint8_t pa_stattime)
{			
	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_2_ADDR,bandrate,0x3f);		//传输速率

	if((bandrate==BPSK_20)||(bandrate==BPSK_40)) {
		AT86RF212_ReadModifyWriteReg(RG_RF_CTRL_0_ADDR,0x03,0x03);			//BPSQ模式及OQPSK模式选择
	} else {
		AT86RF212_ReadModifyWriteReg(RG_RF_CTRL_0_ADDR,0x02,0x03);			//BPSQ模式及OQPSK模式选择
	}
	
	AT86RF212_ReadModifyWriteReg(RG_PHY_TX_PWR_ADDR,txoutpwr,0xff);		//TX输出功率设置
	
	AT86RF212_ReadModifyWriteReg(RG_RF_CTRL_0_ADDR,pa_stattime,0xc0);	//设置PA在发送信号前的启动时间

	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_2_ADDR,0x20,0x20);	//启动扰码器
}

/********************************************************************************************************
** 函数名称: AT86RF212_RFChannelConfig
** 功能描述: AT86RF212信道切换，本函数只针对中国专用频率的信道切换
**					
** 输　入: 	channel :信道编号
**			@ CHINESEBAND780MHZ----------780MHz
**			@ CHINESEBAND782MHZ----------782MHz
			@ CHINESEBAND784MHZ----------784MHz
			@ CHINESEBAND786MHZ----------786MHz
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void AT86RF212_RFChannelConfig(uint8_t RFband,uint16_t frequency)
{
	
    uint8_t state,delay,ccnumber;

    state = AT86RF212_GetTRXStatus();
	if(state==TRX_STATUS_TRX_OFF) {
		delay = 110;		//延时110us
	} else if((state==TRX_STATUS_RX_ON)||(state==TRX_STATUS_PLL_ON)) {
		delay = 11;			//延时11us
	}
	if(RFband==0) {				//IEEE 802.15.4

	} else if((RFband==1)||(RFband==2)||(RFband==3)) {
		if((frequency>769)&&(frequency<794)) {
	   		ccnumber = (frequency-769) * 10;
		} else if((frequency>857)&&(frequency<882)) {
			ccnumber = (frequency-857) * 10;
		} else if((frequency>903)&&(frequency<928)) {
		   	ccnumber = (frequency-903) * 10;
		}
	} else if(RFband==4) {
		ccnumber = frequency-769;
	} else if(RFband==5) {
		ccnumber = frequency-833;
	}
    AT86RF212_ReadModifyWriteReg(RG_CC_CTRL_1_ADDR, RFband, 0x07);
	AT86RF212_ReadModifyWriteReg(RG_CC_CTRL_0_ADDR, ccnumber, 0xff);

	AT86RF212_Dely1us(delay);
}

void AT86RF212_RFChannelConfig_old(uint8_t channel)
{
	
    uint8_t state,delay;
	
    state = AT86RF212_GetTRXStatus();
	if(state==TRX_STATUS_TRX_OFF) {
		delay = 110;		//延时110us
	} else if((state==TRX_STATUS_RX_ON)||(state==TRX_STATUS_PLL_ON)) {
		delay = 11;			//延时11us
	}
	//CC_BAND = 4, CC_NUMBER = 0--94
    AT86RF212_ReadModifyWriteReg(RG_CC_CTRL_1_ADDR, 0x04, 0x07);
	AT86RF212_ReadModifyWriteReg(RG_CC_CTRL_0_ADDR, channel, 0xff);

	AT86RF212_Dely1us(delay);
}

/********************************************************************************************************
** 函数名称: AT86RF212_FrameFilterConfig
** 功能描述: AT86RF212帧滤波器设置
**					
*********************************************************************************************************/
void AT86RF212_FrameFilterConfig(void)
{
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_PAN_ID_0_ADDR, AT86RF212_PAN_ID0);
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_PAN_ID_1_ADDR, AT86RF212_PAN_ID1);
	
	// set short addr
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_SHORT_ADDR_0_ADDR, AT86RF212_SHORT_ADDR0);
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_SHORT_ADDR_1_ADDR, AT86RF212_SHORT_ADDR1);

	//IEEE_ADDR扩展地址设备
	AT86RF212_ReadModifyWriteReg(RG_XAH_CTRL_1_ADDR,AT86RF212_AACK_PROM_MODE,0X01); 	//混杂模式控制
	AT86RF212_ReadModifyWriteReg(RG_XAH_CTRL_1_ADDR,(AT86RF212_AACK_UPLD_RES_FT<<4),0x10); 	//预留帧类型控制
	AT86RF212_ReadModifyWriteReg(RG_XAH_CTRL_1_ADDR,(AT86RF212_AACK_FLTR_RES_FT<<5),0x20); 	//滤波器预留帧类型控制
	AT86RF212_ReadModifyWriteReg(RG_CSMA_SEED_1_ADDR,(AT86RF212_AACK_FVN_MODE<<6),0xc0); 	//FCF帧版本控制	
}

/********************************************************************************************************
** 函数名称: AT86RF212_IRQServe
** 功能描述: AT86RF212,中断处理函数
**					
*********************************************************************************************************/
void AT86RF212_IRQServe(void)
{
	uint8_t	irq_status,trx_status,phy_rssi;

	irq_status = AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_IRQ_STATUS_ADDR,0xff);				//更新中断状态寄存器
	trx_status = AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_TRX_STATUS_ADDR,0xff);				//更新收发器状态

	if(irq_status&REG_IRQ_STATUS_PLL_LOCK) {					//===========PLL锁定状态
	//	printf("=0\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_PLL_UNLOCK) {		//===========PLL未锁定状态
	//	printf("=1\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_RX_START) {			//===========PSDU接收的开始
		//printf("=2\r\n");
		//printf("=start=");
	}
	if(irq_status&REG_IRQ_STATUS_TRX_END) {			//===========发送/接收完成
		if(trx_status&TRX_STATUS_PLL_ON) {			//收发器处于发送模式
		//	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			AT86RF212_TRXStatusModeChange(CMD_RX_ON);			//切换至接收状态
			RF212SendFinish = TRUE;
		} else if(trx_status&TRX_STATUS_RX_ON) {	//收发器处于接收模式
			phy_rssi = AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_PHY_RSSI_ADDR,0xff);
			if(phy_rssi&0x80) {					//CRC校验成功，则接收数据
				
				networkED=AT86RF212_ReadED();//存储ED值
				
				AT86RF212_ReadFrameBuffer(AT86RF212ReceiveData.RxBuffer);
				RF212RevSuccess = TRUE;		
			}  else {
				printf("-2\r\n");
			}
		} 
		//printf("=3");
	}
	if(irq_status&REG_IRQ_STATUS_CCA_ED_DONE) {		//===========多功能中断
		//暂时不使用
	//	printf("=4\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_AMI) {				//===========显示地址匹配
		//printf("=5\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_TRX_UR) {				//===========帧缓冲的访问冲突
	//	printf("=6\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_BAT_LOW) {			//===========电源电压低
	//	printf("=7\r\n");
	}
	delay_ms(1000);
}

/********************************************************************************************************
** 函数名称: AT86RF212_EnterTXModeSendData
** 功能描述: AT86RF212进入发送模式并且发送数据,

**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void AT86RF212_EnterTXModeSendData(uint8_t* pdata,uint8_t Nbyte)
{	
	AT86RF212_WriteFrameBuffer(pdata,Nbyte); 				//写数据

//	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	if(AT86RF212_GetTRXStatus()!=TRX_STATUS_PLL_ON) { 		//无线收发器是否处于发送状态，否，则进入发送状态
		AT86RF212_TRXStatusModeChange(CMD_PLL_ON);			//进入PLL_ON状态
		while(AT86RF212_GetTRXStatus()!=TRX_STATUS_PLL_ON);	//等待进入PLL_ON状态
	}
	if(AT86RF212_SLP_TRStatus()) {			
		AT86RF212_SLP_TRReset();							//如果SLP_TR为高，则变低，启动发送
	}
	AT86RF212_SLP_TRSet();									//L--H的变化启动发送
	AT86RF212_SLP_TRReset();

//	AT86RF212_WriteFrameBuffer(pdata,Nbyte); 				//写数据
}

/********************************************************************************************************
** 函数名称: AT86RF212_StatusModeChange
** 功能描述: AT86RF212状态模式切换
*********************************************************************************************************/
void AT86RF212_TRXStatusModeChange(uint8_t mode)
{
	uint8_t curr_state;
			  	
	curr_state = AT86RF212_GetTRXStatus();		//无线收发器正在忙，则等待
	while((curr_state==TRX_STATUS_BUSY_RX)||(curr_state==TRX_STATUS_BUSY_TX)||(curr_state==TRX_STATUS_BUSY_RX_AACK)||
			(curr_state==TRX_STATUS_BUSY_TX_ARET)||(curr_state==TRX_STATUS_SYS_IN_PROGRESS)) {
		curr_state = AT86RF212_GetTRXStatus();
	}											 
	AT86RF212_ReadModifyWriteReg(RG_TRX_STATE_ADDR,mode,0x1f);		
}

/********************************************************************************************************
** 函数名称: AT86RF212_Init
** 功能描述: AT86RF212初始化
**					
*********************************************************************************************************/
void AT86RF212_Init(void)
{
	uint8_t tmpreg;
	uint32_t	i,ltemp;
	
	AT86RF212_Reset();					//上电处理
	
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_XOSC_CTRL_ADDR,0x40);//设置为外置晶振时钟，云天创模块专用
	
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_IRQ_MASK_ADDR,0x00);			//禁止所有中断

	AT86RF212_ReadModifyWriteReg(RG_TRX_STATE_ADDR,CMD_FORCE_TRX_OFF,0x1f);			//强制关闭无线收发器

   	while (AT86RF212_GetTRXStatus() != TRX_STATUS_TRX_OFF);							//确认无线收发器已经关闭
	//晶体振荡器配置
	AT86RF212_ReadModifyWriteReg(RG_RX_CTRL_ADDR,0x20,0x20);						//激活晶体振荡器

	//设置通道
	AT86RF212_PhysicalConfig(OQPSK_RC_250,TX_OUTPWR_P5DBM,PA_STAT_8US);
	//AT86RF212_RFChannelConfig_old(CHINESEBAND782MHZ);
	AT86RF212_RFChannelConfig(RFBAND_1,FRQ_CHINESEBAND784MHZ);

	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_0_ADDR,0x00,0x3f);						//CLKM配置,无输出
	
	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_2_ADDR,0x40,0x40);						//开启模拟电压调节器（VAREG）

	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_1_ADDR,0x20,0x20);						//设置自动生成CRC

	//AT86RF212_ReadModifyWriteReg(RG_CSMA_SEED_1_ADDR,0x10,0x10);					//关闭自动应答
	
	AT86RF212_FrameFilterConfig();													//滤波器设置

	//AT86RF212_ReadModifyWriteReg(RG_SFD_VALUE_ADDR,0x5f,0xff);						//帧起始介定符============
	//AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_SFD_VALUE_ADDR,0xa7);
	AT86RF212_ReadModifyWriteReg(RG_SFD_VALUE_ADDR,0xa7,0xff);						//帧起始介定符============

	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_1_ADDR, AT86RF212_IRQ_POLARITY,0x01);  	//中断输出电平模式配置
	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_1_ADDR, AT86RF212_IRQMASK_MODE,0x02);  	//中断查询模式配置
	tmpreg = 0;
	tmpreg = ((uint8_t)AT86RF212_IRQMASK_PLL_LOCK<<0)|			//PLL锁定中断控制
			 ((uint8_t)AT86RF212_IRQMASK_PLL_UNLOCK<<1)|		//PLL未锁定中断控制
			 ((uint8_t)AT86RF212_IRQMASK_RX_START<<2)|			//接收数据开始中断控制
			 ((uint8_t)AT86RF212_IRQMASK_TRX_END<<3)|			//帧接收完成中断控制
			 ((uint8_t)AT86RF212_IRQMASK_CCA_ED_DONE<<4)|		//多功能中断控制
			 ((uint8_t)AT86RF212_IRQMASK_AMI<<5)|				//显示地址匹配中断控制
			 ((uint8_t)AT86RF212_IRQMASK_TRX_UR<<6)|			//显示帧缓冲访问冲突中断控制
			 ((uint8_t)AT86RF212_IRQMASK_BAT_LOW<<7);
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_IRQ_MASK_ADDR,tmpreg);  			//中断屏蔽设置

	AT86RF212_TRXStatusModeChange(CMD_RX_ON);		   									//进入接收模式
	AT86RF212_Dely1us(200);										//TRX_OFF---RX_ON为200us
}


/********************************************************************************************************
** 函数名称: ReadyData
** 功能描述: 数据头参数初始化
** 作者:		 OKIMBIN
*********************************************************************************************************/

void ReadyDataTo(uint8_t DestAddress)
{
	//uFCF ufcf;
	//uint16_t d;
	//fcf.Bits.FrameType=0;
	//fcf.Bits.FrameType=0xdd;
	//Send( (char *)&sendCommuPacket , sizeof(CommuPacket) );
	//AT86RF212SendData.TxLength=0x13+2; //19个字节长度+2为CRC
	AT86RF212SendData.MAC_FCF[0]=0x61;//低位FcF  10 00 10 000 1 0 0 0 001
	AT86RF212SendData.MAC_FCF[1]=0x88;//高位FcF
	AT86RF212SendData.Samesign=0x0a;
	AT86RF212SendData.PAN_ID[0]=AT86RF212_PAN_ID0;//ToPANID;////PAN_ID
	AT86RF212SendData.PAN_ID[1]=AT86RF212_PAN_ID1;//0x00;
	AT86RF212SendData.DestAddr[0]=DestAddress;//目标短地址
	AT86RF212SendData.DestAddr[1]=0x00;
	AT86RF212SendData.SourceAddr[0]=AT86RF212_SHORT_ADDR0; //源短地址
	AT86RF212SendData.SourceAddr[1]=AT86RF212_SHORT_ADDR1;	
	
}


uint8_t AT86RF212_ReadED()
{	uint8_t ED;
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_PHY_ED_LEVEL_ADDR, 0);	
	delay_us(12);
	ED= AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_PHY_ED_LEVEL_ADDR,0xff);//能量监测值
	return ED;
}

/********************************************End Of File************************************************/

