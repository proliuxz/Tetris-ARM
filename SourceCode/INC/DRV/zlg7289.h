///////////////ZLG7289的信号线/////////////////////
#define ZLG7289_CS				(0x20)	//GPB5
#define ZLG7289_KEY				(0x10)	//GPG4

#define ZLG7289_ENABLE()		do{Zlg7289SIOBand=rSBRDR;	Zlg7289SIOCtrl=rSIOCON;	\
						rSIOCON=0x31;	rSBRDR=0xff;	rPDATB&=(~ZLG7289_CS);}while(0)
#define ZLG7289_DISABLE()		do{rPDATB|=ZLG7289_CS; rSBRDR=Zlg7289SIOBand; rSIOCON=Zlg7289SIOCtrl;}while(0)

///////////////ZLG7289的命令字/////////////////////
#define ZLG7289_CMD_RST		0xA4	//复位
#define ZLG7289_CMD_TEST		0xBF	//测试模式
#define ZLG7289_CMD_LSHIFT		0xA1	//左移
#define ZLG7289_CMD_RSHIFT		0xA0	//右移
#define ZLG7289_CMD_CLSHIFT	0xA3	//循环左移
#define ZLG7289_CMD_CRSHIFT	0xA2	//循环右移

#define ZLG7289_CMD_DATA0		0x80	//下载数据且按方式0 译码
#define ZLG7289_CMD_DATA1		0xC8	//下载数据且按方式1 译码
#define ZLG7289_CMD_DATA3		0x90	//下载数据但不译码

#define ZLG7289_CMD_FLASH		0x88	//闪烁控制
#define ZLG7289_CMD_HIDE		0x98	//消隐控制
#define ZLG7289_CMD_LDOT		0xE0	//段点亮指令
#define ZLG7289_CMD_HDOT		0xC0	//段关闭指令
#define ZLG7289_CMD_RDKEY		0x15	//读键盘数据指令


void Zlg7289_Reset(void);
unsigned int Zlg7289_ReadKey(void);

