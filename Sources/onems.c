/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/onems.c_v   1.10.1.3.3.3   14 Jul 2016 12:07:20   hasanchez  $
********************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION -----------------------------------

This is the module for controlling all the Timing Related Tasks using Hardware Times

Module name: TIMER

Filename:    onems.c
--------------------------- TECHNICAL NOTES --------------------------------------

This code contains the initalisation and Control module for two hardware timers.


------------------------------ REVISIONS -----------------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/onems.c_v  $
 * 
 *    Rev 1.10.1.3.3.3   14 Jul 2016 12:07:20   hasanchez
 * Uodated the function to make blink the screen and LED on LCD.
 * 
 *    Rev 1.10.1.3.3.2   18 May 2016 13:26:22   hasanchez
 * Updated OneSec Flag to HalfSec.
 * 
 *    Rev 1.10.1.3.3.1   13 May 2016 12:37:44   hasanchez
 * Updated the counter to update alarms from one second to half second.
 * 
 *    Rev 1.10.1.3.3.0   15 Dec 2015 16:15:02   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.10.1.0   25 Aug 2015 11:48:52   hasanchez
 * Update to handle the autoincrements.
 * 
 *    Rev 1.10   10 Sep 2013 03:18:58   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.9   08 Jul 2013 04:41:06   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, and changed frequency offset acc to requirement.
 * 
 *    Rev 1.8   13 Jun 2013 07:54:44   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.7   25 Mar 2013 08:01:16   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.6   03 Dec 2012 08:55:42   akaushal
 * Navigation and Screens Updated
 * 
 *    Rev 1.5   07 Nov 2012 07:04:26   akaushal
 * Screen Navigation Updated
 * 
 *    Rev 1.4   06 Nov 2012 08:44:26   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.3   10 Sep 2012 09:48:54   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.2   05 Sep 2012 07:32:04   akaushal
 * Fixed the Temperature update problem with testcode 
 * 
 *    Rev 1.1   16 Aug 2012 01:58:06   akaushal
 * SFX Code With External Crystal Oscillator and HW Changes
 * 
 *    Rev 1.0   06 Aug 2012 05:31:10   akaushal
 * Initial revision.
 * 
 *    Rev 1.6   31 Jul 2012 04:00:26   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.5   10 Jul 2012 05:35:44   akaushal
 * Digit blinking problem removed
 * 
 *    Rev 1.4   03 Jul 2012 07:38:42   akaushal
 * One Ms Tick Period and Accumulated energy Max. updated
 * 
 *    Rev 1.3   27 Jun 2012 07:50:08   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:33:22   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:46:24   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:38   ygupta
 * Initial revision.
-------------------------------- INCLUDES ----------------------------------------*/

#include "portable.h"
#include "onems.h"
#include "keyboard.h"
#include "PE_Types.h" 
#include "MC9S08LG32.h"
#include "LCD.h"
#include "print.h"
#include "weldhold.h"
#include "p_inputs.h"
#include "p_output.h"
 
#define MINUTES  60               /* Number of seconds in one minute */

#define DEBOUNCE_TIME      5      /* Actual Debounce time is DEBOUNCE_TIME  */ 
#define DIGITAL_IN_RATE    2      /* times DIGITAL_IN_RATE.                 */


/*---------------------------------- CODE ----------------------------------*/
UINT8 KbScanTimer;
BOOLEAN HalfSec;
static UINT8 idleTimerSecs, idleTimerMins;  
UINT8 backlightTimeout;
UINT16 BlinkTimer;

static UINT8  Port1[DEBOUNCE_TIME];
static UINT8 DebounceIndex = 0;
UINT32  TimerValue1;
UINT32  TimerValue2;
UINT32  Timer_RTotalTime;
UINT32  Timer_RTotalOnTime;
UINT32  TimerTime;
UINT32  TimerTimeOut;
UINT32  TimerTotalOnTime;


UINT32  TimerDisplayValue;

volatile UINT8 KeyBoardScanFlag; 

UINT32 RetvalSec;
UINT32 RetvalMin;
UINT16 Current_Energy;

UINT8	volatile FlagTimer1;     
UINT8	volatile FlagTimer2;
UINT8	volatile Flag_RTotalTime;
UINT8	volatile Flag_RTotalOnTime;
UINT8	volatile FlagTime;
UINT8	volatile FlagTimeOut;
UINT8	volatile FlagTotalOnTime;
UINT8 FlagSwitch = FALSE;


void TI_Init(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Initializes the associated peripheral(s) for Timer. The method is called 
*        automatically as a part of the application initialization code.
*                                                                         
***************************************************************************/
{
   SCGC1_TPM2 = 0x1;    // Enable bus clock to Timer2 module
   TPM2SC = 0x00;       // Initialize register to known state
   TPM2SC = 0x40;       // Enable Interrupts
   TPM2MOD = 0x20B1;    // 0x20D3(external clock);  Value for 1ms = SystemClock(8369737)/8369
   TPM2CNT = 0;         // Reset counter
   TPM2SC |= 0x08;      // Start timer with bus clock
}
volatile UINT8 TempPTE;
void DebounceInputs(void)
{
/****************************************************************************/
/*                                                                          */
/* This handler is used for reading and debouncing the digital inputs,      */
/* updates the power reading from the ADC,                                  */
/*                                                                          */
/****************************************************************************/
   
   UINT8  ANDResult1;   
   UINT8  ORResult1;
   UINT8  SameMask1/*,TempPTE*/ ;
   UINT8  CurrentPort1;
   UINT8 LocalIndex;
   
   SensedTemperature = (UINT16)GetTemperatureADC();    
   CurrentTemperature= SensedTemperature + Conf.ConfigData.CalibrationOffset;
   if (CurrentTemperature <= 320)
	   CurrentTemperature=320;
   //Convert Energy into Joules to display.   
   if((screen == SCREEN_RESULTS)){
	   if((IOValue & SONICS_ACTIVE ) == SONICS_ACTIVE)
		   Currentpower = (UINT16)GetPower();  // Get the current value of the input ports 
	   else
		   Currentpower = 0;
      AccumulatedEnergy += Currentpower;
      if (((WeldMode & CONFIG_TEMP) == CONFIG_ENERGY) || ((WeldMode & CONFIG_SUBENERGY)== CONFIG_SUBENERGY))
      {
          PartialEnergy+=Currentpower;
      }

      currentLCDData.CurrentTemp = CurrentTemperature; 
    }
   OUTPUT_PIN = LOW;
   PTEDD = INPUT;   /* need to set the data direction register to inputs */
   TempPTE= PTED;
   INPUT_PIN = SELECT;  
   CurrentPort1 = Port1[DebounceIndex] = PTED;   /* ports right here.*/  
   INPUT_PIN = DESELECT;
 
 //   
   PTEDD = OUTPUT;   /* need to set the data direction register back to outputs */ 
  if (++DebounceIndex >= DEBOUNCE_TIME) DebounceIndex = 0;      

   /* Bitwise debounce algorithm: AND last DEBOUNCE_TIME bytes together */    
   /* from each port to create ANDResultx.  OR last DEBOUNCE_TIME bytes */     
   /* to create ORResultx.  If a bit is the same in all samples, it will*/ 
   /* be either 1 in ANDResult (all samples were 1) or 0 in ORResult all*/    
   /* samples were 0).  Complement the ORResult, now a 1-bit in either  */
   /* mask corresponds to a bit that is the same in all samples. OR     */
   /* together ANDResult and ~ORResult, we have a mask of all bits that */
   /* are the same in all samples. Having done all that, now just use   */
   /* this mask to strip bits out of the KeysPressed and IOValue and    */
   /* substitute the new bits from the port.                            */    

   ANDResult1 = ORResult1 = Port1[0]; 
   for (LocalIndex = 1; LocalIndex < DEBOUNCE_TIME; LocalIndex++){
      ANDResult1 &= Port1[LocalIndex];   
      ORResult1 |= Port1[LocalIndex];     
   }      
   SameMask1 = ANDResult1 | (~ORResult1) ; 
   IOValue = (IOValue & (~SameMask1)) | (CurrentPort1 & SameMask1);    
  PTED = TempPTE;   
   OUTPUT_PIN = HIGH;
}


void ResetBacklightTimer(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Function that resets both the minutes and seconds for the backlight
*        SCREEN_idle timer. Function is called each time a button is pushed during SCREEN_idle,
*        resetting. When timer expires, the backlight is extinguished.
*                                                                         
*   Entry condition:  
*
*   Exit condition: 
*                                                                         
***************************************************************************/
{
   idleTimerSecs = 0;
   idleTimerMins = BACKLIGHT_TIMEOUT;
   backlightTimeout = FALSE;
}


UINT32 GetVar(void * varPtr, UINT16 VarSize) 
/****************************************************************************
* Function Name: GetVar
*
* Agruments:
* 1. varPtr: It specifies the UINT8 type pointer to variable.
* 2. VarSize :  It specifies the actual size of that variable.
*
* Return Type: UINT32 Value of that variable.
*
* Description:
*    It compute the variable value for a required variable size by using UINT8 pointer.
* 
*
****************************************************************************/
{
   UINT32 tempVar = 0; 
   SINT16 indx = 0;    
   tempVar = ((UINT8 *)varPtr)[0]; 
   for(indx = 1; indx < VarSize ; indx++) {
      tempVar <<= ONE_BYTE;      
      tempVar |= ((UINT8 *)varPtr)[indx];
   }     
   return tempVar;  
}

void SetVar(void * varPtr, UINT32 value ,SINT16 VarSize)
/****************************************************************************
* Function Name: SetVar
*
* Agruments:
* 1. varPtr: It specifies the UINT8 type pointer to variable.
* 2. value:  UINT32 value to be set.
* 3. VarSize :  It specifies the actual size of that variable.
*
* Return Type: void.
*
* Description:
*    Function sets the variable value of a required variable size by using UINT8 pointer.
* 
*
****************************************************************************/ 
{
   SINT16 indx = 0;
   ((UINT8 *)varPtr)[VarSize - 1] = (UINT8)value;
   for(indx = VarSize - 2; indx >= 0 ; indx--) {
      value >>= 8;
      ((UINT8 *)varPtr)[indx] = (UINT8)value;         
   }   
}

void CheckTimers(void)
/***************************************************************************/
/*                                                                         */
/*  This function will count down each timer.  If the timer reaches 0, the */
/*  associated flag will be set to FALSE.                                  */
/*                                                                         */
/***************************************************************************/
{
	
	static char TimeKeeper = 0;
	TimeKeeper++;


	if(FlagTime== ON)
	{
		if (--TimerTime <= 0) 
			FlagTime = OFF;
	}
	if(FlagTotalOnTime== ON)
	{
		if (--TimerTotalOnTime <= 0) 
			FlagTotalOnTime = OFF;
	}
	if(FlagTimer2== ON)
	{
		if (--TimerValue2 <= 0) 
			FlagTimer2 = OFF;
	}
	if(FlagTimeOut == ON)
	{
		if (--TimerTimeOut <= 0)
			FlagTimeOut = OFF;
	}
	if(FlagTimer1 == ON)     
	{
		if (--TimerValue1 <= 0) 
			FlagTimer1 = OFF;
	}

	if(Flag_RTotalOnTime== ON)
	{
		Timer_RTotalOnTime++;
		currentLCDData.TotalOnTime=Timer_RTotalOnTime;
	}
	if(Flag_RTotalTime== ON)
	{
		Timer_RTotalTime++;
		currentLCDData.TotalTime=Timer_RTotalTime;
		if (Timer_RTotalTime>=20)
		{
		//if (ONorOFF == TRUE && FlagTimer2 == OFF && StartFlag == TRUE && FlagSwitch == FALSE)
			if (PulsingMode == TRUE)
			{
				//if (ONorOFF == TRUE && (FlagTimer2 == OFF) && StopFlag == FALSE && FlagSwitch == FALSE)
				if (ONorOFF == TRUE && (FlagTimer2 == OFF) && FlagSwitch == FALSE)
				{
					FlagSwitch = TRUE;
					CheckOnParam();
					if ((WeldMode & SET_PUL_TEMP_NO_ZERO)== SET_PUL_TEMP_NO_ZERO)
					{
						if (CurrentTemperature >=PulseTempe && PulseTempeIndex>0)
							TempOffTime=DOAOFF[PulseTempeIndex-1];
						else
							TempOffTime=OffTime;
						StartMSTimer(TIMER2, TempOffTime);
					}
					else
					{
						StartMSTimer(TIMER2, OffTime);
					}
				}
				else
				{
					//if (ONorOFF == FALSE && FlagTimer2 == OFF && StopFlag == FALSE && FlagSwitch == FALSE)
					if (ONorOFF == FALSE && FlagTimer2 == OFF && FlagSwitch == FALSE)
					{
						FlagSwitch = TRUE;
						CheckOffParam();
						if ((WeldMode & SET_PUL_TEMP_NO_ZERO)== SET_PUL_TEMP_NO_ZERO)
						{
							if ((WeldMode & CONFIG_TEMP)== CONFIG_TIME)
							{
								if ( CurrentTemperature >=PulseTempe && PulseTempeIndex > 0)
									TempOnTime=DOAON[PulseTempeIndex-1];
								else
									TempOnTime=OnTime;
								StartMSTimer(TIMER2, TempOnTime);
							}
						}
						else
						{
							if (((WeldMode & CONFIG_TEMP ) != CONFIG_ENERGY) && ((WeldMode & CONFIG_SUBENERGY ) != CONFIG_SUBENERGY))
									StartMSTimer(TIMER2, OnTime);
						}
					}
				}
			}
		}
		FeedTheDog();
	}

	if((TimeKeeper % KB_SCAN_TIME) == 0)
	{
		KeyBoardScanFlag = TRUE;
	}
   if( TimeKeeper >= DISPLAYUPDATE) {
      UpdateLiveReady = TRUE;                                 
      TimeKeeper = 0;     
    } 
}


ISR(TI_Interrupt){
/****************************************************************************/
/*                                                                          */
/* This interrupt handler is used for reading and debouncing the inputs. It */
/* also start the ADC conversion and keep track of the timer and Beepers.   */
/*                                                                          */
/****************************************************************************/
   static UINT16 HalfSecCounter = 0;
   static UINT16 BlinkCounter = 0;
   static UINT8 kbScanTimerCounter = 0;
   static UINT16 KeyRepeatTimer = 0;
   static UINT16 KeyHoldTimer = 0;
   static UINT16 DividerTimer=1;
   static UINT16 DividerCounter=0;
   static UINT16 DividerTarget=5;
   static UINT8 IncrementCounter = 0;
   static UINT16 LockIncrement=0;
   TPM2SC_TOF = 0;                           // reset flag 
   TPM2CNT = 0;                              // Reset counter
   
   if (++kbScanTimerCounter >= KB_SCAN_TIME){
      KbScanTimer = 1;
      kbScanTimerCounter = 0;
   }

   if (++BlinkCounter >= BLINK_TIME){
      BlinkTimer ^= 0xFFFF;
      UpdateLiveReady = TRUE;
      BlinkCounter = 0;
   }
   
   if (AlarmFlag ==TRUE)
   {
	   if (++HalfSecCounter >= MILLISEC_IN_HALF_SEC){   
		   HalfSec = TRUE;
		   HalfSecCounter = 0;
	   }
   }
   else
	   HalfSecCounter=0;
   
   if((KeyUpDown == TRUE) )/*&& (KeyAutoscroll == TRUE))*/{
	   if (++KeyHoldTimer >= KEY_HOLD_TIMER1){
		   KeyHold = TRUE;
		   KeyUpDown = FALSE;  //stop this loop and start KeyRepeatTimer
		   KeyHoldTimer = 0;
		   DividerTimer=1;
		   DividerCounter=0;
		   DividerTarget=5;
		   IncrementCounter=1;
		   Increment=1;
	   }
	   LockIncrement = 0 ;
   }
   if(KeyHold == TRUE){
   	   if (++KeyRepeatTimer >= (KEY_REPEAT_TIMER/DividerTimer))
   	   {
   		   KeyRepeat = TRUE;
   		   KeyRepeatTimer = 0;
   		   if (DividerTimer<25){
   			   if (++DividerCounter >= DividerTarget)
   			   {
   					   DividerTimer*=5;
   					   DividerTarget+=20;
   					   DividerCounter=0;
   			   }
   		   }
   	   }
   }
   
   CheckBeeper();   /* used to keep track of beeper timing */
   CheckTimers();  
   CheckWeldHold();
   StartADCConversion();
   DebounceInputs();
   
}



void StopAllTimers(void)
/***************************************************************************/
/*                                                                         */
/*   This function will stop all timers (except the Display timer) by      */
/*   setting their flags to FALSE.  In addition the minutes and seconds    */
/*   counters will be set to zero.                                         */
/*                                                                         */
/***************************************************************************/
{
	FlagTimer1 			= OFF;
	FlagTimer2			= OFF;
	Flag_RTotalTime		= OFF;
	Flag_RTotalOnTime	= OFF;
	FlagTime			= OFF;
	FlagTimeOut			= OFF;
	FlagTotalOnTime		= OFF;

}

void StartMSTimer(TIMERS Index, UINT32 Time)
/***************************************************************************/
/*                                                                         */
/*  This function will start a timer with ms resolution.  It requires the  */
/*  timer number as the first arg and the time in ms as the second arg.    */
/*                                                                         */
/***************************************************************************/
{
   switch (Index) {
     case TIMER1:
         TimerValue1 = Time;
         FlagTimer1	 = ON;
      break;
      case TIMER2:
    	  TimerValue2 = Time;
    	  FlagTimer2 = ON;
      break;
      case RTOTALTIME:
    	  Timer_RTotalTime=Time; 
    	  Flag_RTotalTime=ON;
    	  break;
      case RTOTALONTIME:
    	  Timer_RTotalOnTime=Time;
    	  Flag_RTotalOnTime= ON;
    	  break;
      case TIMER_TIME:
    	  TimerTime=Time;
    	  FlagTime= ON;
    	  break;
      case TIMER_TIMEOUT:
    	  TimerTimeOut=Time;
    	  FlagTimeOut= ON;
    	  break;
      case TIMER_TOTALONTIME:
    	  TimerTotalOnTime=Time;
    	  FlagTotalOnTime= ON;
    	  break;
   }   
}

void StopMSTimer(TIMERS Index)
/***************************************************************************/
/*                                                                         */
/*  This function will Stop a timer with ms resolution.  It requires       */
/*  the timer number as the 1st arg                                        */
/*                                                                         */
/***************************************************************************/
{
   switch (Index) {
      case TIMER1:
         TimerValue1 = 0;
         FlagTimer1 = OFF;
         break;
      case TIMER2:
    	  TimerValue2 = 0;
         FlagTimer2 = OFF;
         break;
      case TIMER_TIME:
    	  TimerTime=0;
    	  FlagTime= OFF;
    	  break;
      case TIMER_TIMEOUT:
    	  TimerTimeOut=0;
    	  FlagTimeOut= OFF;
    	  break;
      case TIMER_TOTALONTIME:
    	  TimerTotalOnTime=0;
    	  FlagTotalOnTime= OFF;
    	  break;
   }   
}

void PauseMSTimer(TIMERS Index)
/***************************************************************************/
/*                                                                         */
/*  This function will pause a timer with ms resolution.  It requires      */
/*  the timer number as the 1st arg.                                       */
/*                                                                         */
/***************************************************************************/
{
   switch (Index) {
   case TIMER1:
       FlagTimer1= PAUSE;
    break;
    case TIMER2:
  	  FlagTimer2 = PAUSE;
    break;
    case RTOTALTIME:
  	  Flag_RTotalTime=PAUSE;
  	  break;
    case RTOTALONTIME:
  	  Flag_RTotalOnTime= PAUSE;
  	  break;
    case TIMER_TIME:
  	  FlagTime= PAUSE;
  	  break;
    case TIMER_TIMEOUT:
  	  FlagTimeOut= PAUSE;
  	  break;
    case TIMER_TOTALONTIME:
  	  FlagTotalOnTime= PAUSE;
  	  break;
   }   
}

void ResumeMSTimer(TIMERS Index)
/***************************************************************************/
/*                                                                         */
/*  This function will Resume a timer with ms resolution.  It requires     */
/*  the timer number as the 1st arg.                                       */
/*                                                                         */
/***************************************************************************/
{
   switch (Index) {
   case TIMER1:
       FlagTimer1= ON;
    break;
    case TIMER2:
  	  FlagTimer2 = ON;
    break;
    case RTOTALTIME:
  	  Flag_RTotalTime=ON;
  	  break;
    case RTOTALONTIME:
  	  Flag_RTotalOnTime= ON;
  	  break;
    case TIMER_TIME:
  	  FlagTime= ON;
  	  break;
    case TIMER_TIMEOUT:
  	  FlagTimeOut= ON;
  	  break;
    case TIMER_TOTALONTIME:
  	  FlagTotalOnTime= ON;
  	  break;
   }   
}

