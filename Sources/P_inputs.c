/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/P_inputs.c_v   1.6.1.3.3.3   13 Jun 2016 12:02:26   hasanchez  $
*/
/*********************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


------------------------------------ MODULE DESCRIPTION ---------------------------

         This module collects all the inputs from the input pins and then provides decision 
         making Data to the State machine

Module name: INPUTS

Filename:    P_Inputs.c
--------------------------- TECHNICAL NOTES ---------------------------------------

  This module contains functions to deal with the digital and analog inputs

------------------------------ REVISIONS ------------------------------------------

* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/P_inputs.c_v  $
 * 
 *    Rev 1.6.1.3.3.3   13 Jun 2016 12:02:26   hasanchez
 * Updated invalid entries functionality. Function CheckForSeek added.
 * 
 *    Rev 1.6.1.3.3.2   18 May 2016 12:59:32   hasanchez
 * Removed comented lines.
 * 
 *    Rev 1.6.1.3.3.1   13 May 2016 13:04:38   hasanchez
 * Added new functions and flags to avoid blink the ready signal and handle the handheld detection by request of test or request of cycle.
 * 
 *    Rev 1.6.1.3.3.0   15 Dec 2015 16:15:04   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.6.1.0   25 Aug 2015 11:55:40   hasanchez
 * Testes and Reset+Key presses were added to exit if waiting feedback of seek signal on the powerup function.
 * 
 *    Rev 1.6   10 Sep 2013 03:18:40   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.5   08 Jul 2013 04:41:18   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, and changed frequency offset acc to requirement.
 * 
 *    Rev 1.4   13 Jun 2013 07:54:56   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.3   20 Nov 2012 07:28:44   akaushal
 * Lcd Flickering Fixed and Navigation Updated
 * 
 *    Rev 1.2   06 Nov 2012 08:44:40   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   10 Sep 2012 09:49:08   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.0   06 Aug 2012 05:31:26   akaushal
 * Initial revision.
 * 
 *    Rev 1.4   31 Jul 2012 04:00:54   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.3   27 Jun 2012 07:50:32   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:33:52   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:46:52   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:01:10   ygupta
 * Initial revision.

-----------------------------------------------------------------------------------*/

/*------------------------------ INCLUDES ----------------------------------*/

#define P_INPUT  
#include "p_inputs.h"
#include "keyboard.h"
#include "weldhold.h"


/*----------------------------- GLOBAL DATA --------------------------------*/


UINT8  TestKey = 0;
UINT8  TestResetKeys = 0;
UINT8  SetKey = 0;
UINT8  ResetKey = 0;
UINT8  StartFlag = 0;
UINT8  WrongStartFlag = 0 ;
UINT8  StopFlag = 0;
UINT8  SeekRequest = FALSE;
UINT8  SeekActive = FALSE;
UINT8  PauseFlag = 0;
UINT8  StartByPT = FALSE;   //Start by panel Trigger.
UINT8  CycleReqFlag=FALSE;
UINT8  TestReqFlag=FALSE;
UINT8  Tst_N_ExtStart=FALSE;

//UINT8  StartReleased = TRUE;
UINT8 ONorOFF = 0;
UINT8 IOValue = 0; 
UINT8 ScanNum;

UINT16 ValidKeys;

/*-------------------------------- CODE ------------------------------------*/
void HandleKeyPad() 
/***************************************************************************
*                                                                         
*   Purpose:
*        Scans the keyboard hardware at sacn time interval and call ReadKeyboard 
*        function to debounce the keys and call ProcessKeyBoard function to      
*        Process keys pressed.
*        First Scans all the columns and then  
*                                                             
*   Entry condition:  
*
*        A scan of the ports connected to the keyboard
*
***************************************************************************/
{
   if (KbScanTimer == TRUE)
   {
      KbScanTimer = 0;
      if ((ScanNum <3)){
         KeyScan(ScanNum);                 // Scan Col 0 and 1 and 2
         ScanNum++;
      }
      else{
         KeyScan(ScanNum);                 // Scan Col 3
         keys = ReadKeyboard();            // debounce and decode
         ProcessKeyboard(keys);             // Process KB status
         ScanNum = 0;
      }
   }
}


INPUT_CODE GetInputs(void){     
/***************************************************************************/
/*                                                                         */
/* This function reads the 3 digit inputs (overload, ground detect & seek  */
/* request).  Then returns the debounced inputs in INPUT_CODE.             */
/*                                                                         */
/***************************************************************************/
   INPUT_CODE InputsAvailable;
   InputsAvailable = IOValue;
   //Print8(IOValue);
   InputsAvailable = (InputsAvailable ^ GND_DETECT_IN );   /* Invert logic */
   InputsAvailable = (InputsAvailable ^ EXT_SEEK_IN );     /* Invert logic */
   return(InputsAvailable);
}

 
UINT8 GetPower(void){
/***************************************************************************/
/*                                                                         */
/* This function Filter out the power readings presents in ADPower array   */
/* which is filled by interrupt handler every ms. Function removes the     */
/* lowest and highest readings and take the average of remaining readings. */
/* Convert it to a percent(1 to 127%) and return the value.                */
/* PercentPower = POWER_REGISTER shifted right by 9.                       */   
/*                                                                         */
/***************************************************************************/
   UINT8 PercentPower ,i;
   UINT16 Lowest,Highest, Total ; 
   Lowest = ADPower[0];
   Highest = ADPower[0];
   Total = ADPower[0];
   for(i = 1; i < ADCPOWERCOUNT; i++)
   {
      if(Lowest > ADPower[i])
         Lowest = ADPower[i];
      if(Highest < ADPower[i])
         Highest = ADPower[i];
      Total += ADPower[i];
   }
   Total = Total - Lowest - Highest;
   PercentPower = (UINT8)(Total >> 3); // only 7 bit resolution is for use.
   return(PercentPower);
}

UINT16 GetTemperatureADC(void){
/***************************************************************************/
/*                                                                         */
/* This function Filter out the temperature readings presents in ADTempe array   */
/* which is filled by interrupt handler every ms. Function removes the     */
/* lowest and highest readings and take the average of remaining readings. */
/*                                                                         */
/***************************************************************************/
   
   UINT16 Lowest,Highest, Total;
   UINT8 temp1;
   UINT16 Temperature;
   
   Lowest = ADTempe[0];
   Highest = ADTempe[0];
   Total = ADTempe[0];
   for(temp1 = 1; temp1 < ADCTEMPECOUNT; temp1++)
   {
      if(Lowest > ADTempe[temp1])
         Lowest = ADTempe[temp1];
      if(Highest < ADTempe[temp1])
         Highest = ADTempe[temp1];
      Total += ADTempe[temp1];
   }
   Temperature = Total - Lowest - Highest;
   Temperature = Temperature/(ADCTEMPECOUNT - 2);
   Temperature = ConvertTempToF(Temperature);
   if(Temperature <= TEMP_SENSOR_DISCONNECTED)
      TempeProbePresent = FALSE;
   else 
      TempeProbePresent = TRUE;   
   return(Temperature);
}

void ProcessInputs(void)
/***************************************************************************/
/*                                                                         */
/*   ProcessInputs is called once each pass off the main loop.  It will    */
/*   convert the inputs bits, to global flags usable                       */
/*   by the state machine. Menu navigation key will be handled by          */
/*   HandleKeyPad function. Weld related keys and digital inputs will be   */
/*   handled here also. Function will set the variables those will be used */
/*   by state machine to control the weld also.                            */
/***************************************************************************/
{
  
	INPUT_CODE  Inputs ;
	static  UINT8  NoKey = TRUE,  StartReleased = FALSE, EnterReleased = TRUE, PauseReleased = TRUE;
	ValidKeys =0;
/* Handle Overload, Ground Detect and External Seek here */
	Inputs = GetInputs();
	if ((Inputs & OVERLOAD_IN)  == OVERLOAD_IN) {	
      OLFlag = TRUE;     
	}

	if ((Inputs & EXT_SEEK_IN) == EXT_SEEK_IN) 
	{
		SeekRequest = TRUE;
	}
	else 
	{
		SeekRequest = FALSE;
	}   
	

/* Now handle the front panel keypad */
	HandleKeyPad() ;
   
	ValidKeys = GetKeys();

	ValidKeys &= (UINT16)(KB_TEST | KB_START_STOP | EXTSTART | KB_RESET);
   
	switch (ValidKeys) {
		case TEST_N_RESET:
			TestResetKeys=TRUE;
			break;
		case STARTSTOP_N_EXTSTART:                                     /*This stop the cycle if startstop key 
																		and ext start are activated*/
			if(((screen == SCREEN_RESULTS) || (screen == SCREEN_RDY)) && (Conf.ConfigData.PanelTrig == FALSE || HandHeld == TRUE))
			{
				StopFlag = TRUE;
				StartReleased = FALSE; 
			}
			break;
		case KB_TEST:
			TestKey = TRUE;
			TestReqFlag = TRUE;
			if ((HandHeld == TRUE ) && (StartFlag == TRUE))
			{
				StopFlag = TRUE;
				StartFlag = FALSE;
			}

			break; 
		case KB_START_STOP:
			StartByPT = TRUE; 
			if (Conf.ConfigData.PanelTrig == TRUE)
			{
				if(((screen == SCREEN_RESULTS) || (screen == SCREEN_RDY)) )
				{           
					if (Conf.ConfigData.PulseStart == FALSE) 
					{      
						if (StartReleased == TRUE) 
						{
							StartFlag = TRUE;
							StopFlag = FALSE;
							CycleReqFlag=TRUE;
						}                 
					}
					else 
					{
						if (StartReleased == TRUE) 
						{
							if (StopFlag==FALSE)
							{
								StartFlag = FALSE;
								StopFlag = TRUE;
							}
							else
							{
								StartFlag = TRUE;
								StopFlag = FALSE;
								CycleReqFlag=TRUE;
							}
						}
					
					}
					StartReleased = FALSE;
				}
				else 
					if(((screen == SCREEN_ERROR) && (CurrentAlarm == ER_OL))  && (StartReleased ==TRUE))
					{                                                   //to handle when auto rest is set 1             
						if((Conf.ConfigData.AutoReset == TRUE))  
						{ 
							ResetAlarm();
							SwitchScreen(SCREEN_RDY);   
							if(Conf.ConfigData.PulseStart == TRUE)
							{
								StartReleased = TRUE; 
								StopFlag = TRUE;
								StartFlag = FALSE;  
							}                          
						}
					}
			}
			else
			{
				StopFlag = TRUE;
				StartReleased = FALSE;
			}

			break;
         
		case EXTSTART:   
			StartByPT = FALSE; 
			if(((screen == SCREEN_RESULTS) || (screen == SCREEN_RDY)))
			{
				if (Conf.ConfigData.PulseStart == FALSE) 
				{       /* Is Pulse Start allowed ? */
					if (StartReleased == TRUE) 
					{  /* No */
						StartFlag = TRUE;
						StopFlag = FALSE;
						CycleReqFlag=TRUE;
					}
				}
				else 
				{
					if (StartReleased == TRUE) 
					{/* Yes */
						if (StopFlag==FALSE &&( Conf.ConfigData.PanelTrig == FALSE || HandHeld== TRUE))
						//if (StartFlag == TRUE ) 
						{
							StartFlag = FALSE;
							StopFlag = TRUE;
						}
						else 
						{
							StartFlag = TRUE;
							StopFlag = FALSE;
							CycleReqFlag=TRUE;
						}
					}
				}
				StartReleased = FALSE; 
			}         
			else 
				if(((screen == SCREEN_ERROR) && (CurrentAlarm == ER_OL))  && (StartReleased ==TRUE))
				{                                            //to handle when auto rest is set 1             
					if((Conf.ConfigData.AutoReset == TRUE))  
					{ 
						ResetAlarm();
						SwitchScreen(SCREEN_RDY);   
						if(Conf.ConfigData.PulseStart == TRUE)
						{
							StartReleased = TRUE; 
							StopFlag = TRUE;
							StartFlag = FALSE; 
						}
					}
				}
			break;
	case KB_RESET:
		ResetKey = TRUE;
		if (screen == SCREEN_ERROR)
		{
			ResetAlarm();
			if(CurrentAlarm != ER_DE)
				SwitchScreen(SCREEN_RDY);
			else
				SwitchScreen(OldScreen);
		}
		break;
	case NO_KEY:
         //NoKey = TRUE;
		TestKey = FALSE;         
         
         ResetKey = FALSE;
         TestResetKeys=FALSE;   /*set to false TestResetKeys flag if nothing is pressed*/
         StartReleased = TRUE;
         PauseReleased = TRUE;
         if ( HandHeld == FALSE)
         {
        	 if (Conf.ConfigData.PulseStart == FALSE ) {
        		 StopFlag = TRUE;
        		 StartFlag = FALSE;
        	 }
        	 else
        		 StartFlag = FALSE;
         }
         else
         {
        	 StopFlag = TRUE;
        	 StartFlag = FALSE;
         }


         if ( (AlarmFlag == TRUE) && (CurrentAlarm == ER_SS) ) {
            ResetAlarm();
            SwitchScreen(SCREEN_RDY);
         }
         else if ( (AlarmFlag == TRUE) && (CurrentAlarm == ER_OL) && (Conf.ConfigData.AutoReset == TRUE) && (ExperFlag == FALSE) ) {
            LockItFlag = FALSE;
         }         
      break;

      default:   /* Any invalid key combination will end up here */
         //ResetKey = FALSE;
         //TestResetKeys=FALSE;   /*set to false TestResetKeys flag an invalid combination is pressed*/

         //PauseReleased = TRUE;
      break;
   }
}
void CheckForSeek (void)
{
	INPUT_CODE  Inputs ;
	Inputs = GetInputs();


	if ((Inputs & EXT_SEEK_IN) == EXT_SEEK_IN) 
		SeekRequest = TRUE;
	else 
		SeekRequest = FALSE;
}
