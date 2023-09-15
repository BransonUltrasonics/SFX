/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/global.h_v   1.18.1.3.3.1   13 Jun 2016 13:07:30   hasanchez  $
*************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION ----------------------------

Global defines, definitions, and enums

Module name: GLOBAL

Filename:    global.h
--------------------------- TECHNICAL NOTES -------------------------------

This code contains the global defines and enum defs.        */

/*---------------------------- REVISIONS ----------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/global.h_v  $
 * 
 *    Rev 1.18.1.3.3.1   13 Jun 2016 13:07:30   hasanchez
 * HHLIMIT added to the list of parameters.
 * 
 *    Rev 1.18.1.3.3.0   15 Dec 2015 16:14:56   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.18.1.0   25 Aug 2015 12:24:50   hasanchez
 * Added new defines to UP, DOWN, RIGHT, LEFT. New Screens, Parameters, Menus and Preset keys.
 * 
 *    Rev 1.18   18 Sep 2013 08:22:52   akaushal
 * Fixed the Power Up seek issue.
 * 
 *    Rev 1.17   10 Sep 2013 03:25:50   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.16   12 Jul 2013 06:53:56   akaushal
 * Corrected the handelling of Start switch delay.	
 * 
 *    Rev 1.15   08 Jul 2013 04:45:32   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, changed frequency offset acc to the rquirement.
 * 
 *    Rev 1.14   13 Jun 2013 07:59:54   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.13   25 Mar 2013 07:59:34   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.12   20 Dec 2012 01:24:20   akaushal
 * Updated Error codes and fixed Total runtime
 * 
 *    Rev 1.11   03 Dec 2012 08:54:32   akaushal
 * Navigation and Screens Updated
 * 
 *    Rev 1.10   20 Nov 2012 07:27:36   akaushal
 * Lcd Flickering Fixed and Navigation Updated
 * 
 *    Rev 1.9   07 Nov 2012 07:03:44   akaushal
 * Screen Navigation Updated
 * 
 *    Rev 1.8   06 Nov 2012 08:41:50   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.7   10 Sep 2012 09:47:54   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.6   03 Sep 2012 09:43:30   akaushal
 * Automode Corrected for Total time Icon
 * 
 *    Rev 1.5   24 Aug 2012 06:24:32   akaushal
 * Amplitude Issue resolved
 * 
 *    Rev 1.4   22 Aug 2012 04:13:26   akaushal
 * LCD codr corrected for Crystal Oscillator
 * 
 *    Rev 1.3   16 Aug 2012 01:51:20   akaushal
 * SFX Code With External Crystal Oscillator and H/W Changes
 * 
 *    Rev 1.2   10 Aug 2012 02:57:46   akaushal
 * Corrected Microtip and Test Key on Keypad
 * 
 *    Rev 1.1   08 Aug 2012 02:15:54   akaushal
 * Remove Total Time from Automode 
 * 
 *    Rev 1.0   06 Aug 2012 05:26:46   akaushal
 * Initial revision.
 * 
 *    Rev 1.7   31 Jul 2012 03:59:00   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.6   10 Jul 2012 05:36:00   akaushal
 * Digit blinking problem removed
 * 
 *    Rev 1.5   09 Jul 2012 03:26:20   akaushal
 * Keypad code updated
 * 
 *    Rev 1.4   06 Jul 2012 10:02:44   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.3   27 Jun 2012 07:48:40   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:31:52   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:45:04   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:04   ygupta
 * Initial revision.

---------------------------------------------------------------------------*/
#ifndef _global_H
#define _global_H


/* MODULE global */


#define SFX_PASSWORD	   50	//Password to register 50, 51 and 52
#define HIGH            1     
#define LOW             0

#define UP            1
#define DOWN          0

#define RIGHT         1
#define LEFT          0

//#define NULL 0
#define FOREVER 1
#define FeedTheDog() (SRS = 0x00)

#define OUTPUT 0xff
#define INPUT 0

/* chip selects */
#define DESELECT	      1  /* No devices are selected */
#define SELECT		      0
#define DEBUG_ENABLED
/* DAC Control and DATA */
#define AMPCONTROL	   PTFD_PTFD4
#define AMPOUT			   PTED  
#define TOTAL_DECISIONS 29
#define OUTPUTCONTROL	PTFD_PTFD3
#define MAX_DIGIT 6

#define DEFALT_ONTIME_CS 1000

enum CTRL_LEVEL {
   SFX			= 0x01,	/* Low Power SFX version */
   LPX,				/* Low Power LPX version */
   END_CTRL_LEVEL
};

enum eScreens{ 
   SCREEN_POWERUP=0,
   SCREEN_TEST,
   SCREEN_RDY, 
   SCREEN_RESULTS, 
   SCREEN_MODECONFIG,
   SCREEN_SONICCONF,
   SCREEN_SONICCONF2,
   SCREEN_PARAMETER,
   SCREEN_FNSCREEN,
   SCREEN_EDITFNSCREEN,
   SCREEN_ERROR, 
   SCREEN_PRESET,
   SCREEN_CRC,
};

enum Params{ 
              
   SWVERSION=0,
   /*Config Data*/
   PANELTRIG,
   PULSESTART,
   AUTORESET, 
   ENDOFWELDBEEP, 
   PARAMERRORBEEP,
   DISPLOCK,
   PWRUPSEEK,
   TIMEOUT,
   TEMPE_C_F, 
   COLDSTART,
   HHLIMIT,
   CALOFFSET,
   ENDOFCYCLE,
   PASSWORD,
   PSWATTAGE,
   REGION,
   FACTCOLDSTART,
   /*Preset Data*/
   TIME,
   ENERGY,
   ONTIME,
   OFFTIME,
   ONENERGY,
   TOTALONTIME,
   TOTALONENERGY,
   PULSETEMPE,
   MAXTEMPE,
   AMPLITUDE,
   MICROTIP,
   WELDMODE,
   CONF_MODE,
   CONF_CONTROL,
   CONF_CONTROL2,
   /*Running Weld Data*/
   D_TOTALTIME,
   D_TOTALENERGY,
   D_AMPLITUDE,
   D_PROGRESS,
   D_MAXTEMP,
   D_LASTTEMP,
   D_CURRENTTEMP,
   D_PEAKPOWER,
   D_CURRENTPOWER,
   D_ONTIME,
   D_ONENERGY,
   D_OFFTIME,
   D_TOTALONTIME,
   /*Screen related*/
   MESSAGE,
   FN_NUM,
   NOMESSAGE,
   ERROR,
   PRESET_NUM,
   FNEND
};

//#define FUNC_OFFSET PSFREQ

/* Function Screens number.*/

typedef enum MenuDesition {
	MENU_MODE=0,
	CON_SONIC,
	CON_SONIC2,
	PUL_SUNIC,
	PUL_SONIC2,
	CON_TIME,
	CON_TIME_MTEMP,
	MENU_AMPLITUDE,
	CON_TIME_PULSE,
	CON_ENERGY,
	CON_ENER_MTEMP,
	CON_ENER_PULSE,
	CON_MAX,
	PUL_TIME_ON,
	PUL_TIME_OFF,
	PUL_TIME_TT,
	PUL_TIME_MTEMP,
	PUL_TIME_PULSE,
	PUL_ENER_ON,
	PUL_ENER_OFF,
	PUL_ENER_TE,
	PUL_ENER_MTEMP,
	PUL_ENER_PULSE,
	PUL_TEM,
	PUL_TEM_CONROL,
	PUL_TEM_TIME_ON,
	PUL_TEM_TIME_OFF,
	PUL_TEM_ENER_ON,
	PUL_TEM_ENER_OFF
}MENUDESITION;


typedef enum MenuResults {
	R_TOTAL_TIME=0,
	R_TOTAL_ENERGY,
	R_AMPLITUDE,
	R_PROGRESS,
	R_MAXTEMP,
	R_ENDTEMP,
	R_PEAKPOWER,
	R_ON_TIME,
	R_ON_ENERGY,
	R_OFF_TIME,
	R_TOTAL_ON_ENERGY,
	R_TOTAL_ON_TIME,
}MENURESULTS;


#define	CONFIG_CONT            0x800 
#define	CONFIG_PULSE           0x1000
#define	CONFIG_TIME            0x200
#define	CONFIG_ENERGY          0x400
#define	CONFIG_TEMP            0x600
#define	SET_TIME_NOT_ZERO      0x20
#define	SET_TIME_ZERO          0x40
#define	SET_ONTIME_NOT_ZERO    0x60
#define	SET_ONTIME_ZERO        0x80
#define	SET_ONENERGY_NOT_ZERO  0xA0
#define	SET_ONENERGY_ZERO      0xC0
#define	SET_MAXTEMP_ZERO       0x4
#define	SET_MAXTEMP_NOT_ZERO   0x8
#define	CONFIG_SUBTIME         0xC
#define	CONFIG_SUBENERGY       0x10
#define	SET_PUL_TEMP_ZERO      0x1
#define	SET_PUL_TEMP_NO_ZERO   0x2
#define	SET_AMPLITUDE          0x0

#define CS_TIME					( CONFIG_CONT | CONFIG_TIME | SET_TIME_NOT_ZERO | SET_MAXTEMP_ZERO )
#define	CS_ENERGY				( CONFIG_CONT | CONFIG_ENERGY | SET_MAXTEMP_ZERO )
#define CS_INFINITE				( CONFIG_CONT | CONFIG_TIME | SET_TIME_ZERO )
#define PS_TIME					( CONFIG_PULSE | CONFIG_TIME | SET_ONTIME_NOT_ZERO | SET_MAXTEMP_ZERO )
#define PS_ENERGY				( CONFIG_PULSE | CONFIG_ENERGY | SET_ONENERGY_NOT_ZERO | SET_MAXTEMP_ZERO )
#define PS_INFINITE_TIME        ( CONFIG_PULSE | CONFIG_TIME | SET_ONTIME_ZERO )
#define PS_INFINITE_ENERGY		( CONFIG_PULSE | CONFIG_ENERGY | SET_ONENERGY_ZERO )
#define MAX_TEMP_CS				( CONFIG_CONT | CONFIG_TEMP )	
#define MAX_TEMP_PS_TIME		( CONFIG_PULSE | CONFIG_TEMP | CONFIG_SUBTIME )
#define MAX_TEMP_PS_ENERGY		( CONFIG_PULSE | CONFIG_TEMP | CONFIG_SUBENERGY )
#define TEMP_LIMIT_CS_TIME		( CONFIG_CONT | CONFIG_TIME | SET_TIME_NOT_ZERO| SET_MAXTEMP_NOT_ZERO | SET_PUL_TEMP_ZERO)
#define TEMP_LIMIT_CS_ENERGY	( CONFIG_CONT | CONFIG_ENERGY | SET_MAXTEMP_NOT_ZERO | SET_PUL_TEMP_ZERO )
#define TEMP_LIMIT_PS_TIME		( CONFIG_PULSE | CONFIG_TIME | SET_ONTIME_NOT_ZERO | SET_MAXTEMP_NOT_ZERO | SET_PUL_TEMP_ZERO)
#define TEMP_LIMIT_PS_ENERGY	( CONFIG_PULSE | CONFIG_ENERGY | SET_ONENERGY_NOT_ZERO | SET_MAXTEMP_NOT_ZERO | SET_PUL_TEMP_ZERO )
#define PULSE_TEMP_CS_TIME		( CONFIG_CONT | CONFIG_TIME | SET_TIME_NOT_ZERO | SET_MAXTEMP_NOT_ZERO | SET_PUL_TEMP_NO_ZERO )
#define PULSE_TEMP_CS_ENERGY	( CONFIG_CONT | CONFIG_ENERGY | SET_MAXTEMP_NOT_ZERO | SET_PUL_TEMP_NO_ZERO )
#define PULSE_TEMP_PS_TIME		( CONFIG_PULSE | CONFIG_TIME |  SET_ONTIME_NOT_ZERO | SET_MAXTEMP_NOT_ZERO | SET_PUL_TEMP_NO_ZERO)
#define PULSE_TEMP_PS_ENERGY	( CONFIG_PULSE | CONFIG_ENERGY |  SET_ONENERGY_NOT_ZERO | SET_MAXTEMP_NOT_ZERO | SET_PUL_TEMP_NO_ZERO )


typedef enum IdsModes {
	ID_CS_TIME=0,
	ID_CS_ENERGY,
	ID_CS_INFINITE,
	ID_PS_TIME,
	ID_PS_ENERGY,
	ID_PS_INFINITE_TIME,
	ID_PS_INFINITE_ENERGY,
	ID_MAX_TEMP_CS,
	ID_MAX_TEMP_PS_TIME,
	ID_MAX_TEMP_PS_ENERGY,
	ID_TEMP_LIMIT_CS_TIME,
	ID_TEMP_LIMIT_CS_ENERGY,
	ID_TEMP_LIMIT_PS_TIME,
	ID_TEMP_LIMIT_PS_ENERGY,
	ID_PULSE_TEMP_CS_TIME,
	ID_PULSE_TEMP_CS_ENERGY,
	ID_PULSE_TEMP_PS_TIME,
	ID_PULSE_TEMP_PS_ENERGY,
	ID_CS_TIME_B,
	ID_CS_ENERGY_B,
	ID_CS_INFINITE_B,
	ID_PS_TIME_B,
	ID_PS_ENERGY_B,
	ID_PS_INFINITE_TIME_B,
	ID_PS_INFINITE_ENERGY_B,

}IDMODES;


#endif
