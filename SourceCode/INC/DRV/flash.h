#include "..\STARTUP\44b.h"
#include "..\inc\def.h"


/*KM29U128 IO 定义*/
//RB  ->GPE0
//CLE ->GPC11
//ALE ->GPC10
//WP ->
//CE ->GPC15
#define RB	(0x01)
#define CLE	(0x800)
#define ALE	(0x400)
//#define WP	(0x08)	
#define CE	(0x8000)
#define rKM29UXDATA	   (*(volatile unsigned char *)0x04000000)	//bank2              
/*KM29U128操作宏定义*/
#define FC_CMD		{rPDATC |= CLE;	rPDATC &= ~(ALE|CE);}
#define FC_ADDR		{rPDATC |= ALE;	rPDATC&=~(CLE|CE);}
#define FC_DATA        {rPDATC &= ~(ALE|CLE|CE);}
#define FC_INACTIVE	{rPDATC |= CE;	rPDATC&=~(ALE|CLE);}
#define WAITRB		while(!(rPDATE & RB))  //wait tWB and check R/B pin.   
#define KM29U128_ID (0xec73)
#define WRITEVERIFY  (0)  //Verifing is enable when writing flash
/*KM29U128函数声明*/

#define NandPageSize	528

void Flash_Reset(void);//flash reset
unsigned int Check_Flash_Id(void);
unsigned char Erase_Cluster(unsigned int cluster);
void ReadPage(unsigned int block,unsigned int page,unsigned char *pPage);
int WritePage(unsigned int block,unsigned int page,U8 *pPage);
void Flash_Tools(void);
