/*********************************************************************
 *          (C) COPYRIGHT TEXAS INSTRUMENTS, INC. 2000               *
 * FBCmd.h -- Process FlashBurn Target Component commands
 *            for 54x DSK
 */

/* Change Log:
 */
/*
 * $Log: /FlashBurn/PR223_EF/AreaLoader/fbcmd.h $
 * 
 * 1     7/01/05 12:42p Slvmapo
 * 
 * 2     5/30/05 10:40a Slvmapo
 * 
 * 1     4/15/05 12:25p Slvmapo
 * 
 * 1     4/15/05 12:23p Slvmapo
 * Revision 1.8  2001/04/19 18:56:33  heeschen
 * v00.30 Alpha - Updated comments
 *
 * Revision 1.7  2001/04/19 16:29:00  heeschen
 * v00.30 Snapshot
 * Saving this stuff after massive redesign to remove RTDX dependency.
 * Next step is to clean up, fix comments, and do unit testing.
 *
*/

/* Implements FlashBurn communications protocol
 */

#include <type.h>
#include "fbtc54.h"

//#define FLASHE_BIT  0x020

/* -----------------------------
 *  FLASH ADDRESSING  for AM29W400BB

    0x08000-0x09FFF   Block SA0
    0x0A000-0x0AFFF   Block SA1
    0x0B000-0x0BFFF   Block SA2
    0x0C000-0x0FFFF   Block SA3
    0x28000-0x2FFFF   Block SA4
    0x48000-0x4FFFF   Block SA5
    0x68000-0x6FFFF   Block SA6
    0x88000-0x8FFFF   Block SA7
    0xA8000-0xAFFFF   Block SA8
    0xC8000-0xCFFFF   Block SA9
    0xE8000-0xEFFFF   Block SA10
 */

#define FLBASE      0x00008000UL    /* DSP flash base address --> Flash address = 0 */
#define FLEND       0x00048000UL    /* Base + 256 Kword */
#define FLPAGELEN   0x00008000UL    /* 32 Kword  */
#define FLSIZEBYTES 0x00080000UL    /* 512 Kbyte */

#define ShortsToLong(vHighHalf, vLowHalf)  \
        ((long)(((unsigned long)((u16)(vHighHalf)) << 16) | ((unsigned long)((u16)(vLowHalf)))))

#define ShortsToULong(vHighHalf, vLowHalf)  \
        ((unsigned long)(((unsigned long)((u16)(vHighHalf)) << 16) | ((unsigned long)((u16)(vLowHalf)))))

/* Given a message from the Host, Carry out the command and prepare the response, if one is required */
void doCommand(u16 *msg);

/* Set/Get Flash Memory base address. The Host may want me to change where  */
/* I think the flash memory is, rather than have me use my hard-coded flash */
/* base address (FLASHBASE in this header file).                            */
/* It is set in response to a Set FLASHBASE Address command from Host.      */
unsigned long GetFlashBase(void);
void          SetFlashBase(unsigned long val);

/* Set/Get Flash Memory size. The Host may want me to change how big I think */
/* the flash memory is, rather than have me use my hard-coded flash size     */
/* (FLASHSIZE in this header file)                                           */
unsigned long GetFlashSize(void);
void          SetFlashSize(unsigned long val);

/* Initialize flash memory pointers to a known flash memory address */
void InitFlash(void);

/* Initialize flash memory pointers to a given address */
void SetFlashAddr(unsigned long addr);

/* Burn data into flash memory starting at current flash memory pointer */
/* Note that bytecount is #of 8-bit bytes.                              */
/* Thus on 5402 (2 bytes per address unit) indexing is by 16-bit units  */
void BurnFlash(u16 *data, u16 nBytes);

/* Get pointer to next available flash memory location */
volatile u16 *GetNextFlashAddr(void);

/* Erase all of flash memory */
void EraseFlash(void);

/* Called periodically to maintain the Status Busy Bit when flash is erasing */
void CheckFlashErase(void);

/* Checksum Processing. A simple modulo-16 additive cksum which adds the carry */
/* bit back into the accumulator. Initialize to a value */
void CKSSet(u16 val);

 /* Returns the current value */
u16 CKSGet(void);

 /* Accumulate a buffer */
u16 CKSAccumBuf(unsigned long buf, unsigned long len);

/* Return flash contents */
u16 GetFlashVal(unsigned long addr);

/* Sends a block of flash bytes to host */
void SendFlashBufToHost(u16 cmd, unsigned long addr, u16 count);

/*****************************************************************************/
/******** FOLLOWING SECTION HAS BEEN TAKES FROM PR223_EF FLASH.H FILE ********/
/*****************************************************************************/
/* Defines tags for every segment in Flash addressing space */
typedef enum {
    SA00 = 0,
    SA01,
    SA02,
    SA03,
    DSP_SECTORS,

    SA04 = DSP_SECTORS,
    SA05,
    SA06,
    SA07,
    SA08,
    SA09,
    SA10,
    FLASH_SECTORS

}NumSectors;

extern void flashWordWrite (u16 *dstAddress, short data);

/* Defines starting address for each sector */
static short * sectorTbl[DSP_SECTORS] = {
    /*---DSP address---------FLASH address---*/
    (short *)0x08000,     //(short *)0x00000,
    (short *)0x0A000,     //(short *)0x02000,
    (short *)0x0B000,     //(short *)0x03000,
    (short *)0x0C000      //(short *)0x04000,
};

/* Declare control structure to check flash algorithms */
typedef union {

    struct {
        unsigned int dummy          :8; /* DQ8..DQ15 */
        unsigned int dataPolling    :1; /* DQ7 */
        unsigned int toggleBit1     :1; /* DQ6 */
        unsigned int excTimeLimits  :1; /* DQ5 */
        unsigned int unused4        :1; /* DQ4 */
        unsigned int sectEraseTimer :1; /* DQ3 */
        unsigned int toggleBit2     :1; /* DQ2 */
        unsigned int unused1        :1; /* DQ1 */
        unsigned int unused0        :1; /* DQ0 */
    }bits;

    short word;
}Flags;
