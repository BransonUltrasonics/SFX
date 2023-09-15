/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/util.c_v   1.7.1.3.3.6   26 Jul 2016 17:34:54   hasanchez  $ 
*/
/****************************************************************************/ 
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2002                 */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------*/
/*                                                                          */
/* Module name: UTIL                                                        */
/*                                                                         */
/* Filename:    Util.c                                                      */
                                                                          
/*--------------------------- TECHNICAL NOTES ------------------------------- 

Utility function used just about anywhere                                                              

------------------------------ REVISIONS ---------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/util.c_v  $
 * 
 *    Rev 1.7.1.3.3.6   26 Jul 2016 17:34:54   hasanchez
 * updated the version string to 1.3
 * 
 *    Rev 1.7.1.3.3.5   22 Jul 2016 13:55:12   hasanchez
 * Added fix related to Start Switch Alam and Test.
 * 
 *    Rev 1.7.1.3.3.4   14 Jul 2016 12:20:32   hasanchez
 * Added filter to avoid blink the LED on the HH if an ER_DE or ER_TEMP occurs.
 * 
 *    Rev 1.7.1.3.3.3   13 Jun 2016 12:47:46   hasanchez
 * ER_uTIP sets the LockItFlag to true. If a start switch alarm is released and the Start Stop or Ext Start remain active, it set SS alarm.
 * 
 *    Rev 1.7.1.3.3.2   13 May 2016 12:42:26   hasanchez
 * Added functions to update the version from 1.2 to 1.3
 * Updated the reset alarm function to take care about the ultrasonic test.
 * 
 *    Rev 1.7.1.3.3.1   12 Jan 2016 11:35:36   aghuerta
 * Change version to 21
 * 
 *    Rev 1.7.1.3.3.0   15 Dec 2015 16:15:04   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.7.1.0   25 Aug 2015 12:06:26   hasanchez
 * New streing to alarms and Software version were added.
 * 
 *    Rev 1.7   08 Jul 2013 04:41:30   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, and changed frequency offset acc to requirement.
 * 
 *    Rev 1.6   13 Jun 2013 07:55:26   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.5   25 Mar 2013 08:01:42   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.4   20 Dec 2012 01:25:06   akaushal
 * Updated Error codes and fixed Total runtime
 * 
 *    Rev 1.3   06 Nov 2012 08:45:58   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.2   10 Sep 2012 09:49:22   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.1   16 Aug 2012 01:58:44   akaushal
 * SFX Code With External Crystal Oscillator and HW Changes
 * 
 *    Rev 1.0   06 Aug 2012 05:32:06   akaushal
 * Initial revision.
 * 
 *    Rev 1.5   31 Jul 2012 04:01:20   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.4   06 Jul 2012 10:04:02   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.3   27 Jun 2012 07:51:58   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:34:52   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:48:36   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:01:58   ygupta
 * Initial revision.

*****************************************************************************/
 
/*------------------------------ INCLUDES ----------------------------------*/

#define UTIL
#include "util.h"
#include "keyboard.h"
#include "weldhold.h"
#include "p_output.h"
#include "p_inputs.h"


/*---------------------------- LOCAL_EQUATES -------------------------------*/


/*---------------------------- TEXT MESSAGES -------------------------------*/
/* The Messages are in reverse order since we started 
numbering Segment fronm Right most Segment and 
Copied as per a counter i. Thus "r" should go to the
higher numbered segment than "d" */ 
const  STR SoftwareVersion[]  = "  31  ";  
const  STR Off_Msg[]          = "  ffO ";        
const  STR Rdy_Msg[]          = "  ydr ";       // Startup Message
const  STR Rdy_Msg2[]         = "   ydr";       // Startup Message 2
const  STR Er_OL_Msg[]        = "  020E";       // Overload Error Message
const  STR Er_SS_Msg[]        = "  106E";       // Start Switch Error Message

const  STR Er_EEPROM[]        = "  10AE";       // EEPROM Error Message
const  STR Er_TempSens[]      = "  109E";       // Temperature Probe Disconnected Error Message
const  STR Er_MicroTip[]      = "  202E";       // Microtip ON and Amplitude >70% Error Message
const  STR Er_DE_Msg[]        = "  602E";       // Data Entry Error Message (Invalid Entry)
const  STR Er_TempError[]     = "  209E";       // Current Temperature > MAX Temperature at Start of Weld
                                                // meaning MAX Temperature is Less than Facility Temperature
const  STR Er_TimeError2[]    = "  012E";       // TimeOut > TotalOnTime
const  STR Er_TimeOutReach[]  = "  501E";       // TimeOut reached
const  STR Er_HandHeld[]   	  = "  807E";       // Hand held Detected and current preset features a Temp control mode or
												//Time or Total On Time setting exceed 10 minutes. 
const  STR Er_FrontPanel[]    = "  907E";       // Trying start cycles with the Start/Stop key when using HandHeld.


STR Str_CRC[]	    =	"  0000";
 

/*-------------------------------- CODE ------------------------------------*/

void CreateAlarm(ALARMS Alarm)
/************************************************************************************/
/*                                                                                  */
/*   This function will create an alarm.  It has one argument, the enum for the     */
/*   alarm to be created. The overload alarm will have the highest priority, that   */
/*   is, it will override all other alarms and no other alarm can override it.      */
/*                                                                                  */
/*   Exit condition:                                                                */
/*      AlarmFlag:      TRUE                                                        */
/*      CurrentAlarm:   enum of last alarm created (OL has highest priority)        */
/*                                                                                  */
/************************************************************************************/
{
   KEY_CODE  Keys;

   if (AlarmFlag == TRUE) {                        /* Does an alarm already exist ? */
      if ( (CurrentAlarm == ER_OL) || (Alarm == ER_OL) ) 
         CurrentAlarm = ER_OL;                     /* One of them is an Overload    */
      else       
         CurrentAlarm = Alarm;
    
   }
   else {   
      AlarmFlag = TRUE;
      CurrentAlarm = Alarm;
   }
   SetAlarm();

	Beep(ERRORBEEP);           

   
   if (Conf.ConfigData.AutoReset == FALSE) LockItFlag = TRUE;          /* Ar=0 so reset required   */
   if (CurrentAlarm == ER_OL) {                     
      Keys = GetKeys();                   /* See if Start/Stop is pressed after O/L */
      if ((Keys & EXTSTART) == EXTSTART) LockItFlag = TRUE;
   }
   if (CurrentAlarm == ER_uTIP)
	   LockItFlag = TRUE;
   
   KeyMask = ALL_KEYS;
   UpdateTimer= 0;
   BlinkTimer=0;
   if (currentLCDData.HandHeld == TRUE && ((CurrentAlarm != ER_DE) && (CurrentAlarm != ER_TEMP)))
	   ClrUserOut();
   SwitchScreen(SCREEN_ERROR);     
}




void ResetAlarm(void)
/************************************************************************************/
/*                                                                                  */
/*   This function will clear alarms, if possible. Er:SS cannot be cleared if the   */
/*   start switch is still active.                                                  */
/*                                                                                  */
/*   Exit condition:                                                                */
/*      AlarmFlag:      FALSE unless alarm can't be reset, then TRUE                */
/*      Alarm Output:   Off (unless alarm cannot be cleared)                        */
/*                                                                                  */
/************************************************************************************/
{
   UINT16  ValidKeys;
   ValidKeys = GetKeys();
   if ( HandHeld == TRUE &&((ValidKeys & EXTSTART) ==  EXTSTART) && (TestReqFlag == FALSE))
   {
	   LockItFlag = TRUE;
	   CreateAlarm(ER_SS);
   }
   else
   {
	   switch (CurrentAlarm) {
		   case ER_DE:                             /* These alarms can always be cleared */
		   case ER_OL:
		   case ER_RF:
		   case ER_TIMEOUT:
			   AlarmFlag = FALSE;
			   LockItFlag = FALSE;
			   ClrAlarm();                          /* Remove external alarm output       */
			   CurrentAlarm = NO_ALARM;
			   StartFlag = FALSE;                   /* Make sure another cycle doesn't run*/
			   StopFlag = TRUE; 
			   break;
		   case ER_SS:
			   if ( ((ValidKeys & KB_START_STOP) != KB_START_STOP) && ((ValidKeys & EXTSTART) != EXTSTART) ) {
				   AlarmFlag = FALSE;
				   LockItFlag = FALSE;
				   ClrAlarm();                       /* Remove external alarm output       */
				   StartFlag = FALSE;                /* Make sure another cycle doesn't run*/
				   StopFlag = TRUE; 
				   CurrentAlarm = NO_ALARM;
			   }     
			   else
			   {
				   LockItFlag= TRUE;
				   CreateAlarm(ER_SS);
			   }
			   break;
		   case NO_ALARM:
		   default:
			   AlarmFlag = FALSE;
			   LockItFlag = FALSE;
			   ClrAlarm();                          /* Remove external alarm output       */
			   break;   
	   }
   }
   
   ExperFlag = FALSE;                         /* Experiment has to be ended now     */
   if (TestReqFlag == FALSE)
	   ClrUserOut();
} 

UINT8 AmpMicrotipValid(void)
{
	bool ParametersValid = TRUE;
	if ((CurrentPreset.presetData.AmplitudeValue > MICROTIPLIMIT) && (CurrentPreset.presetData.Microtip == TRUE))
	{      
		ParametersValid = FALSE;
		AuxCurrentParam=CurrentParam;
		CreateAlarm(ER_uTIP);
		BlinkIcons = (UINT32)(ALARM_I | MICROTIP_I);
		FixedIcons &= ~ MICROTIP_I;
		SwitchScreen(SCREEN_ERROR);
    }
	return ParametersValid;
}

void ResetClearAlarm(void)
/************************************************************************************/
/*                                                                                  */
/*   This function will call ResetAlarm and PSReset.                                */
/*                                                                                  */
/************************************************************************************/
{
   ResetAlarm();
   PSReset();
   SwitchScreen(SCREEN_RDY);
}


