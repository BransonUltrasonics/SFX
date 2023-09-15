/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/util.h_v   1.2.1.3.3.0   15 Dec 2015 16:15:00   hasanchez  $
**************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION ---------------------------

Utility function used just about anywhere                                                              


Module name: UTIL

Filename:    util.h

------------------------------ REVISIONS ---------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/util.h_v  $
 * 
 *    Rev 1.2.1.3.3.0   15 Dec 2015 16:15:00   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.2.1.0   25 Aug 2015 12:47:40   hasanchez
 * Added new strings.
 * 
 *    Rev 1.2   25 Mar 2013 08:00:16   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.1   03 Sep 2012 10:06:26   akaushal
 * Code CleanUp
 * 
 *    Rev 1.0   06 Aug 2012 05:28:24   akaushal
 * Initial revision.
 * 
 *    Rev 1.4   06 Jul 2012 10:04:14   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.3   27 Jun 2012 07:52:14   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:35:06   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:48:48   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:02:04   ygupta
 * Initial revision.

*****************************************************************************/

/*-------------------------------- INCLUDES ----------------------------------*/
#ifndef _UTIL_H
#define _UTIL_H

#include "portable.h"

/*--------------------------------- DEFINES ----------------------------------*/

typedef enum Alarms 
{
   NO_ALARM,                  // Duh!
   ER_OL,                     // Overload     
   ER_RF,                     // Ram Failure 
   ER_SS,                     // Start Switch 
   ER_DE,                     // Data Entry  
   ER_TEMP,                   // Temperature Probe not present
   ER_uTIP,                   // Microtip ON and Amplitude > 70%
   ER_TIMEOUT,                // TimeOut Value = 0
   ER_TIMEOUTREACH,
   ER_MODEERROR,              // Invalid Mode (Auto+Temp)
   ER_TEMPERROR,              // Current Temperature > MAX Temperature at Start of Weld
                              // meaning MAX Temperature is Less than Facility Temperature
   ER_HH,                     // Trying to start a cycle that has temperature features.
   ER_HH_FPanel,              // Trying to start a cycle using the Start/stop key when a Hand Held is detected.
   //Condition Mismatch                           
   ER_TIME_1,                 // (OnTime + WeldOffTime) > TotalWeldTime) OR ( OnTime >= TotalWeldTime)
   ER_TIME_2,                 // TimeOut > TotalWeldTime 
   ER_TIME_3,                 // WeldOffTime >= TotalWeldTime
   ER_TEMPERATURE,                 // Pulse Temperature > Max temperature
} ALARMS;


#define ACTID               0xAA55
#define ACTID_ADDR          0x00
#define TIME_ON_ADDR        0x02
#define TIME_OFF_ADDR       0x04
#define HOLD_TIME_ADDR      0x06
#define EXP_TIME_ADDR       0x08
#define UNUSED_ADDR         0x0a
#define PULSE_TIME_OFF_ADDR 0x0c
#define ENERGY_ADDR         0x0e
#define PULSE_ENERGY_ADDR   0x10
#define SCRUB_TIME_ADDR     0x12
#define MODE_ADDR           0x14
#define CONTROL_LEVEL_ADDR  0x16
#define SETUP_PT_ADDR       0x18
#define WATTAGE_ADDR        0x1a
#define AMPLITUDE_ADDR      0x1c
#define TIME_ON_HI_ADDR     0x1e
#define TIME_ON_LO_ADDR     0x20
#define SETUP_PS_ADDR       0x22
#define SETUP_AR_ADDR       0x24
#define PULSE_TIME_HI_ADDR  0x26
#define PULSE_TIME_LO_ADDR  0x28  
#define HK_ON_TIME_LO_ADDR  0x2a  
#define HK_ON_TIME_HI_ADDR  0x2c  
#define HK_OFF_TIME_ADDR    0x2e  
#define BEEPER1_ENABLE_ADDR 0x30  
#define BEEPER2_ENABLE_ADDR 0x32  
#define BEEPER3_ENABLE_ADDR 0x34  
#define DISPLAY_LOCK_ADDR   0x36  
#define SEEKPOWERUP_ADDR    0x38
#define PLACEHOLDER         0x18


/*--------------------------- FUNCTION PROTOTYPES ----------------------------*/
UINT8   AmpMicrotipValid(void);
void   CreateAlarm(ALARMS Alarm);
void   ResetAlarm(void);
void   ResetClearAlarm(void);


/*------------------------------ EXTERNAL DATA -------------------------------*/
extern const  STR SoftwareVersion[];
extern const  STR Off_Msg[];
extern const  STR Rdy_Msg[];
extern const  STR Rdy_Msg2[];
extern const  STR Er_OL_Msg[];
extern const  STR Er_SS_Msg[];
extern const  STR Er_EEPROM[] ;
extern const  STR Er_TempSens[];
extern const  STR Er_MicroTip[];
extern const  STR Er_DE_Msg[];
extern const  STR Er_TempError[];
extern const  STR Er_TimeError2[];
extern const  STR Er_TimeOutReach[];
//extern const  STR Er_Temperature[];
extern const  STR Er_HandHeld[];
extern const  STR Er_FrontPanel[];
//extern const  STR Er_TimeOut[];

extern STR Str_CRC[];

//extern const STR  Er_ModeError[];
//extern const  STR Er_TimeError1[];
//extern const  STR Er_TimeError3[];


#ifdef UTIL                    
#define Utiltype                 
#else                             
#define Utiltype  extern         
#endif                            

Utiltype volatile UINT16 NovramTimer;   /* Used to time the 10ms write of the novram */

#endif /* UTIL_H  */
