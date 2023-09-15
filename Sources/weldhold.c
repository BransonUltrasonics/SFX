/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/weldhold.c_v   1.9.1.3.3.6   04 Aug 2016 15:23:16   hasanchez  $
*/
/***************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


------------------------------ MODULE DESCRIPTION ---------------------------

This is the module for controlling all the Timing Related Tasks using Hardware Times

Module name: WELDHOLD

Filename:    weldhold.c

--------------------------- TECHNICAL NOTES -------------------------------*/
/*                                                                          */
/*  This module handles the details of actually running a cycle.  It will   */
/*  start the timers, check them, calculate energy, count overloads and     */
/*  check experiment time remaining.                                        */

/*------------------------------ REVISIONS ---------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/weldhold.c_v  $
 * 
 *    Rev 1.9.1.3.3.6   04 Aug 2016 15:23:16   hasanchez
 * fix to verify StartFlag if Panel Treigger is False and Pulse Start is True. 
 * 
 *    Rev 1.9.1.3.3.5   13 Jun 2016 12:43:56   hasanchez
 * The flag FirstTime is renamed to ReqStartRelease. Updated the weldmode Pulse Temp Continuos Energy on the max temp function. Amplitude verification while Hand Held updated.
 * 
 *    Rev 1.9.1.3.3.4   18 May 2016 12:57:38   hasanchez
 * Deleted Debug function.
 * 
 *    Rev 1.9.1.3.3.3   13 May 2016 13:29:02   hasanchez
 * Added functionsto calculate progress of energy, time and temperature. Bug fixes about showing last temperature higher than max temperature. Added features to handle the request of handheld verification. Fix to the bug that shows wrong segments during test.
 * 
 *    Rev 1.9.1.3.3.2   29 Jan 2016 16:18:40   hasanchez
 * Formulas to calculate the Energy is updated.
 * 
 *    Rev 1.9.1.0   25 Aug 2015 12:10:34   hasanchez
 * Updates temporally to init a basic ultrasonic function.
 * 
 *    Rev 1.9   10 Sep 2013 03:17:42   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.8   12 Jul 2013 06:50:44   akaushal
 * Corrected the handelling of Start switch delay.
 * 
 *    Rev 1.7   08 Jul 2013 04:41:46   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, and changed frequency offset acc to requirement.
 * 
 *    Rev 1.6   13 Jun 2013 07:55:38   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.5   25 Mar 2013 08:01:56   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.4   20 Dec 2012 01:25:20   akaushal
 * Updated Error codes and fixed Total runtime
 * 
 *    Rev 1.3   03 Dec 2012 08:55:58   akaushal
 * Navigation and Screens Updated
 * 
 *    Rev 1.2   06 Nov 2012 08:46:10   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   10 Sep 2012 09:49:36   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.0   06 Aug 2012 05:32:22   akaushal
 * Initial revision.
 * 
 *    Rev 1.5   31 Jul 2012 04:01:32   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.4   06 Jul 2012 10:06:50   akaushal
 * Temperature Flag check in Automode adn formatting
 * 
 *    Rev 1.3   27 Jun 2012 07:52:26   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:35:22   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:49:00   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:02:14   ygupta
 * Initial revision.
 
 *****************************************************************************/

/*---------------------------- INCLUDES -------------------------------*/

#define TEST_UPDATE_RATE     250L  /* How often (ms) to update display */
#define OL_LIMIT             1    /* Maximum acceptable consecutive overloads before auto shutdown */
#define START_RELEASE_DELAY  2000L    /* Delay before stuck start switch alarm */
#define MAX_ACCUMULED    999999000
#define MAX_ENERGY       999999
#define MINTEMPDIFF 54  //Equivalent to 3.0 Celcius ~ 5.4 F (represented as F*10 = 54)
#define HUNDRED_PERCENT 100
#define NINETY_NINE_PERCENT 99
/*------------------------------ INCLUDES ----------------------------------*/

#include "portable.h"
#include "util.h"
#include "keyboard.h"
#include "weldhold.h"
#include "onems.h"
#include "p_inputs.h"
#include "p_output.h"



/*----------------------------- GLOBAL DATA --------------------------------*/

SINT16   CurrentTemperature;
SINT16	 SensedTemperature;
UINT32   AccumulatedEnergy; /* Energy units used so far                     */
UINT32   PartialEnergy;
//UINT32   EnergyUnitsNeeded; /* Energy units needed                          */
//static   UINT8 OverloadCounter;


UINT32	Time;
UINT32	Energy;
UINT32	OnTime;
UINT32	OffTime;
UINT16	OnEnergy;
UINT32	TotalOnTime;
UINT32	TotalOnEnergy;
UINT16	PulseTempe;
UINT16	MaxTempe;
UINT8	Amplitude;
UINT16	WeldMode;
BOOLEAN AmpUpdatedDuringcycleFlag = FALSE;

UINT16   TemperatureFlag;

//UINT16   EnergyFlag;        /* Indicates enough energy has been used        */

UINT32   TimeOut  ;
UINT16	 InitTemp;
UINT16   Wattage;           /* Wattage of power supply in watts             */ 
 

ALARMS   CurrentAlarm;   /* Code for last alarm created since power on      */
//MODES    CurrentMode;    /* Current mode                                    */
UINT16   AvgPower;       /* Displayed Power from the last ADC reading       */
UINT16   Currentpower;   /* Power from the last ADC reading/word for Format */
UINT8    AlarmFlag;      /* A condition to prevent entry into ready         */
UINT8    ExperFlag = FALSE; /* Used to indicate if experiment is running       */
UINT8    GDFlag;         /* External ground detect received                 */
UINT8    GDAllowed;      /* TRUE: in a grd det mode FALSE: not in GD mode   */
UINT8    LockItFlag;     /* Used to lock up system such as ram failure      */
UINT8    OLFlag;         /* Overload detected from power supply             */
UINT8    TimeFlag;       /* Used to indicate if timers are still running    */
UINT8    OkToAdvance;           /* Used to decide if ok to enter ready state */
UINT8  TestResult;
UINT8    HandHeld = FALSE;
UINT8    CycleUpdated = FALSE;

SINT16 EndOfWeldSignalTime;         /*Timer counter to the EndOfWeld signal*/

UINT8 TempeProbePresent;
UINT8 ReqStartRelease = TRUE;     /*check if this is the first time (powerup)*/
//UINT8         
UINT32   DOAON[10];
UINT32   DOAOFF[10];
UINT16   DOATEMP[10];
UINT8  PTemperatureFlag;
UINT8  PulsingMode;
UINT8  PHysterisis;
UINT8  PulseTempeIndex =0;  
UINT32 TempOnTime;
UINT32 TempOffTime;
UINT32 TempOnEnergy;
/*-------------------------------- CODE ------------------------------------*/


void CalcPower(void)
/******************************************************************************/
/*                                                                            */
/*   This function will determine the current value of power.  If called      */
/*   faster than TEST_UPDATE_RATE nothing will be done except for a test for  */
/*   a new value of peak power.  This will be called from the state machine   */
/*   while in the test state or RunningSonicsOn state.                        */
/*                                                                            */
/*   Exit condition:                                                          */
/*       PeakPower:        Updated with new power value if greater than last  */
/*       AvgPower:         New power value if TEST_UPDATE_RATE time has       */
/*                         passed, otherwise still contains last value        */
/*                                                                            */
/******************************************************************************/
{
 
	currentLCDData.CurrentPower   = (Currentpower * Wattage)/100;
	
	if (currentLCDData.CurrentPower > currentLCDData.PeakPower)
		currentLCDData.PeakPower=currentLCDData.CurrentPower;
	
	if ((FlagTimer2 == FALSE) &&  (TestKey == TRUE) ) {
		SetPSRun();   // This delay allows Test signal to be before Run signal //
		StartMSTimer(TIMER2, TEST_UPDATE_RATE);
   }
   
	   //Update only when the Power is On.. 											    
}



void FinalPower(void)
/******************************************************************************/
/*                                                                            */
/*   This function will do a final clean up when test mode is finished.  It   */
/*   should be called when the test key is released or if the power supply    */
/*   overloads.  The ultrasonics will be turned off.  If there's an overload  */
/*   an alarm will be created otherwise switch to RDY screen.                 */
/*                                                                            */
/*   Exit condition:                                                          */
/*      Ultrasonics P/S:      In idle state (sonics off)                      */
/*      Amplitude Led:        Off                                             */
/*                                                                            */
/******************************************************************************/
{
   PSIdle();
   if (OLFlag == TRUE) {
      CreateAlarm(ER_OL);
   } 
   else
      SwitchScreen(SCREEN_RDY);
   
   StopAllTimers();
}



void CalcEnergy(void)
/******************************************************************************/
/*                                                                            */
/*   This function will calculate the energy used.  It will be called from    */
/*   the weld routines that need to know how much energy used so far.         */
/*                                                                            */
/*   Entry condition:                                                         */
/*       AccumulatedEnergy:  contains energy in 'energy units' added in by    */
/*                            1 ms interrupt                                  */
/*       EnergyUnitsNeeded:  Required energy in 'energy units' calculated at  */
/*                            the start of each cycle                         */
/*                                                                            */
/*   Exit condition:                                                          */
/*       EnergyFlag:         FALSE: energy not reached yet                    */
/*                           TRUE: energy value reached                       */
/*                                                                            */
/******************************************************************************/
{
	UINT32 TempVal=0;
	
		if ((((AccumulatedEnergy )/100)* Wattage) < MAX_ACCUMULED)
		{
			//currentLCDData.TotalEnergy = ((( AccumulatedEnergy + 100 ) / 100 ) * Wattage )/1000; 
			TempVal = ((( AccumulatedEnergy) / 100 ) * Wattage )  +  ((((AccumulatedEnergy % 100) * Wattage ))/100); 
			currentLCDData.TotalEnergy = TempVal/1000;
				
		}
		else
		{
			currentLCDData.TotalEnergy=MAX_ENERGY;
			AccumulatedEnergy=MAX_ACCUMULED;
		}
				
}

UINT32 ConverttoEnergy (UINT32 TempEnergy)
{
	return ((TempEnergy * (100000  / Wattage)) + ((100000% Wattage)* TempEnergy)/Wattage);
}

UINT8 CalcTimeProgress(UINT32 Partof, UINT32 Total )
{
	return ((((Partof*10)/Total)*10)+((((Partof*10)%Total)*10)/Total));
}

UINT8 CalcEnergyProgress(UINT32 Total )
{
	return (AccumulatedEnergy / ( (Total * (100000 / Wattage))/100));
}

void CalcTemperature(void)
/******************************************************************************/
/*                                                                            */
/*   This function will check if current temperature is reached to            */
/*    max temperature.  It will be called from the weld routines.             */
/*                                                                            */
/*   Entry condition:                                                         */
/*       Current Temperature:  contains value of current temp                 */
/*       MaxTempe:  Maximum temperature allowed                               */
/*                                                                            */
/*   Exit condition:                                                          */
/*       TemperatureFlag:    FALSE: tempe  not reached yet                    */
/*                           TRUE: tempe  value reached                       */
/*                                                                            */
/******************************************************************************/
{

	if (CurrentTemperature > currentLCDData.MaxTemp)
		currentLCDData.MaxTemp = CurrentTemperature;
	
	if (CurrentTemperature >= MaxTempe)
		TemperatureFlag=TRUE;

		
	if ((TemperatureFlag==TRUE) && ((WeldMode & SET_PUL_TEMP_NO_ZERO) == SET_PUL_TEMP_NO_ZERO))
	{
		PTemperatureFlag=TRUE;
	}
	
	if  ((WeldMode & SET_PUL_TEMP_NO_ZERO) == SET_PUL_TEMP_NO_ZERO)
		CheckPulseTemperature();
	
	if (CurrentTemperature < InitTemp)
		InitTemp=CurrentTemperature;
	
	
	
	if ((WeldMode & SET_PUL_TEMP_NO_ZERO) == SET_PUL_TEMP_NO_ZERO)
	{	
		if (CurrentTemperature >= PulseTempe)
		{
			PHysterisis=TRUE;
			PulsingMode = TRUE;
		}
		else
		{
			PulsingMode = FALSE;
			if (PHysterisis == TRUE && (CurrentTemperature <= (PulseTempe -10)))
				PHysterisis=FALSE;
		}

	}
	currentLCDData.CurrentTemp=CurrentTemperature;

}

static void VerifyPreset(void) 
/****************************************************************************/
/*                                                                          */
/*   This fuction is are called to perform checks on the Validity of        */
/*   Parametersthat are used in Different weldmodes.                        */
/****************************************************************************/
{
   
   UINT16 Alarm_Var = NO_ALARM;
   if (((TimeOut < Time) && (WeldMode & SET_TIME_NOT_ZERO) == SET_TIME_NOT_ZERO) || ((TimeOut < TotalOnTime) && (WeldMode & SET_ONTIME_NOT_ZERO) == SET_ONTIME_NOT_ZERO)) 
   {
	   CreateAlarm(ER_TIME_2);
	   StartFlag = FALSE;
   }
   else
	   if((Amplitude > MICROTIPLIMIT) && (CurrentPreset.presetData.Microtip == TRUE))
	   {
		   CreateAlarm(ER_uTIP);
		   StartFlag = FALSE;
	   }
	   else
		   if ((((WeldMode & CONFIG_TEMP)== CONFIG_TEMP)||((WeldMode & SET_MAXTEMP_NOT_ZERO)== SET_MAXTEMP_NOT_ZERO))&&(TempeProbePresent == FALSE))
		   {
			   CreateAlarm(ER_TEMP);
			   StartFlag = FALSE;
		   }
		   else
			   if((((WeldMode & CONFIG_TEMP)== CONFIG_TEMP)||((WeldMode & SET_MAXTEMP_NOT_ZERO)== SET_MAXTEMP_NOT_ZERO))&&(CurrentTemperature >= MaxTempe) )
			   {
				   CreateAlarm(ER_TEMPERROR);
				   StartFlag = FALSE;
			   }
    
}
      
void InitWeldParam(void)
/***************************************************************************/
/*                                                                         */
/*   This function is called at the begining of the cycle.  It will        */
/*   initialize the weld related parameters from preset and config data.   */
/*   Also Switch to Run Screen.                                            */
/***************************************************************************/
{ 
	Time = CurrentPreset.presetData.Time * MILLISEC_IN_ONE_SEC;
	Energy = CurrentPreset.presetData.Energy;
	OnTime = CurrentPreset.presetData.OnTime;
	OffTime = CurrentPreset.presetData.OffTime;
	OnEnergy = CurrentPreset.presetData.OnEnergy;
	TotalOnTime = CurrentPreset.presetData.TotalOnTime * MILLISEC_IN_ONE_SEC;
	TotalOnEnergy = CurrentPreset.presetData.TotalOnEnergy;
	PulseTempe = CurrentPreset.presetData.PulseTempe;
	MaxTempe = CurrentPreset.presetData.MaxTempe;
	Amplitude = CurrentPreset.presetData.AmplitudeValue;
	SetAmplitude((UINT8)Amplitude);           /* Output proper amplitude */
	WeldMode = CurrentPreset.presetData.WeldMode;
	AccumulatedEnergy=0;
	PartialEnergy=0;
	CycleUpdated = FALSE;
	
	currentLCDData.AmplitudeValue = (UINT8)Amplitude;      //Save it to Display
	TimeOut = (UINT32)Conf.ConfigData.TimeOut * MILLISEC_IN_ONE_SEC;
	
	PulseTempeIndex = 0;
	
	TemperatureFlag = FALSE;   //Added to get for next time...
	PTemperatureFlag = FALSE;
	
	if( StartFlag==TRUE )
	{
		VerifyPreset();
		if (AlarmFlag == TRUE)
			StartFlag==FALSE;
	}
	AmpUpdatedDuringcycleFlag = FALSE;
	ExperFlag = FALSE;
	PauseFlag = FALSE;
	FlagSwitch = FALSE;
	PulsingMode = FALSE;
	PHysterisis = FALSE;

}


void Calc_P_Tempe_ModeSettings(void)
/***************************************************************************/
/*                                                                         */
/*   This function is called at the begining of the cycle in Pulse         */
/*   Temperature mode.  It will calculate the Off Time, On time and        */
/*   Pulse temperatures to be used once preset pulse temperature is reached*/
/*   This is how Pulse temperature mode works:	                           */
/*   sonics come on until the pulse temp is reached, then pulses off and on*/
/*   the further away from the max temp the longer the on time, the closer */
/*   the actual is to the max temp the longer the off time.                */
/***************************************************************************/
{ 
   UINT32   T, D;
   UINT8 k;
   T = OnTime + OffTime;    
   D = (OnTime * 100)/T;
   
   if (( WeldMode == PULSE_TEMP_PS_TIME)|| ( WeldMode == PULSE_TEMP_CS_TIME))
	   for(k=0 ; k<10 ; k++) 
	   {
		   DOAON[k]=((9-k) * (long)T * D + 500)/ 1000;
	  
		   if (DOAON[k] ==0)
			   DOAON[k]=1;
	  
		   DOAOFF[k]= T-DOAON[k];
		   DOATEMP[k] = ((MaxTempe - PulseTempe)* k + 4)/9 + PulseTempe;
	
	   }
   else
   {
	   if (WeldMode==PULSE_TEMP_PS_ENERGY)
	   {
		   for(k=0 ; k<10 ; k++) 
		   {
			   //DOAON[k]=(OnEnergy/10) * (9 - k) ;
			   DOAON[k] = (OnEnergy * (9 - k) / 10) ;
			   if (DOAON[k] ==0)
				   DOAON[k]=1;
			   DOAOFF[k]=(OffTime/100) * (100 + (10*(k+1)));
			   DOATEMP[k] = ((MaxTempe - PulseTempe)* k + 4)/9 + PulseTempe;
		   }
	   }
	   else
		   if (WeldMode==PULSE_TEMP_CS_ENERGY)
		   {

			   for(k=0 ; k<10 ; k++) 
			   {
				   //DOAON[k]=(OnEnergy/10) * (9 - k) ;
				   DOAON[k]= (OnEnergy * (9 - k) / 10) ;
				   if (DOAON[k] ==0)
					   DOAON[k]=1;
				   DOAOFF[k]= OffTime + ((k)*100) ;
				   DOATEMP[k] = ((MaxTempe - PulseTempe)* k + 4)/9 + PulseTempe;
			   }
			   
		   }
   }
		   
}

void StartSonicsTimer(void)
/***************************************************************************/
/*                                                                         */
/*   This function is called from the Running Sonics on state.  It will    */
/*   start the correct timers and clear old energy values.  Finally it     */
/*   will turn on the ultrasonics.                                         */
/*                                                                         */
/*   Exit condition:                                                       */
/*      EnergyNeeded:       if energy mode then value of energy needed,    */
/*                          otherwise 0                                    */
/*      AccumulatedEnergy:  cleared to zero                                */
/*      EnergyCalc:         proper conversion based on wattage and model   */
/*      EnergyFlag:         FALSE (we haven't reached energy required)     */
/*      TimeFlag:           FALSE (we haven't timed out)                   */
/*      TimerSec1 started with timeout or max time out                     */
/*      TimerSec2 started with On time                                     */
/*      Ultrasonics:        On                                             */
/*                                                                         */
/***************************************************************************/
{
   
   bool PresetValid;
   
   currentLCDData.LastTemp = CurrentTemperature;
   currentLCDData.CurrentTemp = CurrentTemperature;
   currentLCDData.OnTime = OnTime;
   currentLCDData.OnEnergy = OnEnergy;
   currentLCDData.OffTime = OffTime;
   currentLCDData.HandHeld = HandHeld;
   currentLCDData.ValidResult = TRUE;
   TestResult = FALSE;
   ONorOFF = TRUE;
   TimeFlag = FALSE;
   PresetValid= FALSE;
   
   if (ExperFlag == FALSE)
   {
	   currentLCDData.Progress=0;
	   currentLCDData.MaxTemp=0;
	   StartMSTimer(RTOTALTIME, (UINT32)0);
	   StartMSTimer(TIMER_TIMEOUT, TimeOut);
	   currentLCDData.PeakPower=0;
   } 
   
   switch (WeldMode) {
   case CS_TIME:
	   
	   StartMSTimer(TIMER_TIME, Time);
	   PresetValid=TRUE;
	   if (TempeProbePresent==TRUE)
		   currentLCDData.IDWeldMode = ID_CS_TIME;
	   else
		   currentLCDData.IDWeldMode = ID_CS_TIME_B;
	   break;
   case	CS_ENERGY:
	   
	   PresetValid=TRUE;
	   if (TempeProbePresent==TRUE)
		   currentLCDData.IDWeldMode = ID_CS_ENERGY;
	   else
		   currentLCDData.IDWeldMode = ID_CS_ENERGY_B;
	   break;
   case CS_INFINITE:
	   PresetValid=TRUE;
	   if (TempeProbePresent==TRUE)
		   currentLCDData.IDWeldMode = ID_CS_INFINITE;
	   else
		   currentLCDData.IDWeldMode = ID_CS_INFINITE_B;
	   break;
   case PS_TIME:
	   if (ExperFlag == FALSE)
	   {
		   StartMSTimer(RTOTALONTIME, 0);
		   StartMSTimer(TIMER_TOTALONTIME, TotalOnTime);
		   PresetValid=TRUE;
		   PulsingMode = TRUE;
		   if (TempeProbePresent==TRUE)
			   currentLCDData.IDWeldMode = ID_PS_TIME;
		   else
			   currentLCDData.IDWeldMode = ID_PS_TIME_B;
	   }
	   if (FlagTimer2== OFF)
		   StartMSTimer(TIMER2, OnTime);
	   break;
   case PS_ENERGY:
	   PresetValid=TRUE;
	   PulsingMode = FALSE;
	   if (TempeProbePresent==TRUE)
		   currentLCDData.IDWeldMode = ID_PS_ENERGY;
	   else
		   currentLCDData.IDWeldMode = ID_PS_ENERGY_B;
	   if (ExperFlag == TRUE)
		   PartialEnergy=0;
	   else
	   {
		   currentLCDData.Progress=0;
		   currentLCDData.MaxTemp=0;
		   StartMSTimer(RTOTALONTIME, 0);
	   }
	   break;
   case PS_INFINITE_TIME:
	   if (ExperFlag == FALSE)
	   {
		   StartMSTimer(RTOTALONTIME, 0);
		   PresetValid=TRUE;
		   PulsingMode = TRUE;
		   if (TempeProbePresent==TRUE)
			   currentLCDData.IDWeldMode = ID_PS_INFINITE_TIME;
		   else
			   currentLCDData.IDWeldMode = ID_PS_INFINITE_TIME_B;
	   }
	   StartMSTimer(TIMER2, OnTime);
	   break;
   case PS_INFINITE_ENERGY:
	   PulsingMode = FALSE;
	   if (ExperFlag==FALSE)
	   {
		   StartMSTimer(RTOTALONTIME, 0);
		   PresetValid=TRUE;
		   if (TempeProbePresent==TRUE)
			   currentLCDData.IDWeldMode = ID_PS_INFINITE_ENERGY;
		   else
			   currentLCDData.IDWeldMode = ID_PS_INFINITE_ENERGY_B;
	   }
	   PartialEnergy=0;
	   break;
   case MAX_TEMP_CS:
	   InitTemp=CurrentTemperature;
	   PresetValid=TRUE;
	   currentLCDData.IDWeldMode = ID_MAX_TEMP_CS;
	   break;
   case MAX_TEMP_PS_TIME: 
	   if (ExperFlag == FALSE)
	   {
		   InitTemp = CurrentTemperature;
		   StartMSTimer(RTOTALONTIME, 0);
		   PresetValid = TRUE;
		   PulsingMode = TRUE;
		   currentLCDData.IDWeldMode = ID_MAX_TEMP_PS_TIME;
	   }
	   if (FlagTimer2== OFF)
	   StartMSTimer(TIMER2, OnTime);
	   break;
	case MAX_TEMP_PS_ENERGY:
		PulsingMode = FALSE;
		if (ExperFlag==FALSE)	
		{
			InitTemp=CurrentTemperature;
			StartMSTimer(RTOTALONTIME, 0);
			PresetValid=TRUE;
			currentLCDData.IDWeldMode = ID_MAX_TEMP_PS_ENERGY;
		}
		PartialEnergy=0;
		break;
	case TEMP_LIMIT_CS_TIME:
	   if (ExperFlag == FALSE)
	   {
		   StartMSTimer(RTOTALONTIME, 0);
		   StartMSTimer(TIMER_TIME, Time);
		   PresetValid=TRUE;
		   currentLCDData.IDWeldMode = ID_TEMP_LIMIT_CS_TIME;
	   }
	   break;
   case TEMP_LIMIT_CS_ENERGY:
	   if (ExperFlag == FALSE)
	   {
		   StartMSTimer(RTOTALONTIME, 0);
		   PresetValid=TRUE;
		   currentLCDData.IDWeldMode = ID_TEMP_LIMIT_CS_ENERGY;
	   }
	   break;
   case TEMP_LIMIT_PS_TIME:
	   if (ExperFlag == FALSE)
	   {
		   StartMSTimer(RTOTALONTIME, 0);
		   StartMSTimer(TIMER_TOTALONTIME, TotalOnTime);
		   StartMSTimer(TIMER2, OnTime);//this line can be deleted
		   PresetValid=TRUE;
		   PulsingMode = TRUE;
		   currentLCDData.IDWeldMode = ID_TEMP_LIMIT_PS_TIME;
	   }
	   if (FlagTimer2== OFF)
		   StartMSTimer(TIMER2, OnTime);
	   break;
   case TEMP_LIMIT_PS_ENERGY:
	   PulsingMode = FALSE;
	   if (ExperFlag == FALSE)
	   {
		   StartMSTimer(RTOTALONTIME, 0);
	   }
	   PresetValid=TRUE;
	   currentLCDData.IDWeldMode = ID_TEMP_LIMIT_PS_ENERGY;
	   break;
   case PULSE_TEMP_CS_TIME:
	   if (ExperFlag == FALSE)
	   {
		   OnTime=DEFALT_ONTIME_CS;
		   OffTime=0;
		   TempOnTime=DEFALT_ONTIME_CS;
		   TempOffTime=0;
		   Calc_P_Tempe_ModeSettings();
		   StartMSTimer(TIMER_TIME, Time);
		   StartMSTimer(RTOTALONTIME, 0);
		   PresetValid=TRUE;
		   PulsingMode = FALSE;
		   currentLCDData.IDWeldMode = ID_PULSE_TEMP_CS_TIME;
	   }
	   else
	   {
		   PulsingMode = TRUE;
		   if (PHysterisis == TRUE)
		   {
			   TempOnTime=DOAON[PulseTempeIndex-1];
		   }
		   else
			   TempOnTime=OnTime;
		   StartMSTimer(TIMER2, TempOnTime);
	   }
	   break;
   case PULSE_TEMP_CS_ENERGY:
	   PulsingMode = TRUE;
	   if (ExperFlag == FALSE)
	   {
		   TempOnEnergy=Energy;
		   OnEnergy=0;
		   StartMSTimer(RTOTALONTIME, 0);
		   StartMSTimer(TIMER2, MILLISEC_IN_ONE_SEC);
	   }
	   else
	   {
		   if (PHysterisis == TRUE)
			   TempOnEnergy=DOAON[PulseTempeIndex-1];
		   else
		   {
			   TempOnEnergy=Energy;
			   ExperFlag=FALSE;
		   }
	   }
	   PresetValid=TRUE;
	   currentLCDData.IDWeldMode = ID_PULSE_TEMP_CS_ENERGY;
	   
	   break;
   case PULSE_TEMP_PS_TIME:
	   if (ExperFlag == FALSE)
	   {
		   Calc_P_Tempe_ModeSettings();
		   TempOnTime=OnTime;
		   TempOffTime=OffTime;
		   StartMSTimer(RTOTALONTIME, 0);
		   StartMSTimer(TIMER_TOTALONTIME, TotalOnTime);
		   PresetValid=TRUE;
		   PulsingMode = TRUE;
		   currentLCDData.IDWeldMode = ID_PULSE_TEMP_PS_TIME;
	   }
	   else
		   if (PHysterisis == TRUE && PulseTempeIndex >0)
			   TempOnTime=DOAON[PulseTempeIndex-1];
		   else
			   TempOnTime=OnTime;
	   
	   StartMSTimer(TIMER2, TempOnTime);
	   break;
   case PULSE_TEMP_PS_ENERGY:
	   PulsingMode = TRUE;
	   if (ExperFlag == FALSE)
	   {
		   Calc_P_Tempe_ModeSettings();
		   TempOnEnergy=OnEnergy;
		   TempOffTime=OffTime;
		   StartMSTimer(RTOTALONTIME, 0);
	   }
	   else
		   if (PHysterisis == TRUE )
			   TempOnEnergy=DOAON[PulseTempeIndex-1];
		   else
			   TempOnEnergy=OnEnergy;
	   
	   PresetValid=TRUE;
	   currentLCDData.IDWeldMode = ID_PULSE_TEMP_PS_ENERGY;
	   break;
   }

   if( AlarmFlag == FALSE)
	   SwitchScreen(SCREEN_RESULTS);

   FlagSwitch = FALSE;
   SetPSRun();  /* Turn on sonics */
   if(Conf.ConfigData.EndOfCycle!=2 || HandHeld == TRUE)
	   SetUserOut();                                /* Output weld on signal */
   UpdateLiveReady = TRUE;
}

void PauseTimers(void)
/***************************************************************************/
/*                                                                         */
/*   This function will Pause the timers those are in On condition.        */
/*   Function will be called from Pause state.                             */
/*                                                                         */
/***************************************************************************/
{
	if(FlagTimer1 == ON)     
		PauseMSTimer(TIMER1);
	if(FlagTimer2== ON)
		PauseMSTimer(TIMER2);
	if(Flag_RTotalTime== ON)
		PauseMSTimer(RTOTALTIME);
	if(Flag_RTotalOnTime== ON)
		PauseMSTimer(RTOTALONTIME);
	if(FlagTime== ON)
		PauseMSTimer(TIMER_TIME);
	if(FlagTimeOut== ON)
		PauseMSTimer(TIMER_TIMEOUT);
	if(FlagTotalOnTime== ON)
		PauseMSTimer(TIMER_TOTALONTIME);
   PSIdle();                                         /* Turn on sonics */
   ClrUserOut();                                /* Output weld on signal */
}

void ResumeTimers(void)
/***************************************************************************/
/*                                                                         */
/*   This function will Resume the timers those are in Pause condition.    */
/*   Function will be called from Pause state when Pause key is pressed    */
/*   again to resume the cycle.                                            */
/***************************************************************************/
{
   //PrintStr("ResumeTimer ");
   PauseFlag = FALSE;	
   
	if(Flag_RTotalTime== PAUSE)
		ResumeMSTimer(RTOTALTIME);
	if (ONorOFF == TRUE)
	{
		if(Flag_RTotalOnTime== PAUSE)
			ResumeMSTimer(RTOTALONTIME);
		if(FlagTotalOnTime== PAUSE)
			ResumeMSTimer(TIMER_TOTALONTIME);
	}
	if(FlagTime== PAUSE)
		ResumeMSTimer(TIMER_TIME);
	if(FlagTimer1 == PAUSE)     
		ResumeMSTimer(TIMER1);
	if(FlagTimer2== PAUSE)
		ResumeMSTimer(TIMER2);
	if(FlagTimeOut== PAUSE)
		ResumeMSTimer(TIMER_TIMEOUT);

   if(ONorOFF == TRUE)
   {  //Turn Sonics On Again.
      ClrReady();
      SetPSRun();                                         /* Turn on sonics */
      SetUserOut();                                /* Output weld on signal */
   }
}


void StartHoldTimer(void)
/***************************************************************************/
/*                                                                         */
/*   This function will start the proper timer with the off value.         */
/*   If the value is zero, then the timer will not be                      */
/*   started but the TimeFlag will be set to FALSE.                        */
/*                                                                         */
/*   Exit condition:                                                       */
/*      TimeFlag:       FALSE unless requested time is 0, then TRUE        */
/*           :          TimerSec2 started with Off time                    */
/*      Ultrasonics:    Off                                                */
/*                                                                         */
/***************************************************************************/
{
   ONorOFF = FALSE;
   TimeFlag = FALSE;                     /* Assume a timer will be started */
    
   CalcEnergy();
   switch (WeldMode) {
	   case PS_TIME:
		   if (FlagTotalOnTime == OFF)
			   TimeFlag = TRUE;
		   else
			   if (FlagTimer2== OFF)
				   StartMSTimer(TIMER2, OffTime);
		   break;   
	   case PS_ENERGY:
		   //if (AccumulatedEnergy >= (TotalOnEnergy * (100000 / Wattage)))
		   if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
			   TimeFlag=TRUE;
		   else
			   if (FlagTimer2== OFF)
				   StartMSTimer(TIMER2, OffTime);
		   break;
	   case PS_INFINITE_TIME:
	   case PS_INFINITE_ENERGY:
		   if (FlagTimer2== OFF)
			   StartMSTimer(TIMER2, OffTime);
		   break;
	   case MAX_TEMP_PS_TIME:
	   case MAX_TEMP_PS_ENERGY:
		   if (TemperatureFlag == TRUE)
			   TimeFlag = TRUE;
		   else
			   if (FlagTimer2== OFF)
				   StartMSTimer(TIMER2, OffTime);
		   break;
	   case TEMP_LIMIT_CS_TIME:
		   if (TemperatureFlag == TRUE)
			   PauseMSTimer(TIMER_TIME);
		   else
			   TimeFlag = TRUE;
		   break;
	   case TEMP_LIMIT_CS_ENERGY:
		   if (TemperatureFlag == FALSE)
			   TimeFlag = TRUE;
		   break;
	   case TEMP_LIMIT_PS_TIME:
		   if (FlagTotalOnTime == OFF)
			   TimeFlag = TRUE;
		   else
			   if (TemperatureFlag == TRUE)
				   PauseMSTimer(TIMER2);
			   else
				   if (FlagTimer2== OFF)
					   StartMSTimer(TIMER2, OffTime);
		   break;
	   case TEMP_LIMIT_PS_ENERGY:
		   if (TemperatureFlag == FALSE)
			   //if (AccumulatedEnergy >= (TotalOnEnergy * 100000 / Wattage))
			   if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
				   TimeFlag=TRUE;
			   else
				   if (FlagTimer2== OFF)
					   StartMSTimer(TIMER2, OffTime);
		   break;
	   case PULSE_TEMP_CS_TIME:
		   if (FlagTime == OFF)
			   TimeFlag = TRUE;
		   else
			   if (TemperatureFlag == TRUE)
			   {
				   PauseMSTimer(TIMER2);
				   PauseMSTimer(TIMER_TIME);
			   }
			   else
			   {
				   PauseMSTimer(TIMER_TIME);
				   TempOffTime=DOAOFF[PulseTempeIndex-1];
				   if (FlagTimer2== OFF)
					   StartMSTimer(TIMER2, TempOffTime);
			   }
		   break;
	   case PULSE_TEMP_CS_ENERGY:
		   if (TemperatureFlag == FALSE)
			   if (AccumulatedEnergy >= ConverttoEnergy(Energy))
				   TimeFlag=TRUE;
			   else
			   {
				   TempOffTime=DOAOFF[PulseTempeIndex-1];
				   if (FlagTimer2== OFF)
					   StartMSTimer(TIMER2, TempOffTime);
			   }
		   break;
	   case PULSE_TEMP_PS_TIME:
		   if (FlagTotalOnTime == OFF)
			   TimeFlag = TRUE;
		   else
			   if (TemperatureFlag == TRUE)
				   PauseMSTimer(TIMER2);
			   else
			   {
				   if ( PHysterisis == TRUE && PulseTempeIndex>0)
					   TempOffTime=DOAOFF[PulseTempeIndex-1];
				   else
					   TempOffTime=OffTime;
				   if (FlagTimer2== OFF)
					   StartMSTimer(TIMER2, TempOffTime);
			   }
				   
		   break;
	   case PULSE_TEMP_PS_ENERGY:
		   if (TemperatureFlag == FALSE)
			   //if (AccumulatedEnergy >= (TotalOnEnergy * 100000 / Wattage))
			   if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
				   TimeFlag=TRUE;
			   else
			   {
				   if ( PHysterisis == TRUE && PulseTempeIndex>0)
					   TempOffTime=DOAOFF[PulseTempeIndex-1];
				   else
					   TempOffTime=OffTime;
				   if (FlagTimer2== OFF)
					   StartMSTimer(TIMER2, TempOffTime);
			   }
		   break;
	   default:
		   TimeFlag = TRUE;
		   break;
      }
   FlagSwitch = FALSE;
   PSIdle();			/* Stop the ultrasonics */
   if (Conf.ConfigData.EndOfCycle == 0 && HandHeld == FALSE) 
	   ClrUserOut();                         /* Remove weldon output */
   UpdateLiveReady = TRUE;
}

void CheckPulseTemperature(void)
/******************************************************************************/
/*                                                                            */
/*   This function will check if current temperature is reached to            */
/*    Pulse temperature in pulse temp mode. It will be called from the        */
/*    weld routines.                                                          */
/*   Entry condition:                                                         */
/*       Current Temperature:  contains value of current temp                 */
/*       DOA:  Array contains on, off and pulse temp.                         */
/*       PulseTempeIndex: Index of current step in which pulse temp experiment is.*/
/*   Exit condition:                                                          */
/*       PTemperatureFlag:    FALSE: tempe  not reached yet                   */
/*                           TRUE: tempe  value reached                       */
/*       PulseTempeIndex:    Increment index by one if not at max.            */
/******************************************************************************/
{
	if( PHysterisis == TRUE)
	{
		if (PTemperatureFlag==FALSE)
		{
			if (CurrentTemperature >= DOATEMP[PulseTempeIndex])
			{  			
				if(PulseTempeIndex < 9) 
					PulseTempeIndex += 1;
			}
		}
		else
		{
			PulseTempeIndex=8;
		}
	}
	else
	{
		TempOnTime=OnTime;
		TempOffTime=OffTime;
	}
			
}  


void CheckOnParam(void)
/***************************************************************************/
/*                                                                         */
/*   This function will check on timers,energy and temp values to determine if  */
/*   the main parameter conditions have been met.  If they have, TimeFlag  */
/*   is set to TRUE.  This function must also call GetInputs() to check on */
/*   overloads  .  This function will be called from the                   */
/*   Running Sonics On state.                                              */
/*                                                                         */
/*   Exit condition:                                                       */
/*      TimeFlag:     TRUE: parameters met, stop sonics                    */
/*                    FALSE: parameters not met, continue sonics           */
/*      OLFlag:       TRUE: if overload detected                           */
/*                                                                         */
/***************************************************************************/
{
	INPUT_CODE   Inputs;
	UINT8 percent=0;
	
	CalcTemperature();
	
	
	Inputs = GetInputs();  //Read inputs to check on overload and ground detect 
	if ((Inputs & OVERLOAD_IN) == OVERLOAD_IN) 
		OLFlag = TRUE;     
	
	if (FlagTimeOut==FALSE)
	{
		CreateAlarm(ER_TIMEOUTREACH);
		StopFlag=TRUE;
	}
		
	if ((((WeldMode & CONFIG_TEMP)== CONFIG_TEMP)||((WeldMode & SET_MAXTEMP_NOT_ZERO)== SET_MAXTEMP_NOT_ZERO))&&(TempeProbePresent == FALSE))
	{
		CreateAlarm(ER_TEMP);
		StopFlag=TRUE;
	}
   
	switch (WeldMode) {
	case CS_TIME:
		if (FlagTime == OFF)
			TimeFlag=TRUE;
		percent = CalcTimeProgress (Timer_RTotalTime, Time);
		break;
	case CS_ENERGY:
		if (FlagTimeOut == OFF)
			TimeFlag = TRUE;
		if (AccumulatedEnergy >= ConverttoEnergy(Energy))
			TimeFlag = TRUE;
		percent = CalcEnergyProgress (Energy);
		break;
	case CS_INFINITE:
		break;
	case PS_TIME:
		if (FlagTotalOnTime == OFF || FlagTimer2 == OFF)
		{
			TimeFlag = TRUE;
		}
		percent = CalcTimeProgress (Timer_RTotalOnTime, TotalOnTime);
		
		break;
	case PS_ENERGY:
		if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
			TimeFlag = TRUE;
		if (PartialEnergy >= (OnEnergy * (100000 / Wattage)))
			TimeFlag = TRUE;
		if (TimeFlag==TRUE)
			PulsingMode = TRUE;
		percent = CalcEnergyProgress (TotalOnEnergy);
		
		break;
	case PS_INFINITE_TIME: 
		if (FlagTimer2 == OFF)
			TimeFlag = TRUE;
		break;
	case PS_INFINITE_ENERGY:
		if (PartialEnergy >= (OnEnergy * (100000 / Wattage)))
		{
			TimeFlag = TRUE;
			PulsingMode = TRUE;
		}
		break;
	case MAX_TEMP_CS:
		if (TemperatureFlag==TRUE)
			TimeFlag=TRUE;
		percent = (UINT8)(((UINT32)CurrentTemperature - (UINT32)InitTemp) * 100 /  ( MaxTempe - InitTemp)); 
		break;
	case MAX_TEMP_PS_TIME:
		if (TemperatureFlag==TRUE)
			TimeFlag=TRUE;
		else
			if (FlagTimer2 == OFF)
				TimeFlag = TRUE;
		percent =(UINT8)(((UINT32)CurrentTemperature - (UINT32)InitTemp) * 100 /  ( MaxTempe - InitTemp)); 
		break;
	case MAX_TEMP_PS_ENERGY:
		if (TemperatureFlag==TRUE)
			TimeFlag=TRUE;
		else
			if (PartialEnergy >= (OnEnergy * (100000 / Wattage)))
			{
				TimeFlag = TRUE;
				PulsingMode = TRUE;
			}
		
		percent = (UINT8)(((UINT32)CurrentTemperature - (UINT32)InitTemp) * 100 /  ( MaxTempe - InitTemp));
		break;
	case TEMP_LIMIT_CS_TIME:
		if (FlagTime == OFF)
			TimeFlag = TRUE;
		else
			if (TemperatureFlag==TRUE)
			{
				TimeFlag=TRUE;
				CycleUpdated=TRUE;
			}
		percent = CalcTimeProgress (Timer_RTotalOnTime,Time);  	
		
		break;
	case TEMP_LIMIT_CS_ENERGY:
		if (FlagTimeOut == OFF)
			TimeFlag = TRUE;
		else
			if (AccumulatedEnergy >= ConverttoEnergy(Energy))
				TimeFlag = TRUE;
			else
				if (TemperatureFlag==TRUE)
					TimeFlag=TRUE;
		percent = CalcEnergyProgress (Energy);
		break;
   case TEMP_LIMIT_PS_TIME:
		if (FlagTotalOnTime == OFF || FlagTimer2 == OFF)
			TimeFlag = TRUE;
		else
			if (TemperatureFlag==TRUE)
			{
				TimeFlag=TRUE;
				CycleUpdated=TRUE;
				PauseMSTimer(TIMER2);
			}
		percent = CalcTimeProgress (Timer_RTotalOnTime,TotalOnTime);
		break;
   case TEMP_LIMIT_PS_ENERGY:
	   if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
		   TimeFlag = TRUE;
	   if (PartialEnergy >= (OnEnergy * (100000 / Wattage)))
	   {
		   TimeFlag = TRUE;
		   PulsingMode = TRUE;
	   }
	   if (TemperatureFlag==TRUE)
		   TimeFlag=TRUE;
	   
	   percent = CalcEnergyProgress (TotalOnEnergy);
	   break;
   case PULSE_TEMP_CS_TIME:
	   if (FlagTime == OFF )
		   TimeFlag = TRUE;
	   else
		   if (FlagTimer2 == OFF && PHysterisis == TRUE )
		   {
			   TimeFlag = TRUE;
			   CycleUpdated=TRUE;
		   }
		   else
			   if (TemperatureFlag==TRUE)
			   {
				   TimeFlag=TRUE;
				   CycleUpdated=TRUE;
				   
			   } 
	   percent = CalcTimeProgress (Timer_RTotalOnTime,Time);
	   
	   break;
   case PULSE_TEMP_CS_ENERGY:
	   if (AccumulatedEnergy >= ConverttoEnergy(Energy))
   		   TimeFlag = TRUE;
	   
	   if (ExperFlag==FALSE )
	   {
		   if (FlagTimer2 == OFF )
		   {
			   if (OnEnergy == 0)
			   {
				   OnEnergy=(AccumulatedEnergy * Wattage )/100000;
				   OffTime=100;
				   Calc_P_Tempe_ModeSettings();
				   PulseTempeIndex=0;  
			   }
			   else
			   {
				   if ( PHysterisis == TRUE &&  PulseTempeIndex > 0 )
					   TimeFlag = TRUE; 
				   
			   }
		   }
	   }
	   else
	   {
		   if ( PHysterisis == TRUE )
			   if (PartialEnergy >= (TempOnEnergy * (100000 / Wattage)))
				   TimeFlag = TRUE;
		   				   
		   
	   }
	   if (TemperatureFlag==TRUE)
	   		TimeFlag=TRUE;
	   percent = CalcEnergyProgress (Energy);
	   break;
   case PULSE_TEMP_PS_TIME:
	   
	   if (FlagTotalOnTime == OFF || FlagTimer2 == OFF)
	   {
		   TimeFlag = TRUE;
		   CycleUpdated=TRUE;
	   }
	   else
		   if (TemperatureFlag==TRUE)
		   {
			   TimeFlag=TRUE;
			   CycleUpdated=TRUE;
		   } 
	   percent = CalcTimeProgress (Timer_RTotalOnTime, TotalOnTime);
	   
	   break;
   case PULSE_TEMP_PS_ENERGY:
	   if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
		   TimeFlag = TRUE;
	   if (PartialEnergy >= (TempOnEnergy * (100000 / Wattage)))
		   TimeFlag = TRUE;
	   if (TemperatureFlag==TRUE)
		   TimeFlag=TRUE; 
	   percent = CalcEnergyProgress (TotalOnEnergy);
	   break;
   }
	
	if (TimeFlag==TRUE)
	{
	   if(Flag_RTotalOnTime== ON)
	      	PauseMSTimer(RTOTALONTIME);
	   if(FlagTotalOnTime== ON)
	   		PauseMSTimer(TIMER_TOTALONTIME);
	   
	}
	CalcEnergy();                              /* Update energy calculation */
	CalcPower();                               /* Check for new peak power  */
	if (percent >= HUNDRED_PERCENT)
		currentLCDData.Progress =NINETY_NINE_PERCENT;
	else
		currentLCDData.Progress=percent;
	
}



void CheckExperiment(void)
/***************************************************************************/
/*                                                                         */
/*   This will check to see if an experiment is running.  If it is, it     */
/*   then checks to see if there's enough time to run another Sonics on    */
/*   cycle.  The off time will not be considered.  If there is enough time */
/*   left in the experiment timer to run the full on time then another     */
/*   cycle will be started.  In the case of energy mode, the maximum       */
/*   timeout will be used.  This is called from the Experiment state.      */
/*                                                                         */
/*   Entry condition:                                                      */
/*      TimerSecFlag1:    TRUE: experiment still running, but still don't  */
/*                              know if enough time for another pass       */
/*                        FALSE: experiment not running, maybe never       */
/*                              started, maybe timed out                   */
/*      CurrentMode:      MODE_P_TIME, MODE_P_TEMPE, or MODE_P_ENERGY then */
/*                              experiment was started                     */
/*                                                                         */
/*                                                                         */
/*   Exit condition:                                                       */
/*      ExperFlag:        TRUE: experiment still running                   */
/*                        FALSE: experiment stopped or never started       */
/*      StartFlag:        FALSE: make sure another cycle does not start    */
/*                               if not in experiment mode                 */
/*      StopFlag:        TRUE: make sure another cycle does not start      */
/*                               if not in experiment mode                 */
/*                                                                         */
/***************************************************************************/
{
	CalcEnergy();                              /* Update energy calculation */
   switch (WeldMode) {
	   case PS_TIME:
		   if (FlagTotalOnTime == OFF)
			   FinishCicle();
		   else
		   {
			   ResumeMSTimer(TIMER_TOTALONTIME);
			   ResumeMSTimer(RTOTALONTIME);
			   ExperFlag = TRUE;
		   }
		   break;
	   case PS_ENERGY:
		   if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
			   FinishCicle();
		   else
		   {
			   ResumeMSTimer(RTOTALONTIME);
			   ExperFlag = TRUE;
		   }
		   break;   
	   case PS_INFINITE_TIME:
	   case PS_INFINITE_ENERGY:
		   ResumeMSTimer(RTOTALONTIME);
		   ExperFlag = TRUE;
		   break;
	   case	MAX_TEMP_PS_TIME:
	   case MAX_TEMP_PS_ENERGY:
		   if (TemperatureFlag==TRUE)
			   FinishCicle();
		   else
		   {
			   ResumeMSTimer(RTOTALONTIME);
			   ExperFlag = TRUE;
		   }
		   break;
	   case TEMP_LIMIT_CS_TIME:
		   if (TemperatureFlag==TRUE)
		   {
			   TemperatureFlag=FALSE;		
			   ExperFlag = TRUE;
			   ResumeMSTimer(TIMER_TIME);
		   }
		   else
			   FinishCicle();
		   break;
	   case TEMP_LIMIT_CS_ENERGY:
		   if (TemperatureFlag==TRUE)
		   {
			   TemperatureFlag=FALSE;		
			   ExperFlag = TRUE;
		   }
		   else
			   FinishCicle();
		   break;
	   case TEMP_LIMIT_PS_TIME:
		   if (TemperatureFlag==TRUE)
		   {
			   TemperatureFlag=FALSE;
			   ExperFlag = TRUE;
			   ResumeMSTimer(RTOTALONTIME);
			   ResumeMSTimer(TIMER_TOTALONTIME);
			   ResumeMSTimer(TIMER2);
			   
		   }
		   else
			   if (FlagTotalOnTime == OFF)
			   {
				   FinishCicle();
			   }
			   else
			   {
				   ResumeMSTimer(TIMER_TOTALONTIME);
				   ResumeMSTimer(RTOTALONTIME);
				   ResumeMSTimer(TIMER2);
				   ExperFlag = TRUE;
			   }
		   break;
	   case TEMP_LIMIT_PS_ENERGY:
		   if (TemperatureFlag==TRUE)
		   {
			   ResumeMSTimer(RTOTALONTIME);
			   TemperatureFlag=FALSE;	
			   ExperFlag = TRUE;
		   }
		   else
			   if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
				   FinishCicle();
			   else
			   {
				   
				   PartialEnergy=0;
				   ResumeMSTimer(RTOTALONTIME);
				   ExperFlag = TRUE;
			   }
		   break;
	   case PULSE_TEMP_CS_TIME:
		   if (TemperatureFlag==TRUE)
		   {
			   TemperatureFlag=FALSE;
			   ResumeMSTimer(RTOTALONTIME);
			   ResumeMSTimer(TIMER_TIME);
			   ExperFlag = TRUE;
		   }
		   else
			   if (FlagTime == OFF)
			   {
				   FinishCicle();
			   }
			   else
			   {
				   ResumeMSTimer(TIMER_TIME);
				   ResumeMSTimer(RTOTALONTIME);
				   ExperFlag = TRUE;
			   }
		   break;
	   case PULSE_TEMP_CS_ENERGY:
		   if (TemperatureFlag==TRUE)
		   {
			   ResumeMSTimer(RTOTALONTIME);
			   PartialEnergy=0;
			   TemperatureFlag=FALSE;	
			   ExperFlag = TRUE;
		   }
		   else
			   if (AccumulatedEnergy >= ConverttoEnergy(Energy))
				   FinishCicle();
			   else
			   {   
				   PartialEnergy=0;
				   ResumeMSTimer(RTOTALONTIME);
				   ExperFlag = TRUE;
			   }
		   break;
	   case PULSE_TEMP_PS_TIME:
		   if (TemperatureFlag==TRUE)
		   {
			   TemperatureFlag=FALSE;
			   ExperFlag = TRUE;
			   ResumeMSTimer(RTOTALONTIME);
			   StopMSTimer(TIMER2);
			   ResumeMSTimer(TIMER_TOTALONTIME);
		   }
		   else
			   if (FlagTotalOnTime == OFF)
			   {
				   FinishCicle();
			   }
			   else
			   {
				   ResumeMSTimer(TIMER_TOTALONTIME);
				   ResumeMSTimer(RTOTALONTIME);
				   StopMSTimer(TIMER2);
				   ExperFlag = TRUE;
			   }
		   break;
	   case PULSE_TEMP_PS_ENERGY:
		   if (TemperatureFlag==TRUE)
		   {
			   ResumeMSTimer(RTOTALONTIME);
			   PartialEnergy=0;
			   TemperatureFlag=FALSE;	
			   ExperFlag = TRUE;
		   }
		   else
			   if (AccumulatedEnergy >= ConverttoEnergy(TotalOnEnergy))
				   FinishCicle();
			   else
			   {
		   				   
				   PartialEnergy=0;
				   ResumeMSTimer(RTOTALONTIME);
				   ExperFlag = TRUE;
			   }
		   break;
  	   default:
  		 FinishCicle();
  		   
  		 break;
   }
   if (FlagTimeOut==OFF)
	   FinishCicle();
}

void FinishCicle(void)
{
	UINT16   ValidKeys;
	UINT32   Aux_Value;
	ValidKeys = GetKeys();
	Beep(RELEASEBEEP);           /* Signal user, ok to remove start   */
    EndOfWeldSignal();           /*Check if reg 19 is set to 2 (End of weld pulse)*/
    
    if (CurrentTemperature >= currentLCDData.MaxTemp)
    {
    	currentLCDData.MaxTemp = CurrentTemperature;
    	currentLCDData.LastTemp = currentLCDData.MaxTemp;
    }
    else
    	currentLCDData.LastTemp = CurrentTemperature;
	currentLCDData.Progress = HUNDRED_PERCENT;
	if (CycleUpdated==FALSE)
	{
		switch (WeldMode) {
			case CS_TIME:
				currentLCDData.TotalTime = Time;
				break;
			case PS_TIME:
				currentLCDData.TotalOnTime= TotalOnTime;
				Aux_Value = (TotalOnTime / OnTime);
				if ((TotalOnTime % OnTime) == 0)
					Aux_Value -= 1;
				currentLCDData.TotalTime= TotalOnTime + (Aux_Value * OffTime) ;
				break;
			case TEMP_LIMIT_CS_TIME:
				currentLCDData.TotalTime = Time;
				break;
			case TEMP_LIMIT_PS_TIME:
				currentLCDData.TotalOnTime= TotalOnTime;
				Aux_Value = (TotalOnTime / OnTime);
				if ((TotalOnTime % OnTime) == 0)
					Aux_Value -= 1;
				currentLCDData.TotalTime= TotalOnTime + (Aux_Value * OffTime) ;
				break;
			case PULSE_TEMP_CS_TIME:
				currentLCDData.TotalTime = Time;
				break;
			case PULSE_TEMP_PS_TIME:
				currentLCDData.TotalOnTime= TotalOnTime;
				if ((TotalOnTime % OnTime) == 0)
					Aux_Value -= 1;
				currentLCDData.TotalTime= TotalOnTime + (Aux_Value * OffTime) ;
				break;

		}
	}
	else
	{
		if (PULSE_TEMP_CS_TIME == WeldMode)
		{
			currentLCDData.TotalOnTime=Time;
		}
	}
	ExperFlag = FALSE;
	if (Conf.ConfigData.PulseStart == TRUE) 
	{    /* Is Pulse Start allowed ?          */
		if (ValidKeys == NO_KEY)
			StopFlag = TRUE;
	}
}

void CountOL(void)
/***************************************************************************/
/*                                                                         */
/*   This function will count the number of consecutive overloads.  Every  */
/*   call to it will increment the counter if experiment is running.       */
/*   ResetOL() will clear the counter.  If the counter reaches OL_LIMIT    */
/*   then an overload alarm is created.                                    */
/*                                                                         */
/*   Exit condition:                                                       */
/*      OverloadCounter:    Number of consecutive overloads so far         */
/*      AlarmFlag:          TRUE: if OL_LIMIT reached                      */
/*                                                                         */
/***************************************************************************/
{

   PSReset();
}


void CheckOffParam(void)
/***************************************************************************/
/*                                                                         */
/*   This function will check the off timers to see if they have           */
/*   expired. If in me than wait for temp to come down 3degree from max temp */
/*   Exit condition:                                                       */
/*      TimeFlag:   TRUE: indicates time has expired                       */
/*                  FALSE: time still going                                */
/*                                                                         */
/***************************************************************************/
{
	//CalcTemperature();
	
	if (CurrentTemperature > currentLCDData.MaxTemp)
				currentLCDData.MaxTemp = CurrentTemperature;
	
	if (FlagTimeOut==FALSE)
	{
		CreateAlarm(ER_TIMEOUTREACH);
		StopFlag=TRUE;
	}
	
	if (MaxTempe != 0 && TempeProbePresent == FALSE )
	{
		CreateAlarm(ER_TEMP);
		StopFlag=TRUE;
	}
	
   switch (WeldMode) {
      case CS_TIME:
      case CS_ENERGY:
      case CS_INFINITE:
      case MAX_TEMP_CS:
    	  TimeFlag = TRUE;
    	  break;
      case PS_TIME:
      case PS_ENERGY:
      case PS_INFINITE_TIME:
      case PS_INFINITE_ENERGY:
      case MAX_TEMP_PS_TIME:
      case MAX_TEMP_PS_ENERGY:
    	  if (FlagTimer2 == OFF)
    		  TimeFlag=TRUE;
    	  break;
      case TEMP_LIMIT_CS_TIME:
      case TEMP_LIMIT_CS_ENERGY:
    	  if (TemperatureFlag == TRUE)
    	  {
    		  if (currentLCDData.CurrentTemp <= (MaxTempe - Three_degrees_F))
    			  TimeFlag=TRUE;
    	  }
    	  else
    		  TimeFlag=TRUE;
    	  break;
      case TEMP_LIMIT_PS_TIME:
      case TEMP_LIMIT_PS_ENERGY:
      case PULSE_TEMP_CS_TIME:
      case PULSE_TEMP_CS_ENERGY:
      case PULSE_TEMP_PS_TIME:
      case PULSE_TEMP_PS_ENERGY:
    	  if (TemperatureFlag == TRUE)
    	  {
    		  if (currentLCDData.CurrentTemp <= (MaxTempe - Three_degrees_F))
    			  TimeFlag=TRUE;
    	  }  
    	  else
    		  if (FlagTimer2 == OFF)
    			  TimeFlag=TRUE;
    	  break;
   }
   if (TimeFlag==TRUE)
   {
	   if(Flag_RTotalOnTime== PAUSE)
		   ResumeMSTimer(RTOTALONTIME);
	   if(FlagTotalOnTime== PAUSE)
		   ResumeMSTimer(TIMER_TOTALONTIME);
   }
}


void EnterReady(void)
/***************************************************************************/
/*                                                                         */
/*   This function is called when ready is entered.  It will set the       */
/*   keymask and make sure the display are correct.  It will               */
/*   also stop all timers just in case a cycle was aborted.                */
/*                                                                         */
/*   Exit condition:                                                       */
/*      Keymask:         Contains mask to allow all keys                   */
/*      External ready:  On                                                */
/*      OLFlag:          FALSE                                             */
/*      All timers:      stopped                                           */
/*                                                                         */
/***************************************************************************/
{   
   ReqStartRelease = FALSE;
   KeyMask = ALL_KEYS;
   RemovePSSeek();                      /* Make sure still not seeking     */
   StopAllTimers();                     /* Stop all timers in case of abort*/
   
   if (AlarmFlag == FALSE)
   {
	   if (SetKey != TRUE)
	   {
		   SwitchScreen(SCREEN_RDY);
		   PSReset();  
		   if (HandHeld == TRUE)
			   ClrUserOut();  
		    
	   }
   }/* Reset power supply, clear OLFlag*/
   else
	   SwitchScreen(SCREEN_ERROR);
   if (screen==SCREEN_RDY)
   {
	   StopFlag=TRUE;
	   SetReady(); 
	   StartByPT=FALSE;
   }
   

   HandHeld = FALSE;
   CycleReqFlag = FALSE;
   TestReqFlag = FALSE;
   Tst_N_ExtStart = FALSE;
   ResultIndex = 0;
}



void PreReady(void)
/***************************************************************************/
/*                                                                         */
/*  This function initialize variables to allow CheckPreReady to check     */
/*  conditions every pass through the state machine.                       */
/*                                                                         */
/***************************************************************************/
{  UINT16 key;
   
   AbortCleanUp();
   if (Conf.ConfigData.EndOfCycle != 2 )    /*Turn off EndOfWeld signal if 
                                              register 19 not configured as 2*/
  	   ClrUserOut();     
   OkToAdvance = FALSE;
   KeyMask = ALL_KEYS;
   key = GetKeys();
   if(Conf.ConfigData.PulseStart == TRUE && HandHeld == FALSE){                  /*This conditional if was updated to include the external stop independient of the start/stop key*/  
	   if (Conf.ConfigData.PanelTrig == TRUE){
		   if((key & KB_START_STOP) != KB_START_STOP)
			   StartFlag = FALSE;
	   }
	   else{
		   if((key & EXTSTART) != EXTSTART)
			   StartFlag = FALSE;
		   if ((key & KB_START_STOP) == KB_START_STOP)
		   {
			   StartFlag = TRUE;
		   }
			   
	   }
	   if (WrongStartFlag==TRUE)
		   StartFlag = TRUE;
   }
   if (TestReqFlag==TRUE)
	   StartFlag = TRUE;

   StartMSTimer(TIMER2, (UINT32)START_RELEASE_DELAY);
}



void CheckPreReady(void)
/***************************************************************************/
/*                                                                         */
/*  This function will check things like the start signal to make sure all */
/*  conditions are acceptable before going to the ready state.  If every-  */
/*  thing is okay then OkToAdvance will be set to TRUE otherwise it will   */
/*  be FALSE.  If the start signal has not gone away within the allowed    */
/*  time, then the ErSS alarm will be created and the state will exit to   */
/*  the not ready state.                                                   */
/*                                                                         */
/***************************************************************************/
{
	UINT16 key; 
	key = GetKeys();  
	if(ReqStartRelease == TRUE )
	{          
		ReqStartRelease = FALSE;
		if(((key &EXTSTART) == EXTSTART) || ((key & KB_START_STOP) == KB_START_STOP) )
		{  //to handle external start on power up 
			LockItFlag = TRUE;                          /* Prevent continuing */
			CreateAlarm (ER_SS);                        /* Set AlarmFlag      */	
		} 
	}
	else
		//if ( CurrentPreset.presetData.AmplitudeValue != currentLCDData.AmplitudeValue  ) //This is added to handle the case of updating amplitude during the cycle.
		if (AmpUpdatedDuringcycleFlag == TRUE)
		{
			PresetDisplay=0;
			SavePresetSelected(PresetDisplay);
			CurrentPreset.presetData.AmplitudeValue = currentLCDData.AmplitudeValue;
		}

	if (OLFlag == TRUE) 
	{
		CreateAlarm(ER_OL);
	} 
	else
		if ( StartFlag == TRUE )
		{
			if(FlagTimer2 == FALSE)
			{
				/* Check elapsed time */
				LockItFlag = TRUE;                          /* Prevent continuing */
				CreateAlarm (ER_SS);     
			}/* Set AlarmFlag      */
		}
		else
			OkToAdvance = TRUE;

}



void AbortCleanUp(void)
/***************************************************************************/
/*                                                                         */
/*   This function will handle the clean up needed when a cycle is stopped */
/*   early.  It will make sure the all flags are in the correct state      */
/*    and whatever else needs to be done.                                  */
/*                                                                         */
/***************************************************************************/
{
   PSIdle();                             /* Turn off sonics                */
   //ClrUserOut();                         /* Turn off sonics on output      */
   StopAllTimers();                      /* Stop all timers                */
   ExperFlag = FALSE;                    /* Experiment must have ended     */
}   



void EnterTest(void)
/***************************************************************************/
/*                                                                         */
/*   This function will set things up for test mode, like removing the     */
/*   ready output and turning off the mode and param leds.                 */
/*                                                                         */
/***************************************************************************/
{
   ResetAlarm();                             /* Clear any leftover alarms  */
   ClrReady();
   KeyMask = KB_TEST;                       /* Allow all keys             */
   KeyMask = (KeyMask & ~PS_SEEK_ACT);       /* except PS_SEEK_ACT         */
   PSTest();
   SwitchScreen(SCREEN_RESULTS);
   TestResult = TRUE;
   CurrentParam = D_CURRENTPOWER;
   FixedIcons = 0;
   FixedIcons |= (UINT32)(WATTAGE);
   currentLCDData.ValidResult = TRUE;
   currentLCDData.AmplitudeValue = CurrentPreset.presetData.AmplitudeValue;      //Save it to Display
}   

void Start_HH_Timer(void)
{
	ResultIndex = 0;
	HandHeld=FALSE;
	ClrReady(); 
	StartMSTimer(TIMER1, WAIT_HH);
}
void Start_HH_Timer2(void)
{
	SetReady();
	StartMSTimer(TIMER1, WAIT_HH);
}

void SetHH(void)
{
	HandHeld = TRUE;
	
	if (Conf.ConfigData.HHLimit == TRUE)
		HandleHH_MICROTIP();

		
	VerifyPreset();
	if (TimeOut > MILLISEC_IN_TENMINUT)
		TimeOut=MILLISEC_IN_TENMINUT;
	if (((WeldMode & CONFIG_TEMP)== CONFIG_TEMP)||((WeldMode & SET_MAXTEMP_NOT_ZERO)== SET_MAXTEMP_NOT_ZERO))
	{
		CreateAlarm(ER_HH);
		StartFlag = FALSE;
	}
	if (((Time > MILLISEC_IN_TENMINUT) && (WeldMode & SET_TIME_NOT_ZERO) == SET_TIME_NOT_ZERO) || ((TotalOnTime > MILLISEC_IN_TENMINUT) && (WeldMode & SET_ONTIME_NOT_ZERO) == SET_ONTIME_NOT_ZERO)) 
	{
		CreateAlarm(ER_HH);
		StartFlag = FALSE;
	}
	if (StartByPT == TRUE)
	{
		CreateAlarm(ER_HH_FPanel);
		StartFlag = FALSE;
	}
	if (AlarmFlag == TRUE)
	{
		StartFlag=FALSE;
		StopFlag=TRUE;
	}
		
}
void SetHHT(void)
{
	HandHeld = TRUE;
	currentLCDData.HandHeld =TRUE;
	
	if (Conf.ConfigData.HHLimit == TRUE){
		HandleHH_MICROTIP();
	
	}
}
void SetStartFlag(void)
{
	WrongStartFlag = TRUE;
}

