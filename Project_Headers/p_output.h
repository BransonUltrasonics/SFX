/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/p_output.h_v   1.4.3.0   15 Dec 2015 16:14:58   hasanchez  $   
*/
/****************************************************************************/ 
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 1996-2007            */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------

This module is to Test the digital outputs

Module name: OUTPUT

Filename:    p_output.h  
  
----------------------------- TECHNICAL NOTES -------------------------------
This file contains macro definitions and enumerated data types for P_outputs  
  
---------------------------------- REVISIONS --------------------------------
 $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/p_output.h_v  $ 
 * 
 *    Rev 1.4.3.0   15 Dec 2015 16:14:58   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.1   10 Sep 2013 03:26:32   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.0   06 Aug 2012 05:27:58   akaushal
 * Initial revision.
 * 
 *    Rev 1.2   27 Jun 2012 07:51:02   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.1   12 Jun 2012 09:47:18   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:01:20   ygupta
 * Initial revision.
  
       
----------------------------- INCLUDE FILES --------------------------------*/

#ifndef P_OUTPUT_H
#define P_OUTPUT_H
#include "portable.h"
#include "lcd.h"
/* ------------------------- TYPE DECLARATIONS -----------------------------*/

typedef enum {            
    USER_OUT = BIT0,
    READY_OUT = BIT1,
    ALARM_OUT = BIT2,
    RUN_OUT   = BIT3,
    OL_RESET_OUT = BIT4,
    TEST_OUT = BIT5,
    SEEK_OUT = BIT6,
    BUZZER_OUT = BIT7
	
} OUTPUT_CODE;


enum BEEPS { /*ALARMBEEP,*/ ERRORBEEP, RELEASEBEEP, STARTUPBEEP, ALLBEEP };

#define DEFAULTBEEPTIME    250      /* ON time for alarm & error beep (ms)   */
#define RELEASEBEEPTIME    350      /* ON time for release beep (ms)         */
#define BEEPOFFTIME        250      /* OFF time for all beepers (ms)         */
#define ENDOFWELDOFFTIME   250		/* OFF time for EndOfWeld signal (ms)         */
#define BACKLIGHT_PIN   PTHD_PTHD6
#define PORTE_DEFAULTOUT 0x01/*FC*/;




/*---------------------------- GLOBALS ------------------------------------*/

extern   UINT8  Digit1Value;
extern   UINT8  Digit2Value;
extern   UINT8  Digit3Value;
extern   UINT8  Digit4Value;
extern UINT8  Ready;

/*------------------------ FUNCTION PROTOTYPES -----------------------------*/

void SetPSRun(void);
void PSIdle(void);
void PSTest(void);
void PSReset(void);
void PSSeek(void);
void RemovePSSeek(void);
void SetReady(void);
void ClrReady(void);
void SetAlarm(void);
void ClrAlarm(void);
void ClrUserOut(void);
void SetUserOut(void);
void SetAmplitude(UINT8 Percent);
void SetBeep(void);
void ClrBeep(void);
void Beep(enum BEEPS Type);
void CheckBeeper(void);
void TurnOffBeeper(void);
void BacklightControl(UINT8);
void EndOfWeldSignal(void);    
void CheckWeldHold(void);


#endif /* P_OUTPUT_H */
