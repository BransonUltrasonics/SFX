/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/CRC.c_v   1.2.3.8.1.1   04 Aug 2016 15:29:44   hasanchez  $   
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

//This is the main module for CRC Calculation.
//Error-detecting code designed to detect accidental changes on the digital information.
//Particularly good at detecting common errors caused by noise in transmission channels. 
//When a codeword is read [received], the digital equipment either compares its check value
//with one freshly calculated from a data block [probably the 256 element lookup table in the 
//program], performs a CRC on the whole codeword and compares the resulting check value with 
//an expected residue constant. If the check values do not match, then the block contains a 
//data error and the device may take corrective action such as rereading or requesting the block
//be sent again, otherwise the data is assumed to be error-free.

//Module name: CRC

//Filename:    CRC.c
/**************************REVISIONS******************************************************
$Log::   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources$   
 * 
 *    Rev 1.2.3.8.1.1   04 Aug 2016 15:29:44   hasanchez
 * Updated program lenght
 * 
 *    Rev 1.2.3.8.1.0   03 Aug 2016 15:12:50   hasanchez
 * Updated program lenght
 * 
 *    Rev 1.2.3.8   22 Jul 2016 13:58:16   hasanchez
 * Updated the Lenght of the program.
 * 
 *    Rev 1.2.3.7   14 Jul 2016 12:30:30   hasanchez
 * Update the CRC Length
 * 
 *    Rev 1.2.3.6   13 Jun 2016 12:55:00   hasanchez
 * Program length updated. After test function the screen will be locked up.
 * 
 *    Rev 1.2.3.5   18 May 2016 12:55:04   hasanchez
 * Changed the size of program segment.
 * 
 *    Rev 1.2.3.4   13 May 2016 13:41:36   hasanchez
 * Changed the size of program segment.
 * 
 *    Rev 1.2.3.3   13 May 2016 13:36:34   hasanchez
 * Changed the size of program segment.
 * 
 *    Rev 1.2.3.2   29 Jan 2016 16:17:30   hasanchez
 * CRC updated
 * 
 *    Rev 1.0   06 Aug 2012 05:30:20   akaushal
 * Initial revision.
 * 
 *    Rev 1.1   21 Jun 2012 07:30:40   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.0   22 May 2012 11:59:28   ygupta
 * Initial revision.

*******************************************************************************************/
/*---------------------------INCLUDES-----------------------------------------------*/

#include "CRC.h"
#include "MC9S08LG32.h"
#include "print.h"                         /*To include the masks (LOBYTEMASK, HIBYTEMASK..)*/
#include "util.h"   
#include "onems.h"
#include "LCD.h"

#define MILLISEC_IN_TENSECOND 10000L


/* to fix the CRC according the cyclone pro algorithm, the last 
 * address on the rom is needed, you can find it on the File LPXProject.map 
 * after compiling the code you will always need to update the value of the
 * constant PROGRAMFLASH_LENGTH this is a substraction of  
 * "the end of the Rom" minus "0x8000h" plus one (the start of the programming code). */

#define PROGRAMFLSH_STARTADDR    	0x8000
#define PROGRAMFLASH_LENGTH      	0x7B97

/* Flash Memory Second Block */
#define Second_Block_Start_Address  0xFFBD
#define Second_Block_Lengh          0x1

/* Flash Memory Third Block */
#define Third_Block_Start_Address 	0xFFBF
#define Third_Block_Lengh        	0x1

/* Flash Memory Fourth Block */
#define Fourth_Block_Start_Address	0xFFCA
#define Fourth_Block_Lengh          0x36


UINT8 CRCLowByte;
UINT8 CRCMidByte;
UINT8 CRCHighByte;


const unsigned char CRCTableCyclone[256] = 
  {0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
   0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
   0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
   0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
   0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
   0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
   0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
   0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
   0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
   0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
   0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
   0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
   0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
   0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
   0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
   0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3};
  

/***************************************************************************
 *  This function will verify with the CRCTableCyclone and get the
 * appropiate number to return the CRC value of a set of information 
 * same way that Cyclone pro they use the CRC on 8 bits.
 * 
 * 
 ***************************************************************************/

UINT8 CRC(UINT8 *Ptr, UINT32 Length, BOOLEAN SecureByte, UINT8 CRC8) 
{
	UINT8 Index1 = 0;
   while (Length-- >0) {
      if (SecureByte == TRUE)
    	  Index1 = (UINT8)(CRC8 ^ 0x7E & LOBYTEMASK);
      else
	      Index1 = (UINT8)(CRC8 ^ *Ptr++ & LOBYTEMASK);
      
      CRC8 = CRCTableCyclone[Index1];
      FeedTheDog();   
   }
   return(CRC8);
}

UINT16 Ck16(UINT8 *Ptr, UINT32 Length, BOOLEAN SecureByte)
/***************************************************************************                                                                       
*   Purpose:
*        Calculate the Checksum 16bits same way that Cyclone pro does 
*                                   
*   Entry condition:  
*   Exit condition:                               
***************************************************************************/
{
	UINT16 Checksum16 = 0;	 
	while (Length-- > 0) {
		if (SecureByte== TRUE )
			Checksum16 += (UINT16)0x7E;   	  
		else
			Checksum16 += (UINT16)*Ptr++;
		FeedTheDog();   
	}
	return (Checksum16);
}

/***************************************************************************                                                                       
*   Purpose:
*        Integrat the CRc and Ck16 on a single variable. 
*                                                      
***************************************************************************/
UINT32 CRC_Ck(UINT8 *Ptr, UINT32 Length)
{
	UINT32 FlashCRC = 0 ;
	UINT32 FlashCk16 = 0;
	FlashCRC = (UINT32)CRC((UINT8 *)Ptr, Length, FALSE , 0 );
	FlashCk16 = (UINT32)Ck16((UINT8 *)Ptr, Length, FALSE );
	FlashCk16 = FlashCk16 << 8;
	FlashCRC = FlashCk16 + FlashCRC;
	
	return (FlashCRC);
}

/***************************************************************************                                                                       
*   Purpose:
*        Integrat the CRc and Ck16 on a single variable of  the 
*        Flash memory
*                                                      
***************************************************************************/
void Calc_Flash_CRC_Ck(void)
{
	UINT32 FlashCRC = 0 ;
	UINT32 FlashCk16 = 0;
	
	FlashCRC = (UINT32)CRC((UINT8 *)PROGRAMFLSH_STARTADDR, PROGRAMFLASH_LENGTH, FALSE, FlashCRC);
	FlashCRC = (UINT32)CRC((UINT8 *)Second_Block_Start_Address, Second_Block_Lengh, FALSE, FlashCRC);
	FlashCRC = (UINT32)CRC((UINT8 *)Third_Block_Start_Address, Third_Block_Lengh, TRUE, FlashCRC);
	FlashCRC = (UINT32)CRC((UINT8 *)Fourth_Block_Start_Address, Fourth_Block_Lengh, FALSE, FlashCRC);
	
	FlashCk16 += (UINT32)Ck16((UINT8 *)PROGRAMFLSH_STARTADDR, PROGRAMFLASH_LENGTH, FALSE );
	FlashCk16 += (UINT32)Ck16((UINT8 *)Second_Block_Start_Address, Second_Block_Lengh, FALSE );
	FlashCk16 += (UINT32)Ck16((UINT8 *)Third_Block_Start_Address, Third_Block_Lengh, TRUE );
	FlashCk16 += (UINT32)Ck16((UINT8 *)Fourth_Block_Start_Address, Fourth_Block_Lengh, FALSE );
	
	FlashCk16 = FlashCk16 << ONE_BYTE;
    FlashCRC = FlashCk16 + FlashCRC;
     
    CRCLowByte = (FlashCRC & LOBYTEMASK);
    
    FlashCRC =FlashCRC >> ONE_BYTE;
    CRCMidByte = (FlashCRC & LOBYTEMASK);
    
    FlashCRC =FlashCRC >> ONE_BYTE;
    CRCHighByte = (FlashCRC & LOBYTEMASK);
	
	
}

void TestCode() 

/****************************************************************************/
/*                                                                          */
/*                                                                          */
/****************************************************************************/
{
	#define CRCSTRLENGTH  6                       /*Length of the CRC string*/

	char FullCrcString[CRCSTRLENGTH];
	UINT8 x;

	FullCrcString[4]=(CRCHighByte & LONIBBLEMASK);  /*changes to make it more readable*/
	FullCrcString[5]=(CRCHighByte>>HALF_BYTE)& LONIBBLEMASK;
   
	FullCrcString[2]=(CRCMidByte & LONIBBLEMASK);
	FullCrcString[3]=(CRCMidByte>>HALF_BYTE)& LONIBBLEMASK;
   
	FullCrcString[0]=(CRCLowByte & LONIBBLEMASK);
	FullCrcString[1]=(CRCLowByte>>HALF_BYTE)& LONIBBLEMASK;
   
	for (x=0 ; x < NUM_7SEGMENT_DIGITS ; x++ )	{          //Getting ASCII of characters
		if ( FullCrcString[x] < 0x0A )
			FullCrcString[x]=FullCrcString[x]+'0'; //if smaller than 10 then move to the '0' character
    	else
    		FullCrcString[x]=(FullCrcString[x]- 0x0A) +'A';//if bigger than 10 then move to the 'A' character but 
  												   //first remove 10 units.
   }
   
    /*********START Show all seconds all segments 10 seconds*********/
         
	for (x = 0; x < TOTALLCDSEGMENTS; x++)                      // clear all icons
		TurnONorOFFLCDSegment( x, (BOOLEAN)ON); 
	
	StartMSTimer (TIMER1, MILLISEC_IN_TENSECOND);
	
	while (FlagTimer1 == TRUE)     /* Wait on time             */
		FeedTheDog();   
	
    /*********END Show all seconds all segments 10 seconds*********/
    
       
     ALLIconsOFF();   
     
     Strcpy(Str_CRC, FullCrcString);
     SwitchScreen(SCREEN_CRC);        // Call the the screen that shows the CRC
     UpdateLive();
     
 	while (FOREVER)     /* Wait on time             */
 		FeedTheDog();   //System locked here until the system is turned off.
     
}

