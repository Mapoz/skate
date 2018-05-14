/**********************************************************************
 *       (C) COPYRIGHT TEXAS INSTRUMENTS, INC. 2000, 2001                 *
 *                                                                    *
 * FBCmd.c -- Process FlashBurn Target Component commands             *
 *              for FBTC54.                                           *
 **********************************************************************
 */
/* Change Log:
 */
/*
 * $Log: /FlashBurn/PR223_EF/AreaLoader/fbcmd.c $
 * 
 * 1     7/01/05 12:42p Slvmapo
 * 
 * 1     4/15/05 12:25p Slvmapo
 * 
 * 1     4/15/05 12:23p Slvmapo
 * Revision 1.9  2001/04/19 18:56:32  heeschen
 * v00.30 Alpha - Updated comments
 *
 * Revision 1.8  2001/04/19 16:29:00  heeschen
 * v00.30 Snapshot
 * Saving this stuff after massive redesign to remove RTDX dependency.
 * Next step is to clean up, fix comments, and do unit testing.
 *
*/

#include "fbtc54.h"
#include "fbcmd.h"
#include <stdio.h>

#define DBPRINT 0

typedef enum {
    NO_OPERATION = 0,          // 0
    QUERY_PROTOCOL_VERSION,    // 1
    QUERY_STATUS,              // 2
    ERASE_FLASH,               // 3
    QUERY_FLASH_BASE_ADDR,     // 4
    READ_FLASH_WORD,           // 5
    READ_FLASH_BLOCK,          // 6
    WRITE_FLASH_FIRST,         // 7
    WRITE_FLASH_CONT,          // 8
    QUERY_CHECKSUM,            // 9
    ACCEPT_USER_DATA,          // 10
    QUERY_FLASH_SIZE,          // 11
    CHANGE_FLASHBASE_ADDR,     // 12
    CHANGE_FLASH_SIZE,         // 13
    QUERY_FBTC_VERSION,        // 14
    QUERY_FLASH_PERMISSION,    // 15
    QUERY_MAX_DATABLOCK_SIZE   // 16

} cmdType;

/* Send my version ID back to the Host */
static void SendVersionID (u16 *msg) {
    msg[CMDINDEX] = 0x000e;             /* Response to this command */
    msg[ARGINDEX+0] = FBTCVersionMajor; /* Arg0 */
    msg[ARGINDEX+1] = FBTCVersionMinor; /* Arg1 */
}

void doCommand (u16 *msg) {
    u16 cmd = msg[CMDINDEX];

#if DBPRINT == 1
    printf("CMD: %x", cmd);
#endif

    /* Dispatch to a command processor. Each case is essentially a message parser. */
    /* Each case will either completely process a simple message, or else call a   */
    /* function to handle a more complicated message.                              */
    switch(cmd) {
        case NO_OPERATION:
        case 0xFFFF:
            break;

        case QUERY_PROTOCOL_VERSION:
            SetErrorBits(CMDOK);
            SendI2ToHost(cmd, GetProtocolVersionID());
            break;

        case QUERY_STATUS:
        #if DBPRINT == 1
            printf("  Status: %x", GetProtocolStatusWord());
        #endif
            SendI2ToHost(cmd, GetProtocolStatusWord());
            break;

        case ERASE_FLASH:
            SetErrorBits(CMDOK);
            EraseFlash();
            SendI2ToHost(cmd, 0);
            break;

        case QUERY_FLASH_BASE_ADDR:
            SetErrorBits(CMDOK);
            SendI4ToHost(cmd, 0x00008000);
            break;

        case READ_FLASH_WORD:
        #if DBPRINT == 1
            printf(" ARGS: %x %x", msg[ARGINDEX+0], msg[ARGINDEX+1]);
        #endif
            SetErrorBits(CMDOK);
            SendI2ToHost(cmd, GetFlashVal(GetFlashBase() + ShortsToULong(msg[ARGINDEX+0], msg[ARGINDEX+1])));
            break;

        case READ_FLASH_BLOCK:
        #if DBPRINT == 1
            printf(" ARGS: %x %x %x", msg[ARGINDEX+0], msg[ARGINDEX+1], msg[ARGINDEX+2]);
        #endif
            SetErrorBits(CMDOK);
            SendFlashBufToHost(cmd, GetFlashBase() + ShortsToULong(msg[ARGINDEX+0], msg[ARGINDEX+1]), msg[ARGINDEX+2]);
            break;

        case WRITE_FLASH_FIRST:
        #if DBPRINT == 1
            printf(" ARGS: %x %x %x",  msg[ARGINDEX+0], msg[ARGINDEX+1], msg[ARGINDEX+2]);
        #endif
            SetFlashAddr(GetFlashBase() + ShortsToULong(msg[ARGINDEX+0], msg[ARGINDEX+1]));
            SetErrorBits(CMDOK);
            BurnFlash(&msg[DATAINDEX], msg[ARGINDEX+2]);

            break;

        case WRITE_FLASH_CONT:
            /* This code should never arrives because we handle flash burning in a single 16K block write */
        #if DBPRINT == 1
            printf(" ARG: %x",  msg[ARGINDEX+0]);
        #endif
            SetErrorBits(CMDOK);
            BurnFlash(&msg[DATAINDEX], msg[ARGINDEX+0]);
            break;

        case QUERY_CHECKSUM:
        #if DBPRINT == 1
            printf(" ARGS: %x %x %x %x",  msg[ARGINDEX+0], msg[ARGINDEX+1], msg[ARGINDEX+2], msg[ARGINDEX+3]);
        #endif
            SetErrorBits(CMDOK);
            CKSSet(0);
            SendI2ToHost(cmd,
                         CKSAccumBuf(GetFlashBase() + ShortsToULong(msg[ARGINDEX+0], msg[ARGINDEX+1]),
                         ShortsToULong(msg[ARGINDEX+2], msg[ARGINDEX+3]))
                        );
            break;

        case ACCEPT_USER_DATA:
            /* Simply accept & toss data.                                         */
            /* I use this for timing tests. It could contain your own proprietary */
            /* commands or data, sort of an escape sequence.                      */
        #if DBPRINT == 1
            printf(" ARG: %x",  msg[ARGINDEX+0]);
        #endif
            SetErrorBits(CMDOK);
            /* This command is used to erase only a specific flash sector, sector Id is in the first data field */
            // flashSectorErase(SA1);
            SendI2ToHost(cmd, 0);
            break;

        case QUERY_FLASH_SIZE:
            SetErrorBits(CMDOK);
            SendI4ToHost(cmd, GetFlashSize());
            break;

        case CHANGE_FLASHBASE_ADDR:
        #if DBPRINT == 1
            printf(" ARGS: %x %x", msg[ARGINDEX+0], msg[ARGINDEX+1]);
        #endif
            SetErrorBits(CMDOK);
            SetFlashBase(ShortsToULong(msg[ARGINDEX+0], msg[ARGINDEX+1]));
            break;

        case CHANGE_FLASH_SIZE:
        #if DBPRINT == 1
            printf(" ARGS: %x %x\n", msg[ARGINDEX+0], msg[ARGINDEX+1]);
        #endif
            SetErrorBits(CMDOK);
            SetFlashSize(ShortsToULong(msg[ARGINDEX+0], msg[ARGINDEX+1]));
            break;

        case QUERY_FBTC_VERSION:
            SetErrorBits(CMDOK);
            SendVersionID(msg);
            break;

        case QUERY_FLASH_PERMISSION:
            SetErrorBits(CMDOK);
            SendI2ToHost(cmd, EDITFLASHPERMIT);
            break;

        case QUERY_MAX_DATABLOCK_SIZE:
            SetErrorBits(CMDOK);
            SendI2ToHost(cmd, MAXDATABYTES);
            break;

        default:
        #if DBPRINT == 1
            printf(" *** UNKNOWN! ***");
        #endif
            break;
    }

#if DBPRINT == 1
    printf("\n");
#endif
}
