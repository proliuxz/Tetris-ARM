#ifndef __ISR_H__
#define __ISR_H__

#define	MAXHNDLRS	26	/* Maximum # of interrupt  handlers */


///////////interrupt offset/////////////////////
#define INT_ADC_OFFSET			0
#define INT_RTC_OFFSET			1
#define INT_UTXD1_OFFSET		2
#define INT_UTXD0_OFFSET		3
#define INT_SIO_OFFSET			4
#define INT_IIC_OFFSET			5
#define INT_URXD1_OFFSET		6
#define INT_URXD0_OFFSET		7
#define INT_TIMER5_OFFSET		8
#define INT_TIMER4_OFFSET		9
#define INT_TIMER3_OFFSET		10
#define INT_TIMER2_OFFSET		11
#define INT_TIMER1_OFFSET		12
#define INT_TIMER0_OFFSET		13
#define INT_UERR01_OFFSET		14
#define INT_WDT_OFFSET			15
#define INT_BDMA1_OFFSET		16
#define INT_BDMA0_OFFSET		17
#define INT_ZDMA1_OFFSET		18
#define INT_ZDMA0_OFFSET		19
#define INT_TICK_OFFSET			20
#define EINT4567_OFFSET		21
#define EINT3_OFFSET			22
#define EINT2_OFFSET			23
#define EINT1_OFFSET			24
#define EINT0_OFFSET			25

///////////Ex interrupt offset/////////////////////
#define EINT4_OFFSET		0
#define EINT5_OFFSET		1
#define EINT6_OFFSET		2
#define EINT7_OFFSET		3


void SetISR_Interrupt(int vector, void (*handler)(), int Exint);
void ISR_IrqHandler(void);

#endif	 //#ifndef __ISR_H__
