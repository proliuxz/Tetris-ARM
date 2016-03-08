// ************* OPTIONS **************
//cowboy
#define MCLK (67500000)
//#define MCLK (60000000)
#define PLLON 1
//        (PLL_M+8)*Fin      (PLL_M+8)*4Mhz
//MCLK=------------------- = --------------
//     (PLL_P+2)*(2^PLL_S)        2*2
#if (MCLK==40000000)
	#define PLL_M (72)
	#define PLL_P (2)
	#define PLL_S (1)

#elif(MCLK==50000000)
	#define PLL_M (92)
	#define PLL_P (2)
	#define PLL_S (1)

#elif (MCLK==60000000)
	#define PLL_M (72)
	#define PLL_P (2)
	#define PLL_S (1)

#elif (MCLK==66000000)
	#define PLL_M (80)
	#define PLL_P (2)
	#define PLL_S (1)

#elif (MCLK==67500000)
	#define PLL_M (82)
	#define PLL_P (2)
	#define PLL_S (1)

#elif (MCLK==75000000)
	#define PLL_M (92)
	#define PLL_P (2)
	#define PLL_S (1)
#endif

#define WRBUFOPT (0x8)   //write_buf_on

#define SYSCFG_0KB (0x0|WRBUFOPT)
#define SYSCFG_4KB (0x2|WRBUFOPT)
#define SYSCFG_8KB (0x6|WRBUFOPT)

#define DRAM	    1		//In case DRAM is used
#define SDRAM	    2		//In case SDRAM is used
#define BDRAMTYPE   SDRAM	//used in power.c,44blib.c

//BUSWIDTH; 16,32
#define BUSWIDTH    (16)
//cowboy//#define Non_Cache_Start	(0x4000000)
#define Non_Cache_Start	(0x2000000)
//cowboy//#define Non_Cache_End 	(0xA000000)
#define Non_Cache_End 	(0xc000000)
#define CACHECFG    SYSCFG_8KB
//cowboy//#define _RAM_STARTADDRESS 0xc100000
#define _RAM_STARTADDRESS 0xc000000
#define _ISR_STARTADDRESS 0xc7fff00     //GCS6:64M DRAM/SDRAM


#define Debug 1			//Now all the code is under Debug State
//#define Debug 0
