/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/Cpu.h_v   1.2.1.1.3.0   15 Dec 2015 16:14:56   hasanchez  $
************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION ----------------------------

This is the main module for controlling the CPU initialisation.

Module name: CPU

Filename:    Cpu.h
--------------------------- TECHNICAL NOTES -------------------------------

Contains the low level register configurations and kicks off ADC, LCD, 
and timer inits 

-------------------------------------- REVISIONS --------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/Cpu.h_v  $
 * 
 *    Rev 1.2.1.1.3.0   15 Dec 2015 16:14:56   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.2   06 Nov 2012 08:41:14   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   16 Aug 2012 01:50:58   akaushal
 * SFX Code With External Crystal Oscillator and H/W Changes
 * 
 *    Rev 1.0   06 Aug 2012 05:26:08   akaushal
 * Initial revision.
 * 
 *    Rev 1.3   06 Jul 2012 10:02:20   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.2   21 Jun 2012 07:30:28   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:44:14   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 11:59:22   ygupta
 * Initial revision.

---------------------------------------------------------------------------*/

#ifndef _Cpu
#define _Cpu

/* Active configuration define symbol */
#define PEcfg_Debug_08LG32CLH 1


/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "MC9S08LG32.h"
#include "DS1804.h"  
#include "p_output.h"
/* MODULE Cpu. */

#define FeedTheDog() (SRS = 0x00)

#define CPU_BUS_CLK_HZ              0x00400000UL /* Initial value of the bus clock frequency in Hz */
#define CPU_INSTR_CLK_HZ            0x00400000UL /* Initial value of the instruction clock frequency in Hz */
#define CPU_INT_CLK_HZ              0x8000UL /* Value of the internal oscillator clock frequency in Hz */
#define CPU_TICK_NS                 0x7736U
#define CPU_CORE_HCS08                 /* Specification of the core type of the selected cpu */
#define CPU_DERIVATIVE_MC9S08LG32      /* Name of the selected cpu derivative */
#define CPU_PARTNUM_MC9S08LG32CLH      /* Part number of the selected cpu */

#define ADC_POWERCH 14
#define ADC_TEMPECH  12
#define ADCTEMPECOUNT 10 //10ms running average for temperature readings.
#define ADCPOWERCOUNT 3 // ignore high and low value out of last 3 ms power readings.

/* Global variables */
extern volatile byte CCR_reg;          /* Current CCR register */

extern UINT16 ADTempe[ADCTEMPECOUNT];
extern UINT16 ADPower[ADCPOWERCOUNT];
extern UINT16 DataHandle;

__interrupt void Cpu_Interrupt(void);
__interrupt void ADC_Interrupt(void);
#define   Cpu_DisableInt()  __DI()     /* Disable interrupts */
#define   Cpu_EnableInt()  __EI()      /* Enable interrupts */

void LowLevelInit(void);
void ADC_Init(void);
void StartADCConversion(void);

#endif /* ifndef __Cpu */
