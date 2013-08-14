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
** ��������: AT86RF212_ReadWriteReg
** ��������: AT86RF212���Ĵ�������
**					
** �䡡  ��: cmd��ָ�addr����ַ��Data������
**
** �䡡  ��: ������һ��SPI����ֵ
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
uint8_t AT86RF212_ReadWriteReg(uint8_t cmd,uint8_t addr,uint8_t Data)
{  
	uint8_t r_data;

	AT86RF212_SELEnable();
  	ReadWriteByte(cmd|addr);			    	//ָ��
  	r_data = ReadWriteByte(Data);				//д����
	AT86RF212_SELDisable(); 
	
	return (r_data); 
}

/********************************************************************************************************
** ��������: AT86RF212_ReadModifyWriteReg
** ��������: AT86RF212�ԼĴ����Ķ���д����
**					
** �䡡  ��: addr����ַ��val������,mask��Ҫ��дλ������
**
** �䡡  ��: ���phy_status״̬
*********************************************************************************************************/
uint8_t AT86RF212_ReadModifyWriteReg(uint8_t addr,uint8_t val,uint8_t mask)
{
	uint8_t tmpreg,phy_status;

	AT86RF212_SELEnable();
  	phy_status = ReadWriteByte(SPI_CMD_READ_REGISTER|addr);		//��ָ��
  	tmpreg = ReadWriteByte(0xff);									//��������
	AT86RF212_SELDisable();
	
	val &= mask;                	// mask off stray bits from val
    tmpreg &= ~mask;               // mask off bits in reg val
    tmpreg |= val;                 // copy val into reg val
    
	AT86RF212_SELEnable();
	ReadWriteByte(SPI_CMD_WRITE_REGISTER|addr);		//дָ��
  	ReadWriteByte(tmpreg);								//д����
	AT86RF212_SELDisable();

	return (phy_status);
}

/********************************************************************************************************
** ��������: AT86RF212_WriteFrameBuffer
** ��������: AT86RF212д֡����
**					
** �䡡  ��: pdata:д������ָ��,Nbyte:д�����ݳ���
**
** �䡡  ��: ���phy_status״̬
*********************************************************************************************************/
uint8_t AT86RF212_WriteFrameBuffer(uint8_t* pdata,uint8_t Nbyte)
{
	uint8_t phy_status,buf[127];// = {0xff};

	AT86RF212_SELEnable();
  
  phy_status = ReadWriteByte(SPI_CMD_WRITE_FRAMEBUF);	//ָ��
	
	ReadWriteByte(Nbyte+2);								//д֡���� +2�ֽڵ�CRC
	ReadWriteBytes(buf,pdata,Nbyte);
	AT86RF212_SELDisable(); 
	
	return (phy_status);
}

/********************************************************************************************************
** ��������: AT86RF212_ReadFrameBuffer
** ��������: AT86RF212��֡����
**					
** �䡡  ��: Rt:��������ָ��
**
** �䡡  ��: ���phy_status״̬
*********************************************************************************************************/
uint8_t AT86RF212_ReadFrameBuffer(uint8_t* Rt)
{
	uint8_t i,phy_status,buf[127];
	
	AT86RF212_SELEnable();

  	phy_status = ReadWriteByte(SPI_CMD_READ_FRAMEBUF);					//ָ��
	AT86RF212ReceiveData.RxLength = ReadWriteByte(0xff) - 2;			//����֡����-2�ֽ�CRC
   	/*
	for(i=0;i<AT86RF212ReceiveData.RxLength;i++) {			 					//
		*Rt++ = ReadWriteByte(0xff);
	} */	  
	ReadWriteBytes(Rt,buf,AT86RF212ReceiveData.RxLength);

	//*RSSI=ReadWriteByte(0xff);				//��·����ָʾ

	ReadWriteByte(0xff);				//RX_STATUS״̬

	AT86RF212_SELDisable(); 

	return (phy_status);
}

/********************************************************************************************************
** ��������: AT86RF212_WriteSRAM
** ��������: AT86RF212дSRAM
**					
** �䡡  ��: pdata:д������ָ��,Nbyte:д�����ݳ���
**
** �䡡  ��: ���phy_status״̬
*********************************************************************************************************/
uint8_t AT86RF212_WriteSRAM(uint8_t addr,uint8_t* pdata,uint8_t Nbyte)
{
	uint8_t i,phy_status;
	
	AT86RF212_SELEnable();

  	phy_status = ReadWriteByte(SPI_CMD_WRITE_SRAM);			//ָ��
	ReadWriteByte(addr);													//д��ַ
	for(i=0;i<(Nbyte+2);i++) {															//д����,Ҫ����2�ֽ�CRC
		ReadWriteByte(*pdata++);
	}
	
	AT86RF212_SELDisable(); 
	
	return (phy_status);
}

/********************************************************************************************************
** ��������: AT86RF212_ReadSRAM
** ��������: AT86RF212��SRAM
**					
** �䡡  ��: Rt:��������ָ�룬Nbyte����ȡ���ݳ���
**
** �䡡  ��: ���phy_status״̬
*********************************************************************************************************/
uint8_t AT86RF212_ReadSRAM(uint8_t addr,uint8_t* Rt,uint8_t Nbyte)
{
	uint8_t i,phy_status;
	
	AT86RF212_SELEnable();

  	phy_status = ReadWriteByte(SPI_CMD_READ_SRAM);			//ָ��
	ReadWriteByte(addr);										//д��ַ
	for(i=0;i<Nbyte;i++) {
		*Rt++ = ReadWriteByte(0xff);
	}
	
	AT86RF212_SELDisable(); 
	return (phy_status);
}

/********************************************************************************************************
** ��������: AT86RF212_GetTRXStatus
** ��������: ��ȡAT86RF212��TRXStatus״̬�Ĵ���
**					
** �䡡  ��: ��
**
** �䡡  ��: ���TRXStatus״̬
*********************************************************************************************************/
uint8_t AT86RF212_GetTRXStatus(void)
{
	return (AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_TRX_STATUS_ADDR,0xff) & 0x1f);
}


/********************************************************************************************************
** ��������: AT86RF212_Reset
** ��������: AT86RF212��λ����
*********************************************************************************************************/
void AT86RF212_Reset(void)
{	
	uint32_t	i,ltemp;
	uint8_t		temp;

	AT86RF212_SLP_TRReset();	//AT86RF212_SLP_TR = L
	AT86RF212_SELDisable();		//SEL = H	
	AT86RF212_ResetDis();		//Reset = H;
	AT86RF212_Dely1us(400);		//��ʱ400us

	AT86RF212_ResetEn();		//Reset = L;
	AT86RF212_Dely1us(1);		//��ʱ625ns,��λ������
	AT86RF212_ResetDis();		//Reset = H;
}

/********************************************************************************************************
** ��������: AT86RF212_PhysicalConfig
** ��������: AT86RF212����Ӳ���������
**					
** �䡡��: 	bandrate :���ÿ�����Ƭ����
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
**			txoutpwr�����͹���
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
**			pa_stattime��PA��ǰ����ʱ��
**						@ PA_STAT_2US------------------��ǰ2us����
**						@ PA_STAT_4US------------------��ǰ4us����
**						@ PA_STAT_6US------------------��ǰ6us����
**						@ PA_STAT_8US------------------��ǰ8us����
**
*********************************************************************************************************/
void AT86RF212_PhysicalConfig(uint8_t bandrate,uint8_t txoutpwr,uint8_t pa_stattime)
{			
	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_2_ADDR,bandrate,0x3f);		//��������

	if((bandrate==BPSK_20)||(bandrate==BPSK_40)) {
		AT86RF212_ReadModifyWriteReg(RG_RF_CTRL_0_ADDR,0x03,0x03);			//BPSQģʽ��OQPSKģʽѡ��
	} else {
		AT86RF212_ReadModifyWriteReg(RG_RF_CTRL_0_ADDR,0x02,0x03);			//BPSQģʽ��OQPSKģʽѡ��
	}
	
	AT86RF212_ReadModifyWriteReg(RG_PHY_TX_PWR_ADDR,txoutpwr,0xff);		//TX�����������
	
	AT86RF212_ReadModifyWriteReg(RG_RF_CTRL_0_ADDR,pa_stattime,0xc0);	//����PA�ڷ����ź�ǰ������ʱ��

	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_2_ADDR,0x20,0x20);	//����������
}

/********************************************************************************************************
** ��������: AT86RF212_RFChannelConfig
** ��������: AT86RF212�ŵ��л���������ֻ����й�ר��Ƶ�ʵ��ŵ��л�
**					
** �䡡��: 	channel :�ŵ����
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
		delay = 110;		//��ʱ110us
	} else if((state==TRX_STATUS_RX_ON)||(state==TRX_STATUS_PLL_ON)) {
		delay = 11;			//��ʱ11us
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
		delay = 110;		//��ʱ110us
	} else if((state==TRX_STATUS_RX_ON)||(state==TRX_STATUS_PLL_ON)) {
		delay = 11;			//��ʱ11us
	}
	//CC_BAND = 4, CC_NUMBER = 0--94
    AT86RF212_ReadModifyWriteReg(RG_CC_CTRL_1_ADDR, 0x04, 0x07);
	AT86RF212_ReadModifyWriteReg(RG_CC_CTRL_0_ADDR, channel, 0xff);

	AT86RF212_Dely1us(delay);
}

/********************************************************************************************************
** ��������: AT86RF212_FrameFilterConfig
** ��������: AT86RF212֡�˲�������
**					
*********************************************************************************************************/
void AT86RF212_FrameFilterConfig(void)
{
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_PAN_ID_0_ADDR, AT86RF212_PAN_ID0);
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_PAN_ID_1_ADDR, AT86RF212_PAN_ID1);
	
	// set short addr
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_SHORT_ADDR_0_ADDR, AT86RF212_SHORT_ADDR0);
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_SHORT_ADDR_1_ADDR, AT86RF212_SHORT_ADDR1);

	//IEEE_ADDR��չ��ַ�豸
	AT86RF212_ReadModifyWriteReg(RG_XAH_CTRL_1_ADDR,AT86RF212_AACK_PROM_MODE,0X01); 	//����ģʽ����
	AT86RF212_ReadModifyWriteReg(RG_XAH_CTRL_1_ADDR,(AT86RF212_AACK_UPLD_RES_FT<<4),0x10); 	//Ԥ��֡���Ϳ���
	AT86RF212_ReadModifyWriteReg(RG_XAH_CTRL_1_ADDR,(AT86RF212_AACK_FLTR_RES_FT<<5),0x20); 	//�˲���Ԥ��֡���Ϳ���
	AT86RF212_ReadModifyWriteReg(RG_CSMA_SEED_1_ADDR,(AT86RF212_AACK_FVN_MODE<<6),0xc0); 	//FCF֡�汾����	
}

/********************************************************************************************************
** ��������: AT86RF212_IRQServe
** ��������: AT86RF212,�жϴ�����
**					
*********************************************************************************************************/
void AT86RF212_IRQServe(void)
{
	uint8_t	irq_status,trx_status,phy_rssi;

	irq_status = AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_IRQ_STATUS_ADDR,0xff);				//�����ж�״̬�Ĵ���
	trx_status = AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_TRX_STATUS_ADDR,0xff);				//�����շ���״̬

	if(irq_status&REG_IRQ_STATUS_PLL_LOCK) {					//===========PLL����״̬
	//	printf("=0\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_PLL_UNLOCK) {		//===========PLLδ����״̬
	//	printf("=1\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_RX_START) {			//===========PSDU���յĿ�ʼ
		//printf("=2\r\n");
		//printf("=start=");
	}
	if(irq_status&REG_IRQ_STATUS_TRX_END) {			//===========����/�������
		if(trx_status&TRX_STATUS_PLL_ON) {			//�շ������ڷ���ģʽ
		//	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			AT86RF212_TRXStatusModeChange(CMD_RX_ON);			//�л�������״̬
			RF212SendFinish = TRUE;
		} else if(trx_status&TRX_STATUS_RX_ON) {	//�շ������ڽ���ģʽ
			phy_rssi = AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_PHY_RSSI_ADDR,0xff);
			if(phy_rssi&0x80) {					//CRCУ��ɹ������������
				
				networkED=AT86RF212_ReadED();//�洢EDֵ
				
				AT86RF212_ReadFrameBuffer(AT86RF212ReceiveData.RxBuffer);
				RF212RevSuccess = TRUE;		
			}  else {
				printf("-2\r\n");
			}
		} 
		//printf("=3");
	}
	if(irq_status&REG_IRQ_STATUS_CCA_ED_DONE) {		//===========�๦���ж�
		//��ʱ��ʹ��
	//	printf("=4\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_AMI) {				//===========��ʾ��ַƥ��
		//printf("=5\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_TRX_UR) {				//===========֡����ķ��ʳ�ͻ
	//	printf("=6\r\n");
	}
	if(irq_status&REG_IRQ_STATUS_BAT_LOW) {			//===========��Դ��ѹ��
	//	printf("=7\r\n");
	}
	delay_ms(1000);
}

/********************************************************************************************************
** ��������: AT86RF212_EnterTXModeSendData
** ��������: AT86RF212���뷢��ģʽ���ҷ�������,

**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void AT86RF212_EnterTXModeSendData(uint8_t* pdata,uint8_t Nbyte)
{	
	AT86RF212_WriteFrameBuffer(pdata,Nbyte); 				//д����

//	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	if(AT86RF212_GetTRXStatus()!=TRX_STATUS_PLL_ON) { 		//�����շ����Ƿ��ڷ���״̬��������뷢��״̬
		AT86RF212_TRXStatusModeChange(CMD_PLL_ON);			//����PLL_ON״̬
		while(AT86RF212_GetTRXStatus()!=TRX_STATUS_PLL_ON);	//�ȴ�����PLL_ON״̬
	}
	if(AT86RF212_SLP_TRStatus()) {			
		AT86RF212_SLP_TRReset();							//���SLP_TRΪ�ߣ����ͣ���������
	}
	AT86RF212_SLP_TRSet();									//L--H�ı仯��������
	AT86RF212_SLP_TRReset();

//	AT86RF212_WriteFrameBuffer(pdata,Nbyte); 				//д����
}

/********************************************************************************************************
** ��������: AT86RF212_StatusModeChange
** ��������: AT86RF212״̬ģʽ�л�
*********************************************************************************************************/
void AT86RF212_TRXStatusModeChange(uint8_t mode)
{
	uint8_t curr_state;
			  	
	curr_state = AT86RF212_GetTRXStatus();		//�����շ�������æ����ȴ�
	while((curr_state==TRX_STATUS_BUSY_RX)||(curr_state==TRX_STATUS_BUSY_TX)||(curr_state==TRX_STATUS_BUSY_RX_AACK)||
			(curr_state==TRX_STATUS_BUSY_TX_ARET)||(curr_state==TRX_STATUS_SYS_IN_PROGRESS)) {
		curr_state = AT86RF212_GetTRXStatus();
	}											 
	AT86RF212_ReadModifyWriteReg(RG_TRX_STATE_ADDR,mode,0x1f);		
}

/********************************************************************************************************
** ��������: AT86RF212_Init
** ��������: AT86RF212��ʼ��
**					
*********************************************************************************************************/
void AT86RF212_Init(void)
{
	uint8_t tmpreg;
	uint32_t	i,ltemp;
	
	AT86RF212_Reset();					//�ϵ紦��
	
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_XOSC_CTRL_ADDR,0x40);//����Ϊ���þ���ʱ�ӣ����촴ģ��ר��
	
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_IRQ_MASK_ADDR,0x00);			//��ֹ�����ж�

	AT86RF212_ReadModifyWriteReg(RG_TRX_STATE_ADDR,CMD_FORCE_TRX_OFF,0x1f);			//ǿ�ƹر������շ���

   	while (AT86RF212_GetTRXStatus() != TRX_STATUS_TRX_OFF);							//ȷ�������շ����Ѿ��ر�
	//������������
	AT86RF212_ReadModifyWriteReg(RG_RX_CTRL_ADDR,0x20,0x20);						//���������

	//����ͨ��
	AT86RF212_PhysicalConfig(OQPSK_RC_250,TX_OUTPWR_P5DBM,PA_STAT_8US);
	//AT86RF212_RFChannelConfig_old(CHINESEBAND782MHZ);
	AT86RF212_RFChannelConfig(RFBAND_1,FRQ_CHINESEBAND784MHZ);

	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_0_ADDR,0x00,0x3f);						//CLKM����,�����
	
	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_2_ADDR,0x40,0x40);						//����ģ���ѹ��������VAREG��

	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_1_ADDR,0x20,0x20);						//�����Զ�����CRC

	//AT86RF212_ReadModifyWriteReg(RG_CSMA_SEED_1_ADDR,0x10,0x10);					//�ر��Զ�Ӧ��
	
	AT86RF212_FrameFilterConfig();													//�˲�������

	//AT86RF212_ReadModifyWriteReg(RG_SFD_VALUE_ADDR,0x5f,0xff);						//֡��ʼ�鶨��============
	//AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_SFD_VALUE_ADDR,0xa7);
	AT86RF212_ReadModifyWriteReg(RG_SFD_VALUE_ADDR,0xa7,0xff);						//֡��ʼ�鶨��============

	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_1_ADDR, AT86RF212_IRQ_POLARITY,0x01);  	//�ж������ƽģʽ����
	AT86RF212_ReadModifyWriteReg(RG_TRX_CTRL_1_ADDR, AT86RF212_IRQMASK_MODE,0x02);  	//�жϲ�ѯģʽ����
	tmpreg = 0;
	tmpreg = ((uint8_t)AT86RF212_IRQMASK_PLL_LOCK<<0)|			//PLL�����жϿ���
			 ((uint8_t)AT86RF212_IRQMASK_PLL_UNLOCK<<1)|		//PLLδ�����жϿ���
			 ((uint8_t)AT86RF212_IRQMASK_RX_START<<2)|			//�������ݿ�ʼ�жϿ���
			 ((uint8_t)AT86RF212_IRQMASK_TRX_END<<3)|			//֡��������жϿ���
			 ((uint8_t)AT86RF212_IRQMASK_CCA_ED_DONE<<4)|		//�๦���жϿ���
			 ((uint8_t)AT86RF212_IRQMASK_AMI<<5)|				//��ʾ��ַƥ���жϿ���
			 ((uint8_t)AT86RF212_IRQMASK_TRX_UR<<6)|			//��ʾ֡������ʳ�ͻ�жϿ���
			 ((uint8_t)AT86RF212_IRQMASK_BAT_LOW<<7);
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_IRQ_MASK_ADDR,tmpreg);  			//�ж���������

	AT86RF212_TRXStatusModeChange(CMD_RX_ON);		   									//�������ģʽ
	AT86RF212_Dely1us(200);										//TRX_OFF---RX_ONΪ200us
}


/********************************************************************************************************
** ��������: ReadyData
** ��������: ����ͷ������ʼ��
** ����:		 OKIMBIN
*********************************************************************************************************/

void ReadyDataTo(uint8_t DestAddress)
{
	//uFCF ufcf;
	//uint16_t d;
	//fcf.Bits.FrameType=0;
	//fcf.Bits.FrameType=0xdd;
	//Send( (char *)&sendCommuPacket , sizeof(CommuPacket) );
	//AT86RF212SendData.TxLength=0x13+2; //19���ֽڳ���+2ΪCRC
	AT86RF212SendData.MAC_FCF[0]=0x61;//��λFcF  10 00 10 000 1 0 0 0 001
	AT86RF212SendData.MAC_FCF[1]=0x88;//��λFcF
	AT86RF212SendData.Samesign=0x0a;
	AT86RF212SendData.PAN_ID[0]=AT86RF212_PAN_ID0;//ToPANID;////PAN_ID
	AT86RF212SendData.PAN_ID[1]=AT86RF212_PAN_ID1;//0x00;
	AT86RF212SendData.DestAddr[0]=DestAddress;//Ŀ��̵�ַ
	AT86RF212SendData.DestAddr[1]=0x00;
	AT86RF212SendData.SourceAddr[0]=AT86RF212_SHORT_ADDR0; //Դ�̵�ַ
	AT86RF212SendData.SourceAddr[1]=AT86RF212_SHORT_ADDR1;	
	
}


uint8_t AT86RF212_ReadED()
{	uint8_t ED;
	AT86RF212_ReadWriteReg(SPI_CMD_WRITE_REGISTER,RG_PHY_ED_LEVEL_ADDR, 0);	
	delay_us(12);
	ED= AT86RF212_ReadWriteReg(SPI_CMD_READ_REGISTER,RG_PHY_ED_LEVEL_ADDR,0xff);//�������ֵ
	return ED;
}

/********************************************End Of File************************************************/

