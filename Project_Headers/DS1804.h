/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/DS1804.h_v   1.4.1.1.3.0   15 Dec 2015 16:14:56   hasanchez  $
*********************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


------------------------------------ MODULE DESCRIPTION ---------------------------

This module deals with Digital Potentiometer connected to MC9S08LG32.
Sets the Digital Potentiometer to a default value at startup.

Module name: DIGITAL POTENTIOMETER

Filename:    DS1804.c
----------------------------------- TECHNICAL NOTES -------------------------------

This code contains the module for digital potentiometer control.

--------------------------------------- REVISIONS ---------------------------------

* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/DS1804.h_v  $
 * 
 *    Rev 1.4.1.1.3.0   15 Dec 2015 16:14:56   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.4   08 Jul 2013 04:45:44   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, changed frequency offset acc to the rquirement.
 * 
 *    Rev 1.3   25 Mar 2013 07:59:22   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.2   06 Nov 2012 08:41:26   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   16 Aug 2012 01:51:08   akaushal
 * SFX Code With External Crystal Oscillator and H/W Changes
 * 
 *    Rev 1.0   06 Aug 2012 05:26:32   akaushal
 * Initial revision.
 * 
 *    Rev 1.2   27 Jun 2012 07:48:00   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.1   21 Jun 2012 07:31:08   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.0   22 May 2012 11:59:48   ygupta
 * Initial revision.

-----------------------------------------------------------------------------------*/
#ifndef __DS1804_H
  #define __DS1804_H
  
//included header file

#include "onems.h"
#include "portable.h"
#include "print.h"
#include "cpu.h"
#include "eeprom.h"
#include "lcd.h"

#define INC PTFD_PTFD5  
#define DIR PTFD_PTFD1
#define CS  PTHD_PTHD4

#define DINC PTFDD_PTFDD5
#define DDIR PTFDD_PTFDD1
#define DCS  PTHDD_PTHDD4


#define MID_DIGIPOT	50
#define INCREMENT 1
#define DECREMENT 0


#define DIGIPOTSTEP_20KHz 8    // Digipot step 1 is equal to 8Hz for 20KHZ
#define DIGIPOTSTEP_30KHz 12   // Digipot step 1 is equal to 12Hz for 30KHZ 
#define DIGIPOTSTEP_40KHz 16   // Digipot step 1 is equal to 16Hz for 40KHZ

#define DIGIPOT_MIN_20KHz -400    
#define DIGIPOT_MIN_30KHz -600  
#define DIGIPOT_MIN_40KHz -800   
#define DIGIPOT_MAX_20KHz  400   
#define DIGIPOT_MAX_30KHz  600   
#define DIGIPOT_MAX_40KHz  800  

#define DIGIPOT_MAX_VALUE_TENS_PLACE_20KHZ 4
#define DIGIPOT_MAX_VALUE_TENS_PLACE_30KHZ 6
#define DIGIPOT_MAX_VALUE_TENS_PLACE_40KHZ 8

#define ASCII(x) ((UINT8)(48 + x))

//enum PSFREQUENCY { FREQ20KHz =20 , FREQ30KHz = 30, FREQ40KHz = 40 };
//Functions
void Init_Port_DS1804(void); 
void ColdStart_DS1804(void);
void DigiPot_Adjust(void);
void CenterDigiPot(void);
void increment(UINT8);
void decrement(UINT8);

//variables

#endif


