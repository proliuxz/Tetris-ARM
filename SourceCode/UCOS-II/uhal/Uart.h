#ifndef __UART_H__
#define __UART_H__

#define UART0_REVBUFFER_SIZE		1024
#define UART1_REVBUFFER_SIZE		1024

void OSInitUart(void);
int OSReadUart0(U8 data[], int num, int ntimeout);
void OpenUart0Rev(void);
int OSReadUart1(U8 data[], int num, int ntimeout);
void OpenUart1Rev(void);


#endif //#ifndef __UART_H__

