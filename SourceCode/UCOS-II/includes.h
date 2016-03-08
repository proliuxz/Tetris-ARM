/* ChronOS Includes.h file for Samsung ARM port
 *
 * Copyright  2002 By InterNiche Technologies Inc. All rights reserved
 *
 * MODULE: SNDS300
 *
 * PORTABLE: no
 */

/* Include Samsung hardware definition for critcal section macros. */

/*#include "snds.h"*/
/*#include "isr.h"*/


#ifndef __INCLUDES_H__
#define __INCLUDES_H__

//****************CPU**************//
#include "../startup/option.h"
#include "../startup/44b.h"
#include "uhal\uhal.h"
#include "../inc/drv.h"
/////////////////////////////////////////////////

#define EXTERN
/* #define EXTERN extern */

/* uCOS constants for Samsung ARM port */

#define INT8U     unsigned char
#define INT16U    unsigned short
#define INT32U    unsigned long
#define OS_STK    unsigned long
#define BOOLEAN   int
#define OS_CPU_SR unsigned long
#define INT8S     char

//****************OS***************//
#include    "os_cfg.h"
#include    "chronos.h"


/* critical section macros use "method 3" (save to local var "cpu_sr") */
extern int  INTS_OFF(void);      /* ASM routines to twiddle bits */
extern void INTS_ON(void);

#define  OS_ENTER_CRITICAL()  { cpu_sr = INTS_OFF(); }
#define  OS_EXIT_CRITICAL()   { if(cpu_sr == 0) INTS_ON(); }

#define  OS_STK_GROWTH        1 	/* Stack grows from HIGH to LOW memory  */


#define	STACKSIZE	256


#endif /* __INCLUDES_H__ */


