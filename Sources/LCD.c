

/*$Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/LCD.c_v   1.19.1.3.3.8   03 Aug 2016 14:01:14   hasanchez  $
*************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION --------------------------------------

This is the LCD module which Displays all the Data and Error messages, and Icons on the Screen.
Display depends on Previous Configuraion and Preset data and Key Pressed

Module name: LCD

Filename:    LCD.c
--------------------------- TECHNICAL NOTES -----------------------------------------

This code contains the moule for Displaying the Data onto the LCD in Proper format.
Icons and Text related to a perticular mode and any detected Failure or Fault is also Displayed

------------------------------ REVISIONS --------------------------------------------

* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/LCD.c_v  $
 * 
 *    Rev 1.19.1.3.3.8   03 Aug 2016 14:01:14   hasanchez
 * Updated the function to display the colons while on Data Entry error.
 * 
 *    Rev 1.19.1.3.3.7   26 Jul 2016 17:33:10   hasanchez
 * Updated the version string to "  1.3  "
 * 
 *    Rev 1.19.1.3.3.6   20 Jul 2016 09:00:40   hasanchez
 * Added comments to the register of Limit amplitude for the HandHeld
 * 
 *    Rev 1.19.1.3.3.5   14 Jul 2016 12:26:10   hasanchez
 * Updated the blinking functio on the LED on the HH.
 * 
 *    Rev 1.19.1.3.3.4   13 Jun 2016 11:54:16   hasanchez
 * Added register 17 (Hand Held limit). ReqReleaseFlag added to start key options. 
 * 
 *    Rev 1.19.1.3.3.3   18 May 2016 13:22:24   hasanchez
 * Updated Name of OneSec to HalfSec
 * 
 *    Rev 1.19.1.3.3.2   13 May 2016 13:15:34   hasanchez
 * Added functions to show the wrong value on Data Entry alarm. 
 * Updated the structure of parameters.
 * 
 *    Rev 1.19.1.3.3.1   12 Jan 2016 11:27:12   aghuerta
 * change DisplayVal Parameters values
 * 
 *    Rev 1.19.1.3.3.0   15 Dec 2015 16:15:02   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.19.1.0   25 Aug 2015 11:45:40   hasanchez
 * Update New Navigation Style
 * 
 *    Rev 1.19   18 Sep 2013 08:19:36   akaushal
 * Fixed the Power Up seek issue
 * 
 *    Rev 1.18   10 Sep 2013 03:19:22   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.17   12 Jul 2013 06:50:30   akaushal
 * Corrected the handelling of Start switch delay.
 * 
 *    Rev 1.16   08 Jul 2013 04:40:48   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, and changed frequency offset acc to requirement.
 * 
 *    Rev 1.15   13 Jun 2013 07:54:14   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.14   25 Mar 2013 08:00:46   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.13   20 Dec 2012 01:24:52   akaushal
 * Updated Error codes and fixed Total runtime
 * 
 *    Rev 1.12   03 Dec 2012 08:55:26   akaushal
 * Navigation and Screens Updated
 * 
 *    Rev 1.11   20 Nov 2012 07:28:28   akaushal
 * Lcd Flickering Fixed and Navigation Updated
 * 
 *    Rev 1.10   07 Nov 2012 07:31:00   akaushal
 * Flickering Screen Problem Solved
 * 
 *    Rev 1.9   07 Nov 2012 07:04:10   akaushal
 * Screen Navigation Updated
 * 
 *    Rev 1.8   06 Nov 2012 08:43:58   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.7   10 Sep 2012 09:48:36   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.6   03 Sep 2012 09:43:56   akaushal
 * Automode Corrected for Total time Icon
 * 
 *    Rev 1.5   24 Aug 2012 06:24:18   akaushal
 * Amplitude Issue resolved
 * 
 *    Rev 1.4   22 Aug 2012 04:11:58   akaushal
 * LCD Code Corrected for Crystal Oscillator
 * 
 *    Rev 1.3   16 Aug 2012 01:57:38   akaushal
 * SFX Code With External Crystal Oscillator and HW Changes
 * 
 *    Rev 1.2   10 Aug 2012 02:55:46   akaushal
 * Digipot adjust on correcting PSFREQ
 * 
 *    Rev 1.1   08 Aug 2012 02:16:24   akaushal
 * Remove Total Time from Automode 
 * 
 *    Rev 1.0   06 Aug 2012 05:30:52   akaushal
 * Initial revision.
 * 
 *    Rev 1.8   31 Jul 2012 03:59:28   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.7   10 Jul 2012 05:36:16   akaushal
 * Digit blinking problem removed
 * 
 *    Rev 1.6   09 Jul 2012 03:25:54   akaushal
 * Keypad code updated
 * 
 *    Rev 1.5   06 Jul 2012 10:03:20   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.4   03 Jul 2012 07:34:00   akaushal
 * Amplitude code and Energy Max reached updation
 * 
 *    Rev 1.3   27 Jun 2012 07:49:22   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:32:36   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:45:44   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:20   ygupta
 * Initial revision.

-------------------------------------------------------------------------------------*/

#include "LCD.h"
#include "MC9S08LG32.h"
#include "portable.h"
#include "onems.h"
#include "string.h"
#include "print.h"
#include "global.h"
#include "eeprom.h"
#include "weldhold.h"
#include "p_inputs.h"
#include "p_output.h"
#include "Keyboard.h"
#include <stddef.h>
#include "util.h"


enum eScreens screen;
UINT8 UpdateLiveReady = FALSE;
UINT32 FixedIcons;                     //display fixed icons
UINT32 BlinkIcons;                     //display Blink Icons
UINT8 AuxDisplayCodes[NUM_7SEGMENT_DIGITS+1];

UINT8 PresetDisplay = 0;
UINT8 ModeSet = TIME_MODE;          //Toggle between modes Temp,Time and Energy
bool ModeSet2 = TIME_MODE;
UINT8 CurrFnScreen;      
bool EscKeyPressed = FALSE;
bool ShowDefaults = FALSE;
bool LoadPreset = TRUE;
SINT32 Increment = 1;
enum Params CurrentParam = 0;
enum Params AuxCurrentParam = 0;
bool ContinuousMode = TRUE;
UINT8 ValueOkay = FALSE;
UINT8 PresetNo = 1;
UINT8 ResultIndex = 0;
UINT8 BlockSelected=0;
UINT8 FirstChar; 
UINT8 QtyOfChars;
UINT32 StorePassword;
static UINT32 FixedIconsVar;        ////Most be deleted?  HSAN
UINT8 DataEntryError = FALSE;       ////Most be deleted?  HSAN
NUMERIC_DATA Num_Current, Num_Min, Num_Max;
STR   FnMSG[NUM_7SEGMENT_DIGITS+1]="  ydr "; /* The Message is in reverse order since we started 
                                                numbering Segment fronm Right most Segment and 
                                                Copied as per a counter i. Thus "r" should go to the
                                                higher numbered segment than "d" */

const STR  NoMSG[NUM_7SEGMENT_DIGITS+1] = "      ";
const STR  FnERR[NUM_7SEGMENT_DIGITS+1] = "------";
const STR  StrZero[NUM_7SEGMENT_DIGITS+1] = "000000";
const STR  MinLimitC[NUM_7SEGMENT_DIGITS+1] = "  0000";
const STR  MaxLimitC[NUM_7SEGMENT_DIGITS+1] = "  9990";
const STR  MaxLimAmp_Micro[NUM_7SEGMENT_DIGITS+1] = "  0700";
const STR  OffTempC[NUM_7SEGMENT_DIGITS+1] = "C^0101";
const STR  OffTempF[NUM_7SEGMENT_DIGITS+1] = "F^0312";


UINT8 DisplayCodes[NUM_7SEGMENT_DIGITS+1];
UINT8 *lcdRam;
UINT8 TempSense = TRUE;
UINT8 BlinkDigitAux;
UINT8 PswCorrectLimit = FACTCOLDSTART;
UINT8 PswWrongLimit = PASSWORD;
CURRENTDIGIT CurrentDigit = {
   NODIGITS, FALSE
};

 // Current LCD data with max values.
struct LCDData currentLCDData={              ////Most be deleted?  HSAN
	0,//TotalTime
	0,//TotalEnergy
	0,//AmplitudeValue
	0,//Progress
	0,//MaxTemp
	0,//LastTemp
	0,//CurrentTemp
	0,//PeakPower
	0,//CurrentPower
	0,//OnTime
	0,//OnEnergy
	0,//OffTime
	0,//TotalOnTime
	0,//WeldMode
	0,//HandHeld
	0,//ValidResult

};

/* Function screen display strings.*/
const struct Fn_Scrn Fn_Screens[FACTCOLDSTART+1]= { 
/*Software Version*/       {SWVERSION,      1},
/*PANEL TRIGGER*/          {PANELTRIG,   	2},
/*Pulse Start*/            {PULSESTART,     3},
/*AUTO RESET*/             {AUTORESET,      4},
/*Trigger*/                {ENDOFWELDBEEP,  5},
/*ERROR*/                  {PARAMERRORBEEP, 6}, 
/*DISPLAY LOCK*/           {DISPLOCK,    	8},
/*SEEK AT POWER UP*/       {PWRUPSEEK,  	9},
/*Timeout Hours*/          {TIMEOUT,        10},
/*TEMP in C or F*/         {TEMPE_C_F,      15},
/*ColdStart*/              {COLDSTART,      16},
/*Hand Held Limit*/        {HHLIMIT,        17},
/*CallibrationOffset*/     {CALOFFSET,      18},
/*EndOfWeld*/              {ENDOFCYCLE,     19},
/*Password*/               {PASSWORD,       20},
/*WATTAGE*/                {PSWATTAGE,      50},
/*REGION*/                 {REGION,         51},
/*Factory ColdStart*/      {FACTCOLDSTART,	55}
};


const struct Wattage MenuWattage[PWR_550+1]= { 
		/*150Watts*/       {PWR_150,    150},
		/*250Watts*/       {PWR_250,   	250},
		/*550Watts*/       {PWR_550,   	550},
};
  
/* Change MIN and MAX also, along with SOFTWAREVERSION for Changed S/W Version */  
const DisplayVal Parameters[] = {
/*  Format        Default Min1 Min2 Min3 Min4 Min5 Min6 Max1 Max2 Max3  max4  Max5 Max6  

  ------         --- -----  ------- ---- ---- ---- ---- ---- ---- ---- ---- ----  ----  ---  ---   */

   {SWVERSION,     36,     '0', '1', '3', '1', ' ', ' ', '0', '1',  '3', '1', ' ', ' ', 0,	0, 	0, 0,	NO_BLINK,  	sizeof(FnMSG)},/* S/W Version  */
   {PANELTRIG,     1,      '0', '2', '0', ' ', ' ', ' ', '0', '2',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.PanelTrig),0},        /* Panel Trigger set*/
   {PULSESTART,    1,      '0', '3', '0', ' ', ' ', ' ', '0', '3',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.PulseStart),1},      /* Pulse Start setup*/
   {AUTORESET,     0,      '0', '4', '0', ' ', ' ', ' ', '0', '4',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.AutoReset),2},        /* Alarm reset setup*/
   {ENDOFWELDBEEP, 1,      '0', '5', '0', ' ', ' ', ' ', '0', '5',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.EndOfWeldBeep),3},/* End of weld beep */
   {PARAMERRORBEEP,1,      '0', '6', '0', ' ', ' ', ' ', '0', '6',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.ErrorBeep),4},        /* Error beeper     */
   {DISPLOCK,      0,      '0', '8', '0', ' ', ' ', ' ', '0', '8',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.DispLock),5},          /* Disp lock   setup*/
   {PWRUPSEEK,     0,      '0', '9', '0', ' ', ' ', ' ', '0', '9',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.PwrUpSeek),6},        /* Seek @ Power up  */
   {TIMEOUT,       7200,   '0', '0', '0', '0', '0', '0', '9', '5',  '9', '5', '9', '9', 2,	2,	2, 2,	BLK_TIME2,	sizeof(Conf.ConfigData.TimeOut),7},    /* TimeOut*/
   {TEMPE_C_F,     1,      '1', '5', '0', ' ', ' ', ' ', '1', '5',  '1', ' ', ' ', ' ', 0,	1,	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.Tempe_C_F),11},        /* Temp C or F setup*/
   {COLDSTART,     0,      '1', '6', '0', ' ', ' ', ' ', '1', '6',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.ColdStart),12},        /* Soft Reset*/
   {HHLIMIT,       1,      '1', '7', '0', ' ', ' ', ' ', '1', '7',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.HHLimit),13},        /* Panel Trigger set*/
   {CALOFFSET,     -40,    'F', '^', '0', '2', '3', '0', '^', 'F',  '9', '1', '1', '2', 0, 	1,	3,	3,	BLK_TEMPE,	sizeof(Conf.ConfigData.CalibrationOffset),14},  /* Temperature Calibration Offset*/
   {ENDOFCYCLE,    1,      '1', '9', '0', ' ', ' ', ' ', '1', '9',  '2', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,   sizeof(Conf.ConfigData.EndOfCycle),16},
   {PASSWORD,      20,     '2', '0', '0', '0', ' ', ' ', '2', '0',  '9', '9', ' ', ' ', 0,	2, 	0,	2,	NO_BLINK,	sizeof(Conf.ConfigData.Password),17},        /* Password to view further registers 5x*/
   {PSWATTAGE,     0,      '5', '0', '1', '5', '0', ' ', '5', '0',  '5', '5', '0', ' ', 0,	3, 	0,	2,	NO_BLINK,	sizeof(Conf.ConfigData.PsWattage),18},         /* P/S Wattage      */
   {REGION,        0,      '5', '1', '0', ' ', ' ', ' ', '5', '1',  '1', ' ', ' ', ' ', 0,	1, 	0, 2,	NO_BLINK,	sizeof(Conf.ConfigData.Region ),20},      /* Pulse Start setup*/
   {FACTCOLDSTART, 0,      '5', '5', '0', ' ', ' ', ' ', '5', '5',  '1', '0', ' ', ' ', 0,	1, 	0,	2,	NO_BLINK,	sizeof(Conf.ConfigData.FactColdStart),21},        /* Soft Reset*/
     
   /*BEGIN TEMPORAL  while we develop the  numeric functions   */
   
   
   {TIME,           60,   '0', '0', '0', '0', '0', '0', '9', '5',  '9', '5', '9', '9', 2,	2,	2,	2,	BLK_TIME, 			sizeof(CurrentPreset.presetData.Time),0},      /* Off Time in mSec*/
   {ENERGY,        	1,    '0', '0', '0', '0', '0', '0', '9', '9',  '9', '9', '9', '9', 2,	2,	2,	1,	BLK_ENERGY,			sizeof(CurrentPreset.presetData.Energy),4},        /* ON time in Msec*/
   {ONTIME,         10,   '0', '0', '0', '0', '0', '0', '9', '9',  '9', '5', '9', '5', 2,	2,	2,	2,	BLK_ONTIME,			sizeof(CurrentPreset.presetData.OnTime),8},  /* Total Time is MSec*/
   {OFFTIME,        10,   '0', '0', '0', '0', '0', '0', '9', '9',  '9', '5', '9', '5', 2,	2,	2,	2,	BLK_OFFTIME,		sizeof(CurrentPreset.presetData.OffTime),12},                /* TimeOut Hour*/
   {ONENERGY,       1,    ' ', ' ', '0', '0', '0', '0', ' ', ' ',  '9', '9', '9', '9', 0,	2,	2,	2,	BLK_ONENERGY,		sizeof(CurrentPreset.presetData.OnEnergy),16},/* Pulse Temperature*/
   {TOTALONTIME,    60,   '0', '0', '0', '0', '0', '0', '9', '5',  '9', '5', '9', '9', 2,	2,	2,	2,	BLK_TOTALONTIME,	sizeof(CurrentPreset.presetData.TotalOnTime),18},  /* Amplitude*/
   {TOTALONENERGY,  1,    '0', '0', '0', '0', '0', '0', '9', '9',  '9', '9', '9', '9', 2,	2,	2,	1,	BLK_TOTALONENERGY,	sizeof(CurrentPreset.presetData.TotalOnEnergy),22},    /* Welding  Mode*/
   {PULSETEMPE,     0,    'F', '^', '0', '2', '3', '0', '^', 'F',  '9', '1', '1', '2', 0,	1,	3,	3,	BLK_PULSETEMPE,		sizeof(CurrentPreset.presetData.PulseTempe),26},/* Pulse Temperature*/
   {MAXTEMPE,       0,    'F', '^', '0', '2', '3', '0', '^', 'F',  '9', '1', '1', '2', 0,	1,	3,	3,	BLK_MAXTEMPE,		sizeof(CurrentPreset.presetData.MaxTempe),28},  /* Pulse Temperature*/
   {AMPLITUDE,     50,    ' ', ' ', '0', '1', '0', ' ', ' ', ' ',  '0', '0', '1', ' ', 0,	3,	0,	2,	BLK_AMPLITUDE,		sizeof(CurrentPreset.presetData.AmplitudeValue),30},  /* Amplitude*/
   {MICROTIP,      1,  	  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ', ' ', ' ', ' ', 0,	0,	0,	0,	NO_BLINK,			sizeof(CurrentPreset.presetData.Microtip),31},    /* Welding  Mode*/
   {WELDMODE,     CS_TIME,' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ', ' ', ' ', ' ', 0,	0,	0,	0,	NO_BLINK,			sizeof(CurrentPreset.presetData.WeldMode),32},    /* Welding  Mode*/
   {CONF_MODE,     1,  	  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ', ' ', ' ', ' ', 0,	0,	0,	0,	NO_BLINK,			sizeof(CurrentPreset.presetData.WeldMode),32},    /* Welding  Mode*/
   {CONF_CONTROL,  1,  	  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ', ' ', ' ', ' ', 0,	0,	0,	0,	NO_BLINK,			sizeof(CurrentPreset.presetData.WeldMode),32},    /* Welding  Mode*/
   {CONF_CONTROL2, 1,  	  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ', ' ', ' ', ' ', 0,	0,	0,	0,	NO_BLINK,			sizeof(CurrentPreset.presetData.WeldMode),32},    /* Welding  Mode*/
   
   /* END OF TEMPORA.*/
   {D_TOTALTIME,   0,    '0', '0', '0', '0', '0', '0', '9', '9',  '9', '9', '9', '9', 6,	0,	0,	1,	NO_BLINK, 			sizeof(currentLCDData.TotalTime)}, 
   {D_TOTALENERGY, 0,    '0', '0', '0', '0', '0', '0', '9', '9',  '9', '9', '9', '9', 6,	0,	0,	1,	NO_BLINK,			sizeof(currentLCDData.TotalEnergy )},
   {D_AMPLITUDE,   0,    ' ', ' ', '0', '1', '0', ' ', ' ', ' ',  '0', '0', '1', ' ', 0,	3,	0,	2,	NO_BLINK,			sizeof(currentLCDData.AmplitudeValue)},  /* Amplitude*/
   {D_PROGRESS,    0,    ' ', ' ', '0', '0', '0', ' ', ' ', ' ',  '9', '9', '9', ' ', 0,	3,	0,	2,	NO_BLINK,			sizeof(currentLCDData.Progress)}, 
   {D_MAXTEMP,     0,    'F', '^', '0', '0', '0', '0', '^', 'F',  '9', '9', '9', '9', 0,	4,	0,	2,	NO_BLINK,			sizeof(currentLCDData.MaxTemp)},/* Pulse Temperature*/
   {D_LASTTEMP,    0,    'F', '^', '0', '0', '0', '0', '^', 'F',  '9', '9', '9', '9', 0,	4,	0,	2,	NO_BLINK,			sizeof(currentLCDData.LastTemp)},/* Pulse Temperature*/
   {D_CURRENTTEMP, 0,    'F', '^', '0', '0', '0', '0', '^', 'F',  '9', '9', '9', '9', 0,	4,	0,	2,	NO_BLINK,			sizeof(currentLCDData.CurrentTemp)},/* Pulse Temperature*/
   {D_PEAKPOWER,   0,    ' ', ' ', '0', '0', '0', ' ', ' ', ' ',  '9', '9', '9', ' ', 0,	3,	0,	2,  NO_BLINK, 			sizeof(currentLCDData.PeakPower)},
   {D_CURRENTPOWER,0,    ' ', ' ', '0', '0', '0', ' ', ' ', ' ',  '9', '9', '9', ' ', 0,	3,	0,	2,  NO_BLINK, 			sizeof(currentLCDData.CurrentPower)},
   {D_ONTIME,      0,    '0', '0', '0', '0', '0', '0', '9', '9',  '9', '9', '9', '9', 6,	0,	0,	1,	NO_BLINK, 			sizeof(currentLCDData.OnTime)}, 
   {D_ONENERGY,    0,    '0', '0', '0', '0', '0', '0', '9', '9',  '9', '9', '9', '9', 6,	0,	0,	1,	NO_BLINK,			sizeof(currentLCDData.OnEnergy)},
   {D_OFFTIME,     0,    '0', '0', '0', '0', '0', '0', '9', '9',  '9', '9', '9', '9', 6,	0,	0,	1,	NO_BLINK, 			sizeof(currentLCDData.OffTime)}, 
   {D_TOTALONTIME, 0,    '0', '0', '0', '0', '0', '0', '9', '9',  '9', '9', '9', '9', 6,	0,	0,	1,	NO_BLINK, 			sizeof(currentLCDData.TotalOnTime)}, 
   
   
   {MESSAGE,       1,     '0', '0', '0', '0', '0', '0', ' ', '4',  '5', '9', '5', '9', 0,	0,	0,	0, NO_BLINK, sizeof(FnMSG)},                                                 /* Ready Message Disply*/
   {FN_NUM,        1,     '1', '0', ' ', 'G', 'E', 'R', '0', '2',  ' ', 'G', 'E', 'R', 2,	0, 	0,  1, NO_BLINK, sizeof(CurrFnScreen),0},        /* Panel Trigger set*/
   {NOMESSAGE,     1,     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ', ' ', ' ', ' ', 0,	0,	0,	0, NO_BLINK, sizeof(NoMSG)},                                                 /* No Message */
   {ERROR,        '-',    '-', '-', '-', '-', '-', '-', '-', '-',  '-', '-', '-', '-', 0,	0,	0,	0, NO_BLINK, sizeof(FnERR)},                                                 /* Error Message Display*/
   {PRESET_NUM,    1 ,    'O', 'L', ' ', ' ', '1', '0', 'A', 'S',  ' ', ' ', '0', '2', 0,	0,	2,	3, NO_BLINK, sizeof(PresetNo)},           /* Display Preset Number and Save or Load*/
    //{FNEND,         0 ,     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' ', ' ', ' ', ' ',  (UINT8*)NULL,0}, 
};


DISP_FORMAT Format[FNEND] = {

/*  Format         PtrVariable       */

/*  ------           ----------------- */
   {SWVERSION,     (UINT8*)&FnMSG},/* S/W Version  */
   {PANELTRIG,     (UINT8*)&Conf.ConfigData.PanelTrig},        /* Panel Trigger set*/
   {PULSESTART,    (UINT8*)&Conf.ConfigData.PulseStart},      /* Pulse Start setup*/
   {AUTORESET,     (UINT8*)&Conf.ConfigData.AutoReset},        /* Alarm reset setup*/
   {ENDOFWELDBEEP, (UINT8*)&Conf.ConfigData.EndOfWeldBeep},/* End of weld beep */
   {PARAMERRORBEEP,(UINT8*)&Conf.ConfigData.ErrorBeep},        /* Error beeper     */
   {DISPLOCK,      (UINT8*)&Conf.ConfigData.DispLock},          /* Disp lock   setup*/
   {PWRUPSEEK,     (UINT8*)&Conf.ConfigData.PwrUpSeek},        /* Seek @ Power up  */
   {TIMEOUT,       (UINT8*)&Conf.ConfigData.TimeOut},    /* TimeOut Hour*/
   {TEMPE_C_F,     (UINT8*)&Conf.ConfigData.Tempe_C_F},        /* Temp C or F setup*/
   {COLDSTART,     (UINT8*)&Conf.ConfigData.ColdStart},        /* Soft Reset*/
   {HHLIMIT,       (UINT8*)&Conf.ConfigData.HHLimit},          /* Limit amplitude for HandHeld*/
   {CALOFFSET,     (UINT8*)&Conf.ConfigData.CalibrationOffset},  /* Temperature Calibration Offset*/
   {ENDOFCYCLE,    (UINT8*)&Conf.ConfigData.EndOfCycle},
   {PASSWORD,      (UINT8*)&Conf.ConfigData.Password},        /* Password to view further registers 5x*/
   {PSWATTAGE,     (UINT8*)&Conf.ConfigData.PsWattage},         /* P/S Wattage      */
   {REGION,        (UINT8*)&Conf.ConfigData.Region},            /* Region setup*/
   {FACTCOLDSTART, (UINT8*)&Conf.ConfigData.FactColdStart},        /* Soft Reset*/
   
   {TIME,          (UINT8*)&TempPreset.presetData.Time, 			(UINT8*)&CurrentPreset.presetData.Time },      /* Off Time in mSec*/
   {ENERGY,        (UINT8*)&TempPreset.presetData.Energy, 			(UINT8*)&CurrentPreset.presetData.Energy},        /* ON time in Msec*/
   {ONTIME,        (UINT8*)&TempPreset.presetData.OnTime, 			(UINT8*)&CurrentPreset.presetData.OnTime},  /* Total Time is MSec*/
   {OFFTIME,       (UINT8*)&TempPreset.presetData.OffTime, 			(UINT8*)&CurrentPreset.presetData.OffTime},  /* Total Time is MSec*/
   {ONENERGY,      (UINT8*)&TempPreset.presetData.OnEnergy,			(UINT8*)&CurrentPreset.presetData.OnEnergy},                /* TimeOut Hour*/
   {TOTALONTIME,   (UINT8*)&TempPreset.presetData.TotalOnTime,		(UINT8*)&CurrentPreset.presetData.TotalOnTime},                /* TimeOut Hour*/
   {TOTALONENERGY, (UINT8*)&TempPreset.presetData.TotalOnEnergy,	(UINT8*)&CurrentPreset.presetData.TotalOnEnergy},/* Pulse Temperature*/
   {PULSETEMPE,    (UINT8*)&TempPreset.presetData.PulseTempe, 		(UINT8*)&CurrentPreset.presetData.PulseTempe},  /* Amplitude*/
   {MAXTEMPE,      (UINT8*)&TempPreset.presetData.MaxTempe, 		(UINT8*)&CurrentPreset.presetData.MaxTempe},    /* Welding  Mode*/
   {AMPLITUDE,     (UINT8*)&TempPreset.presetData.AmplitudeValue, 	(UINT8*)&CurrentPreset.presetData.AmplitudeValue},  /* Amplitude*/
   {MICROTIP,	   (UINT8*)&TempPreset.presetData.Microtip, 		(UINT8*)&CurrentPreset.presetData.Microtip},
   {WELDMODE,      (UINT8*)&TempPreset.presetData.WeldMode, 		(UINT8*)&CurrentPreset.presetData.WeldMode},    /* Welding  Mode*/
   {CONF_MODE,     (UINT8*)&TempPreset.presetData.WeldMode, 		(UINT8*)&CurrentPreset.presetData.WeldMode},    /* Welding  Mode*/  
   {CONF_CONTROL,  (UINT8*)&TempPreset.presetData.WeldMode, 		(UINT8*)&CurrentPreset.presetData.WeldMode},    /* Welding  Mode*/
   {CONF_CONTROL2, (UINT8*)&TempPreset.presetData.WeldMode, 		(UINT8*)&CurrentPreset.presetData.WeldMode},    /* Welding  Mode*/
   
   
   
   {D_TOTALTIME,   (UINT8*)&currentLCDData.TotalTime},
   {D_TOTALENERGY, (UINT8*)&currentLCDData.TotalEnergy},
   {D_AMPLITUDE,   (UINT8*)&currentLCDData.AmplitudeValue},
   {D_PROGRESS,    (UINT8*)&currentLCDData.Progress},
   {D_MAXTEMP,     (UINT8*)&currentLCDData.MaxTemp},
   {D_LASTTEMP,    (UINT8*)&currentLCDData.LastTemp},
   {D_CURRENTTEMP, (UINT8*)&currentLCDData.CurrentTemp},
   {D_PEAKPOWER,   (UINT8*)&currentLCDData.PeakPower},
   {D_CURRENTPOWER,(UINT8*)&currentLCDData.CurrentPower}, /* Current Power*/
   {D_ONTIME,      (UINT8*)&currentLCDData.OnTime},
   {D_ONENERGY,    (UINT8*)&currentLCDData.OnEnergy},
   {D_OFFTIME,     (UINT8*)&currentLCDData.OffTime},
   {D_TOTALONTIME, (UINT8*)&currentLCDData.TotalOnTime},
   
   {MESSAGE,       (UINT8*)&FnMSG},                                                 /* Ready Message Disply*/
   {FN_NUM,        (UINT8*)&CurrFnScreen}, /* P/S Wattage      */
   {NOMESSAGE,     (UINT8*)&NoMSG},                                                 /* No Message */
   {ERROR,         (UINT8*)&FnERR},                                                 /* Error Message Display*/
   {PRESET_NUM,    (UINT8*)&PresetNo},           /* Display Preset Number and Save or Load*/
};

const UINT8 GDisplay_Remap[TOTALLCDSEGMENTS] = {     //register[7:3] and bit[2:0]

   //   0x00 / 0   
      0x4B,       // Digit 6 Seg a     
      0x47,       // Digit 6 Seg b
      0x45,       // Digit 6 Seg c
      0x48,       // Digit 6 Seg d
      0x49,       // Digit 6 Seg e
      0x4A,       // Digit 6 Seg f
      0x46,       // Digit 6 Seg g        ***  
      0x18,       // Place holder

   //   0x08 / 8    
      0x53,       // Digit 5 Seg a     
      0x4F,       // Digit 5 Seg b
      0x4D,       // Digit 5 Seg c
      0x50,       // Digit 5 Seg d
      0x51,       // Digit 5 Seg e
      0x52,       // Digit 5 Seg f
      0x4E,       // Digit 5 Seg g        ***
      0x18,       // Place holder     

   //   0x10 / 16
      0x73,       // Digit 4 Seg a     
      0x8F,       // Digit 4 Seg b
      0x8D,       // Digit 4 Seg c
      0x70,       // Digit 4 Seg d
      0x71,       // Digit 4 Seg e
      0x72,       // Digit 4 Seg f
      0x8E,       // Digit 4 Seg g        ***
      0x18,       // Place holder 

   //   0x18 / 24 
      0x0F,       // Digit 3 Seg a     
      0x93,       // Digit 3 Seg b
      0x91,       // Digit 3 Seg c
      0x0C,       // Digit 3 Seg d
      0x0D,       // Digit 3 Seg e
      0x0E,       // Digit 3 Seg f
      0x92,       // Digit 3 Seg g        ***
      0x18,       // Place holder

   //   0x20 / 32 
      0x83,       // Digit 2 Seg a
      0x0A,       // Digit 2 Seg b
      0x09,       // Digit 2 Seg c
      0x7C,       // Digit 2 Seg d
      0x7D,       // Digit 2 Seg e
      0x7E,       // Digit 2 Seg f
      0x81,       // Digit 2 Seg g	      *** 
      0x18,       // Place holder

   //   0x28 / 40 
      0x77,       // Digit 1 Seg a     ==> Digits Start Here
      0x7B,       // Digit 1 Seg b
      0x79,       // Digit 1 Seg c
      0x74,       // Digit 1 Seg d
      0x75,       // Digit 1 Seg e
      0x76,       // Digit 1 Seg f
      0x7A,       // Digit 1 Seg g        ***
      0x18,       // Place holder

   //   0x30 / 48            
      0x02,       //(ON_ICON)
      0x08,       //(MICROTIP)
      0x42,       //(OFFTIME)
      0x00,       //(CLOCK)  
      0x05,       //(TEMPERARTURE)
      0x44,       //(PERCENT)
      0x11,       //(PULSE)      
      0x80,       //(MIN/SEC)
      0x07,       //(AMPLITUDE)          
      0x10,       //(CONT)       
      0x8C,       //(ALARM)
      0x78,       //(SONICS)
      0x04,       //(JOULES)     
      0x90,       //DP
      0x0B,       //(COLON)
      0x41,       //(TOTAL TIME)
      0x7F,       //(PRESET)
      0x01,       //(LINE2)   
      0x06,       //(LINE3)
      0x43,       //(LINE4)
      0x4C,       //(WATTS)      
      0x82,       // HR/MIN/SEC
      0x03,        //(OFF_ICON)
      0x13,       //(MAX)
      0x12,       //(Pulse2)
      0x40        //(Progress)    
   // 0 to 73 -- 74 Different Entities  
};
               
UINT8 GCHAR_CODE[TOTALASCIICHAR] = {
         SPACE, EXCLAM, QUOTES, HASH, DOLLAR, PERCENT, AMPERSAND,
         APOSTROPHE, L_PAREN, R_PAREN, STAR,PLUS, COMMA, MINUS, DOT, SLASH,
         ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
         COLON, SEMICOLON, LESS_THEN, EQUAL, GREATER_THEN, QUERY, AT_SIGN,
         XAX, XBX, XCX, XDX, XEX, XFX, XGX, XHX, XIX, XJX, XKX, XLX, XMX,
         XNX, XOX, XPX, XQX, XRX, XSX, XTX, XUX, XVX, XWX, XXX, XYX, XZX,
         L_BRACKET, BACKSLASH, R_BRACKET, CIRCUMFLEX, UNDERSCORE, REVERSE_QUOTE ,
         YaY, YbY, YcY, YdY, YeY, YfY, YgY, YhY, YiY, YjY, YkY, YlY, YmY, YnY,
         YoY, YpY, YqY, YrY, YsY, YtY, YuY, YvY, YwY, YxY, YyY, YzY,
         L_BRACES, BAR, R_BRACES, TILDE };


RESULT Results[ID_PS_INFINITE_ENERGY_B+1] = {
		{ID_CS_TIME,				7,	R_TOTAL_TIME,		R_TOTAL_ENERGY,		R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_MAXTEMP,	R_ENDTEMP,	0,			0,				0},
		{ID_CS_ENERGY,				7,	R_TOTAL_ENERGY,		R_TOTAL_TIME,		R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_MAXTEMP,	R_ENDTEMP,	0,			0,				0},
		{ID_CS_INFINITE,			6,	R_TOTAL_TIME,		R_TOTAL_ENERGY,		R_AMPLITUDE,		R_PEAKPOWER,		R_MAXTEMP,		R_ENDTEMP,	0,			0,			0,				0},
		{ID_PS_TIME,				10,	R_TOTAL_ON_TIME,	R_TOTAL_ON_ENERGY,	R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_MAXTEMP,	R_ENDTEMP,	R_ON_TIME,	R_OFF_TIME,		R_TOTAL_TIME},
		{ID_PS_ENERGY,				10,	R_TOTAL_ON_ENERGY,	R_TOTAL_ON_TIME,	R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_MAXTEMP,	R_ENDTEMP,	R_ON_ENERGY,R_OFF_TIME,		R_TOTAL_TIME},
		{ID_PS_INFINITE_TIME,		9,	R_TOTAL_ON_TIME,	R_TOTAL_ON_ENERGY,	R_AMPLITUDE,		R_PEAKPOWER,		R_MAXTEMP,		R_ENDTEMP,	R_ON_TIME,	R_OFF_TIME,	R_TOTAL_TIME,	0},
		{ID_PS_INFINITE_ENERGY,		9,	R_TOTAL_ON_ENERGY,	R_TOTAL_ON_TIME,	R_AMPLITUDE,		R_PEAKPOWER,		R_MAXTEMP,		R_ENDTEMP,	R_ON_ENERGY,R_OFF_TIME,	R_TOTAL_TIME,	0},
		{ID_MAX_TEMP_CS,			7,	R_MAXTEMP,			R_ENDTEMP,			R_TOTAL_TIME,		R_TOTAL_ENERGY,		R_AMPLITUDE,	R_PROGRESS,	R_PEAKPOWER,0,			0,				0},
		{ID_MAX_TEMP_PS_TIME,		10,	R_MAXTEMP,			R_ENDTEMP,			R_TOTAL_ON_TIME,	R_TOTAL_ON_ENERGY,	R_AMPLITUDE,	R_PROGRESS,	R_PEAKPOWER,R_ON_TIME,	R_OFF_TIME,		R_TOTAL_TIME},
		{ID_MAX_TEMP_PS_ENERGY,		10,	R_MAXTEMP,			R_ENDTEMP,			R_TOTAL_ON_ENERGY,	R_TOTAL_ON_TIME,	R_AMPLITUDE,	R_PROGRESS,	R_PEAKPOWER,R_ON_ENERGY,R_OFF_TIME,		R_TOTAL_TIME},
		{ID_TEMP_LIMIT_CS_TIME,		8,	R_TOTAL_TIME,		R_TOTAL_ON_TIME,	R_TOTAL_ENERGY,		R_AMPLITUDE,		R_PROGRESS,		R_PEAKPOWER,R_MAXTEMP,	R_ENDTEMP,	0,			    0},
		{ID_TEMP_LIMIT_CS_ENERGY,	8,	R_TOTAL_ENERGY,		R_TOTAL_ON_TIME,	R_TOTAL_TIME,		R_AMPLITUDE,		R_PROGRESS,		R_PEAKPOWER,R_MAXTEMP,	R_ENDTEMP,	0,				0},
		{ID_TEMP_LIMIT_PS_TIME,		10,	R_TOTAL_ON_TIME,	R_TOTAL_ON_ENERGY,	R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_MAXTEMP,	R_ENDTEMP,	R_ON_TIME,	R_OFF_TIME,		R_TOTAL_TIME},
		{ID_TEMP_LIMIT_PS_ENERGY,	10,	R_TOTAL_ON_ENERGY,	R_TOTAL_ON_TIME,	R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_MAXTEMP,	R_ENDTEMP,	R_ON_ENERGY,R_OFF_TIME,		R_TOTAL_TIME},
		{ID_PULSE_TEMP_CS_TIME,		8,	R_TOTAL_TIME,		R_TOTAL_ON_TIME,	R_TOTAL_ENERGY,		R_AMPLITUDE,		R_PROGRESS,		R_PEAKPOWER,R_MAXTEMP,	R_ENDTEMP,	0,				0},
		{ID_PULSE_TEMP_CS_ENERGY,	8,	R_TOTAL_ENERGY,		R_TOTAL_ON_TIME,	R_TOTAL_TIME,		R_AMPLITUDE,		R_PROGRESS,		R_PEAKPOWER,R_MAXTEMP,	R_ENDTEMP,	0,				0},
		{ID_PULSE_TEMP_PS_TIME,		10,	R_TOTAL_ON_TIME,	R_TOTAL_ON_ENERGY,	R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_MAXTEMP,	R_ENDTEMP,	R_ON_TIME,	R_OFF_TIME,		R_TOTAL_TIME},
		{ID_PULSE_TEMP_PS_ENERGY,	10,	R_TOTAL_ON_ENERGY,	R_TOTAL_ON_TIME,	R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_MAXTEMP,	R_ENDTEMP,	R_ON_ENERGY,R_OFF_TIME,		R_TOTAL_TIME},	
		{ID_CS_TIME_B,				5,	R_TOTAL_TIME,		R_TOTAL_ENERGY,		R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,			0,			0,			0,	0,				0},
		{ID_CS_ENERGY_B,			5,	R_TOTAL_ENERGY,		R_TOTAL_TIME,		R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,			0,			0,			0,	0,				0},
		{ID_CS_INFINITE_B,			4,	R_TOTAL_TIME,		R_TOTAL_ENERGY,		R_AMPLITUDE,		R_PEAKPOWER,		0,						0,			0,			0,	0,				0},
		{ID_PS_TIME_B,				8,	R_TOTAL_ON_TIME,	R_TOTAL_ON_ENERGY,	R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_ON_TIME,	R_OFF_TIME,	R_TOTAL_TIME,0,				0},
		{ID_PS_ENERGY_B,			8,	R_TOTAL_ON_ENERGY,	R_TOTAL_ON_TIME,	R_AMPLITUDE,		R_PROGRESS,			R_PEAKPOWER,	R_ON_ENERGY,R_OFF_TIME,	R_TOTAL_TIME,0,				0},
		{ID_PS_INFINITE_TIME_B,		7,	R_TOTAL_ON_TIME,	R_TOTAL_ON_ENERGY,	R_AMPLITUDE,		R_PEAKPOWER,		R_ON_TIME,		R_OFF_TIME,	R_TOTAL_TIME,	0,		0,				0},
		{ID_PS_INFINITE_ENERGY_B,	7,	R_TOTAL_ON_ENERGY,	R_TOTAL_ON_TIME,	R_AMPLITUDE,		R_PEAKPOWER,		R_ON_ENERGY,	R_OFF_TIME,	R_TOTAL_TIME,	0,		0,				0},
				
};


/* Internal Functions */
void Numout(UINT32 Value, UINT8 Base, UINT8 LC, UINT8 Ptr[], UINT8 Width)
/*****************************************************************************/
/*                                                                           */
/*  ABSTRACT                                                                 */
/*  --------                                                                 */
/*  This function converts a number from binary form to an ASCII string for  */
/*  display or printing.                                                     */
/*                                                                           */
/*  PARAMETERS                                                               */
/*  ----------                                                               */
/*     Value   The number whose ASCII representation is desired.             */
/*     Base    An integer between 2 and 16, inclusive, specifying in what    */
/*             number base 'Value' is to be interpreted.                     */
/*     LC      (Leading Character): Leading zeroes will be designated by     */
/*             'LC', which should be an ASCII character.                     */
/*     Ptr     The address of the buffer of at least 'Width' bytes, into     */
/*             which the ASCII representation is placed.                     */
/*     Width   The number of character positions desired in the ASCII format.*/
/*                                                                           */
/*  DESCRIPTION                                                              */
/*  -----------                                                              */
/*  If the 'Value' is too large to be represented in 'Width' characters of   */
/*  the chosen 'Base', then only the least significant digits will be        */
/*  represented.                                                             */
/*                                                                           */
/*  There is no error checking.  Input values must conform to the restraints:*/
/*             Base > 1 and Base < 17                                        */
/*             Width > 0                                                     */
/*                                                                           */
/*****************************************************************************/
{
   UINT8   Digits[] = "0123456789ABCDEF";
   UINT8 i;

   for (i = 1; i <= Width; i++) {
      Ptr[i-1] = Digits[Value % Base];
      Value /= Base;
   }
   i = Width-1;

   while ((Ptr[i] == '0') && (i >=0)) {
      Ptr[i] = LC;
      i--;
   }
}


void DisplaySegments(void) 
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Function Display characters from DisplayCodes on LCD seven segment display.
*        Function is also responsible to blink the required digit set in CurrentDigit.
*   Exit condition: 
*                                                                         
***************************************************************************/
{
   UINT8 index;
   UINT8 i;
   UINT8 lcdRamLocationCode;
   UINT8 lcdRamBit;
   UINT8 lcdRamIndex;

   lcdRam = &LCDWF0;

   for (index = 0; index < NUM_7SEGMENT_DIGITS; index++){// 6 Digits to be changed
      for (i = 0; i < NUM_SEGMENT_PER_DIGIT + 1; i++) {  // 8 Segments in each digit
         lcdRamLocationCode = GDisplay_Remap[((index ) * (NUM_SEGMENT_PER_DIGIT + 1)) + i];
         lcdRamBit = lcdRamLocationCode & BIT_NUM_MASK;
         lcdRamIndex = (lcdRamLocationCode & PORT_INDEX_MASK) >> 2;
         
         if ((GCHAR_CODE[DisplayCodes[index]- ASCIIOFFSET]) & (0x80 >> i)){
        	if(((CurrentDigit.digit & (0x01 << index)) == (0x01 << index)) && (CurrentDigit.Blinking == TRUE) && ~BlinkTimer)  
        		lcdRam[lcdRamIndex] &= ~(0x01 << lcdRamBit);
            else
            	lcdRam[lcdRamIndex] |= (0x01 << lcdRamBit);
         }
         else {
            lcdRam[lcdRamIndex] &= ~(0x01 << lcdRamBit);
         }
      }
   }// end of for mapping loop
}
 void DisplayIcons(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Sets up display for showing approriate icons(fixed and blinking)
*                                                                         
*   Entry condition: 
*
*        Uses icon global and icon variable to load registers to display. 
*
*   Exit condition: 
*                                                                         
***************************************************************************/
{
   UINT8 *lcdRam;
   UINT8 i;
   volatile UINT8 lcdRamBit, lcdRamIndex;
   volatile UINT8 curIcon;
   UINT32 blinkIcon, TempFixedIcon;
   static UINT32 LastFixedIcons = 0;
   static UINT32 LastBlinkIcons = 0;
   static UINT32 LastBlinkTimer = 0;
   lcdRam = &LCDWF0;
   
   if(BlinkTimer)
      blinkIcon = BlinkIcons;
   else
      blinkIcon=0;
   
   if(AlarmFlag == TRUE)
      BlinkIcons |= (UINT32)ALARM_I;
   if((screen == SCREEN_RDY || screen == SCREEN_RESULTS || CurrentParam == AMPLITUDE) )
	   if (CurrentPreset.presetData.Microtip == TRUE)
		   FixedIcons |= (UINT32)MICROTIP_I;
	   else
		   FixedIcons &= ~(UINT32)MICROTIP_I;
   
   if (screen == SCREEN_PRESET) //This way the previous condition is not too long.
   {
	   if ((LoadPreset==TRUE && CurrentPreset.presetData.Microtip==TRUE) || (LoadPreset==FALSE && TempPreset.presetData.Microtip==TRUE) )
		   FixedIcons|= MICROTIP_I;
	   else
		   FixedIcons &= ~MICROTIP_I;
	   
   }
   
   if((IOValue & SONICS_ACTIVE ) == SONICS_ACTIVE)
      FixedIcons |= SONICS_I;
   else
      FixedIcons &= ~SONICS_I;
   
   TempFixedIcon = FixedIcons & BlinkIcons;
   FixedIcons ^= TempFixedIcon;
   
   
  	if((LastFixedIcons != FixedIcons)||(LastBlinkIcons != BlinkIcons) || (LastBlinkTimer != BlinkTimer)){
  	   for (i = 0; i < NUM_ICONS; i++){                      // clear all icons
         curIcon = GDisplay_Remap[MAP_ICON + i];            // store icon location
         lcdRamBit = curIcon & BIT_NUM_MASK;                // get bit number
         lcdRamIndex = (curIcon & PORT_INDEX_MASK) >> 2;    // get port index
         lcdRam[lcdRamIndex] &= ~(0x01 << lcdRamBit);       // clear bit
      }
      
  	   
  	   
      if ((FixedIcons != 0) || (BlinkIcons != 0)){
         for (i = 0; i < NUM_ICONS; i++){
        	 if (((FixedIcons & (1L << i)))^((blinkIcon & (1L << i))))
            {
               curIcon = GDisplay_Remap[MAP_ICON + i];      // store icon location
               lcdRamBit = curIcon & BIT_NUM_MASK;          // get bit number
               lcdRamIndex = (curIcon & PORT_INDEX_MASK) >> 2; // get port index
               lcdRam[lcdRamIndex] |= (0x01 << lcdRamBit);  // turn on icon
            }
         } 
      }
  	}
    LastFixedIcons = FixedIcons;
	LastBlinkIcons = BlinkIcons;
	LastBlinkTimer = BlinkTimer;
}
      
void TurnONorOFFLCDSegment(UINT8 IconIndex,BOOLEAN OnorOff) 
/***************************************************************************
* Function Name: TurnONorOFFLCDSegment
*
* Agruments:
* 1. IconIndex:  Index of segment stored in GDisplay_Remap.
* 2. OnorOff: ON for turn on and OFF for turn off.
*
* Return Type: void.
*
* Description:
*        Function Turns On Or Off the required Segment on LCD display.                                                  
***************************************************************************/
{
   UINT8 *lcdRam;
   volatile UINT8 lcdRamBit, lcdRamIndex,curIcon;
   if(IconIndex < TOTALLCDSEGMENTS) {
      lcdRam = &LCDWF0;
      curIcon = GDisplay_Remap[IconIndex];
      lcdRamBit = curIcon & BIT_NUM_MASK;                   // get bit number
      lcdRamIndex = (curIcon & PORT_INDEX_MASK) >> 2;       // get port index
      
      if(OnorOff == ON)
         lcdRam[lcdRamIndex] |= (0x01 << lcdRamBit);        // turn on Segment
      else
         lcdRam[lcdRamIndex] &= ~(0x01 << lcdRamBit);       //turn Off Segment 
   }
} 
 
   
void ALLIconsOFF(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Turns OFF all the Icons at power up
*                                                    
***************************************************************************/
{
   UINT8 i;
   for (i = 0; i < NUM_ICONS; i++)                      // clear all icons
      TurnONorOFFLCDSegment(MAP_ICON + i, OFF); 

}
   

void DisplayAllIcons(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Function Display All icons on LCD display at power up.
*        All power level segments will be active one by one.                                                                 
*   Exit condition: 
*                                                                         
***************************************************************************/
{
   UINT8 i; 
   for (i = 0; i < TOTALLCDSEGMENTS; i++)                     // clear all icons
      TurnONorOFFLCDSegment( i, ON);

}
   
void DisplayError() 
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to update error screen flasing between invalid digits and ----
*        or error msg.                                                                
*   Entry condition:  
*
*   Exit condition: 
*                                                                         
***************************************************************************/ 
{
   FixedIcons &= ~(UINT32)(DOT_I | MICROTIP_I | PERCENT_I | SECONDS |WATTAGE | HRMINSEC_I);
   if (HalfSec == TRUE){ 
	      if(UpdateTimer == TRUE) {
	         //every alternate seconds
	         if(CurrentAlarm == ER_DE)
	         {
	        	 Strcpy(DisplayCodes,AuxDisplayCodes);
	        	 switch (AuxCurrentParam)
	        	 {
					 case ENERGY:
					 case ONENERGY:
						 FixedIcons &= ~(UINT32)COLON_I; 
						 break;
					 default:
						 FixedIcons |= (UINT32)COLON_I;
	        	 }
	         }
	         else
	         {
	            Strcpy(DisplayCodes,FnERR);
	            FixedIcons |= (UINT32)COLON_I;
	         }
	         
	         if (currentLCDData.HandHeld == TRUE && ((CurrentAlarm != ER_DE) && (CurrentAlarm != ER_TEMP)))
	        	 ClrUserOut();
	      } 
      else {
    	 FixedIcons |= (UINT32)COLON_I;
         switch(CurrentAlarm)
         {  
            case ER_OL:      /* Overload     */
               Strcpy(DisplayCodes,Er_OL_Msg);
            break;
            case ER_SS:      /* Start Switch */
               Strcpy(DisplayCodes,Er_SS_Msg);
            break;
            case ER_DE: 
            	Strcpy(DisplayCodes,Er_DE_Msg);
            break;
            case ER_TEMP:         /* Temperature Probe Disconnected */
               Memcpy(&DisplayCodes, &Er_TempSens,NUM_7SEGMENT_DIGITS);
            break;
            case ER_uTIP:           /* Microtip Error - Amplitude >70% */
               Memcpy(&DisplayCodes, &Er_MicroTip,NUM_7SEGMENT_DIGITS);
            break;
            case ER_TEMPERROR:   // Current Temperature > MAX Temperature at Start of Weld
                                 // meaning MAX Temperature is Less than Facility Temperature
               Memcpy(&DisplayCodes, &Er_TempError,NUM_7SEGMENT_DIGITS);
            break;
            case ER_TIME_2:     // TimeOut > TotalWeldTime
               Memcpy(&DisplayCodes, &Er_TimeError2,NUM_7SEGMENT_DIGITS);
            break;
            case ER_TIMEOUTREACH:
            	Memcpy(&DisplayCodes, &Er_TimeOutReach,NUM_7SEGMENT_DIGITS);
            break;
            case ER_HH:
            	Memcpy(&DisplayCodes, &Er_HandHeld,NUM_7SEGMENT_DIGITS);
            break;
            case ER_HH_FPanel:
            	Memcpy(&DisplayCodes, &Er_FrontPanel,NUM_7SEGMENT_DIGITS);
            break;
         }
         if (currentLCDData.HandHeld == TRUE && ((CurrentAlarm != ER_DE) && (CurrentAlarm != ER_TEMP)))
        	 SetUserOut();
      }
      UpdateTimer = !UpdateTimer;
      HalfSec = FALSE;
   }
   UpdateLiveReady = TRUE;
}
   

void UpdateLive(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to update screen with live data
*                                                                         
*   Entry condition:  
*
*   Exit condition: 
*                                                                         
***************************************************************************/
{
   if(screen == SCREEN_RESULTS)
      UpdateDisplayValue();

   
   if(screen == SCREEN_ERROR)  {
      CurrentParam = ERROR;
      DisplayError();
   }    
   DisplayIcons();
   DisplaySegments();
}
   
static UINT32 ToSec(UINT32 time)
/***************************************************************************
* Function Name: ToSec
*
* Agruments:
* 1. time:  Time is in Format HHMMSS.. 3hrs 20min 33sec is represented as 
*           032033 in Time variable.
*
* Return Type: 32bit value of Time in Seconds is returned. for 032033 
*              return value is 10800 + 1200 + 33 = 12033sec
*
* Description:
*        this function convers the Time in Format HHMMSS to Time in seconds                                                  
***************************************************************************/
{
   UINT32 Temp[3];
   UINT32 Time;
   Temp[0] = time%100;             //SS stored in  Temp[0]
   time = time/100;
   Temp[1] = time%100;             //MM stored in  Temp[1]
   time = time/100;
   Temp[2] = time%100;             //HH stored in  Temp[2]
   time = time/100;
   Temp[1] = Temp[1]*SEC_IN_MINUTE;           //seconds in MM minutes
   Temp[2] = Temp[2]*SEC_IN_HOUR;         //seconds in HH hour
   Time = Temp[0] + Temp[1] + Temp[2];
   /*PrintStr("\n\r Time Value = ");
   Print32(Time);*/
   return Time;
}
   
UINT32 ToHHMMSS(UINT32 time) 
/***************************************************************************
* Function Name: ToHHMMSS
*
* Agruments:
* 1. time:  32bit value of Time in Seconds is recieved.For Example 12033sec 
*
* Return Type: 3hrs 20min 33sec (12033sec) is represented in Format HHMMSS
*              and 032033 in Returned value
*
* Description:
*        this function convers the  Time in seconds to Format HHMMSS Format                                                  
***************************************************************************/
{
   UINT32 Temp[3];
   UINT32 Time;

   Temp[0] = time%60;
   time = time/60;
   Temp[1] = time%60;
   time = time/60;
   Temp[2] = time;

   Temp[1] = Temp[1]*100;
   Temp[2] = Temp[2]*10000;
   Time = Temp[0] +  Temp[1] + Temp[2];
   return Time;           //HHMMSS format
}

static UINT32 ToMilliSec(UINT32 time)
/***************************************************************************
* Function Name: ToMilliSec
*
* Agruments:
* 1. time:  Time is in Format MMSSMS.. 42min 59sec 910MS is represented as 
*           425991Ms in Time variable.
*
* Return Type: 32bit value of Time in Milli Seconds is returned. for 425991 
*              return value is 42x60x100 +59x100 + 91 = 287991 * 10Ms
*
* Description:
*        this function convers the Time in Format MMSSMS to Time in seconds                                                  
***************************************************************************/
{
   UINT32 Temp[3];
   UINT32 Time;
   Temp[0] = time%100;             //MS stored in  Temp[0]//
   time = time/100;
   Temp[1] = time%100;             //SS stored in  Temp[1]
   time = time/100;
   Temp[2] = time%100;             //MM stored in  Temp[2]
   time = time/100;
   Temp[2] = Temp[2]*SEC_IN_MINUTE*MILLLISEC_IN_SEC;//Milli seconds in MM minutes
   Temp[1] = Temp[1]*MILLLISEC_IN_SEC;              //Milli seconds in SS Seconds
   Temp[0] = Temp[0]*10;                            //Milli seconds in MS
   Time = Temp[0] + Temp[1] + Temp[2];
   return Time;
}
   
static UINT32 ToMMSSMS(UINT32 time) 
/***************************************************************************
* Function Name: ToMMSSMS
*
* Agruments:
* 1. time:  32bit value of Time in Milli Seconds is recieved.For Example 287991Ms 
*
* Return Type: 42min 59sec 910MS is represented in Format MMSSMS
*              and 425991 in Returned value
*
* Description:
*        this function convers the Time in seconds to Format MMSSMS Format                                                  
***************************************************************************/
{
   UINT32 Temp[3];
   UINT32 Time;
   time = time/10;      //Least Significant Digit is removed
   Temp[0] = time%100;
   time = time/100;
   Temp[1] = time%60;
   time = time/60;
   Temp[2] = time;

   Temp[1] = Temp[1]*100;
   Temp[2] = Temp[2]*10000;
   Time = Temp[0] +  Temp[1] + Temp[2];
   return Time;           //MMSSMS format
}

UINT16 ConvertTempToC(UINT16 Tempe) 
/***************************************************************************
* Function Name: ConvertTempToC
*
* Agruments:
* 1. Tempe: Time in Fahrenheit is recieved as a Parameter(320 - 2120)
*
* Return Type: Temperature value in Celcius is returned (00 - 1000.)
*
* Description:
*        this function convers the Temerature in Fahrenheit to Celsius
*        last digit is the first Decimal place value 103 means 10.3                                                  
         Temperature always represented in 10x Format. 15.9C is internally (10*15.9 = 159)
***************************************************************************/
{
     UINT16 Temperature ;
     UINT16 Temp;
     Temp = Tempe - ThirtyTwo_degrees_F;         //Celcius = 5/9 (Fahrenheit-32)
     Temp = (Temp) * 5;
     Temperature = (Temp+5)/9;       //Convert based on config setting
     return  Temperature;
}


UINT16 ConvertTempToF(UINT16 Tempe)
/***************************************************************************
* Function Name: ConvertTempToF
*
* Agruments:
* 1. time:  Temperature value in Celcius is recieved (00 - 100) 
*
* Return Type: Time in Fahrenheit is returned (320 - 2120)
*
* Description:
*        this function convers the Temerature in Celsius to Fahrenheit
*        last digit is the first Decimal place value 103 means 10.3  
*        Temperature always represented in 10x Format. 213F is internally (10*213 = 2130)
***************************************************************************/  
{
   UINT16 Temperature;
   UINT16 Temp;
   Temp = Tempe * 9;
   Temp = Temp/5;
   Temperature =  Temp + ThirtyTwo_degrees_F;       //Fahrenheit = 9/5 (Celcius+32)
   return  (Temperature);
}


bool StringIsOff(void)
/***************************************************************************
* Function Name: StringIsOff
*
* Arguments: N/A
*
* Return Type: it return a flag if the  DisplayCodes match with the Off_Msg string
*
* Description:
*        This function verify if the string " OFF  "  is displayed on the LCD.
***************************************************************************/ 
{
	int e=0;
	int x;
	bool IsOff = FALSE;
	for (x=0; x<NUM_7SEGMENT_DIGITS; x++)   
			if (DisplayCodes[x] == Off_Msg[x])
				e+=1;

	if (e==Six_Char)
		IsOff = TRUE;
	
	return(IsOff);
}


void UpdateParamVal( bool Up )
/***************************************************************************
* Function Name: UpdateParamVal
*
* Arguments: 
* 	Up = TRUE to increment or Up = FALSE to decrement  
*
* Return Type: N/A
*
* Description:
*	This function is designed to increment or decrement the value selected 
*	first convert the Off string to the numeric equivalent then change
*	value rolling the Numbers while updating a parameter pressing up or 
*	down keys. 
***************************************************************************/ 
{ 
	SINT8 x;
	STR  StrOff[NUM_7SEGMENT_DIGITS+1];
	UINT16 TempVal;			
	if (StringIsOff() == TRUE)
	{
		if (CurrentParam == PULSETEMPE || CurrentParam == MAXTEMPE) 
		{
			if (CurrentParam == PULSETEMPE)
			{
				if(Conf.ConfigData.Tempe_C_F == 0)
				{
					StrOff[Zero_Char]= 'C';
					StrOff[One_Char] = '^';
				}
				else
				{
					StrOff[Zero_Char]= 'F';
					StrOff[One_Char] = '^';
				}
				if(Conf.ConfigData.Tempe_C_F == 0)
				{
					if (TempPreset.presetData.MaxTempe>= Two_degrees_C)
					{
						TempVal = TempPreset.presetData.MaxTempe - One_degree_C;
						TempVal = ConvertTempToC(TempVal);
						Numout((UINT32)TempVal,DECIMALBASE,'0', &StrOff[2], Four_Char);
					}
					else
					{
						Memcpy(&StrOff[2], &MinLimitC[2], Four_Char);
					}
				}
				else
				{
					if (TempPreset.presetData.MaxTempe>=ThirtyFive_degrees_F)
						TempVal = TempPreset.presetData.MaxTempe - Two_degrees_F;
					else
						TempVal = ThirtyTwo_degrees_F;
					Numout((UINT32)TempVal,DECIMALBASE,'0', &StrOff[2], Four_Char);
				}	

			}
			else
			{
				if(Conf.ConfigData.Tempe_C_F == 0)
					Strcpy(StrOff, OffTempC);
				else
					Strcpy(StrOff, OffTempF);
			}
			
			Num_Current.NumericData.Value=Num_Max.NumericData.Value;
			Strcpy(DisplayCodes, StrOff);
			BlinkDigitAux=0x38;
		}
		else
		{
			Strcpy(DisplayCodes, StrZero);
			Num_Current.NumericData.Value = 0;
		}
		
		CurrentDigit.digit = BlinkDigitAux;
		BlinkIcons &= (UINT32)(~COLON_I);
		BlinkIcons &= (UINT32)(~DOT_I);
	}	
	
	if (CurrentParam == TIME || CurrentParam == ONTIME || CurrentParam == OFFTIME || CurrentParam == TOTALONTIME)
		BlinkIcons |= COLON_I;
			
	if (CurrentParam == PULSETEMPE || CurrentParam == MAXTEMPE  )
		BlinkIcons |= DOT_I;
	if  (CurrentParam != D_AMPLITUDE)
	{
		if (Num_Current.NumericData.Value >= Num_Max.NumericData.Value && Up == TRUE)
			Num_Current.NumericData.Value = Num_Min.NumericData.Value;
		else 
			if (Num_Current.NumericData.Value <= Num_Min.NumericData.Value && Up == FALSE)
				Num_Current.NumericData.Value = Num_Max.NumericData.Value;
			else
				if (Up == TRUE)
					Num_Current.NumericData.Value+=Increment;
				else
					Num_Current.NumericData.Value-=Increment;
	}
	else
	{
		if (Up == TRUE && Num_Current.NumericData.Value < Num_Max.NumericData.Value)
			Num_Current.NumericData.Value+=Increment;
		else
			if (Up == FALSE && Num_Current.NumericData.Value > Num_Min.NumericData.Value)
				Num_Current.NumericData.Value-=Increment;
	}
	
	
	if (CurrentParam == FN_NUM ) 
		Numout(Fn_Screens[(UINT32)(Num_Current.NumericData.Value)].displayValue, DECIMALBASE,'0', &Num_Current.NumericData.displayValue[0], Six_Char); 
	else
		if (CurrentParam == PSWATTAGE) 
			Numout(MenuWattage[(UINT32)(Num_Current.NumericData.Value)].displayValue, DECIMALBASE,'0', &Num_Current.NumericData.displayValue[0], Six_Char); 
		else
			Numout(Num_Current.NumericData.Value, DECIMALBASE,'0', &Num_Current.NumericData.displayValue[0], Six_Char); 

	if ((CurrentParam == PULSETEMPE || CurrentParam == MAXTEMPE ) && BlockSelected == Third_Block )
	{
		if (Num_Current.NumericData.Value == Num_Min.NumericData.Value && Up ==TRUE)
		{
			DisplayCodes[2]='0';
		}
		else
			if (Num_Current.NumericData.Value == (Num_Max.NumericData.Value-1))
			{
				if (CurrentParam == MAXTEMPE)
				{
					DisplayCodes[2]='0';
					
				}
				else
				{
					if(Conf.ConfigData.Tempe_C_F == 0)
						DisplayCodes[2]=(ConvertTempToC(TempPreset.presetData.MaxTempe)%10)+'0';	
					else
						DisplayCodes[2]=(TempPreset.presetData.MaxTempe%10)+'0';	
				}
			}
	}
	
	for (x=0; x < QtyOfChars; x++)
		DisplayCodes[x+FirstChar]=Num_Current.NumericData.displayValue[x];
	
	if ((Num_Current.NumericData.Value == Num_Max.NumericData.Value && Up ==TRUE) || (Num_Current.NumericData.Value == Num_Min.NumericData.Value && Up ==FALSE))
		ReleaseFlag = FALSE;
		
	if (Num_Current.NumericData.Value ==0 || (( Num_Current.NumericData.Value == Num_Max.NumericData.Value) && BlockSelected == Third_Block))
		VerifyOFF();
	UpdateLiveReady = TRUE;
}
/*
STR* TemperatureOFF(void)    //will be implemented in future revisionts.
{
	STR StrOff[NUM_7SEGMENT_DIGITS+1];
	STR *Pointer;
	UINT16 TempVal = 0;
	if (Conf.ConfigData.Tempe_C_F == 0)
		Strcpy(StrOff, OffTempC);
	else
		Strcpy(StrOff, OffTempF);
				
	if (CurrentParam == PULSETEMPE)
	{
		TempVal = TempPreset.presetData.MaxTempe-20;					
		if(Conf.ConfigData.Tempe_C_F == 0)
			TempVal = ConvertTempToC(TempVal);
		Numout((UINT32)TempVal,DECIMALBASE,'0', &StrOff[2], 4);
	}
	Strcpy(Pointer, StrOff);
	return Pointer;

}*/


void VerifyOFF(void)
/***************************************************************************
* Function Name: VerifyOFF
*
* Arguments: N/A  
*
* Return Type: N/A
*
* Description:
*		Set the "Off" string on the LCD if the string on DisplayCodes is
*		same than StrOff while updating a parameter.
***************************************************************************/ 
{
	int x;
	int e=0;
	STR  StrOff[NUM_7SEGMENT_DIGITS+1];
	UINT16 TempVal;
	if ( CurrentParam == TIME || CurrentParam == TOTALONENERGY || CurrentParam == TOTALONTIME || CurrentParam == PULSETEMPE || (CurrentParam == MAXTEMPE && (TempPreset.presetData.WeldMode & CONFIG_TEMP) != CONFIG_TEMP ) )
	{
		if (CurrentParam == PULSETEMPE || CurrentParam == MAXTEMPE )
		{	
			if (CurrentParam == PULSETEMPE)
			{
				if(Conf.ConfigData.Tempe_C_F == 0)
				{
					if (TempPreset.presetData.MaxTempe>=Two_degrees_C)
					{
						TempVal = TempPreset.presetData.MaxTempe - One_degree_C;
						TempVal = (UINT32)ConvertTempToC(TempVal);
						Numout((UINT32)TempVal,DECIMALBASE,'0', &StrOff[2], Four_Char);
					}
					else
					{
						Strcpy(StrOff, MinLimitC);
					}
				}
				else
				{
					if (TempPreset.presetData.MaxTempe>=ThirtyFive_degrees_F)
						TempVal = TempPreset.presetData.MaxTempe - Two_degrees_F;
					else
						TempVal=ThirtyTwo_degrees_F;
					Numout((UINT32)TempVal,DECIMALBASE,'0', &StrOff[2], Four_Char);
				}
				
			}
			else
			{
				if(Conf.ConfigData.Tempe_C_F == 0)
					Strcpy(StrOff, OffTempC);
				else
					Strcpy(StrOff, OffTempF);
			}
			e=3;
			for (x=3; x<NUM_7SEGMENT_DIGITS; x++){
				if (DisplayCodes[x] == StrOff[x])
					e+=1;
			}
		}
		else	
			for (x=0; x<NUM_7SEGMENT_DIGITS; x++){
				if (DisplayCodes[x] == '0' || DisplayCodes[x] == ' ')
					e+=1;
			}
	}
	if (e == Six_Char)
	{			
		Strcpy(DisplayCodes, Off_Msg);
		BlinkDigitAux = CurrentDigit.digit;
		CurrentDigit.digit = BLINK_OFF_STRING;  
		BlinkIcons &= (UINT32)(~COLON_I);
		BlinkIcons &= (UINT32)(~DOT_I);
	}
	else
	{
		if (CurrentParam == TIME || CurrentParam == ONTIME || CurrentParam == OFFTIME || CurrentParam == TOTALONTIME)
			BlinkIcons |= COLON_I;
						
		if (CurrentParam == PULSETEMPE || CurrentParam == MAXTEMPE  )
				BlinkIcons |= DOT_I;
		
	}	
}
 
void GetLimits(void)
/***************************************************************************
* Function Name: GetLimits
*
* Arguments: N/A  
*
* Return Type: N/A
*
* Description:
*		Depending BlockSelected and CurrentParam it determine which will
*		be the maximum and minimum limit while updating a parameter.
***************************************************************************/
{
	UINT8 *ptrMax, *ptrMin, x;
	UINT32 Exp=1;
	STR TempMaxLimit[] = "      "; 
	UINT32 TempVal = 0;
	//Memcpy(ptrMin, &Parameters[CurrentParam].Min1, 6);
	//Memcpy(ptrMax, &Parameters[CurrentParam].Max1, 6);
	ptrMin = &Parameters[CurrentParam].Min1;
	ptrMax = &Parameters[CurrentParam].Max1;
	
	if (BlockSelected == First_Block){
		FirstChar = 0;
		QtyOfChars=Parameters[CurrentParam].QtyG1;	
	}
	else
	{
		if (BlockSelected == Second_Block){
			FirstChar = Two_Char;
			QtyOfChars=Parameters[CurrentParam].QtyG2;
		}
		else 
		{
			FirstChar = Six_Char - Parameters[CurrentParam].QtyG3;
			QtyOfChars=Parameters[CurrentParam].QtyG3;
		}
	}
	
	SetDigitBlink(); ////Set the blinking to the digits selected.
	for (x=0; x < NUM_7SEGMENT_DIGITS ; x++)
	{
		Num_Min.NumericData.displayValue[x]=0;
		Num_Max.NumericData.displayValue[x]=0;
	}
	
	Num_Current.NumericData.Value=0;
	Num_Min.NumericData.Value=0;
	Num_Max.NumericData.Value=0;
	
	if (CurrentParam == FN_NUM  )
	{
		if (StorePassword == SFX_PASSWORD)
			Num_Max.NumericData.Value=PswCorrectLimit;
		else
			Num_Max.NumericData.Value=PswWrongLimit;
					
		Num_Current.NumericData.Value=CurrFnScreen;
		Num_Min.NumericData.Value=0;	
	}
	else
	{
		if (CurrentParam == PSWATTAGE  )
		{
			Num_Min.NumericData.Value=PWR_150;
			Num_Max.NumericData.Value=PWR_550;
			Num_Current.NumericData.Value=Conf.ConfigData.PsWattage;
		}
		else
		{
			if ((CurrentParam == PULSETEMPE  || CurrentParam == MAXTEMPE || CurrentParam == CALOFFSET ) )
			{
				if (CurrentParam == PULSETEMPE)
				{
					if ( Conf.ConfigData.Tempe_C_F == 0)
					{
						if ( TempPreset.presetData.MaxTempe >= Two_degrees_C)
						{
							TempVal=TempPreset.presetData.MaxTempe;
							TempVal = ConvertTempToC((UINT16)TempVal);
							if ( TempPreset.presetData.MaxTempe >= Tree_degrees_C)
								TempVal=((TempVal-Three_degrees_F)/10)*10+9; 
							else
								TempVal=((TempVal-Two_degrees_F)/10)*10+9;
								
							ptrMin = &MinLimitC;
							Numout((UINT32)TempVal,DECIMALBASE,'0', &TempMaxLimit[2], Four_Char);
							//Memcpy(ptrMax, &TempMaxLimit, 6);
							ptrMax = &TempMaxLimit;
						}
						else
						{
							ptrMin = &MinLimitC;
							ptrMax = &MinLimitC;
						}
							//Memcpy(ptrMin, &MinLimitC, 6);
					}
					else
					{
						if ( TempPreset.presetData.MaxTempe >= ThirtyFive_degrees_F)
						{
							TempVal=TempPreset.presetData.MaxTempe-Four_degrees_F;
							TempVal=(TempVal/10)*10+9;

						}
						else
						{
							TempVal=ThirtyTwo_degrees_F;
						}
							//Memcpy(ptrMin, &Parameters[CurrentParam].Min1, 6);
						ptrMin = &Parameters[CurrentParam].Min1;
						Numout((UINT32)TempVal,DECIMALBASE,'0', &TempMaxLimit[2], Four_Char);
						//Memcpy(ptrMax, &TempMaxLimit, 6);
						ptrMax = &TempMaxLimit;
					}
				}
				else
				{
					if ( Conf.ConfigData.Tempe_C_F == 0)
					{
							//Memcpy(ptrMin, &MinLimitC, 6);
							//Memcpy(ptrMax, &MaxLimitC, 6);
						ptrMin=&MinLimitC;
						ptrMax=&MaxLimitC;
					}
				}
			}
			else
				if (CurrentParam==D_AMPLITUDE && CurrentPreset.presetData.Microtip == TRUE)
				{
					ptrMax = MaxLimAmp_Micro;
				}
			
			for (x=FirstChar; x < (FirstChar + QtyOfChars) ; x++)
			{
				Num_Min.NumericData.displayValue[x-FirstChar]=ptrMin[x];
				Num_Max.NumericData.displayValue[x-FirstChar]=ptrMax[x];
					
				Num_Current.NumericData.Value += ((DisplayCodes[x]-'0') * Exp);
				Num_Min.NumericData.Value += ((ptrMin[x]-'0') * Exp);
				Num_Max.NumericData.Value += ((ptrMax[x]-'0') * Exp);
				Exp*=10;
			}
			if ((CurrentParam == MAXTEMPE || CurrentParam == PULSETEMPE) && BlockSelected ==Third_Block && (TempPreset.presetData.WeldMode & CONFIG_TEMP)!= CONFIG_TEMP )
			{
				if (CurrentParam == MAXTEMPE) 
					Num_Max.NumericData.Value += 2;
				else
					if ((Conf.ConfigData.Tempe_C_F == 0 && TempPreset.presetData.MaxTempe >= Tree_degrees_C) || (Conf.ConfigData.Tempe_C_F == 1 && TempPreset.presetData.MaxTempe >= ThirtyFive_degrees_F))
					{
						Num_Max.NumericData.Value += 2;
					}
					else
						if (Conf.ConfigData.Tempe_C_F == 0 && ( TempPreset.presetData.MaxTempe < Tree_degrees_C  && TempPreset.presetData.MaxTempe >= Two_degrees_C) ) 
							Num_Max.NumericData.Value+=1;
			}
				
			if (CurrentParam == PULSETEMPE &&  BlockSelected ==Second_Block)
			{
				TempVal=0;
				Exp=10;
				for (x=3; x < NUM_7SEGMENT_DIGITS; x++)
				{
					TempVal+=((DisplayCodes[x]-'0')*Exp) ;
					Exp*=10;
				}
				if ( Conf.ConfigData.Tempe_C_F == 0)
				{
					if ( TempPreset.presetData.MaxTempe >= Two_degrees_C &&  (TempVal) ==((ConvertTempToC(TempPreset.presetData.MaxTempe-36))/10)*10)
						Num_Max.NumericData.Value= ConvertTempToC(TempPreset.presetData.MaxTempe) % 10 ;
				}
				else
				{
					if ( TempPreset.presetData.MaxTempe >= ThirtyFive_degrees_F && TempVal ==((TempPreset.presetData.MaxTempe-Three_degrees_F)/10)*10 )
					{
						Num_Max.NumericData.Value=TempPreset.presetData.MaxTempe % 10;
					}
				}
			}
		}
	}						
}

void AcceptValue(void)
/******************************************************************************/
/*                                                                            */
/*   This function will take the current value and combine the individual     */
/*   digits into one value.  this value will be saved in the proper place in  */
/* 	 the EEPROM.                                                               */
/******************************************************************************/
{
   UINT8 Adr = 0;   
   SINT32 PrevValue;
   enum TYPE Type = CONFIG;
   SINT32 expo = 1;
   SINT32 NewValue = 0 ;
   
   if (EscKeyPressed==FALSE)
   {
	   if  (StringIsOff() == FALSE)
	   { 
		   switch (Format[CurrentParam].Format) 
		   {
		   case COLDSTART:
			   Type = CONFIG; 
			   NewValue = DisplayCodes[Two_Char]-'0';
			   if(NewValue != 0) 
			   {
				   UpdateLive();
				   EEPROM_Restore();            
				   NewValue = 0;//Restore Coldstart value to 0.     
				   SavePresetSelected(0);
				   PresetDisplay=0;
			   }
			   ValueOkay = TRUE;
			   break;
		   case FACTCOLDSTART:
			   Type = CONFIG; 
			   NewValue = DisplayCodes[Two_Char]-'0';
			   if(NewValue != 0) 
			   {
			   		ResetEEPROM();//Wrong Magic Number will reset the EEPROM to defaults.
			   		//ColdStart_DS1804();
			   		NewValue = 0;//restore Coldstart value to 0.
			   		SavePresetSelected(0);
			   		PresetDisplay=0;
			   }
			   ValueOkay = TRUE;
			   break;
		   case TIME:
		   case TIMEOUT:
		   case TOTALONTIME:
			   if (Format[CurrentParam].Format == TIMEOUT)
				   Type = CONFIG;
			   else
				   Type = PRESET;
			   NewValue = CalcDisplayValue(Zero_Char,Five_Char);
			   NewValue = ToSec(NewValue); 
			   
			   if (NewValue==0 && CurrentParam==TIMEOUT)
				   ValueOkay = FALSE;
			   else
				   ValueOkay = TRUE;
			   break;
		   case ONTIME:
		   case OFFTIME:
			   Type = PRESET;
			   NewValue = CalcDisplayValue(Zero_Char,Five_Char);
			   NewValue = ToMilliSec(NewValue); 
			   
			   if (NewValue > 0)
				   ValueOkay = TRUE;
			   else
				   ValueOkay = FALSE;
			   break;
		   case PULSETEMPE:
		   case CALOFFSET:
		   case MAXTEMPE:
			   NewValue = CalcDisplayValue(Two_Char,Five_Char);
			   if(Conf.ConfigData.Tempe_C_F == 0) 
				   NewValue = (UINT32)ConvertTempToF((UINT16)NewValue);
			   if (Format[CurrentParam].Format == CALOFFSET)
			   {
				   Type = CONFIG;
				   NewValue =  NewValue - SensedTemperature;
			   }
			   else
			   {
				   Type = PRESET;
			   }
			   ValueOkay = TRUE;
			   break;
		   case AMPLITUDE:
			   Type = PRESET;
			   NewValue = CalcDisplayValue(Two_Char,Four_Char);
  
			   if ((NewValue > MICROTIPLIMIT) && (CurrentPreset.presetData.Microtip == TRUE))
				   ValueOkay = FALSE;
			   else
				   ValueOkay = TRUE;
			   break;
		   case ONENERGY:
			   Type = PRESET;
			   NewValue = CalcDisplayValue(Two_Char,Five_Char);
			   
			   if (NewValue > 0)
				   ValueOkay = TRUE;
			   else
				   ValueOkay = FALSE;
			   break;
		   case ENERGY:
		   case TOTALONENERGY:
			   Type = PRESET;
			   NewValue = CalcDisplayValue(Zero_Char,Five_Char);
			   ValueOkay = TRUE;
			   
			   if (NewValue==0 && CurrentParam==ENERGY )
				   ValueOkay = FALSE;
			   else
				   ValueOkay = TRUE;
			   break;
		   case FN_NUM:
		   //case PSFREQ:
		   case PSWATTAGE:
			   Type = CONFIG;
			   NewValue = Num_Current.NumericData.Value;
			   ValueOkay = TRUE;
			   if ((CurrentParam== FN_NUM) && (NewValue == CALOFFSET) && (TempeProbePresent == FALSE))
				   ValueOkay = FALSE;

			   break;
		   case PASSWORD:
			   Type = CONFIG;
			   NewValue = CalcDisplayValue(Two_Char,Three_Char);
			   ValueOkay = TRUE;
			   break;
		   default:
			   Type = CONFIG;    
			   NewValue = (DisplayCodes[Two_Char]-'0');
			   ValueOkay = TRUE;
			   break;
		   }
	   }
	   else
	   {
		   Type = PRESET;
		   NewValue=0;
		   ValueOkay=TRUE;
	   }
   }
   else
   {
	   Type = PRESET;
	   NewValue=GetParamNumber();
	   ValueOkay=TRUE;
   }
   PrevValue = (SINT32)GetVar(Format[CurrentParam].Ptr,Parameters[CurrentParam].VarSize);
   
   if (ValueOkay == TRUE) { 
      if(Type == PRESET || CurrentParam == FN_NUM)
      {
    	  if( PrevValue != NewValue)    
    		  SetVar(Format[CurrentParam].Ptr , NewValue, Parameters[CurrentParam].VarSize);
    	  //if ((CurrentParam == PULSETEMPE) && ((TempPreset.presetData.WeldMode & CONFIG_TEMP) != CONFIG_TEMP))
    	  if ((CurrentParam == MAXTEMPE) && ((TempPreset.presetData.WeldMode & CONFIG_TEMP) != CONFIG_TEMP))
    	  {
    		  if(Conf.ConfigData.Tempe_C_F == 0)
    		  {
    			  if ((TempPreset.presetData.PulseTempe + 36) > TempPreset.presetData.MaxTempe  )
    			  {
    				  if (TempPreset.presetData.MaxTempe < Two_degrees_C)
    					  TempPreset.presetData.PulseTempe = 0;
    				  else
    					  TempPreset.presetData.PulseTempe = TempPreset.presetData.MaxTempe - 36;
    			  }
    		  }
    		  else
    		  {
    			  if ((TempPreset.presetData.PulseTempe + Three_degrees_F) > TempPreset.presetData.MaxTempe )   //Remove HardCode
    			  {
    				  if (TempPreset.presetData.MaxTempe < ThirtyFive_degrees_F)
    					  TempPreset.presetData.PulseTempe = 0;
    				  else
    					  TempPreset.presetData.PulseTempe = TempPreset.presetData.MaxTempe - Three_degrees_F;      //Remove HardCode
    			  }
    		  }
    	  }
    	  
      }
      else 
    	  if(Type == CONFIG) {
    		  if( PrevValue != NewValue)
    		  {                        
    			  if(CurrentParam != PASSWORD){
    				  SetVar(Format[CurrentParam].Ptr , NewValue ,Parameters[CurrentParam].VarSize); 
    				  EEPROM_ChangeParam(0,CONFIG, (UINT8)Format[CurrentParam].Format) ;
    			  }
    			  else
    				  StorePassword = NewValue; 
    			  DataHandle = (UINT16)EEPROM_ReadConfig();
    		  }
    		  if (CurrentParam ==  PSWATTAGE)
    			  Wattage = MenuWattage[Conf.ConfigData.PsWattage].displayValue; 
    		  else
    		  {
    			  if (CurrentParam == REGION)
    			  {
    				  if (Conf.ConfigData.Region == 0 )
    					  Conf.ConfigData.Tempe_C_F = 1;
    				  else
    					  Conf.ConfigData.Tempe_C_F = 0;
    			  }
    			  EEPROM_ChangeParam(0,CONFIG, TEMPE_C_F) ;
    		  }
    	  }       
	   PerformAction();
   }
   else 
   {
	   if (CurrentParam==FN_NUM)
		   CreateAlarm(ER_TEMP); 
	   else 
		   if (CurrentParam== AMPLITUDE)
		   {
			   AuxCurrentParam=CurrentParam;
			   CreateAlarm(ER_uTIP); 
		   }
		   else
		   {
			   DataEntryError = TRUE;
			   AuxCurrentParam=CurrentParam;
			   Strcpy(AuxDisplayCodes,DisplayCodes);
			   CreateAlarm(ER_DE); 
		   }
   }
 }


void SetDigitBlink(void)
/***************************************************************************
* Function Name: SetDigitBlink
*
* Arguments: N/A  
*
* Return Type: N/A
*
* Description:
*		Set blinking on the digits according the range set between 
*		FirstChar and QtyofChars 
***************************************************************************/ 
{
	UINT8 x;
	CurrentDigit.digit = NODIGITS;
	if (screen != SCREEN_RESULTS )
		for(x=FirstChar; x < (FirstChar + QtyOfChars) ;x++)
			CurrentDigit.digit |= (0x01 << x);
	if (StringIsOff() == TRUE)
		CurrentDigit.digit = BLINK_OFF_STRING;
}



void SelectDigitGroup(bool right)
/***************************************************************************
* Function Name: SelectDigitGroup
*
* Arguments: 
* 	Right (boolean type), True is right, False is Left. 
*
* Return Type: N/A
*
* Description:
*	Select the a new group when updating a parameter and Left or Right 
*	Keys are pressed. 
***************************************************************************/ 
{
	UINT8 x=0;
	if (Parameters[CurrentParam].QtyG1 != 0) x=x+1;
	if (Parameters[CurrentParam].QtyG2 != 0) x=x+1;
	if (Parameters[CurrentParam].QtyG3 != 0) x=x+1;
	
	if ( StringIsOff() == FALSE)    
	{
		if (BlockSelected != 0 && x>1)
		{
			if ( BlockSelected == First_Block )
				if  (right == TRUE)
					if (x==2)
						BlockSelected=Second_Block;
					else
						BlockSelected=Third_Block;
				else
					BlockSelected=2;
			else
				if (BlockSelected==3)
					if  (right == TRUE)
						BlockSelected=Second_Block;
					else
						if (x==2)
							BlockSelected=Second_Block;
						else
							BlockSelected=First_Block;
				else
					if (right == TRUE)
						if (Parameters[CurrentParam].QtyG1 != 0)
							BlockSelected=First_Block;
						else
							BlockSelected=Third_Block;
					else
						if (Parameters[CurrentParam].QtyG3 != 0)
							BlockSelected=Third_Block;
						else
							BlockSelected=First_Block;
		}
		else
			BlockSelected=Parameters[CurrentParam].GroupDefault;
	}
}


UINT32 GetParamNumber()
/***************************************************************************
* Function Name: GetParamNumber
*
* Arguments: N/A
*
* Return Type: 
* 	Number UINT32
*
* Description:
*	Get the value according CurrentParam and ShowDefaults flag. 
***************************************************************************/ 
{
	UINT8 *Value;
	UINT32 Number=0;
	
	if (ShowDefaults == TRUE)
		if (CurrentParam == MAXTEMPE && (TempPreset.presetData.WeldMode & CONFIG_TEMP) == CONFIG_TEMP )
			Number = MAX_TEMPE_LIMIT; 
		else
			Number = Parameters[CurrentParam].Default;
	else
	{
	   Value = Format[CurrentParam].Ptr;
	   Memset(&DisplayCodes,' ', sizeof(DisplayCodes));
	   Number=(UINT32)GetVar(Value,Parameters[CurrentParam].VarSize);
	}
	return (Number);
}

void UpdateDisplayValue()     
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to update screen with current data/Message.Function will fill 
*        DisplayCodes with current data/msg in Ascii format and DisplaySegment 
*        function will display it on LCD.
*                                                                         
*   Entry condition:  
*
*   Exit condition: 
*                                                                         
***************************************************************************/
{
	UINT32 Number;
   
	Number = GetParamNumber();
	switch (Format[CurrentParam].Format){
		case PANELTRIG:
		case PULSESTART:
		case AUTORESET: 
		case ENDOFWELDBEEP: 
		case PARAMERRORBEEP:
		case DISPLOCK:
		case PWRUPSEEK:
		case TEMPE_C_F: 
		case COLDSTART:
		case HHLIMIT:
		case ENDOFCYCLE:
		case PASSWORD:
		case PSWATTAGE:
		case REGION:
		case FACTCOLDSTART:
			Numout((UINT32)Fn_Screens[CurrentParam].displayValue,DECIMALBASE,'0', &DisplayCodes[0], Two_Char);
			Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Two_Char], Parameters[CurrentParam].QtyG2);   
			if (CurrentParam == PSWATTAGE)
				Numout((UINT32)MenuWattage[(Number)].displayValue, DECIMALBASE,'0', &DisplayCodes[Two_Char], Parameters[CurrentParam].QtyG2);
			   
			break;
		case FN_NUM:
		case AMPLITUDE:
		case PRESET_NUM:
		case D_PROGRESS:
		case D_AMPLITUDE:
		case D_PEAKPOWER:
		case D_CURRENTPOWER:
			if (Parameters[CurrentParam].QtyG1!=0)
				Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[0], Parameters[CurrentParam].QtyG1); 
			else
				if (Parameters[CurrentParam].QtyG2!=0)
					Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Two_Char], Parameters[CurrentParam].QtyG2); 
				else
					Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Six_Char - Parameters[CurrentParam].QtyG3], Parameters[CurrentParam].QtyG3);
			
			if (Format[CurrentParam].Format== PRESET_NUM && LoadPreset == TRUE)
			{
				DisplayCodes[Zero_Char]= 'O';
				DisplayCodes[One_Char] = 'L';
			}
			else
				if (Format[CurrentParam].Format== PRESET_NUM && LoadPreset == FALSE)
				{
					DisplayCodes[Zero_Char]= 'A';
					DisplayCodes[One_Char] = 'S';
				}
			   
			if (CurrentParam == FN_NUM)
			{
				Numout(Fn_Screens[(UINT32)(Number)].displayValue, DECIMALBASE,'0', &DisplayCodes[0], Two_Char); 
				DisplayCodes[Five_Char] = 'r';
				DisplayCodes[Four_Char] = 'E';
				DisplayCodes[Three_Char]= 'G';
			}
			break;
		case CALOFFSET:
		case D_MAXTEMP:
		case D_LASTTEMP:
		case D_CURRENTTEMP:
			if (CurrentParam==CALOFFSET)
				Number=(UINT32)(CurrentTemperature);
			if(Conf.ConfigData.Tempe_C_F == 0)
			{
				Number = (UINT32)ConvertTempToC((UINT16)Number);
				DisplayCodes[Zero_Char]= 'C';
				DisplayCodes[One_Char] = '^';
			}
			else
			{
				DisplayCodes[Zero_Char]= 'F';
				DisplayCodes[One_Char] = '^';
			}
			Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Two_Char], Four_Char);
			break;
		case PULSETEMPE:
		case MAXTEMPE:

			if(Conf.ConfigData.Tempe_C_F == 0)
			{
			   DisplayCodes[Zero_Char]= 'C';
			   DisplayCodes[One_Char] = '^';
			}
			else
			{
				DisplayCodes[Zero_Char]= 'F';
				DisplayCodes[One_Char] = '^';
			}
			if (Number == 0)    
			{
				if (CurrentParam == PULSETEMPE)
				{
					if(Conf.ConfigData.Tempe_C_F == 0)
					{
						if (TempPreset.presetData.MaxTempe >= Two_degrees_C)
							Number = TempPreset.presetData.MaxTempe-One_degree_C;
						else
							Number=ThirtyTwo_degrees_F;
						Number = (UINT32)ConvertTempToC((UINT16)Number);
					}
					else
					{
						if (TempPreset.presetData.MaxTempe >= ThirtyFive_degrees_F)
							Number = TempPreset.presetData.MaxTempe-Two_degrees_F;		
						else
							Number=ThirtyTwo_degrees_F;
					}
				}
				else
				{	
					if(Conf.ConfigData.Tempe_C_F == 0)
						Number = HIGH_VALUE_C;    
					else	 
						Number = HIGH_VALUE_F;  
				}
			}
			else
				if(Conf.ConfigData.Tempe_C_F == 0)
					Number = (UINT32)ConvertTempToC((UINT16)Number);
			   
			Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Two_Char], Four_Char);
 
			break;
		case ONENERGY:
		case D_ONENERGY:
			Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Two_Char], Four_Char);
			break;
		case ENERGY:
		case TOTALONENERGY:
		case D_TOTALENERGY:
			Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Zero_Char], Six_Char); 
			break;
		case TIMEOUT:
		case TIME:
		case TOTALONTIME:
			Number=ToHHMMSS(Number);
			Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Zero_Char], Six_Char);
			break;   
		case ONTIME:
		case OFFTIME:
		case D_ONTIME:
		case D_OFFTIME:
			Number=ToMMSSMS(Number);
			Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Zero_Char], Six_Char);
			break;
		case D_TOTALTIME:
		case D_TOTALONTIME:
			if ( Number > MAX_NUMBER_FORMAT_MS )  
			{
				Number/=1000; 
				Number=ToHHMMSS(Number);
				Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Zero_Char], Six_Char);
				FixedIcons |= HRMINSEC_I;
				FixedIcons &= ~(UINT32)(SECONDS);
			}
			else
			{
				Number=ToMMSSMS(Number);
				Numout((UINT32)Number,DECIMALBASE,'0', &DisplayCodes[Zero_Char], Six_Char);
				FixedIcons |= SECONDS;
				FixedIcons &= ~(UINT32)(HRMINSEC_I);
			}
			break;
		case MESSAGE :
		case NOMESSAGE:
			Strcpy(DisplayCodes, FnMSG);
			CurrentDigit.digit = NODIGITS;
			CurrentDigit.Blinking = FALSE;
			if (screen == SCREEN_RDY  )
			{
				if (PresetDisplay != 0)
				{
					Strcpy(FnMSG,Rdy_Msg2);
					Strcpy(DisplayCodes, FnMSG);
					Numout(PresetDisplay,DECIMALBASE,'0', &DisplayCodes[0], Two_Char);
					FixedIcons |= (UINT32)(PRESET_I);
				}
				else
				{
					Strcpy(FnMSG,Rdy_Msg);
					Strcpy(DisplayCodes, FnMSG);
					FixedIcons &= ~(UINT32)(PRESET_I);
				}
			}
			else
				Strcpy(DisplayCodes, FnMSG);
			
			
			
			break;
		case ERROR:
			Strcpy(DisplayCodes, FnERR);
			CurrentDigit.digit = ALLDIGITS;
			CurrentDigit.Blinking = ALLDIGITS;
			break;
		case SWVERSION:
			Strcpy(DisplayCodes, SoftwareVersion);
			FixedIcons = (UINT32)DOT_I;
			if (screen == SCREEN_EDITFNSCREEN)
			{	 
				DisplayCodes[Five_Char]='1';
				DisplayCodes[Four_Char]='0';  
			}
		    CurrentDigit.digit = NODIGITS;
		    CurrentDigit.Blinking = FALSE;                   
		    break;
	}
	if(Parameters[CurrentParam].Min1 == ' ') DisplayCodes[Zero_Char]  = Parameters[CurrentParam].Min1;
	if(Parameters[CurrentParam].Min2 == ' ') DisplayCodes[One_Char]   = Parameters[CurrentParam].Min2;
	if(Parameters[CurrentParam].Min3 == ' ') DisplayCodes[Two_Char]   = Parameters[CurrentParam].Min3;
	if(Parameters[CurrentParam].Min4 == ' ') DisplayCodes[Three_Char] = Parameters[CurrentParam].Min4;
	if(Parameters[CurrentParam].Min5 == ' ') DisplayCodes[Four_Char]  = Parameters[CurrentParam].Min5;
	if(Parameters[CurrentParam].Min6 == ' ') DisplayCodes[Five_Char]  = Parameters[CurrentParam].Min6;
}


void VerifyTempVSCurret (void)
/***************************************************************************
* Function Name: VerifyTempVSCurret
*
* Arguments: N/A  
*
* Return Type: N/A
*
* Description:
*		After save a parameter, compares the CurrentPreset vs the TempPreset
*		to verify if it should show the defaults on the next parameters.
***************************************************************************/
{
	UINT32 TempVal = 0 ;
	UINT32 CurrentVal = 0 ;
	TempVal    = GetVar(Format[CurrentParam].Ptr, Parameters[CurrentParam].VarSize);
	CurrentVal = GetVar(Format[CurrentParam].Pointercurrent,Parameters[CurrentParam].VarSize);
	if (CurrentParam == CONF_MODE)
	{
		if ((TempVal & (UINT32)(CONFIG_CONT | CONFIG_PULSE )) != (CurrentVal & (UINT32)(CONFIG_CONT | CONFIG_PULSE)))
		{
			PresetDisplay=0;
			SavePresetSelected(PresetDisplay);
		}
	}
	else	
	{
		if (CurrentParam == CONF_CONTROL)
		{
			if ((TempVal & (UINT32)(CONFIG_CONT | CONFIG_PULSE | CONFIG_TEMP)) != (CurrentVal & (UINT32)(CONFIG_CONT | CONFIG_PULSE | CONFIG_TEMP)))
			{
				ShowDefaults = TRUE;
				PresetDisplay=0;
				SavePresetSelected(PresetDisplay);
			}
		}
		else
		{
			if (CurrentParam == CONF_CONTROL2)
			{
				if ((TempVal & (UINT32)(CONFIG_SUBENERGY | CONFIG_SUBTIME)) != (CurrentVal & (UINT32)(CONFIG_SUBENERGY | CONFIG_SUBTIME)))
				{
					ShowDefaults = TRUE;
					PresetDisplay=0;
					SavePresetSelected(PresetDisplay);
				}
			}
			else
			{
				if (CurrentParam == ENERGY || CurrentParam == MAXTEMPE || CurrentParam == ONTIME || CurrentParam == TIME || CurrentParam == ONENERGY)
					ShowDefaults = FALSE;
				
				if (TempVal != CurrentVal)
				{
					PresetDisplay=0;
					SavePresetSelected(PresetDisplay);
				}
			}
		}
	}
}

void PerformAction(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Perform Action Related to Different Configuration values 
*                                                                         
*   Entry condition:  Values entered should be valid 
*
*   Exit condition: 
*                                                                         
***************************************************************************/
{      
   switch(CurrentParam)
   {    
      case AMPLITUDE:
         SetAmplitude(CurrentPreset.presetData.AmplitudeValue);
      break;    
      case D_AMPLITUDE:
		  SetAmplitude(currentLCDData.AmplitudeValue);
      break;
      default: 
      //Do Nothing
      break;
   }
}

SINT32 CalcDisplayValue(UINT8 First,UINT8 Last)
/***************************************************************************
* Function Name: CalcDisplayValue
*
* Arguments: First and Last Char of the data that will be calculated 
*
* Return Type: SINT32
*
* Description:
*		Calc the decimal value on a string delimited by the arguments First 
*		and Last.
***************************************************************************/
{
	UINT32 expo=1;
	UINT8  x;
	SINT32 Value=0;
	for (x=First; x<= Last; x++ )
	{	   
		Value+=((DisplayCodes[x]-'0')*expo);
		expo*=10;
	}
	return Value;
}
void ChangeResult(bool Right)
{
	if (StopFlag == FALSE)  
	{
		if ( Right ==TRUE)
			if (ResultIndex >= (Results[currentLCDData.IDWeldMode].length -1)  )
				ResultIndex=0;
			else
				ResultIndex++;
		else
			if (ResultIndex == 0)
			{
				ResultIndex=Results[currentLCDData.IDWeldMode].length-1;
			}
			else
				ResultIndex--;
	}
	else
	{
		if ( Right == TRUE)
		{
			if (ResultIndex >=  (Results[currentLCDData.IDWeldMode].length-1) )
			{ 
				SwitchScreen(SCREEN_RDY);
			}
			else
				ResultIndex++;
		}
		else
			if (ResultIndex == 0)
			{
				SwitchScreen(SCREEN_RDY);
			}
			else
				ResultIndex--;
	}
	
}

void DisplayResult(void)
{
	static UINT8 ScreenResult=0;
		
	if (screen ==SCREEN_RESULTS)
	{
		if (ResultIndex == 0 && StopFlag == FALSE)
		{
			FixedIcons= (UINT32)(LINE_4| LINE_3 | LINE_2);			
			WeldMode_Icon(WeldMode);
			if ((WeldMode & (UINT16)(CONFIG_SUBTIME | CONFIG_SUBENERGY))==SET_MAXTEMP_NOT_ZERO)
			{
				FixedIcons |=(UINT32)( TEMPE );
			}
			if ((WeldMode & (UINT16)(SET_PUL_TEMP_ZERO | SET_PUL_TEMP_NO_ZERO))==SET_PUL_TEMP_NO_ZERO)
			{
				FixedIcons |=(UINT32)( PULSE2 );
			}
		}
		else
			FixedIcons=0;
			
		ScreenResult = Results[currentLCDData.IDWeldMode].ResultValue[ResultIndex];
		
		switch (ScreenResult){
			case R_TOTAL_TIME:
				FixedIcons|= SLD_TOTALTIME;
				CurrentParam=D_TOTALTIME;
				break;
			case R_TOTAL_ENERGY:
				FixedIcons|=SLD_TOTALENERGY;
				CurrentParam=D_TOTALENERGY;
				break;
			case R_AMPLITUDE:
				FixedIcons |= SLD_AMPLITUDE;
				CurrentParam = D_AMPLITUDE;
				break;
			case R_PROGRESS:
				FixedIcons|=SLD_PROGRESS;
				CurrentParam = D_PROGRESS;
				break;
			case R_MAXTEMP:
				FixedIcons|= SLD_MAXTEMP;
				CurrentParam=D_MAXTEMP;
				break;
			case R_ENDTEMP:
				FixedIcons|= SLD_ENDTEMP ;
				if (StopFlag == FALSE)
					CurrentParam=D_CURRENTTEMP;
				else
					CurrentParam=D_LASTTEMP;
				break;
			case R_PEAKPOWER:
				FixedIcons|= SLD_PEAKPOWER;
				if (StopFlag == FALSE)
					CurrentParam=D_CURRENTPOWER;
				else
					CurrentParam=D_PEAKPOWER;
				break;
			case R_ON_TIME:
				FixedIcons|= SLD_ON_TIME;
				CurrentParam=D_ONTIME;
				break;
			case R_ON_ENERGY:
				FixedIcons|= SLD_ON_ENERGY;
				CurrentParam=D_ONENERGY;
				break;
			case R_OFF_TIME:
				FixedIcons|= SLD_OFF_TIME;
				CurrentParam=D_OFFTIME;
				break;
			case R_TOTAL_ON_ENERGY:
				FixedIcons|= SLD_TOTAL_ON_ENERGY;
				CurrentParam=D_TOTALENERGY;
				break;
			case R_TOTAL_ON_TIME:
				FixedIcons|= SLD_TOTAL_ON_TIME;
				CurrentParam=D_TOTALONTIME;
				break;
		}
	}
}

void WeldMode_Icon(UINT16 WeldMode)
/***************************************************************************
*                                                                         
*   Purpose:
*       Displaying appropriate icon according to the weldmode.
*                                                                         
***************************************************************************/
{
   if ((WeldMode & CONFIG_CONT) == (UINT32)(CONFIG_CONT ))
	   FixedIcons |= (UINT32)(AUTO) ;
   else
	   FixedIcons |= (UINT32)(PULSE) ;
   
   if (((WeldMode & CONFIG_TIME) == (UINT32)(CONFIG_TIME)) && ((WeldMode & CONFIG_TEMP) != (UINT32)(CONFIG_TEMP)))
	   FixedIcons |= (UINT32)(CLOCK);
   else
	   if (((WeldMode & CONFIG_ENERGY)== (UINT32)(CONFIG_ENERGY))&& ((WeldMode & CONFIG_TEMP) != (UINT32)(CONFIG_TEMP)))
		   FixedIcons |= (UINT32)(JOULES) ;
       else 
    	   if ((WeldMode & CONFIG_TEMP)== (UINT32)(CONFIG_TEMP))
    		   FixedIcons |=   (UINT32)(TEMPE | MAX) ;
   
   if ((WeldMode & CONFIG_SUBTIME) == (UINT32)(CONFIG_SUBTIME))
	   FixedIcons |= (UINT32)(CLOCK);
   else
	   if ((WeldMode & CONFIG_SUBENERGY) == (UINT32)(CONFIG_SUBENERGY))
		   FixedIcons |= (UINT32)(JOULES) ;
   
   UpdateLiveReady = TRUE; 
}


void SwitchScreen(UINT16 NextScreen)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to change from one screen state to another. Handles all 
*        neccesary changes for entering or leaving a state.
*                                                                         
*   Entry condition:  
*
*        nextScreen - the next screen to switch to.
*
*   Exit condition: 
*                                                                         
***************************************************************************/
{  
   
   SetKey = TRUE;
   screen = NextScreen;
   switch (NextScreen) {
      case SCREEN_TEST:
    	  break;
      case SCREEN_POWERUP:
         //screen = SCREEN_POWERUP;
         CurrentParam = SWVERSION;
         Strcpy(FnMSG, SoftwareVersion);
         FixedIcons = (UINT32)DOT_I;
         CurrentDigit.Blinking = FALSE;
         break;
      case SCREEN_RDY:
         CurrentDigit.digit = NODIGITS;      //In Ready Screen no Digits should Blink
         CurrentDigit.Blinking = FALSE;
         SetKey = FALSE;
         
         OldScreen = SCREEN_RDY;
         EscKeyPressed = FALSE;
         PresetNo = 0;
         ResultIndex = 0;
         FixedIcons = (UINT32)(LINE_4| LINE_3 | LINE_2);
         BlinkIcons=0;
         ShowDefaults = FALSE;
         WeldMode_Icon(CurrentPreset.presetData.WeldMode);
         CurrentParam = MESSAGE;
         StorePassword = Parameters[PASSWORD].Default;
         break;
      case SCREEN_MODECONFIG:
    	  ReqStartRelease = TRUE;
    	 FixedIcons = (UINT32)( AUTO | PULSE |  LINE_4| LINE_3 | LINE_2);
         if(ContinuousMode){
        	 BlinkIcons = (UINT32)(AUTO);
         }
        else{
        	 BlinkIcons = (UINT32)( PULSE);
          }
         break;
      case SCREEN_SONICCONF:
    	 FixedIcons = (UINT32)( LINE_2 |  LINE_3 | LINE_4 | JOULES | CLOCK );
    	 WeldMode_Icon(TempPreset.presetData.WeldMode);
         
         if (TempeProbePresent == TRUE ) 
        	 FixedIcons |= (UINT32) (TEMPE | MAX);
         
         if (ModeSet == TIME_MODE )
        	 BlinkIcons = (UINT32)( CLOCK );
         else 
        	 if ( ModeSet == ENERGY_MODE )
        		 BlinkIcons = (UINT32)( JOULES );
        	 else
        		 BlinkIcons = (UINT32)( TEMPE | MAX);
         break;
      case SCREEN_SONICCONF2:
    	  FixedIcons = (UINT32)( LINE_2 |  LINE_3 | LINE_4 | JOULES | CLOCK | TEMPE | MAX );
    	  WeldMode_Icon(TempPreset.presetData.WeldMode);
    	  if (ModeSet2==TIME_MODE)
    	  {
    		  FixedIcons |= (UINT32)( JOULES );
    		  BlinkIcons = (UINT32)( CLOCK );
    	  }
    	  else
    	  {
    		  FixedIcons |= (UINT32)( CLOCK );
    		  BlinkIcons = (UINT32)( JOULES );
    	  }
    	  break;    
      case SCREEN_PARAMETER:
    	 FixedIcons = (UINT32)( LINE_2 | LINE_3 | LINE_4 );
         WeldMode_Icon(TempPreset.presetData.WeldMode);
         BlinkIcons = Parameters[CurrentParam].BlinkIcons;
         BlockSelected=0;
         CurrentDigit.Blinking = TRUE;
         break;                                                                           
      case SCREEN_FNSCREEN:   
    	  ReqStartRelease = TRUE;
    	  FixedIcons = (UINT32)(0);
          BlinkIcons = (UINT32)(0);
         CurrentParam = FN_NUM;
         break;
      case SCREEN_EDITFNSCREEN:
         CurrentParam = CurrFnScreen;
         BlockSelected=0;
         BlinkIcons = Parameters[CurrentParam].BlinkIcons;
         break;
      case SCREEN_ERROR:
    	  CurrentParam = ERROR;
    	  FixedIconsVar = FixedIcons;
    	  FixedIcons =0;
    	  BlinkIcons |= (UINT32)(ALARM_I); 
    	  break;
      case SCREEN_PRESET:
    	 ReqStartRelease = TRUE;
         CurrentParam = PRESET_NUM;
         BlockSelected=0;
         FixedIcons = 0;
         if (LoadPreset==TRUE)
         {
        	 EEPROM_Recall((UINT8)Num_Current.NumericData.Value);
        	 WeldMode_Icon(CurrentPreset.presetData.WeldMode);
         }
         else
         {
        	 WeldMode_Icon(TempPreset.presetData.WeldMode);
         }
         FixedIcons |= (UINT32)(PRESET_I) ;     
         break;
      case SCREEN_RESULTS:
         SetKey = FALSE;
         DisplayResult();
         break;
      case SCREEN_CRC : 
    	  CurrentParam=MESSAGE;
    	  Strcpy(FnMSG,Str_CRC);
          FixedIcons = 0;
          BlinkIcons = 0;
    	  break;
   }

   UpdateDisplayValue();  
   SelectDigitGroup(TRUE);
   GetLimits();
   VerifyOFF();
   ResetBacklightTimer();
   UpdateLiveReady = TRUE;    
}

void LCD_Init(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Initializes the associated peripheral(s) and the internal 
*        variables. The method is called automatically as a part of the 
*        application initialization code.
*                                                                         
*   Entry condition:  
*
*   Exit condition: 
*                                                                         
***************************************************************************/
{   
   LCDC0 &= ~(byte)0x80;            /* Disable LCD device */ 
   /* LCDSUPPLY: CPSEL=0,??=0,LADJ1=0,LADJ0=0,??=0,??=0,VSUPPLY1=0,VSUPPLY0=1 */
   LCDSUPPLY = LCDSUPPLY_CPSEL_MASK | LCDSUPPLY_VSUPPLY0_MASK; // 0x81;               
   /* LCDC0: LCDEN=0,SOURCE=0,LCLK2=1,LCLK1=1,LCLK0=1,DUTY2=0,DUTY1=1,DUTY0=1 */
   LCDC0 = LCDC0_LCLK2_MASK | LCDC0_LCLK0_MASK | LCDC0_DUTY1_MASK | LCDC0_DUTY0_MASK;// 0x2B;; 
   //LCDC0 = LCDC0_LCLK2_MASK | LCDC0_LCLK0_MASK | LCDC0_DUTY1_MASK | LCDC0_DUTY0_MASK;// 0x2B;; 
    /* LCDC1: LCDIEN=0,??=0,??=0,??=0,??=0,FCDEN=0,LCDWAI=0,LCDSTP=0 */
   LCDC1 = (byte)0x00;                  /* Set base clock prescaler and duty mode */ 
   /* LCDBCTL: BLINK=0,ALT=0,BLANK=0,??=0,BMODE=0,BRATE2=0,BRATE1=0,BRATE0=0 */
   LCDBCTL = (byte)0x00;               
   /* LCDPEN0: PEN7=1,PEN6=1,PEN5=1,PEN4=1,PEN3=1,PEN2=1,PEN1=1,PEN0=1 */
   LCDPEN0 = (byte)0xFF;               
   /* LCDBPEN0: BPEN7=1,BPEN6=1,BPEN5=1,BPEN4=0,BPEN3=0,BPEN2=0,BPEN1=0,BPEN0=0 */
   LCDBPEN0 = (byte)0xE0;              
   /* LCDPEN1: PEN15=0,PEN14=0,PEN13=0,PEN12=0,PEN11=0,PEN10=0,PEN9=0,PEN8=0 */
   LCDPEN1 = (byte)0x00;               
   /* LCDBPEN1: BPEN15=0,BPEN14=1,BPEN13=1,BPEN12=1,BPEN11=1,BPEN10=0,BPEN9=0,BPEN8=0 */
   LCDBPEN1 = (byte)0x00;              
   /* LCDPEN2: PEN23=0,PEN22=0,PEN21=0,PEN20=1,PEN19=1,PEN18=1,PEN17=1,PEN16=1 */
   LCDPEN2 = (byte)0x1F;               
   /* LCDBPEN2: BPEN23=0,BPEN22=0,BPEN21=0,BPEN20=0,BPEN19=0,BPEN18=0,BPEN17=0,BPEN16=0 */
   LCDBPEN2 = (byte)0x00;              
   /* LCDPEN3: PEN31=1,PEN30=1,PEN29=1,PEN28=1,PEN27=0,PEN26=0,PEN25=0,PEN24=0 */
   LCDPEN3 = (byte)0xF0;               
   /* LCDBPEN3: BPEN31=0,BPEN30=0,BPEN29=0,BPEN28=0,BPEN27=0,BPEN26=0,BPEN25=0,BPEN24=0 */
   LCDBPEN3 = (byte)0x00;              
   /* LCDPEN4: PEN39=0,PEN38=0,PEN37=0,PEN36=1,PEN35=1,PEN34=0,PEN33=1,PEN32=1 */
   LCDPEN4 = (byte)0x1B;               
   /* LCDBPEN4: BPEN39=0,BPEN38=0,BPEN37=0,BPEN36=0,BPEN35=0,BPEN34=0,BPEN33=1,BPEN32=0 */
   LCDBPEN4 = (byte)0x02;              
   /* LCDPEN5: ??=0,??=0,??=0,PEN44=0,PEN43=0,PEN42=0,PEN41=0,PEN40=0 */
   LCDPEN5 = (byte)0x00;               
   /* LCDBPEN5: ??=0,??=0,??=0,BPEN44=0,BPEN43=0,BPEN42=0,BPEN41=0,BPEN40=0 */
   LCDBPEN5 = (byte)0x00;              
   /* LCDWF0: BPHLCD0=0,BPGLCD0=0,BPFLCD0=0,BPELCD0=0,BPDLCD0=0,BPCLCD0=0,BPBLCD0=0,BPALCD0=0 */

   // LCD front plane registers (set all to off)
   LCDWF0 = (byte)0x00;
   LCDWF1 = (byte)0x00;
   LCDWF2 = (byte)0x00;
   LCDWF3 = (byte)0x00;
   LCDWF4 = (byte)0x00;
   LCDWF5 = (byte)0x08;
   LCDWF6 = (byte)0x04;
   LCDWF7 = (byte)0x02;
   LCDWF8 = (byte)0x00;
   LCDWF9 = (byte)0x00;
   LCDWF10 = (byte)0x00;
   LCDWF11 = (byte)0x00;
   LCDWF12 = (byte)0x00;
   LCDWF13 = (byte)0x00;
   LCDWF14 = (byte)0x00;
   LCDWF15 = (byte)0x00;
   LCDWF16 = (byte)0x00;
   LCDWF17 = (byte)0x00;
   LCDWF18 = (byte)0x00;
   LCDWF19 = (byte)0x00;
   LCDWF20 = (byte)0x00;
   LCDWF28 = (byte)0x00;
   LCDWF29 = (byte)0x00;
   LCDWF30 = (byte)0x00;
   LCDWF31 = (byte)0x00;
   LCDWF32 = (byte)0x00;
   LCDWF33 = (byte)0x01;
   LCDWF34 = (byte)0x00;
   LCDWF35 = (byte)0x00;
   LCDWF36 = (byte)0x00;
   
   /* LCDC0: LCDEN=1 */
   LCDC0 |= 0x80;            /* Enable device */
}     

