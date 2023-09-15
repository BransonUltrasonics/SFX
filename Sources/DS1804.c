/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/DS1804.c_v   1.4.1.2.3.0   15 Dec 2015 16:15:02   hasanchez  $
******************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------------- MODULE DESCRIPTION ---------------------------

This module deals with Digital Potentiometer connected to MC9S08LG32.
Sets the Digital Potentiometer to a default value at startup.

Module name: DIGITAL POTENTIOMETER

Filename:    DS1804.c
-------------------------------- TECHNICAL NOTES -------------------------------

This code contains the module for digital potentiometer initialization and  control.

------------------------------------ REVISIONS ---------------------------------

* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/DS1804.c_v  $
 * 
 *    Rev 1.4.1.2.3.0   15 Dec 2015 16:15:02   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.4.1.0   25 Aug 2015 11:34:06   hasanchez
 * Digital Pot functions commented
 * 
 *    Rev 1.4   10 Sep 2013 03:20:08   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.3   06 Nov 2012 08:43:20   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.2   16 Aug 2012 01:57:26   akaushal
 * SFX Code With External Crystal Oscillator and HW Changes
 * 
 *    Rev 1.1   10 Aug 2012 02:56:04   akaushal
 * Digipot adjust on correcting PSFREQ
 * 
 *    Rev 1.0   06 Aug 2012 05:30:28   akaushal
 * Initial revision.
 * 
 *    Rev 1.3   06 Jul 2012 10:02:32   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.2   21 Jun 2012 07:30:56   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:44:26   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 11:59:42   ygupta
 * Initial revision.

--------------------------------------------------------------------------------*/

#include "DS1804.h"
#include "lcd.h"

//SINT8 CurrDigiPot = (SINT8)MID_DIGIPOT;
//static void ChangePotVal(UINT8,UINT8);
static void Delay (void);

void Init_Port_DS1804(void) 
/***************************************************************************
* Function Name: Init_Port_DS1804
*
* Return Type: void.
*
* Description:
*		Initialization of the Lines that are connected to the DS1804 IC                                                  
***************************************************************************/
{
   DDIR = 1;		//Data Direction as Output (1)
   DCS  = 1;			
   DINC = 1;
   Delay();
   CS   = 1;		// Chip Select should be High ( Chip is not Selected)
}

void Write_DS_EEPROM(void)
/***************************************************************************
* Function Name: Write_DS_EEPROM
*
* Return Type: void.
*
* Description:
*		write the value to Non Volatile memory location of DS1804 IC
*		This enables the DS1804 to startup at the previously set value.                                                 
***************************************************************************/
{
   INC = 1;
   asm("nop; nop;");
   CS = 0;
   Delay();
   CS = 1;
}

	


void ColdStart_DS1804(void)
/***************************************************************************
* Function Name: ColdStart_DS1804
*
* Return Type: void.
*
* Description:
*		 Re-initialize the digital potentiometer to center value(50)                                                  
***************************************************************************/
{
   //Conf.ConfigData.DigiPot = 0;
   //CenterDigiPot();	
   CurrDigiPot = MID_DIGIPOT;
   Write_DS_EEPROM();
}


void increment(UINT8 Count)
/***************************************************************************
* Function Name: increment
*
* Arguments:
* 	8bit Step count to shift up the Wiper in DS1804
*
* Return Type: void.
*
* Description:
*        	Pin out to DS1804 is toggled to change its Output                                                  
***************************************************************************/
{
   DIR	= 1;
   CS	= 0;
   asm("nop; nop;");
   INC 	= 0;
   while(Count)
   {
      INC	= 1;
      Delay();
      INC	= 0;
      Delay();

      Count--;
   }
   CS	= 1;
}

void decrement(UINT8 Count)
/***************************************************************************
* Function Name: decrement
*
* Arguments:
* 	8bit step count to shift down the Wiper in DS1804
*
* Return Type: void.
*
* Description:
*        	Pin out to DS1804 is toggled to change its Output                                                  
***************************************************************************/
{
   DIR	= 0;
   CS	= 0;
   asm("nop; nop;");
   INC 	= 0;
   while(Count)
   {
      INC	= 1;
      Delay();
      INC	= 0;
      Delay();

      Count--;
   }
   CS	= 1;
}

static void Delay(void)
/***************************************************************************
* Function Name: delay
*
* Arguments:
*
* Return Type: 
*
* Description:
* 	Produces a Delay of 10 clock-cycle/T-cycle
*  There is a requirement ofINC inactive to CS Inactive min 500uS
*  and the Clock period is around 50nS... thus 10 cycle gap is needed
***************************************************************************/
{
   #asm
      nop;
      nop;
      nop;
      nop;
      nop;
      nop;
      nop;
      nop;
      nop;
      nop;
   #endasm
}
