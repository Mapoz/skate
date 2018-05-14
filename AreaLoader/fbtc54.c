/********************************************************************
 *          (C) COPYRIGHT TEXAS INSTRUMENTS, INC. 2000, 2001        *
 * FBTC54.c -- Main Loop and Utility funcs for 54x FlashBurn FBTC   *
 ********************************************************************/

/* Change Log: */
/*
 * $Log: /FlashBurn/PR223_EF/AreaLoader/fbtc54.c $
 * 
 * 1     7/01/05 12:42p Slvmapo
 * 
 * 1     4/15/05 12:25p Slvmapo
 * 
 * 1     4/15/05 12:23p Slvmapo
 * Revision 1.10  2001/06/27 17:55:12  heeschen
 * v00.32 Beta Prep
 * Capturing all files to prepare for Beta Release
 *
 * Revision 1.9  2001/04/19 18:56:34  he  eschen
 * v00.30 Alpha - Updated comments
 *
 * Revision 1.8  2001/04/19 16:29:01  heeschen
 * v00.30 Snapshot
 * Saving this stuff after massive re design RTDX dependency.
 * Next step is to clean up, fix comments, and do unit testing.
 *
*/

#include "target.h"     /* defines TARGET_INITIALIZE() */
#include <stdio.h>      /* C_I/O                       */
#include <board.h>
#include "fbtc54.h"
#include "fbcmd.h"

/* Local Prototypes */

/* -------------
 * Globals
 * -------------
 */
/* The message must be global; Host will use
 * it for sending and receiving messages.
 * This program reads what's in the message block
 * and processes it. When done processing,
 * this program will store any response data back
 * into the same block.
 */
u16 theMessage[1+ARGSIZEWORDS+MAXDATABYTES/2] = {0}; // the latest message from Host

/* This function must be global; Host will
 * set a breakpoint on it.
 * When this program hits the breakpoint,
 * host will detect the halt and process
 * any response info that may be in the message
 * block. When Host wants to send a message
 * to this FBTC, it will store
 * the message into the global message block
 * and start this FBTC running.
 */
void doMessageProc();  // Message Exchange routine

/* -------------
 * Local Variables & funcs
 * -------------
 */
#define FLASHE_BIT  0x020

/* This is the Status Word described
 * in the Comm. Protocol Spec.
 */
static u16 m_ProtocolStatusWord = 0;

int main (void) {

    TARGET_INITIALIZE();

    FlashEnable();

    FLASH_WAITS();

    SetPage(0);

    InitFlash();

    /* Runs forever */
    for(;;) {
        /* After FB load CCS, running in background, place a breakpoint here. */
        /* When this returns, I have a new message.                           */
        doMessageProc();

        /* Process the new message */
        doCommand(theMessage);
    }
}

/* Global Utility Functions */

/* This do-nothing function is here to
 * define a global symbol. The Host app.
 * will set a breakpoint at this loc.
 * When this program steps off the
 * breakpoint (returns from this func),
 * that means that the Host has filled
 * the global message block (at 'theMessage')
 * with a command and data to process.
 * This program will process the message,
 * write any response data back into
 * the message block, and then come back
 * here and hit the breakpoint.
 */
void doMessageProc (void) {
    /* Optimizer will want to delete
     * this function, since it doesn't
     * appear to do anything useful.
     * Assigning a value to a volatile
     * forces optimizer to keep this code.
     */
    volatile int FoolTheOptimizer = 0;
}

u16 GetProtocolVersionID (void) {
    return ((u16)COMMPROTOCOLVERSION);
}

u16 GetProtocolStatusWord (void) {
    return (m_ProtocolStatusWord);
}

/* Set the External Memory to Flash or SRAM */
void FlashEnable()
{
    port4 |= FLASHE_BIT;
}

void FlashDisable()
{
    port4 &= ~FLASHE_BIT;
}

void SendI4ToHost (u16 cmd, long val) {
    theMessage[CMDINDEX]   = cmd;
    theMessage[ARGINDEX+0] = (u16)(val >> 16);
    theMessage[ARGINDEX+1] = (u16)(val & 0x0000ffff);
}

void SendI2ToHost (u16 cmd, u16 val) {
    theMessage[CMDINDEX]   = cmd;
    theMessage[ARGINDEX+0] = val;
}

/* Utility Status Word functions */

/* Set the 4 status bits */
void SetErrorBits (u16 x) {
    m_ProtocolStatusWord &= ~(0x000f);
    m_ProtocolStatusWord |= ((x) & 0x000f);
}
