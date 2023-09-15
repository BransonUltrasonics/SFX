/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/Vectors.c_v   1.2.3.0   15 Dec 2015 16:15:04   hasanchez  $
*/
/********************************************************************************* 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


----------------------------------- MODULE DESCRIPTION ---------------------------


Module name: VECTORS

Filename:    Vectors.c
---------------------------------- TECHNICAL NOTES -------------------------------

This code contains initialization of the CPU and provides basic methods 
and events for CPU core settings.

-------------------------------------- REVISIONS ---------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/Vectors.c_v  $
 * 
 *    Rev 1.2.3.0   15 Dec 2015 16:15:04   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.0   06 Aug 2012 05:32:14   akaushal
 * Initial revision.
 * 
 *    Rev 1.0   22 May 2012 12:02:08   ygupta
 * Initial revision.
 ---------------------------------------------------------------------------------*/


#include "Cpu.h"
#include "onems.h"
extern near void _EntryPoint(void);

void (* near const _vect[])(void) @0xFFCA = { /* Interrupt vector table */
         Cpu_Interrupt,                /* Int.no. 26 Vrtc (at FFCA)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 25 Vmtim (at FFCC)                 Unassigned */
         TI_Interrupt,                 /* Int.no. 24 Vtpm2ovf (at FFCE)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 23 Vtpm2ch5 (at FFD0)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 22 Vtpm2ch4 (at FFD2)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 21 Vtpm2ch3 (at FFD4)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 20 Vtpm2ch2 (at FFD6)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 19 Vtpm2ch1 (at FFD8)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 18 Vtpm2ch0 (at FFDA)              Unassigned */
         ADC_Interrupt,                /* Int.no. 17 Vadc (at FFDC)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 16 Vkeyboard (at FFDE)             Unassigned */
         Cpu_Interrupt,                /* Int.no. 15 Viic (at FFE0)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 14 Vsci2tx (at FFE2)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 13 Vsci2rx (at FFE4)               Unassigned */
         Cpu_Interrupt,                /* Int.no. 12 Vsci2err (at FFE6)              Unassigned */
         Cpu_Interrupt,                /* Int.no. 11 Vspi (at FFE8)                  Unassigned */
         Cpu_Interrupt,                /* Int.no. 10 Vlcd (at FFEA)                  Unassigned */
         Cpu_Interrupt,                /* Int.no.  9 Vsci1tx (at FFEC)               Unassigned */
         Cpu_Interrupt,                /* Int.no.  8 Vsci1rx (at FFEE)               Unassigned */
         Cpu_Interrupt,                /* Int.no.  7 Vsci1err (at FFF0)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  6 Vtpm1ovf (at FFF2)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  5 Vtpm1ch1 (at FFF4)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  4 Vtpm1ch0 (at FFF6)              Unassigned */
         Cpu_Interrupt,                /* Int.no.  3 Vlvw (at FFF8)                  Unassigned */
         Cpu_Interrupt,                /* Int.no.  2 Virq (at FFFA)                  Unassigned */
         Cpu_Interrupt,                /* Int.no.  1 Vswi (at FFFC)                  Unassigned */
         _EntryPoint                   /* Int.no.  0 Vreset (at FFFE)                Reset vector */
};
