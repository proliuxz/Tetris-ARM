        AREA    ISR_STARTADDRESS, DATA, NOINIT

        EXPORT	HandleReset
        EXPORT	HandleUndef
        EXPORT	HandleSWI
        EXPORT	HandlePabort
        EXPORT	HandleDabort
        EXPORT	HandleReserved
        EXPORT	HandleFIQ
        EXPORT	HandleIRQ

        EXPORT	HandleADC
        EXPORT	HandleRTC
        EXPORT	HandleUTXD1
        EXPORT	HandleUTXD0
        EXPORT	HandleSIO
        EXPORT	HandleIIC
        EXPORT	HandleURXD1
        EXPORT	HandleURXD0
        EXPORT	HandleTIMER5
        EXPORT	HandleTIMER4
        EXPORT	HandleTIMER3
        EXPORT	HandleTIMER2
        EXPORT	HandleTIMER1
        EXPORT	HandleTIMER0
        EXPORT	HandleUERR01
        EXPORT	HandleWDT
        EXPORT	HandleBDMA1
        EXPORT	HandleBDMA0
        EXPORT	HandleZDMA1
        EXPORT	HandleZDMA0
        EXPORT	HandleTICK
        EXPORT	HandleEINT4567
        EXPORT	HandleEINT3
        EXPORT	HandleEINT2
        EXPORT	HandleEINT1
        EXPORT	HandleEINT0

HandleReset	SPACE	4
HandleUndef	SPACE	4
HandleSWI	SPACE	4
HandlePabort	SPACE	4
HandleDabort	SPACE	4
HandleReserved	SPACE	4
HandleIRQ	SPACE	4
HandleFIQ	SPACE	4

;Don't use the label 'IntVectorTable',
;because armasm.exe cann't recognize this label correctly.
;the value is different with an address you think it may be.
;IntVectorTable
HandleADC	SPACE	4
HandleRTC	SPACE	4
HandleUTXD1	SPACE	4
HandleUTXD0	SPACE	4
HandleSIO	SPACE	4
HandleIIC	SPACE	4
HandleURXD1	SPACE	4
HandleURXD0	SPACE	4
HandleTIMER5	SPACE	4
HandleTIMER4	SPACE	4
HandleTIMER3	SPACE	4
HandleTIMER2	SPACE	4
HandleTIMER1	SPACE	4
HandleTIMER0	SPACE	4
HandleUERR01	SPACE	4
HandleWDT	SPACE	4
HandleBDMA1	SPACE	4
HandleBDMA0	SPACE	4
HandleZDMA1	SPACE	4
HandleZDMA0	SPACE	4
HandleTICK	SPACE	4
HandleEINT4567	SPACE	4
HandleEINT3	SPACE	4
HandleEINT2	SPACE	4
HandleEINT1	SPACE	4
HandleEINT0	SPACE	4   ;0xc1(c7)fff84

		END