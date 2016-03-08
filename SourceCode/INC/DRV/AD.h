#include"..\startup\44b.h"

#define ADCCON_FLAG		0x40
#define ADCCON_SLEEP		0x20

#define ADCCON_ADIN0		(0x0<<2)
#define ADCCON_ADIN1		(0x1<<2)
#define ADCCON_ADIN2		(0x2<<2)
#define ADCCON_ADIN3		(0x3<<2)
#define ADCCON_ADIN4		(0x4<<2)
#define ADCCON_ADIN5		(0x5<<2)
#define ADCCON_ADIN6		(0x6<<2)
#define ADCCON_ADIN7		(0x7<<2)

#define ADCCON_READ_START	0x2
#define ADCCON_ENABLE_START	0x1

#define init_ADdevice(speed,mode/*ADCCON_SLEEP*/)	do{rADCPSR=speed;	rADCCON=mode;}while(0)
int GetADresult(int channel);