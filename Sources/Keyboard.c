/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/Keyboard.c_v   1.12.1.3.3.2   13 Jun 2016 12:10:16   hasanchez  $
***********************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION ------------------------------------

This is the Keyboard module for controlling the Matrix Keypad Connected.

Module name: KEYBOARD

Filename:    Keyboard.c
--------------------------- TECHNICAL NOTES ----------------------------------------

This Code contains the module for Keypad Scanning and then Handling Key Presses

------------------------------ REVISIONS -------------------------------------------

* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/Keyboard.c_v  $
 * 
 *    Rev 1.12.1.3.3.2   13 Jun 2016 12:10:16   hasanchez
 * Flag to verify the amplitude during cycle was added. Updated pause key functionality, now is ignored if Hand Held is detected.
 * 
 *    Rev 1.12.1.3.3.1   13 May 2016 13:01:30   hasanchez
 * Updated filters of keyboard to handle the test and Ext Start while ultrasonic test.
 * 
 *    Rev 1.12.1.3.3.0   15 Dec 2015 16:15:02   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.12.1.0   25 Aug 2015 11:43:22   hasanchez
 * Updates telated to the new navigation Style and Numeric autoincrements.
 * 
 *    Rev 1.12   10 Sep 2013 03:19:42   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.11   08 Jul 2013 04:40:34   akaushal
 * Updated the Auto reset functionality, error screen when external start is ON during power up, and changed frequency offset acc to requirement.
 * 
 *    Rev 1.10   13 Jun 2013 07:54:02   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.9   25 Mar 2013 08:00:32   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.8   20 Dec 2012 01:24:34   akaushal
 * Updated Error codes and fixed Total runtime
 * 
 *    Rev 1.7   03 Dec 2012 08:55:12   akaushal
 * Navigation and Screens Updated
 * 
 *    Rev 1.6   20 Nov 2012 07:28:14   akaushal
 * Lcd Flickering Fixed and Navigation Updated
 * 
 *    Rev 1.5   07 Nov 2012 07:03:58   akaushal
 * Screen Navigation Updated
 * 
 *    Rev 1.4   06 Nov 2012 08:43:44   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.3   10 Sep 2012 09:48:20   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.2   03 Sep 2012 09:43:42   akaushal
 * Automode Corrected for Total time Icon
 * 
 *    Rev 1.1   08 Aug 2012 02:16:10   akaushal
 * Remove Total Time from Automode 
 * 
 *    Rev 1.0   06 Aug 2012 05:30:44   akaushal
 * Initial revision.
 * 
 *    Rev 1.5   31 Jul 2012 03:59:12   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.4   06 Jul 2012 10:02:54   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.3   27 Jun 2012 07:48:52   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:32:04   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:45:20   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:10   ygupta
 * Initial revision.

------------------------------------------------------------------------------------


-------------------------------- INCLUDES ------------------------------------------*/

#define KEYBOARD 
#include "keyboard.h"
#include "MC9S08LG32.h"
#include "onems.h"
#include "global.h"
#include "LCD.h"
#include "p_inputs.h"
#include "P_OUTPUT.h"
#include "weldhold.h"
#include "state.h"
#include "util.h"

/*---------------------------- LOCAL_EQUATES -------------------------------*/
UINT16 HwKeyScan;
UINT8 keyScrollShiftReady;
UINT8 OldScreen = SCREEN_RDY;
UINT16 KeyMask;
UINT16 KeysPressed;
KEY_CODE keys;
UINT16 CurrentTestCode;
UINT8 ReleaseFlag = TRUE;
BOOLEAN KeyHold = FALSE;
BOOLEAN KeyUpDown = FALSE;
UINT8 KeyRepeat = FALSE;
/*-------------------------------- CODE ------------------------------------*/


void KeyScan(UINT8 ScanNum)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Scans the keyboard hardware and assembles an int that contains     
*        all keys pressed. Key combinations show up as mapped bit combinations
*        Hardware level function.
*                                                             
*   Entry condition:  
*
*        A scan of the ports connected to the keyboard
*
*   Exit condition: 
*                                  
*        Returns a keypress combination.
*                                       
***************************************************************************/
{   
   UINT16 ptad = (UINT8)PTAD;
   ptad = ptad | 0xff00;            //make upper byte all 1's
   if (ScanNum == 0){
                                    // scan col0 and then col1 = low, col1/2/3 = float
      HwKeyScan = (~ptad & 0x70) << 5; // masking and shifting (BIT9/10/11)
      PTAD |= (byte)COL0_REG;          // set Col0 to high
      PTAD &= ~(byte)COL1_REG;         // set Col1 to low
   }
   else if (ScanNum == 1){
                                    // scan col1 and then col2 = low, col0/1/3 = float
      HwKeyScan |= (~ptad & 0x70)<<2;   // masking and shifting
      PTAD |= (byte)COL1_REG;             // set Col1 to high
      PTAD &= ~(byte)COL2_REG;            // set Col2 to low
   }
   else if (ScanNum == 2){      
                                    // scan col2 and then col3 = low. col 0/1/2 = float
      HwKeyScan |= (~ptad & 0x70) >> 1;   // masking and shifting
      PTAD |= (byte)COL2_REG;             // set Col2 to high
      PTAD &= ~(byte)COL3_REG;            // set Col3 to low
   } else if(ScanNum == 3) {
                                    // scan col3 and then col0 = low, col1/2/3 = float
      HwKeyScan |= (~ptad & 0x70) >> 4;        // masking and shifting
      PTAD |= (byte)COL3_REG;             // set Col3 to high
      PTAD &= ~(byte)COL0_REG;            // set Col0 to low
   }
      
} // end of scan_keyboard


KEY_CODE ReadKeyboard(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Processes valid returns from KeyScan() for debounce time, repeat
*        key delays and holdoffs. Keys being held down a given time can be       
*        processed in combination with the appropriate ProcessKeyboard() 
*        subfunction.       
*                                                                         
*   Entry condition:  
*
*        HwKeyScan - scan of the kb
*
*   Exit condition: 
*                                  
*        Returns a validkey press that is debounced.
*                                       
***************************************************************************/
{
	// Local Variables 
	KEY_CODE LastKey;
	static KEY_CODE CurKey;
	static UINT16  Debounce;
	static UINT8 keyPress;
	static UINT8 ValidKey;
	static KEY_CODE LocalKey;
	static bool Start_And_LoR  = FALSE;
	   //**** Read the Keyboard ****
	LastKey = CurKey;
	   //**** Scan Keyboard ****
	CurKey = HwKeyScan;
	LocalKey =  HwKeyScan & ((UINT16)( KB_TEST  | KB_START_STOP | KB_UP | KB_DOWN | KB_RIGHT | KB_LEFT ));
	if (CurKey != NO_KEYS_PRESSED) 
	{
		if (ValidKey == TRUE) 
		{
			if(ReleaseFlag == TRUE) 
			{
				if(screen != SCREEN_TEST) 
				{

					if ((LocalKey & KB_TEST) == KB_TEST)
					{
						LocalKey &= ~KB_UP;
						LocalKey &= ~KB_DOWN;
						LocalKey &= ~KB_RIGHT;
						LocalKey &= ~KB_LEFT;
					}
					switch (LocalKey) 
					{
						case KB_TEST :
						case KB_START_STOP :
							Start_And_LoR = FALSE;
						case ( KB_START_STOP | KB_UP ):
						case ( KB_START_STOP | KB_DOWN ):
							keyPress = TRUE; // Fake the keypress for repeat key
							break;
						case ( KB_START_STOP | KB_RIGHT ):
						case ( KB_START_STOP | KB_LEFT ):
							if (Start_And_LoR == FALSE)
							{
								Start_And_LoR = TRUE;
							}
							else
							{
								CurKey &= ~KB_RIGHT;
								CurKey &= ~KB_LEFT;
							}
							keyPress = TRUE; // Fake the keypress for repeat key
							break;
						case KB_UP:
						case KB_DOWN:
							if(KeyHold == FALSE)   //at first key press
								KeyUpDown = TRUE;	  //wait for key holdtimer to complete               	
							else
							{                  		 
								if(KeyRepeat == TRUE)
								{
									keyPress = TRUE;
									KeyRepeat = FALSE;									
									CurrentDigit.Blinking = FALSE;
								}
							}
							break;
						case ( KB_TEST  | KB_START_STOP):
							if (StartFlag==TRUE)
								keyPress = TRUE;
							else
								keyPress = FALSE;
							break;
						default: // do nothing
							break;
					}
				}
			} else CurKey = NO_KEY;
		}
		else {
			if (Debounce == 0) 
			{
				if (LastKey == CurKey) 
				{
					ValidKey = TRUE;
					keyPress = TRUE;
				}
				else 
				{
					Debounce = DEBOUNCE_TIME;
					ValidKey = FALSE;             
					ReleaseFlag = TRUE;
	    	                
					KeyHold = FALSE;
					KeyUpDown = FALSE;
					CurrentDigit.Blinking = TRUE; 
				}
			}
			else 
				Debounce--;
		}
	}
	else 
	{
	// All keys are released, so reset everything
		Debounce = DEBOUNCE_TIME;
		ValidKey = FALSE;
		ReleaseFlag = TRUE;
		   
		KeyHold = FALSE;
		KeyUpDown = FALSE;
		CurrentDigit.Blinking = TRUE; 
	}
	   if (keyPress) {
		   keyPress = FALSE;
		   return CurKey;
	   }
	   else{
		   return NO_KEY;
	   }
} 

void HandleMICROTIP(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Handle the Microtip Function( Amplitude=<70%) and select appropriate Icon
*                                                                         
*   Entry condition:
*
*   Exit condition: 
*                                      
***************************************************************************/
{
   if(CurrentPreset.presetData.Microtip == TRUE)      
   {
	   CurrentPreset.presetData.Microtip = FALSE;
	   PresetDisplay=0;
	   SavePresetSelected(PresetDisplay);
	   SwitchScreen(SCREEN_RDY); //This line is to delete the preset number of the Ready screen.
   }
   else   
   {        
	   CurrentPreset.presetData.Microtip = TRUE;
      
      if(AmpMicrotipValid() == FALSE)
    	 CurrentPreset.presetData.Microtip = FALSE;  //HSAN shall the microtip be turned off?
      else
      {
         FixedIcons |= (UINT32)MICROTIP_I;
         BlinkIcons &= (UINT32)(~MICROTIP_I);
         PresetDisplay=0;
         SavePresetSelected(PresetDisplay);
         SwitchScreen(SCREEN_RDY);  //This line is to delete the preset number of the Ready screen.
      }        
   }
   TempPreset.presetData.Microtip = CurrentPreset.presetData.Microtip;
   EEPROM_ChangeParam(0,PRESET, MICROTIP);
}

void HandleHH_MICROTIP(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Handle the Microtip Function( Amplitude=<70%) and select appropriate Icon
*                                                                         
*   Entry condition:
*
*   Exit condition: 
*                                      
***************************************************************************/
{
   if(CurrentPreset.presetData.Microtip == FALSE)        
   {        
	   CurrentPreset.presetData.Microtip = TRUE;
      
      if(AmpMicrotipValid() == FALSE)
    	 CurrentPreset.presetData.Microtip = FALSE;  //HSAN shall the microtip be turned off?
      else
      {
         FixedIcons |= (UINT32)MICROTIP_I;
         BlinkIcons &= (UINT32)(~MICROTIP_I);
         PresetDisplay=0;
         SavePresetSelected(PresetDisplay);
      }        
   }
   TempPreset.presetData.Microtip = CurrentPreset.presetData.Microtip;
   EEPROM_ChangeParam(0,PRESET, MICROTIP);
}
void ProcessKeyboard(KEY_CODE keyCode)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Process keyboard buttons for menu navigation or upadte the values
*        Based on what screen is displayed.
*                                                                         
*   Entry condition:  
*
*        keyCode - a debounced valid key press
*        screen - the current screen
*
*   Exit condition: 
*
*        Executes button presses.
*                                      
***************************************************************************/
{

   static  UINT8  MicroTipReleased = FALSE;
   static UINT8 KeyReleased = FALSE;
   static UINT8 Toggle=TRUE;
   UINT16 Key = keyCode;
   UINT16 screen1 = screen;
   UINT8 WayFlag = 0;
   if (keyCode != NO_KEY){ 
      switch (screen1){
         case SCREEN_POWERUP:
            switch (keyCode) {
               case (KB_ENTER | KB_PRESET):
            	   CurrFnScreen = SWVERSION;
				   SwitchScreen(SCREEN_FNSCREEN);
				   break;               
               case KB_TEST:
                  CurrentTestCode = NO_KEY;
                  TestCode();
                  break;
               default:
            	   break;
            }
            break; 
         case SCREEN_TEST:
            CurrentTestCode = keyCode;
            break;           
         case SCREEN_RDY:
            OldScreen = SCREEN_RDY;
            switch (keyCode) {
				case KB_LEFT:
					if(currentLCDData.ValidResult == TRUE) 
					{
						ResultIndex = Results[currentLCDData.IDWeldMode].length-1 ;
						SwitchScreen(SCREEN_RESULTS);
					}
					break;
				case KB_RIGHT:
					if(currentLCDData.ValidResult == TRUE)
					{
						ResultIndex = 0;
						SwitchScreen(SCREEN_RESULTS);
					}
					break;
            }

            
            if(Conf.ConfigData.DispLock == OFF)
            {
               switch (keyCode) {
                  case KB_UP:
                  case KB_DOWN:
                  case KB_ENTER:
                	 TempPreset = CurrentPreset; 
                	 ShowDefaults=FALSE;
                	 IndexMenu = 0; 
                	 NaviMenuEnter();
                	 break;
                  
                  case KB_PRESET:
                	 TempPreset = CurrentPreset; 
                	 LoadPreset=TRUE;
                	 ShowDefaults=TRUE;
                	 Num_Current.NumericData.Value=1;
                	 PresetNo=1;
                     SwitchScreen(SCREEN_PRESET);
                     break;
                  case KB_MICROTIP:
                        if(MicroTipReleased == TRUE)
                        	HandleMICROTIP();
                        break;
                  case KB_ESC:
                     SwitchScreen(SCREEN_RDY);
                     break;
                  case (KB_ENTER | KB_PRESET):
                     CurrFnScreen = SWVERSION;
                     SwitchScreen(SCREEN_FNSCREEN);
                     break;
                  default:
                	  break;
               }
            }
            break;    
         case SCREEN_RESULTS:
            switch (keyCode) {
				case KB_RIGHT:
				case ( KB_START_STOP | KB_RIGHT):
					WayFlag = RIGHT;
               case KB_LEFT:
               case ( KB_START_STOP | KB_LEFT):
            	  ChangeResult(WayFlag);
            	  if (ResultIndex!= 0)
            		  SwitchScreen(SCREEN_RESULTS);
            	  DisplayResult();
            	  
                  break; 
           
               case KB_UP: 
            	   WayFlag = UP;                     
               case KB_DOWN:
                   if(Conf.ConfigData.DispLock == OFF)
                   {
                	   if (StopFlag==TRUE)
                	   {
                		   TempPreset = CurrentPreset; 
                		   ShowDefaults=FALSE;
                		   IndexMenu = 0; 
                		   NaviMenuEnter();
                	   }
                	   if((CurrentParam == D_AMPLITUDE) && (Conf.ConfigData.DispLock == OFF) && StopFlag==FALSE) 
                	   {
                		   UpdateParamVal(WayFlag);
                		   currentLCDData.AmplitudeValue = (UINT8)Num_Current.NumericData.Value;
                		   PerformAction();
                		   AmpUpdatedDuringcycleFlag = TRUE;
                	   }
                   }   
                   
                  break;
               case KB_ESC:
               case (KB_ESC | KB_START_STOP) :
            	   if (StopFlag==FALSE)
            	   {
            		   ResultIndex =0;
            		   DisplayResult();
            	   }
            	   else
            		   if (AlarmFlag==TRUE)
            			   SwitchScreen(SCREEN_ERROR);
            		   else
            			   SwitchScreen(SCREEN_RDY);
            	   break;
               case KB_ENTER:

            	   if (StopFlag==TRUE && Conf.ConfigData.DispLock == OFF )
            	   {
            		   TempPreset = CurrentPreset; 
            		   ShowDefaults=FALSE;
            		   IndexMenu = 0; 
            		   NaviMenuEnter();
            	   }
            	   break;
               case KB_PRESET:
            	   if (StopFlag==TRUE && Conf.ConfigData.DispLock == OFF)
            	   {
            		   TempPreset = CurrentPreset; 
            		   LoadPreset=TRUE;
            		   ShowDefaults=TRUE;
            		   Num_Current.NumericData.Value=1;
            		   PresetNo=1;
            		   SwitchScreen(SCREEN_PRESET);
            		   break;
            	   }
               case KB_MICROTIP:
            	   if (StopFlag==TRUE && Conf.ConfigData.DispLock == OFF)
            	   {
            		   if(MicroTipReleased == TRUE)
            			   HandleMICROTIP();
            	   }
                     break;
               default:
            	   break;
            }
            break;    
         case SCREEN_MODECONFIG:
            switch (keyCode) {
               case KB_LEFT:
               case KB_RIGHT:
            	   ContinuousMode = !ContinuousMode;
                  SwitchScreen(SCREEN_MODECONFIG);
                  break;            
               case KB_ENTER:
                  if (ContinuousMode == TRUE)
                	  TempPreset.presetData.WeldMode = (UINT16)(CONFIG_CONT); 
				  else
					  TempPreset.presetData.WeldMode = (UINT16)(CONFIG_PULSE);
                  VerifyTempVSCurret();
                  NaviMenuLeave(); 
                  break;
               case KB_ESC:
                  SwitchScreen(SCREEN_RDY);
                  break;
            }
         break;      
         case SCREEN_SONICCONF:
            switch (keyCode) {
               case KB_LEFT:
             	  if (( ModeSet == TIME_MODE) &&  (TempeProbePresent == FALSE ))
             		  ModeSet = ENERGY_MODE;
             	  else
             		 if (( ModeSet == TIME_MODE) &&  (TempeProbePresent == TRUE ))
             			 ModeSet = TEMPE_MODE; 
             		 else
             			ModeSet--; 
             	  SwitchScreen(SCREEN_SONICCONF); 
              	break;
               case KB_RIGHT: 
            	  if ((( ModeSet == ENERGY_MODE) &&  (TempeProbePresent == FALSE )) ||  ModeSet == TEMPE_MODE) 
            		  ModeSet = TIME_MODE;
            	  else
            	     ModeSet++;            	  
            	  SwitchScreen(SCREEN_SONICCONF); 
            	  break;   
               case KB_ENTER:
            	   if ( ModeSet == TIME_MODE )   
            		   TempPreset.presetData.WeldMode |= (UINT16)(CONFIG_TIME);
            	   else 
            		   if ( ModeSet == ENERGY_MODE ) 
            			   TempPreset.presetData.WeldMode |= (UINT16)(CONFIG_ENERGY);
            		   else
            			   TempPreset.presetData.WeldMode |= (UINT16)(CONFIG_TEMP);
            	   
            	   VerifyTempVSCurret();
            	   NaviMenuLeave();
            	   
            	   break;
               case KB_ESC:
            	   EscKeyPressed=TRUE;
            	   if (ShowDefaults==TRUE)
            		   TempPreset.presetData.WeldMode |= (CONFIG_TIME); 
            	   else
            		   TempPreset.presetData.WeldMode |= (CurrentPreset.presetData.WeldMode & (UINT16)CONFIG_TEMP);
            	   VerifyTempVSCurret();
            	   NaviMenuLeave();
            	 
                  SwitchScreen(SCREEN_RDY); 
                  break;
               default:
            	   break;
            }
         break;  
         case SCREEN_SONICCONF2:
        	 switch (keyCode) {
				 case KB_LEFT:
        	     case KB_RIGHT:
        	    	 ModeSet2 = !ModeSet2;
        	         SwitchScreen(SCREEN_SONICCONF2);
        	         break;            
        	     case KB_ENTER:
        	     	if (ModeSet2 == TIME_MODE)
        	     		TempPreset.presetData.WeldMode |=  (UINT16)(CONFIG_SUBTIME); 
        			else
        				TempPreset.presetData.WeldMode |= (UINT16)(CONFIG_SUBENERGY);
        	     	VerifyTempVSCurret();
        	     	NaviMenuLeave(); 
        	     	break;
        	     case KB_ESC:
              	   EscKeyPressed=TRUE;
              	   TempPreset.presetData.WeldMode |= (UINT16)(CONFIG_SUBTIME); 
              	   VerifyTempVSCurret();
              	   NaviMenuLeave();
              	   break;
        	 }
        	 
        	 break;
         case SCREEN_PARAMETER:
        	 OldScreen=SCREEN_PARAMETER;
            switch (keyCode) {
				case KB_RIGHT:
					WayFlag = RIGHT;
				case KB_LEFT:
            	   SelectDigitGroup(WayFlag);
            	   GetLimits();
            	   break; 
           
               case KB_UP:
            	   WayFlag = UP;                  
               case KB_DOWN:
            	   UpdateParamVal(WayFlag);
            	   break;
               case KB_ESC:
              	   EscKeyPressed=TRUE;
              	   AcceptValue();
              	   VerifyTempVSCurret();
              	   NaviMenuLeave();
              	   break;
                case KB_ENTER:
                	AcceptValue();
                	if (AlarmFlag==FALSE)
                	{
                		VerifyTempVSCurret();
                		NaviMenuLeave();
                	}
                	else
                		SwitchScreen(SCREEN_ERROR);
                	break;            
               default:
            	   break;
            }
            break;    
         case SCREEN_FNSCREEN:
        	 OldScreen=SCREEN_FNSCREEN;
            switch (Key) {
               case KB_ESC:  
            	   SwitchScreen(SCREEN_RDY);
            	   break;          
               case KB_UP:
            	   WayFlag = UP;                  
               case KB_DOWN:
            	   UpdateParamVal(WayFlag);
            	   break;
               case KB_ENTER:
            	   AcceptValue();
            	   if (AlarmFlag!=TRUE)
            		   SwitchScreen(SCREEN_EDITFNSCREEN);
            	   else
            		   SwitchScreen(SCREEN_ERROR);
            	   break;
               default:
            	   break;
            }
         break;    
         case SCREEN_EDITFNSCREEN:
        	 OldScreen = SCREEN_EDITFNSCREEN;
        	 switch (Key) { 
				case KB_RIGHT:
					WayFlag = RIGHT;
				case KB_LEFT:
					SelectDigitGroup(WayFlag);
					GetLimits();
					break; 
				case KB_UP:
					WayFlag = UP;
				case KB_DOWN:
            	   UpdateParamVal(WayFlag);
            	   break;
				case KB_ESC:
            	   SwitchScreen(SCREEN_FNSCREEN);
                break;
                case KB_ENTER:
                	
                	AcceptValue();
                	if (AlarmFlag==FALSE)
                	{
                		SwitchScreen(SCREEN_FNSCREEN);
                	}
                	else
                		SwitchScreen(SCREEN_ERROR);
                	break;            
               default:
            	   break;
            }
            break; 
         case SCREEN_ERROR:            
            switch (keyCode) {
               case KB_RESET:                  
                  if(CurrentAlarm == ER_OL)
                     SwitchScreen(SCREEN_RDY);
                  else {
                     DataEntryError = FALSE;
                     CurrentParam=AuxCurrentParam;
                     SwitchScreen(OldScreen);
                  }
                  ResetAlarm();
                  break;
            }
            break; 
         case SCREEN_PRESET:
            switch (keyCode){
               case KB_LEFT:
               case KB_RIGHT:
               case KB_PRESET:
            	   ShowDefaults=FALSE;
            	   LoadPreset = !LoadPreset;
                  SwitchScreen(SCREEN_PRESET);
                  break;
               case KB_ESC:
            	  CurrentPreset = TempPreset; 
                  SwitchScreen(SCREEN_RDY);
                  break;
               case KB_UP:
            	   WayFlag = UP;
               case KB_DOWN:
            	   UpdateParamVal(WayFlag);
            	   PresetNo = (UINT8)Num_Current.NumericData.Value;
            	   FixedIcons = 0;
            	   if (LoadPreset==TRUE)
            	   {
            		   EEPROM_Recall(PresetNo);
            		   WeldMode_Icon(CurrentPreset.presetData.WeldMode);
            	   }
            	   else
            		   WeldMode_Icon(TempPreset.presetData.WeldMode);
            	   FixedIcons |= (UINT32)(PRESET_I) ;    
            	   break; 
               case KB_ENTER:
            	   if (LoadPreset == TRUE)
            	   {
            		   EEPROM_Recall((UINT8)Num_Current.NumericData.Value);
            	   }
            	   else
            	   {
            		   CurrentPreset = TempPreset;
            		   EEPROM_SavePreset((UINT8)Num_Current.NumericData.Value);
            	   }
            	   PresetDisplay=(UINT8)Num_Current.NumericData.Value;
            	   SavePresetSelected(PresetDisplay);
            	   
            	   SetAmplitude((UINT8)Amplitude);
                  
                  SwitchScreen(SCREEN_RDY);
                  break;
            }
            break;
      }      
   } 
   
   else
   {
      MicroTipReleased = TRUE;
      KeyReleased = TRUE;
   }
}   
 
KEY_CODE GetKeys(void){
/***************************************************************************/
/*                                                                         */
/* This function combines key pressed with some digital inputs like seek,    */
/* Reset and Start with key pressed so that they can be handled as key press.*/
/*                                                                           */
/***************************************************************************/

   KEY_CODE AvailableKey = NO_KEY;    /* No keys are pressed or they have been masked off */
   KEY_CODE LocalKey = keys;          /* Copy global and invert logic       */
   static UINT8  PauseReleased = TRUE;
   
   LocalKey &= ~(BIT12 | BIT13 | BIT14);           // First clear digital inputs //
   
   if ((IOValue & EXT_RESET_IN) != EXT_RESET_IN){  // Active Low //
      ResetKey = TRUE;
      LocalKey |= KB_RESET;  
   } 
   else 
      ResetKey = FALSE;

   if ((IOValue & EXT_START_IN) != EXT_START_IN)	    // Active Low //
   {
      LocalKey |= EXTSTART;
   }

   if ((IOValue & PS_SEEK_ACT_IN) != PS_SEEK_ACT_IN)	 // Active Low // 
   {
      SeekActive = TRUE;   
   }
   else  
      SeekActive = FALSE;
   
   
   if (((IOValue & EXT_START_IN) != EXT_START_IN) && ((LocalKey & KB_TEST) == KB_TEST))    // Active Low //
	   Tst_N_ExtStart = TRUE;
   else
	   Tst_N_ExtStart = FALSE;
   
   if (HandHeld == FALSE)
   {
	   if((LocalKey & KB_PAUSE) == KB_PAUSE) {
		   if((screen == SCREEN_RESULTS)  &&(PauseReleased == TRUE))
		   {
			   PauseFlag = !PauseFlag;
		   }
		   PauseReleased = FALSE;
	   } 
	   else
		   PauseReleased = TRUE;
	   
   }
   
 
   LocalKey &= ~KB_PAUSE;
   
   if ((LocalKey & KeyMask) != 0)
      AvailableKey = (LocalKey & KeyMask); 
   return(AvailableKey);
}

      
      
