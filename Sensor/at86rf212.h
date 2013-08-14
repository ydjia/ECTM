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

//=============AT86RF212¿ØÖÆ¹Ü½Å¶¨Òhå
/*ÐÞ¸ÄÎªÔÆÌì´´Ä£¿é½Ó¿Ú 2013.05.08
#define		AT86RF212_RESET_PIN		GPIO_Pin_5			//PB5
#define		AT86RF212_SLP_TR_PIN	GPIO_Pin_6 		 //PB6

#define		AT86RF212_IRQ_PIN		GPIO_Pin_4			//PB4

#define		AT86RF212_SEL_PIN		GPIO_Pin_4			//PA4
#define		AT86RF212_MOSI_PIN		GPIO_Pin_7			//PA7
#define		AT86RF212_MISO_PIN		GPIO_Pin_6			//PA6
#define		AT86RF212_SCK_PIN		GPIO_Pin_5		//PA5
#define		AT86RF212_CLK_M_PIN	 	//Ã»ÓÐÒý³ö
*/

#define		AT86RF212_ResetEn()			GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define		AT86RF212_ResetDis()		GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define		AT86RF212_SLP_TRReset()		GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define		AT86RF212_SLP_TRSet()		GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define 	AT86RF212_SLP_TRStatus()	GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_6)

#define		AT86RF212_SELEnable()		GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define		AT86RF212_SELDisable()		GPIO_SetBits(GPIOA,GPIO_Pin_4)

//#define		AT86RF212_DIG2Status()		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)		//¶ÁDIG2×´Ì¬

#define		AT86RF212_IRQStatus()		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)		//¶ÁIRQÒý½Å×´Ì¬


//=============AT86RF212 SPI²Ù×÷Ö¸Áî£¬´Ë´úÂëÎÞÐèÐÞ¸Ä
#define 	SPI_CMD_READ_REGISTER    	(uint8_t)0x80  		// ¶Á¼Ä´æÆ÷
#define 	SPI_CMD_WRITE_REGISTER   	(uint8_t)0xc0  		// Ð´¼Ä´æÆ÷
#define 	SPI_CMD_READ_FRAMEBUF 		(uint8_t)0x20  		// ¶ÁÖ¡»º³å
#define 	SPI_CMD_WRITE_FRAMEBUF 		(uint8_t)0x60  		// Ð´Ö¡»º³å
#define 	SPI_CMD_READ_SRAM   			(uint8_t)0x00  		// ¶ÁSRAM
#define 	SPI_CMD_WRITE_SRAM    		(uint8_t)0x40  		// Ð´SRAM


//=============AT85RF212¼Ä´æÆ÷µØÖ·£¬´Ë´úÂëÎÞÐèÐÞ¸Ä
#define 	RG_TRX_STATUS_ADDR   	(uint8_t)0x01		//ÎÞÏßµçÊÕ·¢Æ÷×´Ì¬
#define 	RG_TRX_STATE_ADDR   	(uint8_t)0x02		//×´Ì¬×ª»»¼Ä´æ
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


//=============AT85RF212¼Ä´æÆ÷×´Ì¬£¬´Ë´úÂëÎÞÐèÐÞ¸Ä
//=======ÖÐ¶Ï×´Ì¬£¨IRQ_STATUS£©¼Ä´æÆ÷Î»¶¨Òå
#define 	REG_IRQ_STATUS_PLL_LOCK   		(uint8_t)0x01			//Ö¸Ê¾PLLËø¶¨×´Ì¬
#define 	REG_IRQ_STATUS_PLL_UNLOCK   	(uint8_t)0x02			//Ö¸Ê¾PLLÎ´Ëø¶¨×´Ì¬
#define 	REG_IRQ_STATUS_RX_START		  	(uint8_t)0x04			//Ö¸Ê¾PSDU½ÓÊÕµÄ¿ªÊ¼×´Ì¬
#define 	REG_IRQ_STATUS_TRX_END		  	(uint8_t)0x08			//Ö¸Ê¾·¢ËÍ/½ÓÊÕÍê³É
#define 	REG_IRQ_STATUS_CCA_ED_DONE	  	(uint8_t)0x10			//¶à¹¦ÄÜÖÐ¶Ï£¬1,×´Ì¬×ª»»;2,CCA_ED_DONE(CCA»òED²âÁ¿½áÊø)
#define 	REG_IRQ_STATUS_AMI			  	(uint8_t)0x20			//ÏÔÊ¾µØÖ·Æ¥Åä
#define 	REG_IRQ_STATUS_TRX_UR		  	(uint8_t)0x40			//ÏÔÊ¾Ö¡»º³åµÄ·ÃÎÊ³åÍ»
#define 	REG_IRQ_STATUS_BAT_LOW		  	(uint8_t)0x80			//µçÔ´µçÑ¹µÍ

//=======AT86RF212×´Ì¬×ª»»ÃüÁî
#define 	CMD_NOP   						(uint8_t)0x00
#define 	CMD_TX_START   					(uint8_t)0x02	 	//¿ªÊ¼·¢ËÍÊý¾Ý
#define 	CMD_FORCE_TRX_OFF   			(uint8_t)0x03		//Ç¿ÖÆ¹Ø±ÕÎÞÏßÇ°¶Ë£¬¿ìËÙÖÐ¶ÏËùÓÐµÄÎÞÏß²Ù×÷
#define 	CMD_FORCE_PLL_ON   				(uint8_t)0x04		//Ç¿ÖÆ½øÈë×¼±¸·¢ËÍÊý¾Ý×´Ì¬
#define 	CMD_RX_ON   					(uint8_t)0x06		//ÎÞÏßÊÕÌý×´Ì¬
#define 	CMD_TRX_OFF   					(uint8_t)0x08		//µÈ´ýµ±Ç°²Ù×÷Íê³Éºó£¬¹Ø±ÕÎÞÏßÇ°¶Ë
#define 	CMD_PLL_ON   					(uint8_t)0x09	   	//½øÈë×¼±¸·¢ËÍÊý¾Ý×´Ì¬
#define 	CMD_RX_AACK_ON   				(uint8_t)0x16	  	//À©Õ¹Ä£Ê½--
#define 	CMD_TX_ARET_ON   				(uint8_t)0x19		//À©Õ¹Ä£Ê½--×Ô¶¯ÖØ´«

//=======AT86RF212ÎÞÏßÊÕ·¢×´Ì¬¶¨Òå
#define 	TRX_STATUS_P_ON					(uint8_t)0x00		//ÉÏµç×´Ì¬
#define 	TRX_STATUS_BUSY_RX				(uint8_t)0x01		//ÎÞÏßÕýÔÚ½ÓÊÕÊý¾Ý
#define 	TRX_STATUS_BUSY_TX				(uint8_t)0x02		//ÎÞÏßÕýÔÚ·¢ËÍÊý¾Ý
#define 	TRX_STATUS_RX_ON				(uint8_t)0x06		//ÎÞÏß´¦ÓÚÊÕÌý×´Ì¬
#define 	TRX_STATUS_TRX_OFF				(uint8_t)0x08		//ÎÞÏßÇ°¶Ë¹Ø±Õ
#define 	TRX_STATUS_PLL_ON				(uint8_t)0x09		//½øÈë×¼±¸·¢ËÍ×´Ì¬
#define 	TRX_STATUS_SLEEP				(uint8_t)0x0f		//½øÈëÐÝÃß
#define 	TRX_STATUS_BUSY_RX_AACK			(uint8_t)0x11		//À©Õ¹Ä£Ê½--Ã¦ÓÚ½ÓÊÕÓ¦´ð
#define 	TRX_STATUS_BUSY_TX_ARET			(uint8_t)0x12		//À©Õ¹Ä£Ê½--Ã¦ÓÚ×Ô¶¯ÖØ´«
#define 	TRX_STATUS_RX_AACK_ON			(uint8_t)0x16		//À©Õ¹Ä£Ê½--½ÓÊÕÓ¦´ð¿ªÆô
#define 	TRX_STATUS_TX_ARET_ON			(uint8_t)0x19		//À©Õ¹Ä£Ê½--×Ô¶¯ÖØ´«¿ªÆô
#define 	TRX_STATUS_RX_ON_NOCLK			(uint8_t)0x1c		//½øÈëÊÕÌý×´Ì¬£¬ÎÞCLKMÊ±ÖÓÊä³ö
#define 	TRX_STATUS_RX_AACK_ON_NOCLK		(uint8_t)0x1d		//À©Õ¹Ä£Ê½--
#define 	TRX_STATUS_BUSY_RX_AACK_NOCLK	(uint8_t)0x1e		//À©Õ¹Ä£Ê½--
#define 	TRX_STATUS_SYS_IN_PROGRESS		(uint8_t)0x1f		//´Ë×´Ì¬Ê±£¬²»ÔÊÐí½øÐÐ×´Ì¬×ª»»

//========AT86RF212ÂëÆ¬ËÙÂÊ¶¨Òå
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

//=========Ê¹ÓÃOQPSK-RC-250/500/1000Ê±µÄ¹¦ÂÊÉèÖÃ¶¨Òå
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

//=========PAÔÚ·¢ËÍÇ°Æô¶¯Ê±¼ä¶¨Òå
#define		PA_STAT_2US						(uint8_t)0x00			//ÌáÇ°2usÆô¶¯
#define		PA_STAT_4US						(uint8_t)0x40			//ÌáÇ°4usÆô¶¯
#define		PA_STAT_6US						(uint8_t)0x80			//ÌáÇ°6usÆô¶¯
#define		PA_STAT_8US						(uint8_t)0xc0			//ÌáÇ°8usÆô¶¯


//========AT86RF212ÖÐ¹úÐÅµÀÉèÖÃ¶¨Òå,CC_BAND = 4,
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

//======Éè±¸µØÖ·¼°PIN_IDÉèÖÃ£¬¸ù¾ÝÐèÒªÐÞ¸Ä
enum {
	AT86RF212_PAN_ID0				=0x12,// ThePANID,
	AT86RF212_PAN_ID1				= 0x34,	 
	AT86RF212_SHORT_ADDR0			= TheAddress,//;
	AT86RF212_SHORT_ADDR1			= 0x00,//;
	AT86RF212_AACK_PROM_MODE 		= 0x00,		//»ìÔÓÄ£Ê½£¬0x01-ÆôÓÃ,0x00-½ûÓÃ
	AT86RF212_AACK_UPLD_RES_FT		= 0x00,		//Ô¤ÁôÖ¡ÀàÐÍÊÇ·ñ½ÓÊÕ,0x01-ÆôÓÃ,0x00-½ûÓÃ
	AT86RF212_AACK_FLTR_RES_FT		= 0x00,		//ÂË²¨Æ÷Ô¤ÁôÀàÐÍÊÇ·ñ½ÓÊÕ,0x01-ÆôÓÃ,0x00-½ûÓÃ
	AT86RF212_AACK_FVN_MODE			= 0x00,	//Ö¡°æ±¾½ÓÊÕ¿ØÖÆ
};

//======IRQÖÐ¶ÏÆÁ±Î¿ØÖÆÉèÖÃ£¬¸ù¾ÝÐèÒªÐÞ¸Ä
//====0x01--Æô¶¯ÖÐ¶Ï£¬0x00--ÆÁ±ÎÏàÓ¦ÖÐ¶Ï
enum {
	AT86RF212_IRQMASK_PLL_LOCK		= 0x01,		
	AT86RF212_IRQMASK_PLL_UNLOCK	= 0x01,		//PLL 
	AT86RF212_IRQMASK_RX_START		= 0x01,		//¿ªÊ¼½ÓÊÕÊý¾ÝÖÐ¶Ï
	AT86RF212_IRQMASK_TRX_END		= 0x01,		//Ö¡½ÓÊÕÍê³ÉÖÐ¶Ï
	AT86RF212_IRQMASK_CCA_ED_DONE 	= 0x01,		//¶à¹¦ÄÜÖÐ¶Ï
	AT86RF212_IRQMASK_AMI			= 0x01,		//µØÖ·Æ¥ÅäÖÐ¶Ï
	AT86RF212_IRQMASK_TRX_UR		= 0x01,		//Ö¡»º³å·ÃÎÊ³åÍ»
	AT86RF212_IRQMASK_BAT_LOW		= 0x01,		//µç³ØµçÑ¹µÍÖÐ¶Ï
   	AT86RF212_IRQMASK_MODE			= 0x01,		//ÖÐ¶ÏÄ£Ê½¿ØÖÆ,0x00--½ûÖ¹ÖÐ¶Ï²éÑ¯£¬0x01--ÆôÓÃÖÐ¶Ï²éÑ¯,ÓëMASKÅäºÏ
	AT86RF212_IRQ_POLARITY			= 0x01,		//ÖÐ¶ÏµçÆ½Ä£Ê½£¬0x00--·¢ÉúÖÐ¶ÏÊä³ö¸ßµçÆ½£¬0x01--·¢ÉúÖÐ¶ÏÊä³öµÍµçÆ½
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
			Ó¦ÓÃ±äÁ¿¼°ºê¶¨Òå
*************************************/
#define 	AT86RF212TRXLENGTH				34					//RF·¢ËÍ¼°½ÓÊÕ×Ö½Ú³¤¶È
//³ÌÐòÂË²¨ËùÐèµÄ¸÷ÖÖ½ÓÊÕÆ«ÒÆÁ¿  OKIMBIN¼ÓµÄ
#define DestPos 5  //½ÓÊÕ»º³åÇøÖÐ´æ´¢½ÓÊÕµØÖ·µÄÆ«ÒÆÁ¿
#define SourcePos 7  //·¢ËÍµØÖ·µÄÆ«ÒÆÁ¿
#define DataStartPos 9
#define DataEndPos 20


AT86RF212_GLOBAL uint8_t AT86RF212RxBuffer[129];					//½ÓÊÕÊý¾Ý»º³åÇø

//=================================================================
//			AT86RF212 MAC²ãÎÞÏß·¢ËÍ½ÓÊÕÊý¾Ý½á¹¹¶¨Òå
//=================================================================
typedef union			//FCF½á¹¹¶¨Òå
{
	uint16_t	Halfword;
	struct { //Ã°ºÅºóÃæ±íÊ¾Î»Óò£¬±íÊ¾È¡¼¸Î»À´ÓÃ
		uint16_t	FrameType			:3;		//Ö¡ÀàÐÍ
		uint16_t	SecurityBitEn		:1;		//¼ÓÃÜÎ»
		uint16_t	FramePending		:1;		//ºóÐøÖ¡¿ØÖÆ
		uint16_t	ACKRequest			:1;		//Ó¦´ðÇëÇó
		uint16_t	PAN_IDCompr			:1;		//Í¬Ò»PANÖ¸Ê¾
		uint16_t	Rese0				:3;		//±£Áô
		uint16_t	DestAddrMode		:2;		//Ä¿µÄµØÖ·Ä£Ê½
		uint16_t	FrameVersion		:2;		//Ö¡°æ±¾
		uint16_t	SourAddrMode		:2;		//Ô´µØÖ·Ä£Ê½
	} Bits;
}uFCF;

typedef union			//Ô´µØÖ·¶¨Òå
{
	uint16_t	Halfword;
	struct {
		uint16_t	DeviceAddr			:12;	//³ÉÌ×Éè±¸±àºÅ
		uint16_t	SubDeviceAddr		:4;		//ÖÕ¶Ë×Ó±àºÅ		
	} Bits;
}uAddr;

typedef struct 								//·¢ËÍÏà¹Ø OKimbinÐÞ¸Ä
{
//	uint8_t	TxLength;					//·¢ËÍ³¤¶È
	uint8_t	MAC_FCF[2]; 				//2×Ö½ÚFCF½á¹¹	 
	uint8_t	Samesign;
	uint8_t PAN_ID[2];
	uint8_t DestAddr[2];
	uint8_t	SourceAddr[2];				//2×Ö½ÚÔ´µØÖ·
	//uAddr	NativeAddr;								//
	uint8_t	Data[20];		   	//·¢ËÍÊý¾Ý
	uint8_t	TxLength;					//·¢ËÍ³¤¶È
	//uint8_t	CRC16[2];					//16Î»CRCÐ£Ñé
} AT86RF212_TypeSend; 

AT86RF212_GLOBAL AT86RF212_TypeSend AT86RF212SendData;

typedef struct		  						//½ÓÊÕÏà¹Ø
{
	uint8_t	RxLength;					//½ÓÊÕÊý¾Ý³¤¶È£¬(°üº¬2×Ö½ÚCRC?)
	//unsigned int DeviceAddr;  				//³ÉÌ×Éè±¸µØÖ·
	//unsigned char SubAddr;					//×ÓÉè±¸±àºÅ
	uAddr	RxDataAddr;
	uint8_t	RxBuffer[128];

	uint8_t RSSI;//OKIMBIN¼ÓµÄ
	uint8_t ENERGY;
} AT86RF212_TypeReceive; 

AT86RF212_GLOBAL AT86RF212_TypeReceive AT86RF212ReceiveData;

AT86RF212_GLOBAL uint8_t RF212RevSuccess,RF212SendFinish; 		//RF½ÓÊÕ³É¹¦£¬·¢ËÍÍê³É±êÖ¾


/*************************************
			Ó¦ÓÃº¯ÊýÉêÃ÷
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
