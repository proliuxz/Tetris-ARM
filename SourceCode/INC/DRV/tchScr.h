#include "..\inc\def.h"

#define ADS7843_CTRL_START		0x80
#define ADS7843_GET_X			0x50
#define ADS7843_GET_Y			0x10
#define ADS7843_CTRL_12MODE	0x0
#define ADS7843_CTRL_8MODE	0x8
#define ADS7843_CTRL_SER		0x4
#define ADS7843_CTRL_DFR		0x0
#define ADS7843_CTRL_DISPWD	0x3	// Disable power down
#define ADS7843_CTRL_ENPWD	0x0	// enable power down

#define ADS7843_PIN_CS		(1<<6)	//GPF6
#define ADS7843_PIN_PEN	(1<<5)	//GPG5
//#define ADS7843_PIN_BUSY	(1<<6)

/////////´¥ÃþÆÁ¶¯×÷////////
#define TCHSCR_ACTION_NULL			0	
#define TCHSCR_ACTION_CLICK		1	//´¥ÃþÆÁµ¥»÷
#define TCHSCR_ACTION_DBCLICK		2	//´¥ÃþÆÁË«»÷
#define TCHSCR_ACTION_DOWN		3	//´¥ÃþÆÁ°´ÏÂ
#define TCHSCR_ACTION_UP			4	//´¥ÃþÆÁÌ§Æð
#define TCHSCR_ACTION_MOVE		5	//´¥ÃþÆÁÒÆ¶¯

#define TCHSCR_IsPenNotDown()		(rPDATG&ADS7843_PIN_PEN)

void TchScr_init(void);
void TchScr_GetScrXY(int *x, int *y);


