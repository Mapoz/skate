;**********************************************************************
;*       (C) COPYRIGHT TEXAS INSTRUMENTS, INC. 2000                   *
;*                                                                    *
;* dspSetup.asm -- Some setup stuff for FlashBurn
;*                 5402 FBTC (target component)
;**********************************************************************
;


;/* Change Log:
; */
;/*
;$Log: /FlashBurn/PR223_EF/AreaLoader/dspsetup.asm $
; 
; 1     7/01/05 12:43p Slvmapo
; 
; 1     4/15/05 12:25p Slvmapo
; 
; 1     4/15/05 12:23p Slvmapo
;Revision 1.1  2000/10/20 19:56:37  heeschen
;Initial Version
;
;*/

;******************************************************************************
;** DSP Control Register Definitions
;******************************************************************************
SWWSR        .set (28h)
SWCR         .set (2Bh)

FLWAIT_SWWSR .set (7E38h)      ; Wait States for Flash, see PR223_EF.cdb
INIT_SWCR    .set (0001h)      ; Wait State Multipler bit set for x1, see PR223_EF.cdb

    .global _dspInitFl
    .text

_dspInitFl:
        stm     #INIT_SWCR, SWCR       ; Set Wait State Multipler Bit
        stm     #FLWAIT_SWWSR, SWWSR   ; Set I/O and Ext memory wait states
        ret
    .end
