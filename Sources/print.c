/*
$Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/print.c_v   1.5.3.0   15 Dec 2015 16:15:04   hasanchez  $
*/
/****************************************************************************/
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 2010-11              */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------*/

//This is the main module for controlling Serial.
//This is the main module for controlling Serial. It has a routine for sending/printing 
//data string over UART, configure baud rate, enabling TX and RX, etc.

//Module name: PRINT

//Filename:    print.c
/**************************REVISIONS******************************************************
$Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/print.c_v  $
 * 
 *    Rev 1.5.3.0   15 Dec 2015 16:15:04   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.3   10 Sep 2013 03:18:14   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.2   06 Nov 2012 08:45:06   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   16 Aug 2012 01:58:32   akaushal
 * SFX Code With External Crystal Oscillator and HW Changes
 * 
 *    Rev 1.0   06 Aug 2012 05:31:18   akaushal
 * Initial revision.
 * 
 *    Rev 1.1   12 Jun 2012 09:47:42   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:01:00   ygupta
 * Initial revision.
******************************************************************************************/

/*--------------------------------------------------------------------------*/
  
#include "print.h"
#include "MC9S08LG32.h"


/* Internal Functions */

void SCI_Init(void)
/****************************************************************************
* Function Name: SCI_Init
*
* Agruments:
* NONE
*
* Return Type: VOID
*
* Description:
*    It initializes the UART port for prints to be used.
*
****************************************************************************/
{
   SCGC1_SCI1 =1;    //enable clock
   PINPS4_TX1 = 0;   //configure PIN MUXING according to the board
   PINPS4_RX1 = 0;
   SCI1BDH = 0;
   SCI1BDL = 0x09;     //configure baud rate 8.3MHz/16/7 = 58123 best equal to 57600
   SCI1C1  = 0;
   SCI1C2  = 0x28;   //enable TX 
}


#ifdef DEBUG_ENABLE
void TERMIO_PutChar(SINT8   character) 
/****************************************************************************
* Function Name: TERMIO_PutChar
*
* Agruments:
* 1. character: The character to be sent to UART
*
* Return Type: VOID
*
* Description:
*    It is the low level routine for printing a character on UART
*
****************************************************************************/
{
   SINT8 dummy;

   while(!SCI1S1_TDRE);        //Check the status
   dummy = SCI1S1;
   SCI1D  = character;         //write the character to be Txd
}



void PrintStr(SINT8* string)
/****************************************************************************
* Function Name: PrintStr
*
* Agruments:
* 1. character: The string to be printed
*
* Return Type: VOID
*
* Description:
*    It is the routine for printing on UART, a NULL terminated string
*
****************************************************************************/
{
#ifdef DEBUG_ENABLE
   SINT16 i=0; 
   while(string[i]) {
      TERMIO_PutChar(string[i]);      //print the string character by character
      i++;
   }
#endif   
 
}

void PrintHex8(UINT8 character)
/****************************************************************************
* Function Name: PrintHex8
*
* Agruments:
* 1. character: The character to be printed
*
* Return Type: VOID
*
* Description:
*    It is the routine for printing characters in HEX format
*
****************************************************************************/
{
   UINT8 c1= (0xF0&character);     //take upper nibble of character to be printed

   // convert and print the upper nibble to HEX character
   c1=c1>>4;
   if (c1>9)
   {
      c1 = c1 - 10 + 'A' ;
   }
   else
   {
      c1 = c1 + '0';
   }
   TERMIO_PutChar(c1);     //print the half character

   c1= (0x0F&character);   // take lower nibble of character to be printed
   // convert and print the lower nibble to HEX character
   if (c1>9)
   {
      c1 = c1 - 10 + 'A' ;
   }
   else
   {
      c1 = c1 + '0';
   }
   TERMIO_PutChar(c1);   //print the remaining character
}

void Print32(UINT32 number)
/****************************************************************************
* Function Name: Print16
*
* Agruments:
* 1. character: The number to be printed
*
* Return Type: VOID
*
* Description:
*    It is the routine for printing on UART, the 2byte value
*
****************************************************************************/
{     
#ifdef DEBUG_ENABLE
   TERMIO_PutChar('0');      //for HEX display print "0x" before the value
   TERMIO_PutChar('x');
   PrintHex8((UINT8)((0xFF000000 & number)>>24)); //print the MSB 1 Byte
   PrintHex8((UINT8)((0x00FF0000 & number)>>16)); //print the MSB 1 Byte
   PrintHex8((UINT8)((0x0000FF00 & number)>>8));  //print the MSB 1 Byte
   PrintHex8((UINT8)(0x00FF & number));           //print the LSB 1 Byte
#endif
}

void Print8(UINT8 character)
/****************************************************************************
* Function Name: Print8
*
* Agruments:
* 1. character: The character to be printed
*
* Return Type: VOID
*
* Description:
*    It is the routine for printing on UART, the 1byte value
*
****************************************************************************/
{
#ifdef DEBUG_ENABLE
   TERMIO_PutChar('0');      //for HEX display print "0x" before the value
   TERMIO_PutChar('x');
   PrintHex8(character);     //print the character afterwards
#endif
}

void Print16(UINT16 number)
/****************************************************************************
* Function Name: Print16
*
* Agruments:
* 1. character: The number to be printed
*
* Return Type: VOID
*
* Description:
*    It is the routine for printing on UART, the 2byte value
*
****************************************************************************/
{
#ifdef DEBUG_ENABLE
   TERMIO_PutChar('0');      //for HEX display print "0x" before the value
   TERMIO_PutChar('x');
   PrintHex8((UINT8)((HIBYTEMASK & number)>>8)); //print the MSB 1 Byte
   PrintHex8((UINT8)(LOBYTEMASK & number));      //print the LSB 1 Byte
#endif      
}
#endif
