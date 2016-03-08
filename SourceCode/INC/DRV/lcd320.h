#include "..\inc\def.h"


////////////////定义LCD的类型/////////////
#define LCDTYPE_320x240x16GRAY		1
#define LCDTYPE_320x240x256COLOR		2
#define LCDTYPE_640x480x256STN		3

#define LCDTYPE		LCDTYPE_320x240x256COLOR


#if (LCDTYPE==LCDTYPE_640x480x256STN)
#define LCDDisplayPin			0x0010	//GPB4
#define LCDWIDTH		640
#define LCDHEIGHT		480
#define CLKVAL			(5)	// 60Mhz

#else
//#define LCDBkLightPin			0x8000	//GPC15
#define LCDDisplayPin			0x0010	//GPB4
#define LCDWIDTH		320
#define LCDHEIGHT		240
#define CLKVAL		(10)	// 60Mhz, fr=100Hz (CLKVAL=38.6)

#endif

#define LCDAREA			(LCDWIDTH*LCDHEIGHT)

#define DspTxtMode		0
#define DspGraMode		1

void LCD_Cls(void); //文本模式下清除屏幕?
void LCD_ChangeMode(U8 mode);
void LCD_Refresh(void);

void LCD_Init(void);
void LCD_printf(const char *format,...);

//void LCDBkLight(U8 isOpen);
void LCDDisplayOpen(U8 isOpen);
void LCD_TestShow(void);
