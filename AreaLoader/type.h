/* type.h
 *
 * Copyright (c) 2000 Texas Instruments Incorporated
 *
 *  Originally created by DNA Software,
 *  Modified by Russ Heeschen for the FlashBurn
 *  project.
 */

/*
 * $Log: /FlashBurn/PR223_EF/AreaLoader/type.h $
 * 
 * 1     7/01/05 12:42p Slvmapo
 * 
 * 1     4/15/05 12:25p Slvmapo
 * 
 * 1     4/15/05 12:23p Slvmapo
 * Revision 1.2  2000/12/07 19:57:25  heeschen
 * v0.00002 of the FBTC54 program.
 * Changes made as a result of code review 12/05/00.
 * Doc. CR_12_05_00.doc is the minutes of the review.
 * Biggest change was to the RTDX comm. method, and
 * changing several macros into functions.
 *
 */

#if !defined( TYPE__H )
#define       TYPE__H

typedef enum
{
   FALSE = 0,
   TRUE  = !FALSE
} BOOL;

typedef float          f32;

typedef long           s32;
typedef unsigned long  u32;

typedef short          s16;
typedef unsigned short u16;

#endif

