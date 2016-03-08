	GET option.s

	[ BUSWIDTH=16
BWSCON_cfg EQU   0x11111011	;Bank0=16bit BootRom(AT29LV160DB) :0x0
;                  ||||||--	 Bank1=low 8bit D12               :0x4
;                  |||||---	 Bank2=8bit  NAND-Flash(KM29U128T):0x2
;                  ||||----	 Bank3=16bit Expend               :0x6
;                  |||-----	 Bank4=16bit Expend		  :0x8
;                  ||------	 Bank5=16bit ISA                  :0xA
;                  |-------   Bank6=16bit SDRAM                :0xc
;                  --------   Bank7=16bit NoUsed
	| ;BUSWIDTH=32
BWSCON_cfg EQU  0x22222220	;Bank0=OM[1:0], Bank1~Bank7=32bit
	]
;**********MEMORY CONTROL PARAMETERS*******************************
;When MCLK=66MHz,1clk=0.0152us=15.2ns
;Bank 0 parameter for Monitor Rom
                GBLS    BDRAMTYPE
BDRAMTYPE	SETS    "SDRAM"
B0_Tacs		EQU	0x0	;0clk
B0_Tcos		EQU	0x0	;0clk
B0_Tacc		EQU	0x6	;10clk
B0_Tcoh		EQU	0x0	;0clk
B0_Tah		EQU	0x0	;0clk
B0_Tacp		EQU	0x0	;0clk
B0_PMC		EQU	0x0	;normal(1data)

;Bank 1 parameter for SRAM
B1_Tacs		EQU	0x3	;4clk
B1_Tcos		EQU	0x3	;4clk
B1_Tacc		EQU	0x7	;14clk
B1_Tcoh		EQU	0x3	;4clk
B1_Tah		EQU	0x3	;4clk
B1_Tacp		EQU	0x3	;6clk
B1_PMC		EQU	0x0	;normal(1data)

;Bank 2 parameter for KM29U128T
B2_Tacs		EQU	0x1	;4clk
B2_Tcos		EQU	0x2	;4clk
B2_Tacc		EQU	0x7	;14clk
B2_Tcoh		EQU	0x2	;4clk
B2_Tah		EQU	0x2	;4clk
B2_Tacp		EQU	0x3	;6clk
B2_PMC		EQU	0x0	;normal(1data)

;Bank 3 parameter for USBN9603
B3_Tacs		EQU	0x1	;(Address set-up before nGCSn)
B3_Tcos		EQU	0x2;(Chip selection set-up nOE)
B3_Tacc		EQU	0x7	;14clk(Access cycle)
B3_Tcoh		EQU	0x2;(Chip selection hold on nOE)
B3_Tah		EQU	0x2;(Address holding time after nGCSn)
B3_Tacp		EQU	0x3	;(Page mode access cycle @ Page mode)
B3_PMC		EQU	0x0	;normal(1data)

;Bank 4 parameter for IDE
B4_Tacs		EQU	0x3	;4clk
B4_Tcos		EQU	0x4	;4clk
B4_Tacc		EQU	0x6	;12clk
B4_Tcoh		EQU	0x3	;4clk
B4_Tah		EQU	0x3	;4clk
B4_Tacp		EQU	0x3	;6clk
B4_PMC		EQU	0x0	;normal(1data)

;Bank 5 parameter
B5_Tacs		EQU	0x3	;4clk	0x3	;4clk
B5_Tcos		EQU	0x3	;4clk
B5_Tacc		EQU	0x7	;14clk	0x7	;14clk
B5_Tcoh		EQU	0x3	;4clk
B5_Tah		EQU	0x3	;4clk	0x3	;4clk
B5_Tacp		EQU	0	;0x3	;6clk
B5_PMC		EQU	0x0	;normal(1data)

;Bank 6(if SROM) parameter
;B6_Tacs		EQU	0x3	;4clk
;B6_Tcos		EQU	0x3	;4clk
;B6_Tacc		EQU	0x7	;14clk
;B6_Tcoh		EQU	0x3	;4clk
;B6_Tah		EQU	0x3	;4clk
;B6_Tacp		EQU	0x3	;6clk
;B6_PMC		EQU	0x0	;normal(1data)

;Bank 7(if SROM) parameter
;B7_Tacs		EQU	0x3	;4clk
;B7_Tcos		EQU	0x3	;4clk
;B7_Tacc		EQU	0x7	;14clk
;B7_Tcoh		EQU	0x3	;4clk
;B7_Tah		EQU	0x3	;4clk
;B7_Tacp		EQU	0x3	;6clk
;B7_PMC		EQU	0x0	;normal(1data)

;Bank 6 parameter
	[ BDRAMTYPE="DRAM"	;MT=01(FP DRAM) or 10(EDO DRAM) 
B6_MT		EQU	0x2	;EDO DRAM
B6_Trcd		EQU	0x1	;2clk
B6_Tcas		EQU	0x1	;2clk
B6_Tcp		EQU	0x1	;2clk
B6_CAN		EQU	0x2	;10bit
	| ;"SDRAM"		;MT=11(SDRAM)
B6_MT		EQU	0x3	;SDRAM
B6_Trcd		EQU	0x0	;2clk
B6_SCAN		EQU	0x0	;8bit
	]

;Bank 7 parameter
	[ BDRAMTYPE="DRAM"	;MT=01(FP DRAM) or 10(EDO DRAM) 
B7_MT		EQU	0x2	;EDO DRAM
B7_Trcd		EQU	0x1	;2clk
B7_Tcas		EQU	0x1	;2clk
B7_Tcp		EQU	0x1	;2clk
B7_CAN		EQU	0x2	;10bit
	| ;"SDRAM"		;MT=11(SDRAM)
B7_MT		EQU	0x3	;SDRAM
B7_Trcd		EQU	0x0	;2clk
B7_SCAN		EQU	0x0	;8bit
	]

;REFRESH parameter
REFEN		EQU	0x1	;Refresh enable
TREFMD		EQU	0x0	;CBR(CAS before RAS)/Auto refresh
Trp		EQU	0x1	;3clk
Trc		EQU	0x1	;5clk
Tchr		EQU	0x2	;3clk
REFCNT		EQU	1425     ;1019	;period=15.6us, MCLK=66Mhz
;************************************************
	END
