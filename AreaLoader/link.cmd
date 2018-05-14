/****************************************************************************
* FILENAME: $Source: /db/visual/users/ptt/CVS/FlashBurner/src/FBTC54/Link.cmd,v $
* VERSION : $Revision: 1.3 $
* DATE    : $Date: 2001/04/19 18:56:34 $
* Copyright (c) 2001 Texas Instruments Incorporated
*
* COMMAND FILE FOR LINKING TMS5402 FBTC
*
*
*   Description: This file is used for linking an FBTC built with
*                the TMS54x C Compiler.   
*                Use it as a guideline; you may want to change
*                the allocation scheme according to the size of your
*                program and the memory layout of your target system.
*
*
****************************************************************************/
/*
 * $Log: Link.cmd,v $
 * Revision 1.3  2001/04/19 18:56:34  heeschen
 * v00.30 Alpha - Updated comments
 *
 */

-c
-stack 0x0256
-heap  0x0256


MEMORY  
{
/* Note: Assume PMST = 0xffe0
        PMST    Bit     Name   Value
        		15-7	IPTR	0xff80
                6       MP/!MC  1
                5       OVLY    1
                3       DROM    0
*/
	PAGE 0:		/* Program Space */
		RSV1	(R)		: o=00000h l=00080h	/* Reserved 				*/
  		DARAM	(RWIX)	: o=00080h l=03f80h	/* On-Chip Dual-Access RAM	*/
  		EXT0	(R)		: o=04000h l=0bf80h	/* External Page 0			*/
		VECS	(RWIX)	: o=0ff80h l=00080h	/* Interrupt Vector Table	*/
		EAP0    (RW)    : o=018000h l=8000h /* Extended addr. page 0    */
		
	PAGE 1:		/* Data Space */
		MMRS	(RW)	: o=00000h l=00060h	/* Memory-Mapped Registers	*/
		SPAD	(RW)	: o=00060h l=00020h	/* Scratch-Pad RAM			*/
  		DARAM	(RWIX)  : o=00080h l=03f80h	/* On-Chip Dual-Access RAM	*/
  		EXT0	(R)		: o=04000h l=0c000h	/* External Page 0			*/
}

SECTIONS
{
	GROUP : > DARAM	/* group sections in overlay for contiguous addresses	*/
	{	
		.text    	/* User code					*/
		.cinit   	/* initialization tables		*/
		.pinit   	/* initialization functions		*/
		.switch  	/* for C-switch tables			*/
	
		.cio		/* C-IO Buffer					*/
		.bss		/* global & static vars			*/
		.const		/* C constant tables			*/

		.sysmem		: fill = 0xDEAD	/* dynamic heap		*/
		.stack		: fill = 0xBEEF	/* system stack		*/
	}

	.intvecs 	: > VECS  PAGE 0	/* interrupt vector table			*/

	.data		: > SPAD  PAGE 1	/* asm data area					*/
}
