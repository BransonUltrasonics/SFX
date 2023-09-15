/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/eeprom.h_v   1.4.1.3.3.2   27 Jul 2016 09:17:48   hasanchez  $
********************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


----------------------------------- MODULE DESCRIPTION ---------------------------

This is the main module for controlling the i2c and EEPROM.

Module name: EEPROM

Filename:    eeprom.h
---------------------------------- TECHNICAL NOTES -------------------------------

This code controls MC9S08LG32  i2c to talk to a  EEPROM.


-------------------------------------- REVISIONS ---------------------------------
 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/eeprom.h_v  $
 * 
 *    Rev 1.4.1.3.3.2   27 Jul 2016 09:17:48   hasanchez
 * Upadted the data type to HHLimit from BOOLEAN to INT8
 * 
 *    Rev 1.4.1.3.3.1   13 Jun 2016 13:02:44   hasanchez
 * HHLimit register added to the configuration structure.
 * 
 *    Rev 1.4.1.3.3.0   15 Dec 2015 16:14:56   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.4.1.0   25 Aug 2015 12:15:58   hasanchez
 * Update the Preset and Configuration structure.
 * 
 *    Rev 1.4   13 Jun 2013 07:59:42   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.3   20 Nov 2012 07:27:24   akaushal
 * Lcd Flickering Fixed and Navigation Updated
 * 
 *    Rev 1.2   06 Nov 2012 08:41:38   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   10 Sep 2012 09:47:40   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.0   06 Aug 2012 05:26:38   akaushal
 * Initial revision.
 * 
 *    Rev 1.3   27 Jun 2012 07:48:24   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:31:40   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:44:54   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:00   ygupta
 * Initial revision.
 
----------------------------------------------------------------------------------*/

#ifndef _eeprom_H
#define _eeprom_H

#include "CPU.h"
#include "onems.h"
#include "global.h"
#include "portable.h"         //using user-defined datatypes
#include "lcd.h"
#include "print.h"
#include "crc.h"

#define PASS 1
#define FAIL 0
#define I2C_FAILSAFE_TIME		(5)				//5 tries
#define I2C_PASSED_WRITE		0x8100
#define I2C_PASSED_READ			0x4100
#define I2C_FAILED_WRITE		0x8000
#define I2C_FAILED_READ			0x4000

#define BIGENDIAN 1

#define EEPROM_ADDRESS 			0xA0

#define EE_SDA		        PTID_PTID4     //Port I4
#define EE_SCL		        PTID_PTID5     //Port I5  
#define EE_SDA_DIR			PTIDD_PTIDD4		  
#define EE_SCL_DIR		  	PTIDD_PTIDD5

//Primary Storage Start
#define MAX_SAVED_PRESETS     20
#define EE_START              0 
#define EE_CONFIGURATION_LOC  (EE_START + ((sizeof(EE_PRESET)+CRC_OFFSET)*(MAX_SAVED_PRESETS+1)))
#define HALF_BYTE             4
#define ONE_BYTE              8
#define TWO_BYTE              16
#define PRESET_SELECTED_ADD	(EE_CONFIGURATION_LOC+CONFIG_MAXSIZE+CRC_OFFSET)

//Backup Storage on another Page
#define EE_BACKUP_START      2048
#define EE_BACKUP_CONFIGURATION_LOC (EE_BACKUP_START + ((sizeof(EE_PRESET)+CRC_OFFSET)*(MAX_SAVED_PRESETS+1)))
#define EEPROM_END           8191 	//8K EEPROM

/* Magic Number is used to check the validity of EEPROM at
 * First Startup of at Coldstart, when the data is EEPROM is
 * invalidated. Magic Number is stored at predefined locations.
 */
#define EE_MAGIC1            EEPROM_END-1
#define EE_MAGIC2            EEPROM_END

#define MAGIC_NUM1            0xAA           // 0101 0101
#define MAGIC_NUM2            0x55           // 1010 1010


#define PRESET_MAXSIZE        sizeof(PRESET_STRUCT)
#define CONFIG_MAXSIZE        sizeof(EE_CONFIG)

#define CRC_OFFSET            3 // 1 byte CRC, 2 bytes CheckSum 

   
//define the Preset
typedef struct preset {
   UINT32 Time;
   UINT32 Energy;
   UINT32 OnTime;
   UINT32 OffTime;
   UINT16 OnEnergy;
   UINT32 TotalOnTime;
   UINT32 TotalOnEnergy;
   UINT16 PulseTempe;
   UINT16 MaxTempe;
   UINT8  AmplitudeValue;
   bool   Microtip;
   UINT16 WeldMode;
}PRESET_STRUCT;

typedef union EE_PRESET1 {
   UINT8 dat[PRESET_MAXSIZE];
   PRESET_STRUCT presetData;
} EE_PRESET;


//define the Configuration Data

typedef struct configuration{
   UINT8  PanelTrig;          //Offset      
   UINT8  PulseStart;         //1
   UINT8  AutoReset;          //2
   UINT8  EndOfWeldBeep;      //3
   UINT8  ErrorBeep;          //4
   UINT8  DispLock;           //6
   UINT8  PwrUpSeek;          //7
   UINT32  TimeOut;        //8
   UINT8  Tempe_C_F;          //15
   UINT8  ColdStart;          //16
   UINT8 HHLimit;           //17
   SINT16 CalibrationOffset;  //18
   UINT8  EndOfCycle;        //19
   UINT8  Password;           
   UINT16 PsWattage;          //20
   UINT8  Region;             //  1: European region  0: American region 
   UINT8  FactColdStart;     //24
} EE_CONFIG;

typedef union ee_config_data {
   UINT8 cdat[CONFIG_MAXSIZE];
   EE_CONFIG ConfigData;
}EE_CONFIG_DATA;

extern EE_PRESET CurrentPreset, TempPreset;
extern EE_CONFIG_DATA Conf;
enum TYPE { CONFIG =0 , PRESET };
 
 
// Prototypes
void EEPROM_Recall(UINT8);  // display and change the current parameter
UINT8 EEPROM_Read(UINT8);    // only Display
void EEPROM_SavePreset(UINT8 Presetno);
UINT8 EEPROM_ReadPreset(UINT8 Presetno);
void EEPROM_SaveConfig(void) ;
UINT8 EEPROM_ReadConfig(void) ;
void EEPROM_Restore(void); 
void SavePresetSelected(UINT8 PresetSelected);
void EEPROM_ChangeParam(UINT8,enum TYPE,UINT8); //change a parameter in preset
void RecallPresetSelected(void);
void WriteCRC2EEPROM (UINT16 Address, UINT32 CrcCk );

void ResetEEPROM(void);
#endif
