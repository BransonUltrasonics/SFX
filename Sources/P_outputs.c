/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/P_outputs.c_v   1.6.1.2.3.0   15 Dec 2015 16:15:04   hasanchez  $
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
/*--------------------------- MODULE DESCRIPTION ---------------------------*/
/* This module is use to check the digital output                           */

/*----------------------------- TECHNICAL NOTES --------------------------------*/
/* This module contains all function that set and clear the digital outputs.*/
/* It also sets the power supply amplitude.                                 */

/* Module name: OUTPUTS                                                   */
/*                                                                          */
/* Filename:    P_outputs.c                                                                                                                          */
/*------------------------------ REVISIONS ---------------------------------*/

/* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/P_outputs.c_v  $ 
 * 
 *    Rev 1.6.1.2.3.0   15 Dec 2015 16:15:04   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.6   10 Sep 2013 03:18:26   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.5   13 Jun 2013 07:55:14   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.4   06 Nov 2012 08:44:52   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.3   24 Aug 2012 10:36:38   akaushal
 * Amplitude Code Corrected
 * 
 *    Rev 1.2   24 Aug 2012 06:24:04   akaushal
 * Amplitude Issue resolved
 * 
 *    Rev 1.1   16 Aug 2012 01:58:18   akaushal
 * SFX Code With External Crystal Oscillator and HW Changes
 * 
 *    Rev 1.0   06 Aug 2012 05:31:34   akaushal
 * Initial revision.
 * 
 *    Rev 1.3   27 Jun 2012 07:51:14   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:34:04   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:47:30   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:01:26   ygupta
 * Initial revision.

******************************************************************************/



/*------------------------------ INCLUDES ----------------------------------*/

#include "P_output.h"
#include "util.h"
#include "weldhold.h"
#include "onems.h"
#include "DS1804.h"

/*---------------------------- LOCAL_EQUATES -------------------------------*/

#define  ORS_POWER_LIMIT   125       /* ORS limits displayed power to 125 % */
 
/*----------------------------- GLOBAL DATA --------------------------------*/

UINT8  Ready;

UINT8 IOMirror = PORTE_DEFAULTOUT;	
static UINT8  NumberOfBeeps;    
static UINT8  BeeperOn = FALSE;    /* Flag for timer routine to clear       */
static SINT16 BeepOnTime;          /* Timer for off/on time for beeper      */

/*-------------------------------- CODE ------------------------------------*/

void SetPSRun(void){
/***************************************************************************/
/*                                                                         */
/* This function sets the Run Signal on the power supply module.  Release  */
/* the Reset signal incase just coming from an overload.                   */
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;    /* need to set the data direction register to outputs */ 
   IOMirror |= OL_RESET_OUT; /* Clear Reset if set */
   IOMirror &= ~RUN_OUT;   /* Active Low */
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;   /* Latch Data */
   OUTPUTCONTROL = LOW;
}



void PSIdle(void){
/***************************************************************************/
/*                                                                         */
/* This function clears the Run & Test Signals on the power supply module. */
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;    /* need to set the data direction register to outputs */ 
   IOMirror |= (RUN_OUT | TEST_OUT);
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;       /* Latch Data */
   OUTPUTCONTROL = LOW;
}



void PSTest(void){
/***************************************************************************/
/*                                                                         */
/* This function sets the Run and Test Signals on the power supply module. */
/* Release the Reset signal incase just coming from an overload.           */
/***************************************************************************/
   PTEDD = OUTPUT;   /* need to set the data direction register to outputs */ 
   IOMirror |= OL_RESET_OUT; /* Clear Reset if set */
   IOMirror &= (~(TEST_OUT));
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;       /* Latch Data */
   OUTPUTCONTROL = LOW;
}



void PSReset(void){
/***************************************************************************/
/*                                                                         */
/* This function sends a Reset pulse to the power supply module. The Reset */
/* is cleared in either PSTest or PSRun.                                   */
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;    /* need to set the data direction register to outputs */ 
   IOMirror &= ~OL_RESET_OUT; /* Active Low */
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;   /* Latch Data */
   OUTPUTCONTROL = LOW;
   //ClrUserOut();                           /* Turn off sonics on output    */
   OLFlag = FALSE;                         /* Assume resetting P/S works   */
                                           /* If not, "I'll be back"       */
}


void PSSeek(void)
/***************************************************************************/
/*                                                                         */
/* This function starts a power supply seek.                               */
/*                                                                         */
/***************************************************************************/
{
   PTEDD = OUTPUT;    /* need to set the data direction register to outputs */ 
   IOMirror |= OL_RESET_OUT;    /* Clear Reset if set */
   IOMirror |= SEEK_OUT;        /* Active Low, but an inverter on PC board */
   IOMirror |= READY_OUT;
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
}



void RemovePSSeek(void)
/***************************************************************************/
/*                                                                         */
/* This function removes the power supply seek.                            */
/*                                                                         */
/***************************************************************************/
{
   PTEDD = OUTPUT;    /* need to set the data direction register to outputs */ 
   IOMirror &= ~SEEK_OUT;     /* Active Low , but an inverter on PC board */
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
}



void SetReady(void){
/***************************************************************************/
/*                                                                         */
/* This function outputs the Ready Signal on the 9 pin user connector.     */
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;   /* need to set the data direction register to outputs */ 
   IOMirror &= ~READY_OUT; /* Active Low */
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
   Ready = TRUE;
}


void ClrReady(void){
/***************************************************************************/
/*                                                                         */
/* This function clears the Ready Output on the 9pin user connector.       */
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;   /* need to set the data direction register to outputs */ 
   IOMirror |= READY_OUT;
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
   Ready = FALSE;
}


void SetAlarm(void){
/***************************************************************************/
/*                                                                         */
/* This function sets the Alarm Output signal on the 9pin user connector.  */
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;   /* need to set the data direction register to outputs */ 
   IOMirror &= ~ALARM_OUT; /* Active Low */
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
}


void ClrAlarm(void){
/***************************************************************************/
/*                                                                         */
/* This function removes the Alarm Output signal on the 9pin user connector*/
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;   /* need to set the data direction register to outputs */ 
   IOMirror |= ALARM_OUT;
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
}


void ClrUserOut(void){
/***************************************************************************/
/*                                                                         */
/* This function removes the UserOut signal on the 9pin user connector.    */
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;   /* need to set the data direction register to outputs */ 
   IOMirror |= USER_OUT;
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
}


void SetUserOut(void){
/***************************************************************************/
/*                                                                         */
/* This function sets the UserOut signal on the 9pin user connector.       */
/*                                                                         */
/***************************************************************************/
   PTEDD = OUTPUT;   /* need to set the data direction register to outputs */ 
   IOMirror &= ~USER_OUT;    /*Active low */
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
}


void SetAmplitude(UINT8 Percent){
/***************************************************************************/
/*                                                                         */
/* This function sets the amplitude for the power supply using a           */
/* Motorola/ON DAC0808LCM which has its value latched in by 74HC573ADW     */
/* preceding its parallel inputs. This function converts 1-100% to         */
/* 1-127 counts.                                                           */
/* "Counts" are sent out on A0-A7. Counts = Percent + Percent shifted      */
/* right by 2. Next chip select the latch, then deselect it.               */
/*                                                                         */
/***************************************************************************/
   UINT8 Counts, Temp_val;
   Temp_val = PTED ;
   OUTPUT_PIN = LOW ;
   Counts = Percent + (Percent >>2);
   Counts &= 0x7F;   //make MSBit zero
   Counts <<= 1;
   TPM2SC &= ~(0x40);  //disabling timer interrupts
   AMPOUT = Counts;
   AMPCONTROL = HIGH;
   AMPCONTROL = LOW;
   TPM2SC |= (0x40);  //enabling timer interrupts
   PTED = Temp_val;
   OUTPUT_PIN = HIGH ;
}



void SetBeep(void)
/***************************************************************************/
/*                                                                         */
/* This function turns the beeper on if enabled.                           */
/*                                                                         */
/***************************************************************************/
{
   PTEDD = OUTPUT;            /* Set the data direction register to outputs */ 
   IOMirror |= BUZZER_OUT;
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
}



void ClrBeep(void)
/***************************************************************************/
/*                                                                         */
/* This function turns the beeper off.                                     */
/*                                                                         */
/***************************************************************************/
{
   PTEDD = OUTPUT;    /* need to set the data direction register to outputs */ 
   IOMirror &= ~BUZZER_OUT;
   PTED = IOMirror;
   OUTPUTCONTROL = HIGH;             /* Latch Data */
   OUTPUTCONTROL = LOW;
}


void Beep(enum BEEPS Type)
/***************************************************************************/
/*                                                                         */
/*  This function will take the passed parameters and saved them so that   */
/*  ProcessBeeper can actually turn the beeper on and off.   The parameter */
/*  is what kind of sound is it (ALARM, ERROR, RELEASE or STARTUP).                 */
/*                                                                         */
/***************************************************************************/
{
   switch (Type) {
      case ERRORBEEP:
         if (Conf.ConfigData.ErrorBeep == TRUE) {
            BeepOnTime = DEFAULTBEEPTIME;
            SetBeep();                           /* Turn Beeper on         */
            NumberOfBeeps = 6;                   // Value is Twice Number of BEEPS
            BeeperOn = TRUE;
         }
      break;

      case RELEASEBEEP:
         if (Conf.ConfigData.EndOfWeldBeep == TRUE) {
            BeepOnTime = RELEASEBEEPTIME;
            SetBeep();                           /* Turn Beeper on         */
            NumberOfBeeps = 2;                   // Value is Twice Number of BEEPS
            BeeperOn = TRUE;
         }
      break;
      case STARTUPBEEP:
         BeepOnTime = DEFAULTBEEPTIME;
         SetBeep();                           /* Turn Beeper on         */
         NumberOfBeeps = 4;                   // Value is Twice Number of BEEPS
         BeeperOn = TRUE;
      break;
        
      default:
         BeepOnTime = 0;
         BeeperOn = FALSE;
      break;
   }
   
   
   
}
void EndOfWeldSignal(void)
/***************************************************************************/
/*                                                                         */
/*  This function will turn the User out signal on if the register 19 is    */
/*  set to 2 (EndOfWeldPulse function). the value of ENDOFWELDOFFTIME is   */
/* 1/4 of second                                                           */
/*                                                                         */
/***************************************************************************/
{
	if (Conf.ConfigData.EndOfCycle == 2 && HandHeld == FALSE)
	{
		SetUserOut();                           /*Turn on the User out signal    */
		EndOfWeldSignalTime = ENDOFWELDOFFTIME; /*Set the timer to the User out signal   */
	}
}
void CheckBeeper(void)
{
   if (NumberOfBeeps > 0) {
      if (--BeepOnTime <= 0) {
         TurnOffBeeper();
      }
   }
}

void CheckWeldHold(void)
/***************************************************************************/
/*                                                                         */
/*  This function will turn the EndOfWeld signal off if the time is        */
/*  consumed.                                                              */
/*                                                                         */
/***************************************************************************/
{
	if(Conf.ConfigData.EndOfCycle == 2 && EndOfWeldSignalTime > 0 )
		EndOfWeldSignalTime--;
	else
		if (Conf.ConfigData.EndOfCycle == 2 && EndOfWeldSignalTime == 0)
			ClrUserOut();
}

void TurnOffBeeper(void) 
{  
   if (NumberOfBeeps-- <= 1) {
      ClrBeep();                    /* Turn Beeper off        */
      BeepOnTime = 0;
      BeeperOn = FALSE;
      NumberOfBeeps = 0;
   }
   else {
      if (BeeperOn == TRUE) {
         BeepOnTime = BEEPOFFTIME;
         ClrBeep();                 /* Turn Beeper off        */
         BeeperOn = FALSE;
      }
      else {
         BeepOnTime = DEFAULTBEEPTIME;
         SetBeep();                 /* Turn Beeper on         */
         BeeperOn = TRUE;
      }
   }
}

void BacklightControl(UINT8 status)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Controls backlight
*                                                                         
*   Entry condition:  
*
*        status - 1 turns backlight on, 0 turns it off.
*        BACKLIGHT_PIN - current state of backlight, and used to set
*
*   Exit condition: 
*
*        Turn on or off backlight, according to status
*                                                                         
***************************************************************************/
{
   if (status ^ BACKLIGHT_PIN){
      BACKLIGHT_PIN = status;
   }
}
