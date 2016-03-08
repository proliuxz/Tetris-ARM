#ifndef __EXIO_H__
#define __EXIO_H__

#define EXIOADDR		(*(volatile unsigned short*)0xa000000)		//bank5

extern unsigned short int EXIOReg;

#define SETEXIOBIT(bit)		do{EXIOReg|=bit; 	EXIOADDR=EXIOReg;}while(0)
#define CLREXIOBIT(bit)		do{EXIOReg&=(~bit); 	EXIOADDR=EXIOReg;}while(0)

#endif
