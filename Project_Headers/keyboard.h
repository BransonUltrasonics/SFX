/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/keyboard.h_v   1.3.1.3.3.0   15 Dec 2015 16:14:56   hasanchez  $
***********************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION ------------------------------------

This is the Keyboard module for controlling the Matrix Keypad Connected.

Module name: KEYBOARD

Filename:    Keyboard.h
--------------------------- TECHNICAL NOTES ---------------------------------------

This Header contains the declations and Constant Macros for Matrix Keypad Module

------------------------------ REVISIONS ------------------------------------------

* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/keyboard.h_v  $
 * 
 *    Rev 1.3.1.3.3.0   15 Dec 2015 16:14:56   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.3.1.0   25 Aug 2015 12:27:02   hasanchez
 * TEST_N_RESET  key combination is added 
 * 
 *    Rev 1.3   20 Nov 2012 07:27:48   akaushal
 * Lcd Flickering Fixed and Navigation Updated
 * 
 *    Rev 1.2   06 Nov 2012 08:42:04   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   10 Aug 2012 02:58:00   akaushal
 * Corrected Microtip and Test Key on Keypad
 * 
 *    Rev 1.0   06 Aug 2012 05:26:54   akaushal
 * Initial revision.
 * 
 *    Rev 1.6   31 Jul 2012 03:59:54   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.5   09 Jul 2012 03:26:08   akaushal
 * Keypad code updated
 * 
 *    Rev 1.4   06 Jul 2012 10:03:08   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.3   27 Jun 2012 07:49:06   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:32:24   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:45:32   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:16   ygupta
 * Initial revision.

-----------------------------------------------------------------------------------*/
#ifndef _KEYBOARD_H
   #define _KEYBOARD_H

/*------------------------------ INCLUDE FILES -------------------------------*/

#include "portable.h"
#include "string.h"
#include "eeprom.h"
#include "LCD.h"
/*--------------------------------- DEFINES ----------------------------------*/


#define NO_KEYS_PRESSED       0x0000 
#define ALL_KEYS_PRESSED      0x7FFF 

#define COL0_REG              0x08  //Column Bits in PTAD register
#define COL1_REG              0x04
#define COL2_REG              0x02
#define COL3_REG              0x01
#define COL_RESET             0x7F  //Reset value has COL 0 as low


#define TIME_MODE    1
#define ENERGY_MODE  2
#define TEMPE_MODE   3

#define MICROTIPLIMIT 70            //70% amplitude
#define RUNSCREEN_READY       1
#define RUNSCREEN_PARAMETER   2
/*---------------------------- TYPE DECLARATIONS -----------------------------*/
typedef enum { 
   KB_START_STOP        = BIT0,
   KB_TEST              = BIT1,   
   KB_RIGHT             = BIT2,
   KB_PAUSE             = BIT3,
   KB_ESC               = BIT4,
   KB_LEFT              = BIT5,
   KB_RESET             = BIT6,
   KB_PRESET            = BIT7,
   KB_DOWN              = BIT8,
   KB_MICROTIP          = BIT9,
   KB_ENTER             = BIT10,
   KB_UP                = BIT11,
   EXTSTART             = BIT12,  
   EXTRESET             = BIT13,    
   PS_SEEK_ACT          = BIT14,                     
   ALL_KEYS             = ALL_KEYS_PRESSED,                                         
   NO_KEY               = NO_KEYS_PRESSED,
   STARTSTOP_N_EXTSTART = (BIT0 | BIT12), /* KB KB_START_STOP and EXTSTART pressed at the same time*/
   TEST_N_RESET         = (BIT1 | BIT6),
}KEY_CODE;

  
/*------------------------------ EXTERNAL DATA -------------------------------*/
extern UINT16 KeyMask;
extern UINT16 KeysPressed;
extern UINT8 PresetNo;
extern UINT32 FixedIcons;
extern UINT32 BlinkIcons;
//extern UINT8 MicrotipFlag;
extern UINT8 TempSense;
extern KEY_CODE keys;
extern UINT8 Digit1Value;
extern UINT8 Digit2Value;
extern UINT8 Digit3Value;
extern UINT8 Digit4Value;
extern UINT8 Digit5Value;
extern UINT8 Digit6Value;
extern UINT8 GCHAR_CODE[95];
extern UINT8 CurrDigiPot;
extern UINT16 CurrentTestCode;
extern UINT8 OldScreen;
extern BOOLEAN KeyHold;
extern BOOLEAN KeyUpDown;
extern UINT8 KeyRepeat;
extern UINT8 ReleaseFlag;

/*--------------------------- FUNCTION PROTOTYPES ----------------------------*/

void KeyScan(UINT8 ScanNum);
KEY_CODE ReadKeyboard(void);
void ProcessKeyboard(KEY_CODE keyCode);
//void PreviousScreen(void); 
KEY_CODE GetKeys(void);
void TestCode(void);
void DisplayLastWeldDataRight(void);
void HandleMICROTIP(void);
void HandleHH_MICROTIP(void);
#endif /* KEYBOARD_H  */
