/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/p_inputs.h_v   1.6.3.2   13 Jun 2016 13:08:52   hasanchez  $
*/
/********************************************************************************* 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------------- MODULE DESCRIPTION ---------------------------

         This module collects all the inputs from the input pins and then provides decision 
         making Data to the State machine

Module name: INPUTS

Filename:    P_Inputs.h
--------------------------------- TECHNICAL NOTES -------------------------------

   This module collects all the inputs from the input pins and then provides decision 
         making Data to the State machine
------------------------------------- REVISIONS ---------------------------------

* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/p_inputs.h_v  $
 * 
 *    Rev 1.6.3.2   13 Jun 2016 13:08:52   hasanchez
 * ChackForSeek function declared.
 * 
 *    Rev 1.6.3.1   13 May 2016 13:47:42   hasanchez
 * Added new flags to handle the handheld detection by request of test or request of cycle.
 * 
 *    Rev 1.6.3.0   15 Dec 2015 16:14:58   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.3   25 Aug 2015 12:39:16   hasanchez
 * Added the flag to the Test+Reset key conbination.
 * 
 *    Rev 1.2   13 Jun 2013 08:00:34   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.1   06 Nov 2012 08:42:42   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.0   06 Aug 2012 05:27:50   akaushal
 * Initial revision.
 * 
 *    Rev 1.2   27 Jun 2012 07:50:50   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.1   12 Jun 2012 09:47:02   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:01:16   ygupta
 * Initial revision.

---------------------------------------------------------------------------------*/

#ifndef P_INPUTS_H
#define P_INPUTS_H
/*------------------------------ INCLUDE FILES -------------------------------*/

#include "portable.h"

/*---------------------------- TYPE DECLARATIONS -----------------------------*/
#define TEMP_SENSOR_DISCONNECTED 320
typedef enum {            
   GND_DETECT_IN = BIT0,    /* From banana jack on back on unit */
   EXT_RESET_IN = BIT1,     /* From 9-pin external d-shell      */
   EXT_START_IN = BIT2,     /* From 9-pin external d-shell      */
   OVERLOAD_IN   = BIT3,    /* From power supply control cable  */
   SONICS_ACTIVE = BIT4,    /* Sonics active                    */
   EXT_SEEK_IN   = BIT5,    /* From 9-pin external d-shell      */
   PS_SEEK_ACT_IN = BIT6    /* From power supply advanced cable */   
} INPUT_CODE;

/*------------------------ Function Prototypes -----------------------------*/

INPUT_CODE GetInputs(void);     
UINT8 GetPower(void);
UINT16 GetTemperatureADC(void);
void ProcessInputs(void);
void CheckForSeek(void);
/*------------------------------ EXTERNAL DATA -------------------------------*/

extern   UINT8  TestKey;
extern   UINT8  TestResetKeys;
extern   UINT8  SetKey;
extern   UINT8  ResetKey;
extern   UINT8  StartFlag;
extern   UINT8  WrongStartFlag;
extern   UINT8  StopFlag;
extern   UINT8  SeekRequest;
extern   UINT8  SeekActive;
extern   UINT8  PauseFlag;
extern   UINT8  ONorOFF;
extern   UINT8  IOValue;             /* Contains the debounced digital Inputs */
extern   UINT8  GDAllowed;      /* TRUE: in a grd det mode FALSE: not in GD mode   */
extern   UINT8  ExperFlag;      /* Used to indicate if experiment is running       */
extern   UINT8  StartByPT;   //Start by panel Trigger.
extern   UINT8  CycleReqFlag;
extern   UINT8  TestReqFlag;
extern   UINT8 Tst_N_ExtStart;



//extern   UINT8  StartReleased;
#endif /* P_INPUTS_H */
