#include "..\startup\def.h"
#include "..\startup\44b.h"
#include "..\ucos-ii\includes.h"

#include "isr.h"
#include "uart.h"

OS_EVENT *Uart0_Rev_mbox=NULL;                   //Uart0 receive mail box
OS_EVENT *Uart1_Rev_mbox=NULL;                   //Uart1 receive mail box

U8 Uart0_Buffer[UART0_REVBUFFER_SIZE],Uart1_Buffer[UART1_REVBUFFER_SIZE];

static U8* pUart0_rev=Uart0_Buffer,	//Uart0 ���ջ�����ָ��
		*pUart0_read=Uart0_Buffer;	//Uart0 ��������ָ��

static U8* pUart1_rev=Uart1_Buffer,	//Uart1 ���ջ�����ָ��
		*pUart1_read=Uart1_Buffer;	//Uart1 ��������ָ��

static int nRev_Uart0=1,nRev_Uart1=1;	//�趨��Uart�����ַ���

void irq_OSRevUart1()
{
	int nrev;

	if(!(rUTRSTAT1 & 0x1))
		return;

	//Receive data
	*pUart1_rev=RdURXH1();
	pUart1_rev++;
	if(pUart1_rev==Uart1_Buffer+UART1_REVBUFFER_SIZE)
		pUart1_rev=Uart1_Buffer;

	nrev=pUart1_rev-pUart1_read;

	if(nrev>=nRev_Uart1)
		OSMboxPost(Uart1_Rev_mbox, (void*)nrev);
}

void irq_OSRevUart0()
{
	int nrev;

	if(!(rUTRSTAT0 & 0x1))
		return;

	//Receive data
	*pUart0_rev=RdURXH0();
	pUart0_rev++;
	if(pUart0_rev==Uart0_Buffer+UART0_REVBUFFER_SIZE)
		pUart0_rev=Uart0_Buffer;

	nrev=pUart0_rev-pUart0_read;

	if(nrev>=nRev_Uart0)
		OSMboxPost(Uart0_Rev_mbox, (void*)nrev);
}

/*************************
	������0������
	����ֵ,ʵ�ʶ�ȡ������
****************************/
int OSReadUart0(U8 data[], int num, int ntimeout)
{
	int i;
	INT8U err;
	int nrev;

	nRev_Uart0=num;	//����Uart1����������

	nrev=pUart0_rev-pUart0_read;
	if(nrev<0)//ѭ������������
		nrev+=UART0_REVBUFFER_SIZE;

	if(nrev<num){
		while((nrev=(int)OSMboxPend(Uart0_Rev_mbox, ntimeout,&err))<num){
			if(err==OS_TIMEOUT){
				num=nrev;
				break;
			}
		}
	}

	for(i=0;i<num;i++){
		data[i]=*pUart0_read;
		pUart0_read++;
		if(pUart0_read==Uart0_Buffer+UART0_REVBUFFER_SIZE)
			pUart0_read=Uart0_Buffer;
	}
	return nrev;
}

/*************************
	������1������
	����ֵ,ʵ�ʶ�ȡ������
****************************/
int OSReadUart1(U8 data[], int num, int ntimeout)
{
	int i;
	INT8U err;
	int nrev;

	nRev_Uart1=num;	//����Uart1����������

	nrev=pUart1_rev-pUart1_read;
	if(nrev<0)//ѭ������������
		nrev+=UART1_REVBUFFER_SIZE;

	if(nrev<num){
		while((nrev=(int)OSMboxPend(Uart1_Rev_mbox, ntimeout,&err))<num){
			if(err==OS_TIMEOUT){
				num=nrev;
				break;
			}
		}
	}

	for(i=0;i<num;i++){
		data[i]=*pUart1_read;
		pUart1_read++;
		if(pUart1_read==Uart1_Buffer+UART1_REVBUFFER_SIZE)
			pUart1_read=Uart1_Buffer;
	}
	return nrev;
}

/*************************
	�ȴ�����1������
****************************/
void OSWartUart1()
{
	INT8U err;
	OSMboxPend(Uart1_Rev_mbox, 0,&err);
}

/*************************
	�ȴ�����0������
****************************/
void OSWartUart0()
{
	INT8U err;
	OSMboxPend(Uart0_Rev_mbox, 0,&err);
}

void OSInitUart()
{
	if(Uart0_Rev_mbox==NULL)
		Uart0_Rev_mbox=OSMboxCreate((void*)NULL);

	if(Uart1_Rev_mbox==NULL)
		Uart1_Rev_mbox=OSMboxCreate((void*)NULL);
}

/*************************
	��������0�����ж�
****************************/
void OpenUart0Rev()
{
	U32 temp;
	SetISR_Interrupt(INT_URXD0_OFFSET,irq_OSRevUart0, 0);

/*	temp=rINTMSK;
	rINTMSK=0xffffffff;

	rI_ISPC=BIT_URXD0;
	rINTMSK=(~BIT_URXD0)&temp;*/

}

/*************************
	��������1�����ж�
****************************/
void OpenUart1Rev()
{
	U32 temp;
	SetISR_Interrupt(INT_URXD1_OFFSET,irq_OSRevUart1,0);

/*	temp=rINTMSK;
	rINTMSK=0xffffffff;

	rI_ISPC=BIT_URXD1;
	rINTMSK=(~BIT_URXD1)&temp;*/
}

