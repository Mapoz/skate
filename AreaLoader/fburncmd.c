/*
 ********************************************************************
 *   (C) COPYRIGHT TEXAS INSTRUMENTS, INC. 2000, 2001
 *   FlashBurn Programming Functions
 *   for FBTC54
 *
 * $Log: /FlashBurn/PR223_EF/AreaLoader/fburncmd.c $
 * 
 * 1     7/01/05 12:42p Slvmapo
 * 
 * 2     5/30/05 10:40a Slvmapo
 * 
 * 1     4/15/05 12:25p Slvmapo
 * 
 * 1     4/15/05 12:23p Slvmapo
 * Revision 1.11  2001/06/27 17:55:13  heeschen
 * v00.32 Beta Prep
 * Capturing all files to prepare for Beta Release
 *
 * Revision 1.10  2001/04/19 18:56:34  heeschen
 * v00.30 Alpha - Updated comments
 *
 * Revision 1.9  2001/04/19 16:29:01  heeschen
 * v00.30 Snapshot
 * Saving this stuff after massive redesign to remove RTDX dependency.
 * Next step is to clean up, fix comments, and do unit testing.
 *
 */
#include <stdio.h>
#include "fbtc54.h"
#include <board.h>
#include "fbcmd.h"

/* Extern of data structure coming from Host */
extern u16 theMessage[];

static int ErsStatus = 0;
/* --------------------------
 * EXTERNAL MEMORY ADDRESSING
 *         HELPERS
 * --------------------------
 */

/* Used by the checksum calculator function */
static unsigned long cksum = 0;

/* flashstart and flashnext are used to track where we are in a "flat" Flash memory */
/* Paging, etc. are handled by helper funcs. flashbaseoverride is 0 normally, but   */
/* changes if Host sends the Change FLASHSTART Address command.                     */
/* Thus if it's not 0xffffffff, then it should be used instead of the FLBASE value. */
static unsigned long flashstart  = 0x00008000L;
static unsigned long flashnext   = 0x00080000L;

/* These are "override" values, in case the  Host has sent new flash base addr and size */
static unsigned long flashbaseov = 0xFFFFFFFFL;
static unsigned long flashsizeov = 0xFFFFFFFFL;

/* Set and get the flash base address */
void SetFlashBase (unsigned long val) {
    flashbaseov = val;
}

unsigned long GetFlashBase (void) {
    return (flashbaseov != 0xffffffffL ? flashbaseov : FLBASE);
}

/* Set and get the flash size */
void SetFlashSize (unsigned long val) {
    flashsizeov = val;
}

unsigned long GetFlashSize(void) {
    return (flashsizeov != 0xffffffffL ? flashsizeov : FLSIZEBYTES);
}

/* Set the External Memory Page Register */
void SetPage (u16 pnum) {
    port2 = pnum & 0x1f;
}

/* Flash memory addressing on the 54x
 * ----------------------------------
 * Pages are 32K words, starting at 0x8000.
 * Comm Protocol uses a logical address in a
 * "flat" address space. Host also uses
 * 8-bit byte counts instead of word counts.
 *
 * Thus when Host wants 256 bytes written at
 * logical address 0x20004, we need to convert
 * from 0x20004 to page and address within page:
 * 0x20004 = Page 4, addr 4
 */

/* Returns page number given "flat" address */
u16 GetPageNum (unsigned long addr) {
    return ((u16)((addr - GetFlashBase()) / FLPAGELEN));
}

/* Returns the on-page address for a given "flat" address */
u16 *GetPageAddr (unsigned long addr) {
    unsigned long myoffset = addr - GetFlashBase();

    return ((u16 *)(GetFlashBase() + (myoffset % FLPAGELEN)));
}

/* Returns the remaining # of MAUs on the page for a given "flat" address */
u16 GetPageMAURemaining (unsigned long addr) {
    unsigned long myoffset = addr - GetFlashBase();

    return (FLPAGELEN - (myoffset % FLPAGELEN));
}

/* --------------------------------
 * Flash memory handlers
 * --------------------------------
 */
void InitFlash (void) {
    flashstart = GetFlashBase();
    flashnext  = flashstart;
}

void SetFlashAddr (unsigned long addr) {
    flashstart = addr;
    flashnext  = flashstart;
}

volatile u16 *GetNextFlashAddr (void) {
    return (volatile u16 *)flashnext;
}

/* This function erase sector involved in Boottable loading */
/* and segments that holds EFDP areas                       */
void EraseFlash(void) {
    volatile u16 *p5 = (u16 *)(GetFlashBase()+0x5555UL);
    volatile u16 *pA = (u16 *)(GetFlashBase()+0x2AAAUL);

    InitFlash();
    FlashEnable();
    SetPage(0);

    /* Erase ESEG1 */
    *p5 = 0xAA;
    *pA = 0x55;
    *p5 = 0x80;
    *p5 = 0xAA;
    *pA = 0x55;
    *((u16 *)0xA000) = 0x30;

    FlashDisable();

    ErsStatus = 1;

    /* Returns when erase is done */
    while(ErsStatus != 0) {
        FlashEnable();
        SetPage(0);
        /* */
        if((*(u16 *)0xAFFF) == 0xFFFF) {
            ErsStatus = 0;
        }
        FlashDisable();
    }

    InitFlash();
    FlashEnable();
    SetPage(0);

    /* Erase ESEG2 */
    *p5 = 0xAA;
    *pA = 0x55;
    *p5 = 0x80;
    *p5 = 0xAA;
    *pA = 0x55;
    *((u16 *)0xB000) = 0x30;

    FlashDisable();

    ErsStatus = 1;

    /* Returns when erase is done */
    while(ErsStatus != 0) {
        FlashEnable();
        SetPage(0);
        /* */
        if((*(u16 *)0xBFFF) == 0xFFFF) {
            ErsStatus = 0;
        }
        FlashDisable();
    }
}

/*******************************************************************************
 *
 * Function: flashWordWrite
 *
 * Input parameters : short - data, the word to write to flash
 *                    short - address, destination address of the word
 *
 * Output parameters: None.
 *
 * Returned value: int result of operation
 *                    < 0 = Error
 *                    >=0 = OK
 *
 * Description: This function writes a single word to flash.
 *              The function is accomplished with PROGRAM sequence.
 *              The sequence takes 4 cycles.
 *
 ******************************************************************************/
void flashWordWrite (u16* dstAddress, short data) {

    /* Convenient pointers for sending commands to flash chip */
    volatile u16 *p5 = (u16 *)(GetFlashBase()+0x5555UL);
    volatile u16 *pA = (u16 *)(GetFlashBase()+0x2AAAUL);
    unsigned long timeoutcount;
    volatile u16 *pBurn;
    u16           page;

    FlashEnable();

    /* All words to be written are within the same page */
    pBurn = dstAddress;

    /* Set paging and enable flash access */
    page = GetPageNum(flashnext);

    SetPage(0);

    *p5 = 0xaa;
    *pA = 0x55;
    *p5 = 0xa0;

    SetPage(page);

    *pBurn = data;

    timeoutcount = 0;
    do {
        timeoutcount += 1;
    } while(*pBurn != data && timeoutcount < 0xffff);

    SetPage(0);
    FlashDisable();
}

/* Flash is accesed via external memory pages */
void BurnFlash(u16 *data, u16 nBytes) {
    /* Convenient pointers for sending commands to flash chip */
    volatile u16 *p5 = (u16 *)(GetFlashBase()+0x5555UL);
    volatile u16 *pA = (u16 *)(GetFlashBase()+0x2AAAUL);
    volatile u16 *pF = (u16 *)(GetFlashBase());

    volatile u16 *pBurn;
    u16 page, pagelast;
    static unsigned long timeoutcount;
    int nWords = nBytes >> 1;

    if(nWords <= 0) {
        return;
    }

    /* Set paging and enable flash access */
    page     = GetPageNum(flashnext);
    pagelast = GetPageNum(flashnext + nWords - 1);

    FlashEnable();

    if(pagelast == page) {
        /* All words to be written are within the same page */
        pBurn      = GetPageAddr(flashnext);
        flashnext += nWords; /* For next time */

        SetPage(0);
        *p5 = 0xaa;
        *pA = 0x55;
        *p5 = 0x20;

        while(nWords-- > 0) {
            timeoutcount = 0;

            SetPage(0);
            *pF = 0xa0;

            SetPage(page);
            *pBurn = *data;

            while (*pBurn != *data) {
                timeoutcount++;
            }

            pBurn++;
            data++;
        }

        /* Close burning session */
        *pF = 0x90;
        *pF = 0x00;

    } else {
        /* Programming crosses at least one page boundary */
        /* In our project we expect to not cross the chosen page boundary */
        while(nWords-- > 0) {
            timeoutcount = 0;

            SetPage(0);
            *pF = 0xa0;

            SetPage(page);
            *pBurn = *data;

            while (*pBurn != *data) {
                timeoutcount++;
            }

            pBurn++;
            data++;
            flashnext++;
        }
    }

    SetPage(0);
    FlashDisable();
}

/* ------------------
 * Checksum Functions
 * ------------------
 */
void CKSSet (u16 val) {
    cksum = val;
}

u16 CKSGet (void) {
    return ((u16)cksum);
}

/* bufptr is start address in flash memory, len is length in 8-bit Bytes! */
u16 CKSAccumBuf (unsigned long bufptr, unsigned long len) {

    u16   page;
    u16 * pPage;
    long  n;

    len /= BYTESPERMAU;  // MAUs

    bufptr &= 0x0000FFFF;

    while (len > 0) {
        /* How many bytes can I read from this page? */
        page = GetPageNum(bufptr);
        n = GetPageMAURemaining(bufptr);
        if (n > len) {
            n = len;
        }
        len -= n;    // for next time thru

        /* Set paging and enable flash access */
        pPage = GetPageAddr(bufptr);
        bufptr += n;

        SetPage(page);

        /* Accumulate bytes into the checksum  */
        while (n-- > 0) {
            unsigned long val = *pPage++;
            cksum += val >> 8;          //MSB
            if (cksum >= 65536) {
                cksum += 1;
                cksum &= 0xffff;
            }

            cksum += val & 0x000000ff;  // LSB

            if(cksum >= 65536) {
                cksum += 1;
                cksum &= 0xffff;
            }
        }

        SetPage(0);
    }

    return ((u16)cksum);
}

u16 GetFlashVal (unsigned long addr) {
    u16 myval;

    SetPage(GetPageNum(addr));
    myval = *GetPageAddr(addr);

    return myval;
}

void SendFlashBufToHost (u16 cmd, unsigned long addr, u16 bytecount) {
    u16 *pagememptr;
    u16 pagestart;
    u16 pagelast;
    u16 i;
    u16 n;

    pagestart = GetPageNum(addr);
    pagelast  = GetPageNum(addr + bytecount / BYTESPERMAU - 1);

    /* Prepare the response message header */
    theMessage[CMDINDEX]   = cmd;
    theMessage[ARGINDEX+0] = bytecount;

    if(pagestart == pagelast) {
        /* All desired data are in same page. */
        SetPage(pagestart);
        pagememptr = GetPageAddr(addr);
        n = bytecount / 2;
        for(i = 0; i < n; i++) {
            theMessage[DATAINDEX+i] = *pagememptr++;
        }
    } else {
        /* Desired data cross at least one page boundary */
        u16 len = bytecount / 2;
        u16 n;
        u16 *dataptr;

        dataptr = &theMessage[DATAINDEX];

        while(len > 0) {
            /* How many bytes can I read from this page? */
            pagestart = GetPageNum(addr);
            n         = GetPageMAURemaining(addr);

            if(n > len) {
                n = len;
            }

            len -= n;    // for next time thru

            /* Set paging and enable flash access */
            pagememptr = GetPageAddr(addr);
            addr      += n;  // also for next time thru

            SetPage(pagestart);
            while(n-- > 0) {
                *dataptr++ = *pagememptr++;
            }
        }
    }

    SetPage(0);
}
