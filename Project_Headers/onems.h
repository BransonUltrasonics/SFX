/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/onems.h_v   1.6.1.3.3.3   14 Jul 2016 12:50:00   hasanchez  $
*/
/************************************************************************************ 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION --------------------------------------

This is the module for controlling all the Timing Related Tasks suing Hardware Times

Module name: TIMER

Filename:    onems.h
--------------------------- TECHNICAL NOTES -----------------------------------------

This Header contains the declations and Constant Macros for timer modules


------------------------------ REVISIONS --------------------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/onems.h_v  $
 * 
 *    Rev 1.6.1.3.3.3   14 Jul 2016 12:50:00   hasanchez
 * Updated the data type of HalfSec to BOOLEAN.
 * 
 *    Rev 1.6.1.3.3.2   18 May 2016 13:29:24   hasanchez
 * Updated OneSec Flag to HalfSec.
 * 
 *    Rev 1.6.1.3.3.1   13 May 2016 13:49:38   hasanchez
 * updated KB_SCAN_TIME from 5 to 3 ms.
 * 
 *    Rev 1.6.1.3.3.0   15 Dec 2015 16:14:58   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.6.1.0   25 Aug 2015 12:35:30   hasanchez
 * Added definition of key presses limits, and update the I2C_DELAY_ENABLED to 6ms according the specification on the eeprom.
 * 
 *    Rev 1.6   10 Sep 2013 03:26:20   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.5   08 Jul 2013 04:45:20   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, changed frequency offset acc to the rquirement.
 * 
 *    Rev 1.4   13 Jun 2013 08:00:24   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.3   25 Mar 2013 08:00:04   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.2   03 Dec 2012 08:54:58   akaushal
 * Navigation and Screens Updated
 * 
 *    Rev 1.1   06 Nov 2012 08:42:28   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.0   06 Aug 2012 05:27:20   akaushal
 * Initial revision.
 * 
 *    Rev 1.4   31 Jul 2012 04:00:42   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.3   27 Jun 2012 07:50:22   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:33:36   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:46:40   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:44   ygupta
 * Initial revision.
-------------------------------------------------------------------------------------*/

#ifndef _ONEMS_H
#define _ONEMS_H

/*------------------------------ INCLUDES ---------------------------------*/

#include "portable.h"


/*--------------------------------- DEFINES -------------------------------*/


#define BACKLIGHT_TIMEOUT     15             // 15 mins
#define KB_SCAN_TIME          3
#define DISPLAYUPDATE         99
#define BLINK_TIME            500
#define MILLISEC_IN_HALF_SEC  500L
#define MILLISEC_IN_ONE_SEC   1000L
#define MILLISEC_IN_TWOSECOND 2000L
#define MILLISEC_IN_TENMINUT  600000L
#define I2C_DELAY_ENABLED     0x6  //According the datasheet Write Cycle Time is 5ms maximum we left 1 ms extra.
#define DEBOUNCE_TIME         5
#define ONESEC                1
#define INPUT_PIN             PTHD_PTHD7
#define OUTPUT_PIN            PTGD_PTGD1

#define MAX_TIME_IN_SEC       359999
#define MAX_TIME_IN_MS        3599999        //59min99sec990ms   

#define KEY_REPEAT_TIMER		500
#define KEY_HOLD_TIMER1			750
#define KEY_HOLD_TIMER2			500
#define TIME_INCREMENT_LOCKED   1000

extern UINT8 KbScanTimer;
extern BOOLEAN HalfSec;
extern UINT8 backlightTimeout;
extern UINT16 Current_Energy;


extern volatile UINT8	FlagTimer1;     
extern volatile UINT8	FlagTimer2;
extern volatile UINT8	Flag_RTotalTime;
extern volatile UINT8	Flag_RTotalOnTime;
extern volatile UINT8	FlagTime;
extern volatile UINT8	FlagTimeOut;
extern volatile UINT8	FlagTotalOnTime;
extern UINT8 FlagSwitch;
extern volatile UINT8 KeyBoardScanFlag; 

extern UINT8 IOMirror;

extern UINT32  TimerValue1;
extern UINT32  TimerValue2;
extern UINT32  Timer_RTotalTime;
extern UINT32  Timer_RTotalOnTime;
extern UINT32  TimerTime;
extern UINT32  TimerTimeOut;
extern UINT32  TimerTotalOnTime;
extern UINT16 Current_Energy;

typedef enum Timers
{ 
	TIMER1 = 1,
	TIMER2,
	RTOTALTIME,
	RTOTALONTIME,
	TIMER_TIME,
	TIMER_TIMEOUT,
	TIMER_TOTALONTIME
} TIMERS;

  
  
/*-------------------------- FUNCTION PROTOTYPES ----------------------------*/

void   CheckTimers(void);
void   StopAllTimers(void);
void   StartMSTimer(TIMERS Index, UINT32 Time);
void   StopMSTimer(TIMERS Index);
void   PauseMSTimer(TIMERS Index);
void   ResumeMSTimer(TIMERS Index);
void   StartSECTimer(TIMERS Index, UINT32 Time);
void   StartMINTimer(TIMERS Index, UINT16 Time);
UINT32 ReadSECTimer(TIMERS Index) ;
UINT16 ReadMINTimer(TIMERS Index) ;
void   PauseSECTimer(TIMERS Index);
void   ResumeSECTimer(TIMERS Index);
void   StopSECTimer(TIMERS Index);
/*------------------------ Function Prototypes -----------------------------*/

__interrupt void TI_Interrupt(void);
void TI_Init(void);
void ResetBacklightTimer(void);
UINT32 GetVar(void * varPtr, UINT16 VarSize);
void SetVar(void * varPtr, UINT32 value ,SINT16 VarSize);
void DebounceInputs(void);
                          

#endif /* ONEMS_H */
