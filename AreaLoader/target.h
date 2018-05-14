#ifndef __TARGET_H
#define __TARGET_H

/***********************************************************************
* Copyright (c) 2000 Texas Instruments Incorporated
*
* - Target specific inititlaization details
*
************************************************************************/
/*
 * $Log: /FlashBurn/PR223_EF/AreaLoader/target.h $
 * 
 * 1     7/01/05 12:42p Slvmapo
 * 
 * 1     4/15/05 12:25p Slvmapo
 * 
 * 1     4/15/05 12:23p Slvmapo
 * Revision 1.3  2001/04/19 16:29:01  heeschen
 * v00.30 Snapshot
 * Saving this stuff after massive redesign to remove RTDX dependency.
 * Next step is to clean up, fix comments, and do unit testing.
 *
 */

#define TARGET_INITIALIZE()

/* Set wait states for external Flash memory */
#define FLASH_WAITS() {extern void dspInitFl(void); dspInitFl(); }

#endif // __TARGET_H
