;***************************************************************
;* TMS320C54x C/C++ Codegen                  PC Version 3.831  *
;* Date/Time created: Thu Apr 17 15:15:22 2008                 *
;***************************************************************
	.mmregs
FP	.set	AR7
	.c_mode
	.file	"fbcmd.c"
;	c:\program files\ti\c5400\cgtools\bin\acp500.exe -@C:\Temp\TI1752_4 

	.sect	".text"
	.sym	_SendVersionID,_SendVersionID, 32, 3, 0
	.func	56

;***************************************************************
;* FUNCTION DEF: _SendVersionID                                *
;***************************************************************
_SendVersionID:
	.line	1
;----------------------------------------------------------------------
;  56 | static void SendVersionID (u16 *msg) {                                 
;----------------------------------------------------------------------
;* A     assigned to _msg
	.sym	_msg,0, 30, 17, 16
	.sym	_msg,0, 30, 1, 16
        PSHM      AR1
        FRAME     #-1
        NOP
        STL       A,*SP(0)
	.line	2
;----------------------------------------------------------------------
;  57 | msg[CMDINDEX] = 0x000e;             /* Response to this command */     
;----------------------------------------------------------------------
        STLM      A,AR1
        NOP
        NOP
        ST        #14,*AR1              ; |57| 
	.line	3
;----------------------------------------------------------------------
;  58 | msg[ARGINDEX+0] = FBTCVersionMajor; /* Arg0 */                         
;----------------------------------------------------------------------
        MVDK      *SP(0),*(AR1)
        ST        #1,*AR1(1)            ; |58| 
	.line	4
;----------------------------------------------------------------------
;  59 | msg[ARGINDEX+1] = FBTCVersionMinor; /* Arg1 */                         
;----------------------------------------------------------------------
        MVDK      *SP(0),*(AR1)
        ST        #10,*AR1(2)           ; |59| 
	.line	5
        FRAME     #1
        POPM      AR1
        RET
        ; return occurs
	.endfunc	60,000000400h,2



	.sect	".text"
	.global	_doCommand
	.sym	_doCommand,_doCommand, 32, 2, 0
	.func	62

;***************************************************************
;* FUNCTION DEF: _doCommand                                    *
;***************************************************************
_doCommand:
	.line	1
;----------------------------------------------------------------------
;  62 | void doCommand (u16 *msg) {                                            
;----------------------------------------------------------------------
;* A     assigned to _msg
	.sym	_msg,0, 30, 17, 16
	.sym	_msg,4, 30, 1, 16
	.sym	_cmd,5, 14, 1, 16
        PSHM      AR1
        FRAME     #-8
        NOP
        STL       A,*SP(4)
	.line	2
;----------------------------------------------------------------------
;  63 | u16 cmd = msg[CMDINDEX];                                               
;  65 | #if DBPRINT == 1                                                       
;  66 | printf("CMD: %x", cmd);                                                
;  67 | #endif                                                                 
;----------------------------------------------------------------------
        STLM      A,AR1
        NOP
        NOP
        LD        *AR1,A
        STL       A,*SP(5)
	.line	11
;----------------------------------------------------------------------
;  72 | switch(cmd) {                                                          
;  73 |     case NO_OPERATION:                                                 
;  74 |     case 0xFFFF:                                                       
;  75 |         break;                                                         
;  77 |     case QUERY_PROTOCOL_VERSION:                                       
;----------------------------------------------------------------------
        B         L17                   ; |72| 
        ; branch occurs ; |72| 
L1:    
	.line	17
;----------------------------------------------------------------------
;  78 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |78| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |78| 
	.line	18
;----------------------------------------------------------------------
;  79 | SendI2ToHost(cmd, GetProtocolVersionID());                             
;  80 | break;                                                                 
;  82 | case QUERY_STATUS:                                                     
;  83 | #if DBPRINT == 1                                                       
;  84 | printf("  Status: %x", GetProtocolStatusWord());                       
;  85 | #endif                                                                 
;----------------------------------------------------------------------
        CALL      #_GetProtocolVersionID ; |79| 
        ; call occurs [#_GetProtocolVersionID] ; |79| 
        STL       A,*SP(0)
        LD        *SP(5),A
        CALL      #_SendI2ToHost        ; |79| 
        ; call occurs [#_SendI2ToHost] ; |79| 
        B         L19                   ; |79| 
        ; branch occurs ; |79| 
L2:    
	.line	25
;----------------------------------------------------------------------
;  86 | SendI2ToHost(cmd, GetProtocolStatusWord());                            
;  87 | break;                                                                 
;  89 | case ERASE_FLASH:                                                      
;----------------------------------------------------------------------
        CALL      #_GetProtocolStatusWord ; |86| 
        ; call occurs [#_GetProtocolStatusWord] ; |86| 
        STL       A,*SP(0)
        LD        *SP(5),A
        CALL      #_SendI2ToHost        ; |86| 
        ; call occurs [#_SendI2ToHost] ; |86| 
        B         L19                   ; |86| 
        ; branch occurs ; |86| 
L3:    
	.line	29
;----------------------------------------------------------------------
;  90 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |90| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |90| 
	.line	30
;----------------------------------------------------------------------
;  91 | EraseFlash();                                                          
;----------------------------------------------------------------------
        CALL      #_EraseFlash          ; |91| 
        ; call occurs [#_EraseFlash] ; |91| 
	.line	31
;----------------------------------------------------------------------
;  92 | SendI2ToHost(cmd, 0);                                                  
;  93 | break;                                                                 
;  95 | case QUERY_FLASH_BASE_ADDR:                                            
;----------------------------------------------------------------------
        ST        #0,*SP(0)             ; |92| 
        LD        *SP(5),A
        CALL      #_SendI2ToHost        ; |92| 
        ; call occurs [#_SendI2ToHost] ; |92| 
        B         L19                   ; |92| 
        ; branch occurs ; |92| 
L4:    
	.line	35
;----------------------------------------------------------------------
;  96 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |96| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |96| 
	.line	36
;----------------------------------------------------------------------
;  97 | SendI4ToHost(cmd, 0x00008000);                                         
;  98 | break;                                                                 
; 100 | case READ_FLASH_WORD:                                                  
; 101 | #if DBPRINT == 1                                                       
; 102 | printf(" ARGS: %x %x", msg[ARGINDEX+0], msg[ARGINDEX+1]);              
; 103 | #endif                                                                 
;----------------------------------------------------------------------
        RSBX      SXM
        NOP
        LD        #32768,A
        DST       A,*SP(0)              ; |97| 
        LD        *SP(5),A
        CALL      #_SendI4ToHost        ; |97| 
        ; call occurs [#_SendI4ToHost] ; |97| 
        B         L19                   ; |97| 
        ; branch occurs ; |97| 
L5:    
	.line	43
;----------------------------------------------------------------------
; 104 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |104| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |104| 
	.line	44
;----------------------------------------------------------------------
; 105 | SendI2ToHost(cmd, GetFlashVal(GetFlashBase() + ShortsToULong(msg[ARGIND
;     | EX+0], msg[ARGINDEX+1])));                                             
; 106 | break;                                                                 
; 108 | case READ_FLASH_BLOCK:                                                 
; 109 | #if DBPRINT == 1                                                       
; 110 | printf(" ARGS: %x %x %x", msg[ARGINDEX+0], msg[ARGINDEX+1], msg[ARGINDE
;     | X+2]);                                                                 
; 111 | #endif                                                                 
;----------------------------------------------------------------------
        CALL      #_GetFlashBase        ; |105| 
        ; call occurs [#_GetFlashBase] ; |105| 
        MVDK      *SP(4),*(AR1)
        DST       A,*SP(6)              ; |105| 
        LDU       *AR1(1),A
        SFTL      A,#15,B               ; |105| 
        SFTL      B,#1,B                ; |105| 
        LDU       *AR1(2),A
        OR        B,A                   ; |105| 
        DADD      *SP(6),A,A            ; |105| 
        CALL      #_GetFlashVal         ; |105| 
        ; call occurs [#_GetFlashVal] ; |105| 
        STL       A,*SP(0)
        LD        *SP(5),A
        CALL      #_SendI2ToHost        ; |105| 
        ; call occurs [#_SendI2ToHost] ; |105| 
        B         L19                   ; |105| 
        ; branch occurs ; |105| 
L6:    
	.line	51
;----------------------------------------------------------------------
; 112 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |112| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |112| 
	.line	52
;----------------------------------------------------------------------
; 113 | SendFlashBufToHost(cmd, GetFlashBase() + ShortsToULong(msg[ARGINDEX+0],
;     |  msg[ARGINDEX+1]), msg[ARGINDEX+2]);                                   
; 114 | break;                                                                 
; 116 | case WRITE_FLASH_FIRST:                                                
; 117 | #if DBPRINT == 1                                                       
; 118 | printf(" ARGS: %x %x %x",  msg[ARGINDEX+0], msg[ARGINDEX+1], msg[ARGIND
;     | EX+2]);                                                                
; 119 | #endif                                                                 
;----------------------------------------------------------------------
        CALL      #_GetFlashBase        ; |113| 
        ; call occurs [#_GetFlashBase] ; |113| 
        MVDK      *SP(4),*(AR1)
        DST       A,*SP(6)              ; |113| 
        LDU       *AR1(1),B
        SFTL      B,#15,A               ; |113| 
        SFTL      A,#1,A                ; |113| 
        LDU       *AR1(2),B
        OR        A,B                   ; |113| 
        DADD      *SP(6),B,B            ; |113| 
        DST       B,*SP(0)              ; |113| 
        MVDK      *SP(4),*(AR1)
        LD        *AR1(3),A
        STL       A,*SP(2)
        LD        *SP(5),A
        CALL      #_SendFlashBufToHost  ; |113| 
        ; call occurs [#_SendFlashBufToHost] ; |113| 
        B         L19                   ; |113| 
        ; branch occurs ; |113| 
L7:    
	.line	59
;----------------------------------------------------------------------
; 120 | SetFlashAddr(GetFlashBase() + ShortsToULong(msg[ARGINDEX+0], msg[ARGIND
;     | EX+1]));                                                               
;----------------------------------------------------------------------
        CALL      #_GetFlashBase        ; |120| 
        ; call occurs [#_GetFlashBase] ; |120| 
        MVDK      *SP(4),*(AR1)
        DST       A,*SP(6)              ; |120| 
        LDU       *AR1(1),A
        SFTL      A,#15,B               ; |120| 
        SFTL      B,#1,B                ; |120| 
        LDU       *AR1(2),A
        OR        B,A                   ; |120| 
        DADD      *SP(6),A,A            ; |120| 
        CALL      #_SetFlashAddr        ; |120| 
        ; call occurs [#_SetFlashAddr] ; |120| 
	.line	60
;----------------------------------------------------------------------
; 121 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |121| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |121| 
	.line	61
;----------------------------------------------------------------------
; 122 | BurnFlash(&msg[DATAINDEX], msg[ARGINDEX+2]);                           
; 124 | break;                                                                 
; 126 | case WRITE_FLASH_CONT:                                                 
; 128 | #if DBPRINT == 1                                                       
; 129 | printf(" ARG: %x",  msg[ARGINDEX+0]);                                  
; 130 | #endif                                                                 
;----------------------------------------------------------------------
        MVDK      *SP(4),*(AR1)
        LD        *AR1(3),A
        STL       A,*SP(0)
        LD        *SP(4),A
        CALLD     #_BurnFlash           ; |122| 
        ADD       #16,A,A               ; |122| 
        ; call occurs [#_BurnFlash] ; |122| 
        B         L19                   ; |122| 
        ; branch occurs ; |122| 
L8:    
	.line	70
;----------------------------------------------------------------------
; 131 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |131| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |131| 
	.line	71
;----------------------------------------------------------------------
; 132 | BurnFlash(&msg[DATAINDEX], msg[ARGINDEX+0]);                           
; 133 | break;                                                                 
; 135 | case QUERY_CHECKSUM:                                                   
; 136 | #if DBPRINT == 1                                                       
; 137 | printf(" ARGS: %x %x %x %x",  msg[ARGINDEX+0], msg[ARGINDEX+1], msg[ARG
;     | INDEX+2], msg[ARGINDEX+3]);                                            
; 138 | #endif                                                                 
;----------------------------------------------------------------------
        MVDK      *SP(4),*(AR1)
        LD        *AR1(1),A
        STL       A,*SP(0)
        LD        *SP(4),A
        CALLD     #_BurnFlash           ; |132| 
        ADD       #16,A,A               ; |132| 
        ; call occurs [#_BurnFlash] ; |132| 
        B         L19                   ; |132| 
        ; branch occurs ; |132| 
L9:    
	.line	78
;----------------------------------------------------------------------
; 139 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |139| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |139| 
	.line	79
;----------------------------------------------------------------------
; 140 | CKSSet(0);                                                             
;----------------------------------------------------------------------
        CALLD     #_CKSSet              ; |140| 
        NOP
        LD        #0,A
        ; call occurs [#_CKSSet] ; |140| 
	.line	80
;----------------------------------------------------------------------
; 141 | SendI2ToHost(cmd,                                                      
; 142 |              CKSAccumBuf(GetFlashBase() + ShortsToULong(msg[ARGINDEX+0]
;     | , msg[ARGINDEX+1]),                                                    
; 143 |              ShortsToULong(msg[ARGINDEX+2], msg[ARGINDEX+3]))          
; 144 |             );                                                         
; 145 | break;                                                                 
; 147 | case ACCEPT_USER_DATA:                                                 
; 151 | #if DBPRINT == 1                                                       
; 152 | printf(" ARG: %x",  msg[ARGINDEX+0]);                                  
; 153 | #endif                                                                 
;----------------------------------------------------------------------
        CALL      #_GetFlashBase        ; |141| 
        ; call occurs [#_GetFlashBase] ; |141| 
        MVDK      *SP(4),*(AR1)
        DST       A,*SP(6)              ; |141| 
        LDU       *AR1(1),A
        SFTL      A,#15,B               ; |141| 
        SFTL      B,#1,B                ; |141| 
        LDU       *AR1(2),A
        OR        B,A                   ; |141| 
        DADD      *SP(6),A,A            ; |141| 
        DST       A,*SP(6)              ; |141| 
        LDU       *AR1(4),A
        LDU       *AR1(3),B
        SFTL      B,#15,B               ; |141| 
        SFTL      B,#1,B                ; |141| 
        OR        B,A                   ; |141| 
        DST       A,*SP(0)              ; |141| 
        DLD       *SP(6),A              ; |141| 
        CALL      #_CKSAccumBuf         ; |141| 
        ; call occurs [#_CKSAccumBuf] ; |141| 
        STL       A,*SP(0)
        LD        *SP(5),A
        CALL      #_SendI2ToHost        ; |141| 
        ; call occurs [#_SendI2ToHost] ; |141| 
        B         L19                   ; |141| 
        ; branch occurs ; |141| 
L10:    
	.line	93
;----------------------------------------------------------------------
; 154 | SetErrorBits(CMDOK);                                                   
; 156 | // flashSectorErase(SA1);                                              
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |154| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |154| 
	.line	96
;----------------------------------------------------------------------
; 157 | SendI2ToHost(cmd, 0);                                                  
; 158 | break;                                                                 
; 160 | case QUERY_FLASH_SIZE:                                                 
;----------------------------------------------------------------------
        ST        #0,*SP(0)             ; |157| 
        LD        *SP(5),A
        CALL      #_SendI2ToHost        ; |157| 
        ; call occurs [#_SendI2ToHost] ; |157| 
        B         L19                   ; |157| 
        ; branch occurs ; |157| 
L11:    
	.line	100
;----------------------------------------------------------------------
; 161 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |161| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |161| 
	.line	101
;----------------------------------------------------------------------
; 162 | SendI4ToHost(cmd, GetFlashSize());                                     
; 163 | break;                                                                 
; 165 | case CHANGE_FLASHBASE_ADDR:                                            
; 166 | #if DBPRINT == 1                                                       
; 167 | printf(" ARGS: %x %x", msg[ARGINDEX+0], msg[ARGINDEX+1]);              
; 168 | #endif                                                                 
;----------------------------------------------------------------------
        CALL      #_GetFlashSize        ; |162| 
        ; call occurs [#_GetFlashSize] ; |162| 
        DST       A,*SP(0)              ; |162| 
        LD        *SP(5),A
        CALL      #_SendI4ToHost        ; |162| 
        ; call occurs [#_SendI4ToHost] ; |162| 
        B         L19                   ; |162| 
        ; branch occurs ; |162| 
L12:    
	.line	108
;----------------------------------------------------------------------
; 169 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |169| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |169| 
	.line	109
;----------------------------------------------------------------------
; 170 | SetFlashBase(ShortsToULong(msg[ARGINDEX+0], msg[ARGINDEX+1]));         
; 171 | break;                                                                 
; 173 | case CHANGE_FLASH_SIZE:                                                
; 174 | #if DBPRINT == 1                                                       
; 175 | printf(" ARGS: %x %x\n", msg[ARGINDEX+0], msg[ARGINDEX+1]);            
; 176 | #endif                                                                 
;----------------------------------------------------------------------
        MVDK      *SP(4),*(AR1)
        LDU       *AR1(1),A
        SFTL      A,#15,B               ; |170| 
        LDU       *AR1(2),A
        CALLD     #_SetFlashBase        ; |170| 
        SFTL      B,#1,B                ; |170| 
        OR        B,A                   ; |170| 
        ; call occurs [#_SetFlashBase] ; |170| 
        B         L19                   ; |170| 
        ; branch occurs ; |170| 
L13:    
	.line	116
;----------------------------------------------------------------------
; 177 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |177| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |177| 
	.line	117
;----------------------------------------------------------------------
; 178 | SetFlashSize(ShortsToULong(msg[ARGINDEX+0], msg[ARGINDEX+1]));         
; 179 | break;                                                                 
; 181 | case QUERY_FBTC_VERSION:                                               
;----------------------------------------------------------------------
        MVDK      *SP(4),*(AR1)
        LDU       *AR1(1),A
        SFTL      A,#15,B               ; |178| 
        LDU       *AR1(2),A
        CALLD     #_SetFlashSize        ; |178| 
        SFTL      B,#1,B                ; |178| 
        OR        B,A                   ; |178| 
        ; call occurs [#_SetFlashSize] ; |178| 
        B         L19                   ; |178| 
        ; branch occurs ; |178| 
L14:    
	.line	121
;----------------------------------------------------------------------
; 182 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |182| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |182| 
	.line	122
;----------------------------------------------------------------------
; 183 | SendVersionID(msg);                                                    
; 184 | break;                                                                 
; 186 | case QUERY_FLASH_PERMISSION:                                           
;----------------------------------------------------------------------
        LD        *SP(4),A
        CALL      #_SendVersionID       ; |183| 
        ; call occurs [#_SendVersionID] ; |183| 
        B         L19                   ; |183| 
        ; branch occurs ; |183| 
L15:    
	.line	126
;----------------------------------------------------------------------
; 187 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |187| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |187| 
	.line	127
;----------------------------------------------------------------------
; 188 | SendI2ToHost(cmd, EDITFLASHPERMIT);                                    
; 189 | break;                                                                 
; 191 | case QUERY_MAX_DATABLOCK_SIZE:                                         
;----------------------------------------------------------------------
        ST        #1,*SP(0)             ; |188| 
        LD        *SP(5),A
        CALL      #_SendI2ToHost        ; |188| 
        ; call occurs [#_SendI2ToHost] ; |188| 
        B         L19                   ; |188| 
        ; branch occurs ; |188| 
L16:    
	.line	131
;----------------------------------------------------------------------
; 192 | SetErrorBits(CMDOK);                                                   
;----------------------------------------------------------------------
        CALLD     #_SetErrorBits        ; |192| 
        NOP
        LD        #0,A
        ; call occurs [#_SetErrorBits] ; |192| 
	.line	132
;----------------------------------------------------------------------
; 193 | SendI2ToHost(cmd, MAXDATABYTES);                                       
; 194 | break;                                                                 
; 196 | default:                                                               
; 197 | #if DBPRINT == 1                                                       
; 198 | printf(" *** UNKNOWN! ***");                                           
; 199 | #endif                                                                 
; 200 | break;                                                                 
; 203 | #if DBPRINT == 1                                                       
; 204 | printf("\n");                                                          
; 205 | #endif                                                                 
;----------------------------------------------------------------------
        ST        #4096,*SP(0)          ; |193| 
        LD        *SP(5),A
        CALL      #_SendI2ToHost        ; |193| 
        ; call occurs [#_SendI2ToHost] ; |193| 
        B         L19                   ; |193| 
        ; branch occurs ; |193| 
L17:    
	.line	11
        STLM      A,AR1
        NOP
        STM       #1,AR0
        MAR       *AR1+
        CMPR      2,AR1                 ; |72| 
        BC        L19,NTC               ; |72| 
        ; branch occurs ; |72| 
        STLM      A,AR1
        NOP
        STM       #15,AR0
        MAR       *AR1-
        CMPR      2,AR1                 ; |72| 
        BC        L18,NTC               ; |72| 
        ; branch occurs ; |72| 
        B         L19                   ; |72| 
        ; branch occurs ; |72| 
L18:    
        SUB       #1,A,A                ; |72| 
        ADD       #SW0,A
        LDM       AL,A                  ; |72| 
        READA     *(AL)                 ; |72| 
        BACC      A                     ; |72| 
        ; branch occurs ; |72| 

	.sect	".switch"
SW0:	.word	L1	; 1
	.word	L2	; 2
	.word	L3	; 3
	.word	L4	; 4
	.word	L5	; 5
	.word	L6	; 6
	.word	L7	; 7
	.word	L8	; 8
	.word	L9	; 9
	.word	L10	; 10
	.word	L11	; 11
	.word	L12	; 12
	.word	L13	; 13
	.word	L14	; 14
	.word	L15	; 15
	.word	L16	; 16

	.sect	".text"
L19:    
	.line	145
        FRAME     #8
        POPM      AR1
        RET
        ; return occurs
	.endfunc	206,000000400h,9


;***************************************************************
;* UNDEFINED EXTERNAL REFERENCES                               *
;***************************************************************
	.global	_GetProtocolStatusWord
	.global	_GetProtocolVersionID
	.global	_SetErrorBits
	.global	_SendI4ToHost
	.global	_SendI2ToHost
	.global	_GetFlashBase
	.global	_SetFlashBase
	.global	_GetFlashSize
	.global	_SetFlashSize
	.global	_SetFlashAddr
	.global	_BurnFlash
	.global	_EraseFlash
	.global	_CKSSet
	.global	_CKSAccumBuf
	.global	_GetFlashVal
	.global	_SendFlashBufToHost

;***************************************************************
;* TYPE INFORMATION                                            *
;***************************************************************
	.sym	_u16, 0, 14, 13, 16
