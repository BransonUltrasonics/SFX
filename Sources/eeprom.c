/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/eeprom.c_v   1.3.1.3.3.0   15 Dec 2015 16:15:02   hasanchez  $
********************************************************************************* 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------------- MODULE DESCRIPTION ---------------------------

This is the main module for controlling the i2c and EEPROM.

Module name: EEPROM

Filename:    eeprom.c
--------------------------------- TECHNICAL NOTES -------------------------------

This code controls EEPROM and Data read and Write Modules to read Byte by Byte data on to 
the EEPROM over I2C 


------------------------------------- REVISIONS ---------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/eeprom.c_v  $
 * 
 *    Rev 1.3.1.3.3.0   15 Dec 2015 16:15:02   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.3.1.0   25 Aug 2015 11:36:30   hasanchez
 * Update on the Function EEPROM_Restore related to the coldstart and the new preset structure.
 * 
 *    Rev 1.3   10 Sep 2013 03:19:56   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.2   13 Jun 2013 07:53:48   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.1   06 Nov 2012 08:43:32   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.0   06 Aug 2012 05:30:36   akaushal
 * Initial revision.
 * 
 *    Rev 1.4   31 Jul 2012 03:58:48   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.3   27 Jun 2012 07:48:12   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:31:22   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:44:40   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 11:59:54   ygupta
 * Initial revision.
 
---------------------------------------------------------------------------------*/

#include "eeprom.h"

// Macros
#define WriteEE(a, d)   WriteI2C(EEPROM_ADDRESS, a, d, TRUE)					// WEF these were both passing TRUE for a long address instead of false
#define ReadEE(a)       ((UINT8) (ReadI2C(EEPROM_ADDRESS, a, TRUE) & 0xff))	// WEF these were both passing TRUE for a long address instead of false

#define I2CLowScl()     {EE_SCL = 0; EE_SCL_DIR = 1;}
#define I2CHighScl()    {EE_SCL_DIR = 0;}
#define I2CLowSda()     {EE_SDA = 0; EE_SDA_DIR = 1; }
#define I2CHighSda()    {EE_SDA_DIR = 0;}

enum TYPE Type_Var;

//Variables stored in EEPROM
EE_PRESET CurrentPreset, TempPreset;				//holds in-memory Current Preset data
EE_CONFIG_DATA Conf;                //holds in-memory Configuration Data 

//Functions
static void I2CStart(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to send a start sequence for an I2C command. 
*
*   Entry condition:                                                      
* 
*   Exit condition:                                                      
*                                                                         
***************************************************************************/
{
   I2CLowScl();
   I2CHighSda();
   I2CHighScl();
   I2CLowSda();
   I2CLowScl();
}


static void I2CStop(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to send a stop sequence for an I2C command.  
*
*   Entry condition:                                                      
* 
*   Exit condition:                                                      
*                                                                           
***************************************************************************/
{
   I2CLowScl();
   I2CLowSda();
   I2CHighScl();
   I2CHighSda();                             // bring SDA high while SCL is high
}



static UINT8 I2CNack(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Performs the action required of nack.
*
*   Entry condition:                                                      
*        
*   Exit condition:                                                      
*
*        Returns if there is a nack or not 
*                                                                        
***************************************************************************/
{
   UINT8 Ret;

   I2CHighSda();
   I2CHighScl();
   if (EE_SDA == 1){                         // no ack - error!
      Ret = FAIL;
   }
   else{
      Ret = PASS;
   }

   I2CLowScl();                                                                                  
   return (Ret);		
}	


static void I2COutByte(UINT8 O_byte)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Method for shifting data onto the I2C bus.
*
*   Entry condition:                                                      
*        
*        Byte to be sent out over I2c.                                                     
* 
*   Exit condition:                                                      
*  
***************************************************************************/
{
   UINT8 n;

   for (n=0; n<ONE_BYTE; n++) {
      if (O_byte & 0x80) {                   // Get the MSB
         I2CHighSda();                       // Set the data pin accordingly
      }
      else {
         I2CLowSda();
      }

      I2CHighScl();                          // clock it out
      I2CLowScl();
      O_byte = O_byte << 1;                  // Shift the byte and do it again, 8 times.
   }

   I2CHighSda();                             // set the data pin high
}   


static UINT8 I2CInByte(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Method for reading a reply on I2C bus.
*
*   Entry condition:                                                        
*
*   Exit condition:                                                      
*
*        Returns a byte of data that is read in.
*                                                                        
***************************************************************************/
{
   UINT8 I_byte, n;

   I2CHighSda();                             // Set SDA high
   for ( n=0; n<ONE_BYTE; n++ ) {
      I2CHighScl();                          // Set SCL high

      if ( EE_SDA ) {                        // If the Data pin  from the EERPOM is high, shift in a 1
         I_byte = (I_byte << 1) | 0x01;         // msbit first
      }
      else {
         I_byte = I_byte << 1;               // Otherwise shift in a 0.
      }
      I2CLowScl();                           // Clock the clock line (which is a good thing to do with it)
   }
   return(I_byte);                           // Return the information received.
}


static UINT16 WriteI2C(  UINT8 Dev_Adr, 
                        UINT16 Mem_Adr, 
                        UINT8 Dat, 
                        UINT8 Long_Mem_Addr)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Writes a byte to the requested I2C Address and  memory location.
*                                                                         
*   Entry condition:                                                      
*
*        mem_adr - the address in the device to write to. 
*        dat - the data to write there.
*        dev_adr - the address of the device to write to 
*        long_mem_addr - whether it is 8 bit addressing or 16 bit
*
*   Exit condition:
*
*        Returns if function passes or fails.   
*                                                                         
***************************************************************************/
{
   UINT8 DeviceBusy = FAIL;

   UINT8 I2CFailsafeTimer = I2C_FAILSAFE_TIME;

   while(DeviceBusy == FAIL){
      I2CStart();															   // Send out the start bit
      I2COutByte(Dev_Adr);												   // the control byte...
      DeviceBusy = I2CNack();												   // wait for the ack/nack.  If the device is busy, it won't ack.
      if(I2CFailsafeTimer-- == 0){
         return(I2C_FAILED_WRITE);
      }
   }

   if(Long_Mem_Addr == TRUE){
      I2COutByte((Mem_Adr >> ONE_BYTE) & 0xff);								   	   // high byte of memory address
      DataHandle = (UINT16)I2CNack();
   }

   I2COutByte(Mem_Adr & 0xff);											   // low byteof the address.
   DataHandle = (UINT16)I2CNack();																   // Wait for ack/nack

   I2COutByte(Dat);														   // and finally the data
   DataHandle = (UINT16)I2CNack();																   // Wait for ack/nack

   I2CStop();																   // Send stop

  StartMSTimer(TIMER1,I2C_DELAY_ENABLED);
   while (FlagTimer1 == TRUE) //wait here
      FeedTheDog();  	
   return(I2C_PASSED_WRITE);												 // Return what we got.	
}



static UINT16 ReadI2C(   UINT8 Dev_Adr, 
                        UINT16 Mem_Adr, 
                        UINT8 Long_Mem_Addr)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        This will read and return the byte at the requested location.
*                                                                         
*   Entry condition:                                                      
*
*        mem-adr - where to read, dev_addr the device to accessmem_adr - the 
*                  address in the device to write to. 
*        long_mem_addr - whether it is 8 bit addressing or 16 bitdat - the data
*                        to write there.
*
*   Exit condition:
*
*        The data at the requested location.
*                                                                         
***************************************************************************/
{
   UINT8  y;
   UINT8 DeviceBusy = FAIL;
   UINT8 I2CFailsafeTimer = I2C_FAILSAFE_TIME;


   while(DeviceBusy == FAIL){
      I2CStart();                            // Send out the start bit
      I2COutByte(Dev_Adr);              // the control byte...
      DeviceBusy = I2CNack();                // wait for the ack/nack.  If the device is busy, it won't ack.
      if(I2CFailsafeTimer-- == 0){
         return(I2C_FAILED_READ);
      }
   }

   if(Long_Mem_Addr == TRUE){
      I2COutByte((Mem_Adr >> ONE_BYTE) & 0xff);
      DataHandle = (UINT16)I2CNack();
   }

   I2COutByte(Mem_Adr & 0xff);											   // Send the rest of the address..
   DataHandle = (UINT16)I2CNack();																   // Wait for an ack/nack

   I2CStart();															   // no intermediate stop, just send out start again...
   I2COutByte((Dev_Adr) | 1); 	  									   // out with the control byte

   DataHandle = (UINT16)I2CNack();																   // Do the ack/Nack thing

   y = I2CInByte();														   // Finally, get the data...

   I2CStop();																   // Send the stop bit

   return(I2C_PASSED_READ | (UINT16) y);        // Return what we got.
}

static UINT8 CheckValidity(UINT16 CurrentAdr,UINT8 Size, enum TYPE Type1)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to Check the Validity of the CRC.
*        Compares CRC for Read Preset or Config with th e Saved CRC
*
*   Entry condition:                                                      
*
*   Exit condition: Returns the Validity as TRUE or FALSE
*                                                                 
***************************************************************************/
{   
   UINT32 Crc_Eeprom,Crc_Result;
   UINT8 Ret;

   Crc_Eeprom = ReadEE(CurrentAdr + Size);
   Crc_Eeprom = Crc_Eeprom<<ONE_BYTE;
   Crc_Eeprom |= ReadEE(CurrentAdr + Size+1);
   Crc_Eeprom = Crc_Eeprom<<ONE_BYTE;
   Crc_Eeprom |= ReadEE(CurrentAdr + Size+2);
   
   if(Type1 == PRESET) 
      Crc_Result = CRC_Ck(&CurrentPreset.dat[0], (UINT32)Size); 
   else
      Crc_Result = CRC_Ck(&Conf.cdat[0], (UINT32)Size);

   if(Crc_Eeprom  == Crc_Result)  {
#ifdef DEBUG_ENABLE     
      PrintStr("   Valid CRC ");
      Ret = TRUE;
#endif
   }
   else  {
      //PrintStr("   InValid CRC ");
      Ret = FALSE;  
   }
   return Ret;
}

void ResetEEPROM(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to reset values. Used for debug. Writes over magic numbers and
*        re-writes the defualt values.
*
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/
{
   DataHandle = (UINT16)WriteEE(EE_MAGIC1, 0xFF);                 //invalidate Magic Numbers
   DataHandle = (UINT16) WriteEE(EE_MAGIC2, 0xFF);
#ifdef DEBUG_ENABLE     
   PrintStr("\n\rEEPROM RESET");
#endif
   EEPROM_Restore();
}



void EEPROM_Restore(void) 
/***************************************************************************
*                                                                         
*   Purpose:
*
*        This function reinitialises all the Presets and Config Area to Defaults
*        Backup Area is also reinitialised.
*
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/
{
	UINT8 n;
	//UINT8 loc_val = CurrentPreset.presetData.PresetStored;     
	
	for(n=TIME;n<=WELDMODE;n++)//Read the Default values for Preset only
		SetVar((UINT8*)Format[n].Pointercurrent , (UINT32)Parameters[n].Default ,(UINT8)Parameters[n].VarSize);  
	
	EEPROM_SavePreset(0);
	
	for(n=PANELTRIG;n<=PASSWORD;n++)//Read the Default values for Preset & Config
		SetVar((UINT8*)Format[n].Ptr , (UINT32)Parameters[n].Default ,(UINT8)Parameters[n].VarSize); 
	
	if (Conf.ConfigData.Region == TRUE)  //Here we can set an setup to different regions.
		Conf.ConfigData.Tempe_C_F = FALSE; //Set Temp  to Celsius
	
	EEPROM_SaveConfig();
	 
	
	if((Parameters[CurrentParam].Format) == FACTCOLDSTART  ){ 
		for (n = 0;n< MAX_SAVED_PRESETS+1;n++)     
			EEPROM_SavePreset(n);
		(void)WriteEE(EE_MAGIC1, MAGIC_NUM1);                           
		(void)WriteEE(EE_MAGIC2, MAGIC_NUM2); 
		
	}
	
}



void EEPROM_SavePreset(UINT8 Presetno)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to update all fields in Preset structure in EEPROM.  
*        Storage location is deduced from the Preset Number. 
*        Save a Copy to BACKUP AREA
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  
{
   UINT8 n = 0;
   UINT16 CurrentAdr = 0;
   UINT16 CurrentBackupAdr = 0;
   UINT32 Crc_Result = 0;
#ifdef DEBUG_ENABLE     
   PrintStr("\n\r PresetNo to Write = ");
   Print8(Presetno);
#endif
   //Generate the current address 
   CurrentAdr = EE_START + ((PRESET_MAXSIZE+CRC_OFFSET)*Presetno);

   //calculate CRC
   Crc_Result = CRC_Ck(&CurrentPreset.dat[0], (UINT32)PRESET_MAXSIZE);
#ifdef DEBUG_ENABLE     
   PrintStr("\n\rWRITE PRESET ");
   Print16(CurrentAdr);
#endif
   // Write the Preset structure 
   for(n=0; n<PRESET_MAXSIZE; n++)
      DataHandle = (UINT16)WriteEE(CurrentAdr+n,(UINT8) (CurrentPreset.dat[n])); 
      
   //Write the CRC  
   WriteCRC2EEPROM ((CurrentAdr+PRESET_MAXSIZE), Crc_Result );
   FeedTheDog();
   
   //Generate the current address
   CurrentBackupAdr = EE_BACKUP_START + ((PRESET_MAXSIZE+CRC_OFFSET)*Presetno);
#ifdef DEBUG_ENABLE     
   PrintStr("\n\rWRITE PRESET TO backup");
#endif
   // Write the Preset structure to BACKUP AREA
   for(n=0; n<PRESET_MAXSIZE; n++)
	   DataHandle = (UINT16)WriteEE(CurrentBackupAdr+n,(UINT8) (CurrentPreset.dat[n])); 

   //Write CRC to Backup Area  
   
   WriteCRC2EEPROM ((CurrentBackupAdr+PRESET_MAXSIZE), Crc_Result );

   FeedTheDog();  

}

void EEPROM_SaveConfig(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to update all fields in Configuration structure in EEPROM.  
*        Save a Copy to BACKUP AREA
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  
{
   UINT8 n = 0;
   UINT32 Crc_Result = 0;


   // Calculate CRC  
   Crc_Result = CRC_Ck(&Conf.cdat[0], (UINT32)CONFIG_MAXSIZE);
#ifdef DEBUG_ENABLE     
   PrintStr("\n\rWRITE CONFIG ");
#endif
   // write the Config data
   for(n=0; n<CONFIG_MAXSIZE; n++)
      DataHandle = (UINT16)WriteEE(EE_CONFIGURATION_LOC+n,(UINT8)(Conf.cdat[n]));        

   // write the CRC  
   
   WriteCRC2EEPROM ((EE_CONFIGURATION_LOC+CONFIG_MAXSIZE), Crc_Result );

   FeedTheDog(); 
#ifdef DEBUG_ENABLE     
   PrintStr("\n\rWRITE CONFIG To BACKUP");
#endif
   //Write the Configuration structure To BACKUP AREA
   for(n=0; n<CONFIG_MAXSIZE; n++)
	   DataHandle = (UINT16)WriteEE(EE_BACKUP_CONFIGURATION_LOC+n,(UINT8)(Conf.cdat[n]));      

   // write the CRC To BACKUP AREA 
   
   
   WriteCRC2EEPROM ((EE_BACKUP_CONFIGURATION_LOC+CONFIG_MAXSIZE), Crc_Result );
 
   FeedTheDog(); 


}

static void ReadPresetBackup(UINT8 Presetno)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to Read all fields Preset structure in EEPROM from BACKUP AREA
*        Storage location is deduced from the Preset Number.
*
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  

{
   UINT16 CurrentAdr =0;
   UINT32 Crc_Result;
   UINT8  Validity = 0;
   UINT8  n = 0;

   //Generate the current address
   CurrentAdr  = EE_BACKUP_START + (PRESET_MAXSIZE + CRC_OFFSET)*Presetno; 

   // Read the structure
   for(n=0; n<PRESET_MAXSIZE; n++)
   {
      CurrentPreset.dat[n] = (UINT8)(ReadEE(CurrentAdr+n));
   }

   //Check the Validity
   Validity = CheckValidity(CurrentAdr,PRESET_MAXSIZE,PRESET);
   if (Validity == FALSE)
   { 
#ifdef DEBUG_ENABLE     
      PrintStr("\n\rPreset Backup Invalid -- Restoring Default");
#endif
      //If Invalid Restore defaults for Preset Structure
      for(n = OFFTIME; n<= WELDMODE; n++);
         SetVar((UINT8*)Format[n].Ptr , (UINT32)Parameters[n].Default ,(UINT8)Parameters[n].VarSize); 
      
      EEPROM_SavePreset(Presetno)  ;
   } 
   else    	 	//write it to the main EEPROM AREA
   {
#ifdef DEBUG_ENABLE     
      PrintStr("\n\rPreset Backup Valid -- Write To Main Area");
#endif
      //Generate the current address 
      CurrentAdr = EE_START + ((PRESET_MAXSIZE+CRC_OFFSET)*Presetno);

      //calculate CRC
      Crc_Result = CRC_Ck(&CurrentPreset.dat[0], (UINT32)PRESET_MAXSIZE);

      // Write the Preset structure 
      for(n=0; n<PRESET_MAXSIZE; n++)
    	  DataHandle = (UINT16)WriteEE(CurrentAdr+n,(UINT8) (CurrentPreset.dat[n])); 
         
      //Write the CRC   
      
      WriteCRC2EEPROM ((CurrentAdr+PRESET_MAXSIZE), Crc_Result );

      FeedTheDog();

   }
}
   
UINT8 EEPROM_ReadPreset(UINT8 Preset_num) 
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to Read all fields Preset structure in EEPROM. 
*        Storage location is deduced from the Preset Number.
*
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  
{  
   UINT16 CurrentAdr = 0,Test = 0;
   UINT8 Validity = 0;
   UINT8 n,Ret;
   Test = (UINT16)((ReadEE(EE_MAGIC1)<<8) | ReadEE(EE_MAGIC2));
   if(Test == ((MAGIC_NUM1<<8) | MAGIC_NUM2)) 
   {
      //Generate the current address
      CurrentAdr  = EE_START + ((PRESET_MAXSIZE + CRC_OFFSET)*Preset_num); 
#ifdef DEBUG_ENABLE     
      PrintStr("\n\r Read Preset  ");
      Print16(CurrentAdr);
#endif
      //Read the structure  
      for(n=0; n<PRESET_MAXSIZE; n++)
      {
         CurrentPreset.dat[n] = (UINT8)(ReadEE(CurrentAdr+n));
      }

      //Check the Validity
      Validity = CheckValidity(CurrentAdr,PRESET_MAXSIZE,PRESET);
      if (Validity == FALSE)
      {    
#ifdef DEBUG_ENABLE     
         PrintStr("\n\r Preset InValid");
#endif
         //if Invalid then read from BACKUP AREA
            ReadPresetBackup(Preset_num);
         Ret = FALSE;
      } 
      else  
      {
         Ret = TRUE;
      }
   } 
   else
   {
      EEPROM_Restore();
      Ret = TRUE;
   }
   return Ret;
 }
 
 
static void ReadConfigBackup(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to Read all fields Configuration structure in EEPROM from BACKUP AREA
*        Storage location is deduced from the Preset Number.
*
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  
{
   UINT8 Validity;
   UINT8 n = 0;
   UINT32 Crc_Result = 0;

   //Read the Configuration Structure
   for(n=0; n<CONFIG_MAXSIZE; n++)
      Conf.cdat[n] = (UINT8)(ReadEE(EE_BACKUP_CONFIGURATION_LOC+n));

   //Check the Validity
   Validity = CheckValidity(EE_BACKUP_CONFIGURATION_LOC,CONFIG_MAXSIZE,CONFIG);
   if (Validity == FALSE)
   {        
#ifdef DEBUG_ENABLE     
      PrintStr("\n\r Config Backup InValid -- Restore Defaults");
#endif
      //If Restore defaults for Configuration Structure
      for(n=PANELTRIG;n<=PSWATTAGE;n++)        
         SetVar((UINT8*)Format[n].Ptr , (UINT32)Parameters[n].Default ,(UINT8)Parameters[n].VarSize);   
      EEPROM_SaveConfig();
   } 
   else  // If the Data is Valid Write To the MAIN EEPROM AREA
   {    
#ifdef DEBUG_ENABLE     
      PrintStr("\n\r Config Backup Valid -- Write To Main Area");
#endif
   // Calculate CRC  
      Crc_Result = CRC_Ck(&Conf.cdat[0], (UINT32)CONFIG_MAXSIZE);

   // write the Config data
      for(n=0; n<CONFIG_MAXSIZE; n++)
         DataHandle = (UINT16)WriteEE(EE_CONFIGURATION_LOC+n,(UINT8)(Conf.cdat[n]));        

   // write the CRC  
      WriteCRC2EEPROM ((EE_CONFIGURATION_LOC+CONFIG_MAXSIZE), Crc_Result );
      FeedTheDog(); 
   }
}

UINT8 EEPROM_ReadConfig(void) 
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to Read all fields Configuration structure in EEPROM.
*
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  
{
   UINT16 Test = 0;
   UINT8 n = 0;
   UINT8 Validity = 0,Ret;

   Test = (UINT16)((ReadEE(EE_MAGIC1)<<8) | ReadEE(EE_MAGIC2));
   if(Test == ((MAGIC_NUM1<<8) | MAGIC_NUM2)) 
   {
      //Read the Configuration Structure   
      for(n=0; n<CONFIG_MAXSIZE; n++)
         Conf.cdat[n] = (UINT8)(ReadEE(EE_CONFIGURATION_LOC + n));
#ifdef DEBUG_ENABLE     
      PrintStr("\n\r EECONFIG ADDR = ");
      Print16(EE_CONFIGURATION_LOC); 
#endif
      //Check the Validity
      Validity = CheckValidity(EE_CONFIGURATION_LOC,CONFIG_MAXSIZE,CONFIG);
      if (Validity == FALSE)
      {    
#ifdef DEBUG_ENABLE     
         PrintStr("\n\rConfig InValid");
#endif
         //if Invalid then read from BACKUP AREA
         ReadConfigBackup();
         Ret = FALSE;
      }
      else { 
         Ret = TRUE;
      }
   } 
   else 
   {
      EEPROM_Restore(); 
      Ret = TRUE;
   }
   return Ret;
}
UINT8 EEPROM_Read(UINT8 Presetno) 
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to Read all fields in EE_PRESET and EE_CONFIG_DATA structure in EEPROM. Storage location 
*        is deduced from the Preset Number.
*
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  
{
 
   UINT8 Result ;
#ifdef DEBUG_ENABLE     
   PrintStr("\n\rREAD EEPROM ");
#endif
   if(Presetno >  MAX_SAVED_PRESETS)
      Result = FALSE;     
   else
   {      //Read the Preset and Configuration 
      DataHandle = (UINT16)EEPROM_ReadPreset(Presetno);
      DataHandle = (UINT16)EEPROM_ReadConfig();
      Result = TRUE;              
   }
   return Result;
}


void EEPROM_Recall(UINT8 Presetno)                                                                                     
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to Read all fields in EE_PRESET & EE_CONFIG_DATA structure in EEPROM. 
*        Storage location is deduced from the Preset Number. 
*        Also Upates Current Preset when the function called.
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  
{  
   UINT16 Test = 0;
#ifdef DEBUG_ENABLE     
   PrintStr("\n\rREACALL EEPROM ");
#endif
   //Check the Magic Number
   Test = (UINT16)((ReadEE(EE_MAGIC1)<<8) | ReadEE(EE_MAGIC2));
   if(Test == ((MAGIC_NUM1<<8) | MAGIC_NUM2)) 
   {
      DataHandle = (UINT16)EEPROM_ReadPreset(Presetno);                                  //Write the in-memory Structure 
      EEPROM_SavePreset(0);                                         //Save To the Current Preset
   } 

   else //if Invalid 
   {
#ifdef DEBUG_ENABLE     
      PrintStr("\n\r Magic InValid");
#endif
      EEPROM_Restore();
   }
}

void EEPROM_ChangeParam(UINT8 Presetno,enum TYPE Type_Var, UINT8 ParamNo) 
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Used to change a field in EE_PRESET or EE_CONFIG_DATA structure  in EEPROM. 
*        Storage location is deduced from the Preset Number. Upates when the function called.
*
*   Entry condition:                                                      
*
*   Exit condition:
*                                                                 
***************************************************************************/  
{ 
   UINT8 n =0;
   UINT16 Test = 0;
   UINT16 CurrentAdr = 0,CurrentBackupAdr = 0;
   UINT32 Crc_Result;

   
   Test = (UINT16)((ReadEE(EE_MAGIC1)<<8) | ReadEE(EE_MAGIC2));
   if(Test == ((MAGIC_NUM1<<8) | MAGIC_NUM2)) 
   {
      switch(Type_Var) 
      {
         case CONFIG:   
            CurrentAdr = EE_CONFIGURATION_LOC;
            CurrentBackupAdr = EE_BACKUP_CONFIGURATION_LOC;
            Crc_Result = CRC_Ck(&Conf.cdat[0], (UINT32)CONFIG_MAXSIZE);

            /* Update CRC */
            WriteCRC2EEPROM ((EE_CONFIGURATION_LOC+CONFIG_MAXSIZE), Crc_Result );

            /* Update CRC into Backup area also.*/
            
            WriteCRC2EEPROM ((EE_BACKUP_CONFIGURATION_LOC+CONFIG_MAXSIZE), Crc_Result );
         break;
         case PRESET:
            CurrentAdr = (EE_START + (PRESET_MAXSIZE + CRC_OFFSET)*Presetno);

            CurrentBackupAdr = EE_BACKUP_START + (PRESET_MAXSIZE + CRC_OFFSET)*Presetno;

            /* Update CRC */
            Crc_Result = CRC_Ck(&CurrentPreset.dat[0], (UINT32)PRESET_MAXSIZE);
            
            WriteCRC2EEPROM ((CurrentAdr+PRESET_MAXSIZE), Crc_Result );

            /* Update CRC into Backup area also.*/
            WriteCRC2EEPROM ((CurrentBackupAdr+PRESET_MAXSIZE), Crc_Result );
         break;
      }

      CurrentAdr += Parameters[ParamNo].Offset;
      CurrentBackupAdr += Parameters[ParamNo].Offset;	
      for(n = 0;n<Parameters[ParamNo].VarSize;n++)
      {      
    	  (void)WriteEE(CurrentAdr+n,*((UINT8*)Format[ParamNo].Ptr+n)); 
    	  (void)WriteEE(CurrentBackupAdr+n,*((UINT8*)Format[ParamNo].Ptr+n));
      }
   }

   else
   {  
#ifdef DEBUG_ENABLE     
      PrintStr("\n\r EEPROM INVALID");
#endif
      EEPROM_Restore();
   }

}

void SavePresetSelected(UINT8 PresetSelected)
{
	(void)WriteEE(PRESET_SELECTED_ADD, PresetSelected);
}
void RecallPresetSelected(void)
{
	PresetDisplay=ReadEE(PRESET_SELECTED_ADD);
}
void WriteCRC2EEPROM (UINT16 Address, UINT32 CrcCk )
{
	(void)WriteEE(Address,  (UINT8)(CrcCk>>TWO_BYTE));
	(void)WriteEE(Address+1,(UINT8)(CrcCk>>ONE_BYTE));
	(void)WriteEE(Address+2,(UINT8)(CrcCk));
}
