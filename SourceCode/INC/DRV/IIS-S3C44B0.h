
#define AUDIO_CLK			44100	//44.1KHz的音频时钟
#define AUDIO_BIT			16	//16位音频

#define AUDIO_OUT_BUFFERSIZE		(AUDIO_CLK/2)	//500ms的缓冲区
#define AUDIO_IN_BUFFERSIZE		(AUDIO_CLK/2)	//500ms的缓冲区

#define AUDIO_OUT_BUFFERSIZE_B	(AUDIO_OUT_BUFFERSIZE*4)
#define AUDIO_IN_BUFFERSIZE_B		(AUDIO_IN_BUFFERSIZE*4)

///////////////////////////////////S3C44B0 IIS////////////////////////////////////////
#define IISCON_RIGHT_INDEX		0x100		//Right Channel
#define IISCON_TXFIFO_NE		0x80		//Transmit FIFO not empty flag
#define IISCON_RXFIFO_NE		0x40		//Transmit FIFO not empty flag
#define IISCON_TXDMA			0x20		//Transmit DMA service request enable
#define IISCON_RXDMA			0x10		//Receive DMA service request enable
#define IISCON_TXIDLE			0x08		//Transmit channel idle
#define IISCON_RXIDLE			0x04		//Receive channel idle
#define IISCON_PRESCALE			0x02		//IIS prescaler enable
#define IISCON_ENABLE			0x01		//IIS interface enable(start)

#define IISMOD_SLAVE			0x100		//slave mode select
#define IISMOD_TX				0x80		//Transmit mode
#define IISMOD_RX				0x40		//receive mode
#define IISMOD_RACTIVE			0x20		//Active level of right
#define IISMOD_MSBJ				0x10		//MSB-justified Serial interface format
#define IISMOD_16BIT			0x08		//Serial data 16bit per channel
#define IISMOD_MCLK_384FS		0x04		//Master clock frequency
#define IISMOD_32FS				0x01		//Serial bit clock frequency=32fs
#define IISMOD_48FS				0x02		//Serial bit clock frequency=48fs

#define IISFCON_TXDMA			0x800		//Transmit FIFO access DMA mode
#define IISFCON_RXDMA			0x400		//Receive FIFO access DMA mode
#define IISFCON_TXFIFO			0x200		//Transmit FIFO enable
#define IISFCON_RXFIFO			0x100		//Receive FIFO enable

///////////////////////////UDA1341/////////////////////////////////////////////////
#define UDA1341_MODE			(1<<14)		//GPC14

#define UDAADDR					0x14		//UDA1341 address
#define UDA_DATA0				0x00		//UDA1341 data0
#define UDA_DATA1				0x01		//UDA1341 data1
#define UDA_STATUS				0x02		//UDA1341 status

#define UDASTATUS0_RST			0x40		//UDA1341 status reset
#define UDASTATUS0_SC512		0x00		//UDA1341 status SC
#define UDASTATUS0_SC384		0x10		//UDA1341 status SC
#define UDASTATUS0_SC256		0x20		//UDA1341 status SC
#define UDASTATUS0_IIS			0x00		//UDA1341 status IF2-0=0 IIS
#define UDASTATUS0_DC			0x01		//UDA1341 status DC-filtering
#define UDASTATUS1_OGS6		(0x80|0x40)	//UDA1341 status gain of DAC 6dB
#define UDASTATUS1_IGS6		(0x80|0x20)	//UDA1341 status gain of ADC 6dB
#define UDASTATUS1_PADINV		(0x80|0x10)	//UDA1341 status ADC inverting
#define UDASTATUS1_PDAINV		(0x80|0x08)	//UDA1341 status DAC inverting
#define UDASTATUS1_DBSPD		(0x80|0x04)	//UDA1341 status double speed
#define UDASTATUS1_ADCON		(0x80|0x02)	//UDA1341 status ADC on
#define UDASTATUS1_DACON		(0x80|0x01)	//UDA1341 status DAC on

#define UDADATA0_VOL			0x00		//UDA1341 data0 volume value 6bit

#define L3CLK			0x100		//GPC8
#define L3DATA			0x200		//GPC9

void  Init_Audio(BOOL isChangePLL);
void Init_IIS(BOOL isChangePLL);
void Init_UDA1341();

void Send2AudioBuffer(unsigned char* pdata, int ndata, BOOL ForcePlay);
BOOL Play_WavFile(char filename[]);

void Audio_Test();

