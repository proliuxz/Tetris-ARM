///////////////ZLG7289���ź���/////////////////////
#define ZLG7289_CS				(0x20)	//GPB5
#define ZLG7289_KEY				(0x10)	//GPG4

#define ZLG7289_ENABLE()		do{Zlg7289SIOBand=rSBRDR;	Zlg7289SIOCtrl=rSIOCON;	\
						rSIOCON=0x31;	rSBRDR=0xff;	rPDATB&=(~ZLG7289_CS);}while(0)
#define ZLG7289_DISABLE()		do{rPDATB|=ZLG7289_CS; rSBRDR=Zlg7289SIOBand; rSIOCON=Zlg7289SIOCtrl;}while(0)

///////////////ZLG7289��������/////////////////////
#define ZLG7289_CMD_RST		0xA4	//��λ
#define ZLG7289_CMD_TEST		0xBF	//����ģʽ
#define ZLG7289_CMD_LSHIFT		0xA1	//����
#define ZLG7289_CMD_RSHIFT		0xA0	//����
#define ZLG7289_CMD_CLSHIFT	0xA3	//ѭ������
#define ZLG7289_CMD_CRSHIFT	0xA2	//ѭ������

#define ZLG7289_CMD_DATA0		0x80	//���������Ұ���ʽ0 ����
#define ZLG7289_CMD_DATA1		0xC8	//���������Ұ���ʽ1 ����
#define ZLG7289_CMD_DATA3		0x90	//�������ݵ�������

#define ZLG7289_CMD_FLASH		0x88	//��˸����
#define ZLG7289_CMD_HIDE		0x98	//��������
#define ZLG7289_CMD_LDOT		0xE0	//�ε���ָ��
#define ZLG7289_CMD_HDOT		0xC0	//�ιر�ָ��
#define ZLG7289_CMD_RDKEY		0x15	//����������ָ��


void Zlg7289_Reset(void);
unsigned int Zlg7289_ReadKey(void);

