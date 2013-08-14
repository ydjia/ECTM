#define __at86rf212_h
#ifndef _AT86RF212_H_
#define	_AT86RF212_H_
#ifndef _AT86RF212_GLOBAL_
#define	AT86RF212_GLOBAL	extern
#else
#define	AT86RF212_GLOBAL
#endif

#include "sourcetype.h"
#include "network.h"
#include "global.h"
#define TRUE 1
typedef		unsigned char 		uint8_t;
typedef		unsigned short 			uint16_t;
typedef		unsigned int 			uint32_t;

//=============AT86RF212���ƹܽŶ��h�
/*�޸�Ϊ���촴ģ��ӿ� 2013.05.08
#define		AT86RF212_RESET_PIN		GPIO_Pin_5			//PB5
#define		AT86RF212_SLP_TR_PIN	GPIO_Pin_6 		 //PB6

#define		AT86RF212_IRQ_PIN		GPIO_Pin_4			//PB4

#define		AT86RF212_SEL_PIN		GPIO_Pin_4			//PA4
#define		AT86RF212_MOSI_PIN		GPIO_Pin_7			//PA7
#define		AT86RF212_MISO_PIN		GPIO_Pin_6			//PA6
#define		AT86RF212_SCK_PIN		GPIO_Pin_5		//PA5
#define		AT86RF212_CLK_M_PIN	 	//û������
*/

#define		AT86RF212_ResetEn()			GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define		AT86RF212_ResetDis()		GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define		AT86RF212_SLP_TRReset()		GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define		AT86RF212_SLP_TRSet()		GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define 	AT86RF212_SLP_TRStatus()	GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6)

#define		AT86RF212_SELEnable()		GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define		AT86RF212_SELDisable()		GPIO_SetBits(GPIOA,GPIO_Pin_4)

//#define		AT86RF212_DIG2Status()		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)		//��DIG2״̬

#define		AT86RF212_IRQStatus()		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)		//��IRQ����״̬


//=============AT86RF212 SPI����ָ��˴��������޸�
#define 	SPI_CMD_READ_REGISTER    	(uint8_t)0x80  		// ���Ĵ���
#define 	SPI_CMD_WRITE_REGISTER   	(uint8_t)0xc0  		// д�Ĵ���
#define 	SPI_CMD_READ_FRAMEBUF 		(uint8_t)0x20  		// ��֡����
#define 	SPI_CMD_WRITE_FRAMEBUF 		(uint8_t)0x60  		// д֡����
#define 	SPI_CMD_READ_SRAM   			(uint8_t)0x00  		// ��SRAM
#define 	SPI_CMD_WRITE_SRAM    		(uint8_t)0x40  		// дSRAM


//=============AT85RF212�Ĵ�����ַ���˴��������޸�
#define 	RG_TRX_STATUS_ADDR   	(uint8_t)0x01		//���ߵ��շ���״̬
#define 	RG_TRX_STATE_ADDR   	(uint8_t)0x02		//״̬ת���Ĵ�
#define 	RG_TRX_CTRL_0_ADDR   	(uint8_t)0x03
#define 	RG_TRX_CTRL_1_ADDR   	(uint8_t)0x04
#define 	RG_PHY_TX_PWR_ADDR  	(uint8_t)0x05
#define 	RG_PHY_RSSI_ADDR   		(uint8_t)0x06
#define 	RG_PHY_ED_LEVEL_ADDR   	(uint8_t)0x07
#define 	RG_PHY_CC_CCA_ADDR   	(uint8_t)0x08
#define 	RG_CCA_THRES_ADDR   	(uint8_t)0x09
#define 	RG_RX_CTRL_ADDR   		(uint8_t)0x0a
#define 	RG_SFD_VALUE_ADDR   	(uint8_t)0x0b
#define 	RG_TRX_CTRL_2_ADDR   	(uint8_t)0x0c
#define 	RG_ANT_DIV_ADDR   		(uint8_t)0x0d
#define 	RG_IRQ_MASK_ADDR   		(uint8_t)0x0e
#define 	RG_IRQ_STATUS_ADDR   	(uint8_t)0x0f
#define 	RG_VREG_CTRL_ADDR   	(uint8_t)0x10
#define 	RG_BATMON_ADDR   		(uint8_t)0x11
#define 	RG_XOSC_CTRL_ADDR   	(uint8_t)0x12
#define 	RG_CC_CTRL_0_ADDR   	(uint8_t)0x13
#define 	RG_CC_CTRL_1_ADDR   	(uint8_t)0x14
#define 	RG_RX_SYN_ADDR   		(uint8_t)0x15
#define 	RG_RF_CTRL_0_ADDR   	(uint8_t)0x16
#define 	RG_XAH_CTRL_1_ADDR   	(uint8_t)0x17
#define 	RG_FTN_CTRL_ADDR   		(uint8_t)0x18
#define 	RG_RF_CTRL_1_ADDR   	(uint8_t)0x19
#define 	RG_PLL_CF_ADDR   		(uint8_t)0x1a
#define 	RG_PLL_DCU_ADDR   		(uint8_t)0x1b
#define 	RG_PART_NUM_ADDR   		(uint8_t)0x1c
#define 	RG_VERSION_NUM_ADDR   	(uint8_t)0x1d
#define 	RG_MAN_ID_0_ADDR   		(uint8_t)0x1e
#define 	RG_MAN_ID_1_ADDR   		(uint8_t)0x1f
#define 	RG_SHORT_ADDR_0_ADDR   	(uint8_t)0x20
#define 	RG_SHORT_ADDR_1_ADDR   	(uint8_t)0x21
#define 	RG_PAN_ID_0_ADDR   		(uint8_t)0x22
#define 	RG_PAN_ID_1_ADDR   		(uint8_t)0x23
#define 	RG_IEEE_ADDR_0_ADDR   	(uint8_t)0x24
#define 	RG_IEEE_ADDR_1_ADDR   	(uint8_t)0x25
#define 	RG_IEEE_ADDR_2_ADDR   	(uint8_t)0x26
#define 	RG_IEEE_ADDR_3_ADDR   	(uint8_t)0x27
#define 	RG_IEEE_ADDR_4_ADDR   	(uint8_t)0x28
#define 	RG_IEEE_ADDR_5_ADDR   	(uint8_t)0x29
#define 	RG_IEEE_ADDR_6_ADDR   	(uint8_t)0x2a
#define 	RG_IEEE_ADDR_7_ADDR   	(uint8_t)0x2b
#define 	RG_XAH_CTRAL_0_ADDR   	(uint8_t)0x2c
#define 	RG_CSMA_SEED_0_ADDR   	(uint8_t)0x2d
#define 	RG_CSMA_SEED_1_ADDR   	(uint8_t)0x2e
#define 	RG_CSMA_BE_ADDR   		(uint8_t)0x2f


//=============AT85RF212�Ĵ���״̬���˴��������޸�
//=======�ж�״̬��IRQ_STATUS���Ĵ���λ����
#define 	REG_IRQ_STATUS_PLL_LOCK   		(uint8_t)0x01			//ָʾPLL����״̬
#define 	REG_IRQ_STATUS_PLL_UNLOCK   	(uint8_t)0x02			//ָʾPLLδ����״̬
#define 	REG_IRQ_STATUS_RX_START		  	(uint8_t)0x04			//ָʾPSDU���յĿ�ʼ״̬
#define 	REG_IRQ_STATUS_TRX_END		  	(uint8_t)0x08			//ָʾ����/�������
#define 	REG_IRQ_STATUS_CCA_ED_DONE	  	(uint8_t)0x10			//�๦���жϣ�1,״̬ת��;2,CCA_ED_DONE(CCA��ED��������)
#define 	REG_IRQ_STATUS_AMI			  	(uint8_t)0x20			//��ʾ��ַƥ��
#define 	REG_IRQ_STATUS_TRX_UR		  	(uint8_t)0x40			//��ʾ֡����ķ��ʳ�ͻ
#define 	REG_IRQ_STATUS_BAT_LOW		  	(uint8_t)0x80			//��Դ��ѹ��

//=======AT86RF212״̬ת������
#define 	CMD_NOP   						(uint8_t)0x00
#define 	CMD_TX_START   					(uint8_t)0x02	 	//��ʼ��������
#define 	CMD_FORCE_TRX_OFF   			(uint8_t)0x03		//ǿ�ƹر�����ǰ�ˣ������ж����е����߲���
#define 	CMD_FORCE_PLL_ON   				(uint8_t)0x04		//ǿ�ƽ���׼����������״̬
#define 	CMD_RX_ON   					(uint8_t)0x06		//��������״̬
#define 	CMD_TRX_OFF   					(uint8_t)0x08		//�ȴ���ǰ������ɺ󣬹ر�����ǰ��
#define 	CMD_PLL_ON   					(uint8_t)0x09	   	//����׼����������״̬
#define 	CMD_RX_AACK_ON   				(uint8_t)0x16	  	//��չģʽ--
#define 	CMD_TX_ARET_ON   				(uint8_t)0x19		//��չģʽ--�Զ��ش�

//=======AT86RF212�����շ�״̬����
#define 	TRX_STATUS_P_ON					(uint8_t)0x00		//�ϵ�״̬
#define 	TRX_STATUS_BUSY_RX				(uint8_t)0x01		//�������ڽ�������
#define 	TRX_STATUS_BUSY_TX				(uint8_t)0x02		//�������ڷ�������
#define 	TRX_STATUS_RX_ON				(uint8_t)0x06		//���ߴ�������״̬
#define 	TRX_STATUS_TRX_OFF				(uint8_t)0x08		//����ǰ�˹ر�
#define 	TRX_STATUS_PLL_ON				(uint8_t)0x09		//����׼������״̬
#define 	TRX_STATUS_SLEEP				(uint8_t)0x0f		//��������
#define 	TRX_STATUS_BUSY_RX_AACK			(uint8_t)0x11		//��չģʽ--æ�ڽ���Ӧ��
#define 	TRX_STATUS_BUSY_TX_ARET			(uint8_t)0x12		//��չģʽ--æ���Զ��ش�
#define 	TRX_STATUS_RX_AACK_ON			(uint8_t)0x16		//��չģʽ--����Ӧ����
#define 	TRX_STATUS_TX_ARET_ON			(uint8_t)0x19		//��չģʽ--�Զ��ش�����
#define 	TRX_STATUS_RX_ON_NOCLK			(uint8_t)0x1c		//��������״̬����CLKMʱ�����
#define 	TRX_STATUS_RX_AACK_ON_NOCLK		(uint8_t)0x1d		//��չģʽ--
#define 	TRX_STATUS_BUSY_RX_AACK_NOCLK	(uint8_t)0x1e		//��չģʽ--
#define 	TRX_STATUS_SYS_IN_PROGRESS		(uint8_t)0x1f		//��״̬ʱ�����������״̬ת��

//========AT86RF212��Ƭ���ʶ���
#define 	BPSK_20							(uint8_t)0x00
#define 	BPSK_40							(uint8_t)0x04
#define 	OQPSK_SIN_RC_100				(uint8_t)0x08
#define 	OQPSK_SIN_RC_200				(uint8_t)0x09
#define 	OQPSK_SIN_RC_400_SCR_ON 		(uint8_t)0x2a
#define 	OQPSK_SIN_RC_400_SCR_OFF		(uint8_t)0x0a
#define 	OQPSK_SIN_250 					(uint8_t)0x0c
#define 	OQPSK_SIN_500 					(uint8_t)0x0d
#define 	OQPSK_SIN_1000_SCR_ON 			(uint8_t)0x2e
#define 	OQPSK_SIN_1000_SCR_OFF 			(uint8_t)0x0e 
#define 	OQPSK_RC_250					(uint8_t)0x1c
#define 	OQPSK_RC_500 					(uint8_t)0x1d	//===
#define 	OQPSK_RC_1000_SCR_ON 			(uint8_t)0x3e
#define 	OQPSK_RC_1000_SCR_OFF			(uint8_t)0x1e

//=========ʹ��OQPSK-RC-250/500/1000ʱ�Ĺ������ö���
#define		TX_OUTPWR_P5DBM					(uint8_t)0xe8			//+5dBm
#define		TX_OUTPWR_P4DBM					(uint8_t)0xe9			//+4dBm
#define		TX_OUTPWR_P3DBM					(uint8_t)0xea			//+3dBm
#define		TX_OUTPWR_P2DBM					(uint8_t)0xca			//+2dBm
#define		TX_OUTPWR_P1DBM					(uint8_t)0xaa			//+1dBm
#define		TX_OUTPWR_P0DBM					(uint8_t)0xab			//+0dBm
#define		TX_OUTPWR_N1DBM					(uint8_t)0xac			//-1dBm
#define		TX_OUTPWR_N2DBM					(uint8_t)0x46			//-2dBm
#define		TX_OUTPWR_N3DBM					(uint8_t)0x25			//-3dBm
#define		TX_OUTPWR_N4DBM					(uint8_t)0x03			//-4dBm
#define		TX_OUTPWR_N5DBM					(uint8_t)0x04			//-5dBm
#define		TX_OUTPWR_N6DBM					(uint8_t)0x05			//-6dBm
#define		TX_OUTPWR_N7DBM					(uint8_t)0x06			//-7dBm
#define		TX_OUTPWR_N8DBM					(uint8_t)0x07			//-8dBm
#define		TX_OUTPWR_N9DBM					(uint8_t)0x08			//-9dBm
#define		TX_OUTPWR_N10DBM				(uint8_t)0x09			//-10dBm
#define		TX_OUTPWR_N11DBM				(uint8_t)0x0a			//-11dBm

//=========PA�ڷ���ǰ����ʱ�䶨��
#define		PA_STAT_2US						(uint8_t)0x00			//��ǰ2us����
#define		PA_STAT_4US						(uint8_t)0x40			//��ǰ4us����
#define		PA_STAT_6US						(uint8_t)0x80			//��ǰ6us����
#define		PA_STAT_8US						(uint8_t)0xc0			//��ǰ8us����


//========AT86RF212�й��ŵ����ö���,CC_BAND = 4,
#define 	CHINESEBAND780MHZ				(uint8_t)0x0b
#define 	CHINESEBAND782MHZ				(uint8_t)0x0d
#define 	CHINESEBAND784MHZ				(uint8_t)0x0f
#define 	CHINESEBAND786MHZ				(uint8_t)0x11

#define RFBAND_0		0			//IEEE 802.15.4
#define RFBAND_1		1
#define RFBAND_2		2
#define RFBAND_3		3
#define RFBAND_4		4
#define RFBAND_5		5

#define 	FRQ_CHINESEBAND780MHZ				(uint16_t)780
#define 	FRQ_CHINESEBAND782MHZ				(uint16_t)782
#define 	FRQ_CHINESEBAND784MHZ				(uint16_t)784
#define 	FRQ_CHINESEBAND786MHZ				(uint16_t)786





enum
{
    OQPSK_868MHZ    = 0,
    OQPSK_915MHZ    = 1,
    OQPSK_780MHZ    = 2,
    BPSK40_915MHZ   = 3
};

//======�豸��ַ��PIN_ID���ã�������Ҫ�޸�
enum {
	AT86RF212_PAN_ID0				=0x12,// ThePANID,
	AT86RF212_PAN_ID1				= 0x34,	 
	AT86RF212_SHORT_ADDR0			= TheAddress,//;
	AT86RF212_SHORT_ADDR1			= 0x00,//;
	AT86RF212_AACK_PROM_MODE 		= 0x00,		//����ģʽ��0x01-����,0x00-����
	AT86RF212_AACK_UPLD_RES_FT		= 0x00,		//Ԥ��֡�����Ƿ����,0x01-����,0x00-����
	AT86RF212_AACK_FLTR_RES_FT		= 0x00,		//�˲���Ԥ�������Ƿ����,0x01-����,0x00-����
	AT86RF212_AACK_FVN_MODE			= 0x00,	//֡�汾���տ���
};

//======IRQ�ж����ο������ã�������Ҫ�޸�
//====0x01--�����жϣ�0x00--������Ӧ�ж�
enum {
	AT86RF212_IRQMASK_PLL_LOCK		= 0x01,		
	AT86RF212_IRQMASK_PLL_UNLOCK	= 0x01,		//PLL 
	AT86RF212_IRQMASK_RX_START		= 0x01,		//��ʼ���������ж�
	AT86RF212_IRQMASK_TRX_END		= 0x01,		//֡��������ж�
	AT86RF212_IRQMASK_CCA_ED_DONE 	= 0x01,		//�๦���ж�
	AT86RF212_IRQMASK_AMI			= 0x01,		//��ַƥ���ж�
	AT86RF212_IRQMASK_TRX_UR		= 0x01,		//֡������ʳ�ͻ
	AT86RF212_IRQMASK_BAT_LOW		= 0x01,		//��ص�ѹ���ж�
   	AT86RF212_IRQMASK_MODE			= 0x01,		//�ж�ģʽ����,0x00--��ֹ�жϲ�ѯ��0x01--�����жϲ�ѯ,��MASK���
	AT86RF212_IRQ_POLARITY			= 0x01,		//�жϵ�ƽģʽ��0x00--�����ж�����ߵ�ƽ��0x01--�����ж�����͵�ƽ
};

// random defines
enum
{
    CHB_MAX_FRAME_RETRIES_POS   = 4,
    CHB_MAX_CSMA_RETIRES_POS    = 1,
    CHB_CSMA_SEED1_POS          = 0,
    CHB_CCA_MODE_POS            = 5,
    CHB_AUTO_CRC_POS            = 5,
    CHB_TRX_END_POS             = 3,
    CHB_TRAC_STATUS_POS         = 5,
    CHB_FVN_POS                 = 6,
    CHB_OQPSK_TX_OFFSET         = 2,
    CHB_BPSK_TX_OFFSET          = 3,
    CHB_MIN_FRAME_LENGTH        = 3,
    CHB_MAX_FRAME_LENGTH        = 0x7f,
    CHB_PA_EXT_EN_POS           = 7
};

/*************************************
			Ӧ�ñ������궨��
*************************************/
#define 	AT86RF212TRXLENGTH				34					//RF���ͼ������ֽڳ���
//�����˲�����ĸ��ֽ���ƫ����  OKIMBIN�ӵ�
#define DestPos 5  //���ջ������д洢���յ�ַ��ƫ����
#define SourcePos 7  //���͵�ַ��ƫ����
#define DataStartPos 9
#define DataEndPos 20


AT86RF212_GLOBAL uint8_t AT86RF212RxBuffer[129];					//�������ݻ�����

//=================================================================
//			AT86RF212 MAC�����߷��ͽ������ݽṹ����
//=================================================================
typedef union			//FCF�ṹ����
{
	uint16_t	Halfword;
	struct { //ð�ź����ʾλ�򣬱�ʾȡ��λ����
		uint16_t	FrameType			:3;		//֡����
		uint16_t	SecurityBitEn		:1;		//����λ
		uint16_t	FramePending		:1;		//����֡����
		uint16_t	ACKRequest			:1;		//Ӧ������
		uint16_t	PAN_IDCompr			:1;		//ͬһPANָʾ
		uint16_t	Rese0				:3;		//����
		uint16_t	DestAddrMode		:2;		//Ŀ�ĵ�ַģʽ
		uint16_t	FrameVersion		:2;		//֡�汾
		uint16_t	SourAddrMode		:2;		//Դ��ַģʽ
	} Bits;
}uFCF;

typedef union			//Դ��ַ����
{
	uint16_t	Halfword;
	struct {
		uint16_t	DeviceAddr			:12;	//�����豸���
		uint16_t	SubDeviceAddr		:4;		//�ն��ӱ��		
	} Bits;
}uAddr;

typedef struct 								//������� OKimbin�޸�
{
//	uint8_t	TxLength;					//���ͳ���
	uint8_t	MAC_FCF[2]; 				//2�ֽ�FCF�ṹ	 
	uint8_t	Samesign;
	uint8_t PAN_ID[2];
	uint8_t DestAddr[2];
	uint8_t	SourceAddr[2];				//2�ֽ�Դ��ַ
	//uAddr	NativeAddr;								//
	uint8_t	Data[20];		   	//��������
	uint8_t	TxLength;					//���ͳ���
	//uint8_t	CRC16[2];					//16λCRCУ��
} AT86RF212_TypeSend; 

AT86RF212_GLOBAL AT86RF212_TypeSend AT86RF212SendData;

typedef struct		  						//�������
{
	uint8_t	RxLength;					//�������ݳ��ȣ�(����2�ֽ�CRC?)
	//unsigned int DeviceAddr;  				//�����豸��ַ
	//unsigned char SubAddr;					//���豸���
	uAddr	RxDataAddr;
	uint8_t	RxBuffer[128];

	uint8_t RSSI;//OKIMBIN�ӵ�
	uint8_t ENERGY;
} AT86RF212_TypeReceive; 

AT86RF212_GLOBAL AT86RF212_TypeReceive AT86RF212ReceiveData;

AT86RF212_GLOBAL uint8_t RF212RevSuccess,RF212SendFinish; 		//RF���ճɹ���������ɱ�־


/*************************************
			Ӧ�ú�������
*************************************/
AT86RF212_GLOBAL uint8_t AT86RF212_ReadWriteReg(uint8_t cmd,uint8_t addr,uint8_t Data);
AT86RF212_GLOBAL uint8_t AT86RF212_GetTRXStatus(void);
AT86RF212_GLOBAL void AT86RF212_RFChannelConfig(uint8_t RFband,uint16_t frequency);
AT86RF212_GLOBAL uint8_t AT86RF212_ReadED(void);

AT86RF212_GLOBAL void AT86RF212_Init(void);
AT86RF212_GLOBAL void AT86RF212_IRQServe(void);
AT86RF212_GLOBAL void AT86RF212_EnterTXModeSendData(uint8_t* pdata,uint8_t Nbyte);
AT86RF212_GLOBAL void AT86RF212_TRXStatusModeChange(uint8_t mode);

AT86RF212_GLOBAL void ReadyDataTo(uint8_t);

#endif

/********************************************End Of File************************************************/
