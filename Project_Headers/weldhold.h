/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/weldhold.h_v   1.5.3.4   13 Jun 2016 13:00:30   hasanchez  $ 
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

 This module handles the function prototypes and defines for weldhold.c 

Module name: WELDMODE

Filename:    weldhold.h
--------------------------- TECHNICAL NOTES -------------------------------

This code controls the Weldming mode and thus all outputs from the SFX


------------------------------------- REVISIONS ---------------------------------
 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/weldhold.h_v  $
 * 
 *    Rev 1.5.3.4   13 Jun 2016 13:00:30   hasanchez
 * Declaration of global flags.
 * 
 *    Rev 1.5.3.3   18 May 2016 13:31:36   hasanchez
 * Deleted debug function.
 * 
 *    Rev 1.5.3.2   13 May 2016 13:51:38   hasanchez
 * Added declaration to new flags and functions.
 * 
 *    Rev 1.5.3.1   12 Jan 2016 11:52:00   aghuerta
 * added a function ConverttoEnergy
 * 
 *    Rev 1.5.3.0   15 Dec 2015 16:15:00   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.2   08 Jul 2013 04:45:04   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, changed frequency offset acc to the rquirement.
 * 
 *    Rev 1.1   13 Jun 2013 08:00:50   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.0   06 Aug 2012 05:28:30   akaushal
 * Initial revision.
 * 
 *    Rev 1.1   12 Jun 2012 09:49:16   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:02:20   ygupta
 * Initial revision.

-------------------------------------------------------------------------*/


#ifndef WELDHOLD_H
#define WELDHOLD_H
/*----------------------------- INCLUDE FILES ------------------------------*/

#include "util.h"
#include "portable.h"

/*-------------------------- FUNCTION PROTOTYPES ---------------------------*/
#define WAIT_HH          20L

void CalcPower(void);
void FinalPower(void);
void CalcEnergy(void);
UINT32 ConverttoEnergy (UINT32 TempEnergy);
UINT8 CalcTimeProgress(UINT32 Partof, UINT32 Total );
UINT8 CalcEnergyProgress(UINT32 Total );
void StartSonicsTimer(void);
void StartHoldTimer(void);
void CheckOnParam(void);
void CheckExperiment(void);
void CountOL(void);
void CheckOffParam(void);
void EnterReady(void);
void PreReady(void);
void CheckPreReady(void);
void AbortCleanUp(void);
void EnterTest(void);
void InitWeldParam(void);
void PauseTimers(void);
void ResumeTimers(void);
void FinishCicle(void);
void CheckPulseTemperature(void);
void Calc_P_Tempe_ModeSettings(void);
void Start_HH_Timer(void);
void Start_HH_Timer2(void);
void SetHH(void);
void SetHHT(void);
void SetStartFlag(void);

/*---------------------------- EXTERNAL DATA --------------------------------*/

/* Process parameters */

extern  SINT16   CurrentTemperature;
extern  SINT16   SensedTemperature;
extern  UINT32   AccumulatedEnergy; /* Energy units used so far                        */
extern	UINT32   PartialEnergy;
extern  UINT32   EnergyUnitsNeeded; /* Energy units needed                             */


extern UINT32 Time;
extern UINT32 Energy;
extern UINT32 OnTime;
extern UINT32 OffTime;
extern UINT16 OnEnergy;
extern UINT32 TotalOnTime;
extern UINT32 TotalOnEnergy;
extern UINT16 PulseTempe;
extern UINT16 MaxTempe;
extern UINT8  Amplitude;          /* Sonics on time for SLP low range and LP         */
extern UINT16 WeldMode;
extern UINT8  TestResult;
extern UINT8  TempeProbePresent;
extern UINT8 FirstTime;
extern UINT8 HandHeld;
extern SINT16 EndOfWeldSignalTime;
extern UINT8  CycleUpdated;
extern BOOLEAN AmpUpdatedDuringcycleFlag;
extern UINT8  ReqStartRelease;
extern UINT8 PTemperatureFlag;
extern UINT32 TempOnEnergy;
extern UINT32 TempOnTime;
extern UINT32 TempOffTime;
extern UINT8 PulseTempeIndex;
extern UINT8  PulsingMode;
extern UINT32   DOAON[10];
extern UINT32   DOAOFF[10];
extern UINT16   DOATEMP[10];
/* Setup parameters */

/****************************************************************************/
/* These two typedefs allow access to the same variable by either using a   */
/* UINT8 or UINT16.  This is necessary because the state machine requires   */
/* UINT8 but functions that deal with parameters require UINT16.  The 5     */
/* variables Ar, Ps, Pt, Be and Dl needed to be used by both functions.     */
/****************************************************************************/


extern   UINT16   Wattage;        /* Wattage of power supply in watts                */
extern	 UINT16	 InitTemp;

/* Current Status */

extern   UINT8    AlarmFlag;      /* A condition to prevent entry into ready         */
extern   UINT16   AvgPower;       /* Displayed Power from the last ADC reading       */
extern   ALARMS   CurrentAlarm;   /* Code for last alarm created since power on      */
//extern   MODES    CurrentMode;    /* Current mode                                    */
extern   UINT16   Currentpower;   /* Power from the last ADC reading                 */
extern   UINT8    ExperFlag;      /* Used to indicate if experiment is running       */
extern   UINT8    GDFlag;         /* External ground detect received                 */
extern   UINT8    LockItFlag;     /* Used to lock up system such as ram failure      */
extern   UINT8    OLFlag;         /* Overload detected from power supply             */
extern   UINT8    TimeFlag;       /* Used to indicate if timers are still running    */
extern   UINT8    OkToAdvance;    /* Used to decide if ok to enter ready state       */



#endif /* WELDHOLD_H  */
