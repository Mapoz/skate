/*********************************************************************
 *          (C) COPYRIGHT TEXAS INSTRUMENTS, INC. 2000                *
 * FlashBurn 54x Target Code header
 * for main module. Includes the
 * Host/Target protocol interfaces.
 */

/* Change Log:
 */
/*
$Log: /FlashBurn/PR223_EF/AreaLoader/fbtc54.h $
 * 
 * 2     12/28/05 12:11p Slvmapo
 * Changed Version ID, now is 1.00010
 * 
 * 1     7/01/05 12:42p Slvmapo
 * 
 * 2     5/30/05 10:40a Slvmapo
 * 
 * 1     4/15/05 12:25p Slvmapo
 * 
 * 1     4/15/05 12:23p Slvmapo
Revision 1.8  2001/04/19 18:56:34  heeschen
v00.30 Alpha - Updated comments

Revision 1.7  2001/04/19 16:29:01  heeschen
v00.30 Snapshot
Saving this stuff after massive redesign to remove RTDX dependency.
Next step is to clean up, fix comments, and do unit testing.

Revision 1.6  2001/03/21 16:05:34  heeschen
v00.21 Alpha
Updates to expand RTDX buffer, plan to release this soon.
Fixed Hex file converter bugs, added comments.

Revision 1.5  2001/03/14 19:43:05  heeschen
v00.21 Alpha Again
Added flash memory edit capability. Required up-rev
of Comm. Protocol and FBTCs to add a Query Edit
Permission command.
New flashburn.hlp and flashburn.cnt files, too

Revision 1.4  2000/12/07 19:57:25  heeschen
v0.00002 of the FBTC54 program.
Changes made as a result of code review 12/05/00.
Doc. CR_12_05_00.doc is the minutes of the review.
Biggest change was to the RTDX comm. method, and
changing several macros into functions.

Revision 1.3  2000/11/14 19:56:20  heeschen
FlashBurn v0.00.1 -- Preparing for testing & alpha release. Added
a Get FBTC Version ID command to the comm. protocol.

Revision 1.2  2000/11/09 20:27:59  heeschen
Reorganized: moved all RTDX and message parsing funcs into
FBTC54.c.
Added processors for Get Flash Size, Set Flash Base Addr.,
and Set Flash Size commands.
Added some comments.

Revision 1.1  2000/10/20 19:56:37  heeschen
Initial Version

*/

#ifndef __fbtch__
#define __fbtch__

#include <type.h>   /* Mainly for u16 type */

/****************************************/
/* Version ID of this FBTC.             */
/* Major.Minor 0.0 to 65535.65535       */
/* (thus if major = 1, minor = 20, then */
/* version is 0001.0020)                */
/*                                      */
/* MUST BE EDITED FOR ANY NEW RELEASE!  */
/****************************************/
#define FBTCVersionMajor   0x0001
#define FBTCVersionMinor   0x000A

/* Comm. Protocol Version in force when this FBTC was created. Add 1 if the protocol */
/* changes, even if this software did not change. For Protocol Version ID, see the   */
/* Exchange Communications Protocol document.                                        */
/* This is in the C file instead of the header file 'cause the header file is used   */
/* by other C files, which could compromise encapsulation.                           */
/* v1 - original version                                                             */
/* v2 - added the Query Edit Flash Permission command                                */
/*      and expanded msg data buffer to 768 8-bit bytes.                             */
/* v3 - added the Query Max Data Buffer Bytes command.                               */
/*      and revised the comm. scheme to eliminate RTDX.                              */
#define COMMPROTOCOLVERSION  3

/* This is the response to send when host queries for Edit Flash Permission.  */
/* Set this to 0 or 1 to indicate whether this FBTC allows editing individual */
/* flash memory locations.                                                    */
#define EDITFLASHPERMIT  1

/* Message data block length, in bytes.                           */
/* Use BYTESPERMAU to convert from bytes to Memory address units. */
/* On a 54x / 55x DSP, BYTESPERMAU is 2                           */
/* On a 62x / 67x DSP, BYTESPERMAU is 1                           */
#define BYTESPERMAU                 2
#define ARGSIZEBYTES               30
#define ARGSIZEWORDS (ARGSIZEBYTES/2)
/* This value is customizable up to 32768 but it could be harmful if we exceed the  */
/* internal RAM address space, in our case 16Kw, a little value has only the effect */
/* to partition burning phase into more blocks, but time is shorter in any case,    */
/* With 4096 the programming phase take only 2.1 sec.                               */
#define MAXDATABYTES             4096

#define CMDINDEX  0
#define ARGINDEX  1
#define DATAINDEX (1+ARGSIZEWORDS)

/* Predefined Status Codes for returning to Host processor */
#define CMDOK              0x0
#define UNEXP_DATA_LENGTH  0x1
#define ILL_DATA_VAL       0x2
#define CMD_NOT_DONE       0x3

/* Returns the current Communications Protocol status word */
u16 GetProtocolStatusWord(void);

/* Returns the Host Communications Protocol Version ID (0 to 65535) which this FBTC */
/* was designed for. Host uses this to determine backward compatibility             */
u16 GetProtocolVersionID(void);

/* Set the External Memory to Flash or SRAM */
void FlashEnable(void);
void FlashDisable(void);

/* Handy func to set the 4 status bits of the Program Status Word */
void SetErrorBits(u16 x);

/* Send a long value to Host, formats a response message and sends it */
void SendI4ToHost(u16 cmd, long val);

/* Send a u16 value to Host, formats a response message and sends it */
void SendI2ToHost(u16 cmd, u16 val);

#endif


