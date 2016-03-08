/*These are the standard ANSI bit operation macros.  They are used by */
/*microcontrollers that don't have special bit operation instructions.*/

#define BIT(x)  (1 << (x))                  /*used by bit op macros   */

#define BIT0 0x01                           /*used by bit op macros   */
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

/* this macro sets a bit in the specified register or memory location */
#define SETBIT(reg,bit) reg|=bit

/* this macro clrs a bit in the specified register or memory location */
#define CLRBIT(reg,bit) reg&=(~bit)

/* this macro toggles a bit in the spec'd register or memory location */
#define TGLBIT(reg,bit) reg^=bit

/* this macro tests a bit, returning a non-zero val if bit is set *****/
#define TSTBIT(reg,bit) (reg&bit)

