/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/main.c_v   1.6.1.3.3.4   14 Jul 2016 11:59:08   hasanchez  $
************************************************************************************ 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION -------------------------------------

This is the main module for controlling all the Onchip devices along with the 
State Machine.

Module name: MAIN

Filename:    main.c
--------------------------- TECHNICAL NOTES ----------------------------------------

This code contains the main function which initializes hardware and contains main loop


------------------------------ REVISIONS -------------------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/main.c_v  $
 * 
 *    Rev 1.6.1.3.3.4   14 Jul 2016 11:59:08   hasanchez
 * updated type of UpdateTimer to BOOLEAN
 * 
 *    Rev 1.6.1.3.3.3   13 Jun 2016 11:58:12   hasanchez
 * Updated Hand Held verification.
 * 
 *    Rev 1.6.1.3.3.2   18 May 2016 13:04:56   hasanchez
 * Updated the while to feed the dog.
 * 
 *    Rev 1.6.1.3.3.1   13 May 2016 12:54:46   hasanchez
 * Changed the passes to the state machine.
 * Craete a Handheld detection function.
 * Update powerup function to handle the handheld.
 * 
 *    Rev 1.6.1.3.3.0   15 Dec 2015 16:15:02   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.6.1.0   25 Aug 2015 11:47:00   hasanchez
 * Updates on the Powerup seek function
 * 
 *    Rev 1.6   18 Sep 2013 08:19:22   akaushal
 * Fixed the Power Up seek issue
 * 
 *    Rev 1.5   10 Sep 2013 03:19:10   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.4   13 Jun 2013 07:54:32   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.3   25 Mar 2013 08:01:02   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.2   06 Nov 2012 08:44:12   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   16 Aug 2012 01:57:54   akaushal
 * SFX Code With External Crystal Oscillator and HW Changes
 * 
 *    Rev 1.0   06 Aug 2012 05:31:02   akaushal
 * Initial revision.
 * 
 *    Rev 1.4   31 Jul 2012 04:00:08   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.3   27 Jun 2012 07:49:50   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:33:10   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:46:00   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:32   ygupta
 * Initial revision.
------------------------------ INCLUDES --------------------------------------------*/

#include "portable.h"
#include "keyboard.h"
#include "LCD.h"
#include "Cpu.h"
#include "onems.h"
#include "print.h"
#include "global.h"
#include "eeprom.h"
#include "weldhold.h"
#include "p_output.h"
#include "p_inputs.h"
#include "state.h"
#include "CRC.h"
/* Macros */
#define STATE_PASSES    5        /* # of calls for state machine in each loop */  
#define START_OF_RAM    0x0100    /* Where internal ram starts                 */
#define SIZE_OF_RAM     0x071F    /* How many bytes there are                  */
#define SIZE_OF_STACK   0x180     /* How much reserved for stack, don't test   */

#define START_OF_TEST   (START_OF_RAM)
#define SIZE_OF_TEST    (SIZE_OF_RAM - SIZE_OF_STACK -1)
#define TEST1           0x055
#define TEST2           0x0AA 


#define POWER_UP_DELAY   500      /* Power up delay before final reading of keys */
#define SEEK_ON_TIME      20L      /* Time (ms) for seek on                       */
#define SEEK_OFF_TIME     30L      /* Time (ms) for seek off                      */


#define PROGRAMFLSH_STARTADDR    0x8000
#define PROGRAMFLASH_LENGTH      0x7BAF

void PowerUpDiagnostics(void);
/*----------------------------- GLOBAL DATA --------------------------------*/
BOOLEAN UpdateTimer;
//const UINT8 SoftwareVersion = SOFTWAREVERSION;

/*-------------------------------- CODE ------------------------------------*/


void DoRamTest(void)
/***************************************************************************/
/*                                                                         */
/*       This routine will perform an internal ram check.  The test will   */
/*   be a non destructive check.  That is, each location will be saved     */
/*   before the write is performed and then restored after the read has    */
/*   verified the location is okay.  Obviously, if ram is bad, this might  */
/*   not work as predicted.  If an error is detected an endless loop will  */
/*   be started and no return will be made.  The system will effectively   */
/*   be shut off.                                                          */
/*                                                                         */
/***************************************************************************/
{
   UINT8   *Ptr;
   UINT8   Save;
   UINT16  Index;
   Ptr = (SINT8*) START_OF_TEST;
   for ( Index = 0; Index < SIZE_OF_TEST; Index++) {
      Save = *(Ptr+Index);             /* Save data at test location       */
      *(Ptr+Index) = TEST1;            /* Attempt to write test pattern 1  */
      if (*(Ptr+Index) != TEST1)       /* If data does not compare..       */
         {
#ifdef DEBUG_ENABLE        
         PrintStr("\n\rRAM_ERROR\n\r");
#endif
            while(FOREVER);                     /* Lock up system                */
         }
      else {
         *(Ptr+Index) = TEST2;         /* Attempt to write test pattern 2  */
         if (*(Ptr+Index) != TEST2)    /* If data does not compare..       */
         {
#ifdef DEBUG_ENABLE     
            PrintStr("RAM_ERROR\n\r");
#endif
            while(1);                     /* Lock up system             */
         }                     
         else
            *(Ptr+Index) = Save;       /* Restore original contents        */
      }
   }
}
UINT8 CheckForHH()
{
	UINT8 HH=FALSE;
	ClrReady(); 
	StartMSTimer(TIMER2, WAIT_HH);
	while (FlagTimer2 == ON )
		FeedTheDog();
	CheckForSeek();
	if (SeekRequest ==TRUE)
	{
		SetReady();
		StartMSTimer(TIMER2, WAIT_HH);
		while (FlagTimer2 == ON )
			FeedTheDog();
		CheckForSeek();
		if (SeekRequest ==FALSE)
			HH=TRUE;
	}
		
	return (HH);
}


void PowerUpDiagnostics(void)
/***************************************************************************/
/*                                                                         */
/*   This function will be called at power up.  It will check the ram      */
/*   (except stack area),Calculate Flash CRC,turn on all LCD icons, Powerup*/
/*    seek, display the software version number for two seconds before returning. */
/*    It also checks for power board presence by reading 10VRef ADC        */
/*   Exit conditions:                                                      */
/*       7-segment display:  Displaying zeroes in the power format         */
/*                                                                         */
/***************************************************************************/
{
   UINT8 Eeprom_OK;
   BOOLEAN FlipFlop = 0;
   DoRamTest();                     /* Will never return if ram test fails */
#ifdef DEBUG_ENABLE     
   PrintStr("\n\r RAM TEST COMPLETE");
#endif
   Calc_Flash_CRC_Ck();             // Calculate Flash CRC for program Area.
#ifdef DEBUG_ENABLE     
   PrintStr("\n\r FLASH CHECK COMPLETE");
#endif
   currentLCDData.CurrentTemp = TEMP_SENSOR_DISCONNECTED+1; //initial value is greater than 32F                              
/* If we get here, then ram is okay.  Now read the EEPROM */
   Eeprom_OK = EEPROM_Read(0);
   if( Eeprom_OK == FALSE)
   {
	   Memcpy(&DisplayCodes, &Er_EEPROM,NUM_7SEGMENT_DIGITS);
      UpdateLive();    
      StartMSTimer (TIMER1, MILLISEC_IN_TWOSECOND);
      while (FlagTimer1  == TRUE)     /* Wait on time             */
         FeedTheDog();                        // Reset Watchdog timer       
   }
#ifdef DEBUG_ENABLE     
   PrintStr("\n\r EEPROM TEST COMPLETE");
#endif
   HandHeld=CheckForHH();  //While all the icons on verify if a hand held is connected.
   Amplitude =  CurrentPreset.presetData.AmplitudeValue;
   SetAmplitude((UINT8)Amplitude);           /* Output proper amplitude */
   Wattage = MenuWattage[Conf.ConfigData.PsWattage].displayValue;
   RecallPresetSelected();
   //CenterDigiPot(); Visitar
   //DigiPot_Adjust();                        /* Adjust Dig Pot according to configuration parameter.*/
#ifdef DEBUG_ENABLE     
   PrintStr("\n\r DIGITAL POT Adjusted");
#endif
   Beep(STARTUPBEEP); 
   DisplayAllIcons();   
/*  While waiting for 1 second to display leds, do a power up seek if PwrUpSeek=true */ 
   
   
   StartMSTimer (TIMER1, MILLISEC_IN_ONE_SEC);
   
   currentLCDData.HandHeld=HandHeld;
   while (FlagTimer1  == TRUE)     /* Wait on time             */
      FeedTheDog();  
/* change to power up screen */  
   ALLIconsOFF();
   SwitchScreen(SCREEN_POWERUP);
   UpdateLive();
/* Start seek               */  
   StopAllTimers();
   
   StartMSTimer (TIMER1, MILLISEC_IN_TWOSECOND);
   if (Conf.ConfigData.PwrUpSeek == TRUE) {    /* If power up seek on */
	   if  (HandHeld== FALSE)
	   {

	   
		   KeyMask = ALL_KEYS;                      /* Allow all keys           */
		   while (SeekActive == FALSE && TestResetKeys == FALSE)  {           /* Wait until seek starts  or  Test + Reset keys  */
			   if (FlagTimer2 == FALSE){ /*Timer to re-send the Seekrequest. */
				   if (FlipFlop == TRUE)
					   PSSeek();
				   else
					   RemovePSSeek();
				   FlipFlop = !FlipFlop;
				   StartMSTimer (TIMER2, SEEK_ON_TIME);
				   ProcessInputs();
				   FeedTheDog();   // Reset Watchdog timer 
			   }
		   }
		   /*Once detected SeekActive signal, The seek request can be removed.*/
		   UpdateLive();
		   RemovePSSeek();   
		   while (SeekActive == TRUE){    /* Wait until the seek finish*/
			   ProcessInputs();
			   FeedTheDog();    // Reset Watchdog timer                     
		   }
	   }
   }
    /* Here is waiting to complete the 2 seconds on the POWERUP screen */
     UpdateLive();
     while (FlagTimer1  == TRUE){     /* Wait seek time. THE DURATION OF THE SEEK IS DETERMINED BY THE MODULE SEE DRAWING 913-242-390R SECTION 1[3c]*/
        ProcessInputs();
        //UpdateLive();
        FeedTheDog();  // Reset Watchdog timer 
     }     
     
/* Display 'rdy' */

   if (screen ==  SCREEN_POWERUP) {
      PresetNo = 0;
      SetReady(); 
      SwitchScreen(SCREEN_RDY); 
   }
   
   UpdateLive();
}


void main(void)
/***************************************************************************/
/* The main program will setup and initialize the CPU registers and        */
/* peripherials, do a power on test, then enter a loop to check inputs,    */
/* Process Keypad, process the state machine and write to the outputs and  */
/* update the LCD.                                                         */
/***************************************************************************/  
{
   UINT16	Loop = 0;
   //bool 	Flag = FALSE;
   LowLevelInit();
   FeedTheDog();                         // Reset Watchdog timer 
#ifdef DEBUG_ENABLE     
   PrintStr("\n\n\n\r START ");  
#endif
   BacklightControl(HIGH);               // Backlight LED ON
#ifdef DEBUG_ENABLE     
   PrintStr("\n\r Starting Power Up Diagnostics");
#endif

   PowerUpDiagnostics();
   //StorePassword = (UINT8)GetVar(Format[PASSWORD].Ptr,Parameters[PASSWORD].VarSize);
   /*------------------Now start the endless loop ---------------------------*/

   while (FOREVER) 
   {

      /*Process KeyPad and Digial and analog inputs.*/
	   
	   if( KeyBoardScanFlag == TRUE )
	   {
		  // Flag=TRUE;
		   KeyBoardScanFlag = FALSE;
		   ProcessInputs();
	   }

      for (Loop = 0; Loop < STATE_PASSES; Loop++) {
         StateMachine();
      }
          
      /* Control screen updating */      
      if ( UpdateLiveReady == TRUE )
    	//  if ( UpdateLiveReady == TRUE && Flag==FALSE )
      {                                    // Do screen updates here
         UpdateLive();
         UpdateLiveReady = FALSE;          // reset
      }
      //Flag=FALSE;
      FeedTheDog(); 
   }
}

