/* ARM board targetting source file.
Eric Rong
*/

//Define CPU Type
#define S3C3400		1
#define S3C3410		2
#define S3C44A0X	3
#define S3C44B0X	4

#define CPU_TYPE	S3C44B0X

//Indicate debug mode.
//#define DEBUG


//Define version of uC/OS Hardware Associated Layer code.
#define uHAL_VERSION_STRING	"Version 1.00  COiNS"


//Output through LCD or serial port to print information.
int uHALr_printf(const char *format, ...);


/*** Called by ARMTargetInit() ***/
//Reset memory management unit.
void uHALr_ResetMMU(void);

//Define pre & post-process routines for Interrupt.
void uHALir_DefineIRQ(void *is, void *iq, void *n);

//Initialze interrupts.
void uHALr_InitInterrupts(void);


//Initialize timer that is used OS.
void uHALr_InitTimers(void);


/*** Called by ARMTargetStart() ***/
//Request the system timer.
//return value 1:success   0:fail
int uHALr_RequestSystemTimer(void *tick, const unsigned char *str);


//Start system timer & enable the interrupt.
void uHALr_InstallSystemTimer(void);



//add by eric rong
void ChangePllValue(int mdiv,int pdiv,int sdiv);
void Port_Init(void);
void Uart_Init(int Uartnum, int mclk,int baud);
void Led_Display(int data);
void Delay(int time);
void Uart_SendByte(int Uartnum, unsigned char data);
void Uart_Printf(char *fmt,...);
char Uart_Getch(char* Revdata, int Uartnum, int timeout);
void Cache_Flush(void);
void Timer_Start(int divider);
int Timer_Stop(void);
void Uart_TxEmpty(int Uartnum);
//for debugging
void BreakPoint(void);
//void ARMTargetInit(void);

void SendSIOData(unsigned char data);
unsigned char ReadSDIO(void);

#define WriteSDIO(data) 		SendSIOData((data))
//void EnableURXD(int Uartnum, void* func);

