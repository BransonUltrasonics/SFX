/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/LCD.h_v   1.7.1.3.3.2   14 Jul 2016 12:42:42   hasanchez  $
*************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION --------------------------------------

This is the LCD module which Displays all the Error, Data and Icons on thr Screen
Based on Previous Configuraion and Key Pressed

Module name: LCD

Filename:    LCD.h
--------------------------- TECHNICAL NOTES -----------------------------------------

This Header contains the declations and Constant Macros


------------------------------ REVISIONS --------------------------------------------
* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/LCD.h_v  $
 * 
 *    Rev 1.7.1.3.3.2   14 Jul 2016 12:42:42   hasanchez
 * Updated type of the variable UpdateTimer
 * 
 *    Rev 1.7.1.3.3.1   13 May 2016 13:43:04   hasanchez
 * Updated the structure of parameters.
 * 
 *    Rev 1.7.1.3.3.0   15 Dec 2015 16:14:56   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.7.1.0   25 Aug 2015 12:30:10   hasanchez
 * Added new LCD icons, new definitions of quantity of segments, quenatity of icons and numeric constants.
 * 
 *    Rev 1.7   10 Sep 2013 03:26:02   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.6   13 Jun 2013 08:00:10   akaushal
 * Updated the Pulse temperature mode, storing password, energy timeout issue.
 * 
 *    Rev 1.5   25 Mar 2013 07:59:48   akaushal
 * LCD board changed and pulse mode corrected
 * 
 *    Rev 1.4   03 Dec 2012 08:54:44   akaushal
 * Navigation and Screens Updated
 * 
 *    Rev 1.3   20 Nov 2012 07:28:02   akaushal
 * Lcd Flickering Fixed and Navigation Updated
 * 
 *    Rev 1.2   06 Nov 2012 08:42:16   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.1   10 Sep 2012 09:48:06   akaushal
 * Ready screen navigation for Last Weld Data and Testmode Displays Power
 * 
 *    Rev 1.0   06 Aug 2012 05:27:02   akaushal
 * Initial revision.
 * 
 *    Rev 1.5   31 Jul 2012 03:59:42   akaushal
 * Overload Screen to get data on overload and On and Off timer's resolution in Milliseconds 
 * 
 *    Rev 1.4   06 Jul 2012 10:03:36   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.3   27 Jun 2012 07:49:38   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:32:52   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:46:12   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:00:26   ygupta
 * Initial revision.

-------------------------------------------------------------------------------------*/

#ifndef _LCD_H
   #define _LCD_H

#include "global.h"
#include "portable.h"
#include "keyboard.h"
#include "CPU.h"
#include "onems.h"
#include <string.h>
#include "DS1804.h"

#define MSG                   0x80

#define NUM_ICONS             27
#define MAP_ICON              0x30
#define TOTALLCDSEGMENTS      74

#define MAP_RUN_ICON          0x6D
#define LZ_BL                 0x20
#define DECIMALBASE           10
#define NUM_7SEGMENT_DIGITS   0x06
#define SEC_IN_MINUTE         60
#define SEC_IN_HOUR           3600
#define MILLLISEC_IN_SEC      1000
#define BIT_NUM_MASK          0x03
#define PORT_INDEX_MASK       0xFC             
#define NUM_SEGMENT_PER_DIGIT 0x07
#define ASCIIOFFSET           32
#define TOTALASCIICHAR        95
#define DEGREE                94
#define ASCIIOFFSET           32
#define TOTAL_RESULTS		  10
#define TOTAL_WELDMODES		  18

#define BIT_0           (0x0001)
#define BIT_1           (0x0002)
#define BIT_2           (0x0004)
#define BIT_3           (0x0008)
#define BIT_4           (0x0010)
#define BIT_5           (0x0020)
#define BIT_6           (0x0040)
#define BIT_7           (0x0080)

#define  SEGA    (UINT8)(BIT_7)  /*    +---------------+    */
#define  SEGB    (UINT8)(BIT_6)  /*    |     --A--     |    */
#define  SEGC    (UINT8)(BIT_5)  /*    |    F     B    |    */
#define  SEGD    (UINT8)(BIT_4)  /*    |     --G--     |    */
#define  SEGE    (UINT8)(BIT_3)  /*    |    E     C    |    */
#define  SEGF    (UINT8)(BIT_2)  /*    |     --D--     |    */
#define  SEGG    (UINT8)(BIT_1)  /*    |            DP |    */
                                         /*    +---------------+    */

/***********************************************************************/
/*                                                                     */
/*       Here is a cryptic version of the ASCIITABLE.  Every letter is   */
/*  defined as some combination.  Most are straight forward, but some  */
/*  are a little strange.  It will allow, however, all letters to be   */
/*  displayed in some format.  If both an upper case and a lower case  */
/*  version are possible, then the upper case is defined as XxX and    */
/*  the lower case is defined as YxY, where the little x here shows    */
/*  what letter is being defined.  If only one case is possible then   */
/*  it is defined as XxX.                                              */
/*                                                                     */
/***********************************************************************/
#define  SPACE    (UINT8)(0)
#define  EXCLAM   (UINT8)(SEGA | SEGB )  // '!'
#define  QUOTES   (UINT8)(SEGB | SEGF )  // '"'
#define  HASH     (UINT8)(SEGB | SEGC | SEGE | SEGF )  // '#'
#define  DOLLAR   (UINT8)(SEGA | SEGC | SEGD | SEGF | SEGG) //"$".
#define  PERCENT  (UINT8)(SEGB | SEGG | SEGE )//"%".
#define  AMPERSAND (UINT8)(SEGA | SEGC | SEGD | SEGF | SEGG)//"&".
#define  APOSTROPHE (UINT8)(SEGB )//"'".
#define  L_PAREN  (UINT8)(SEGA | SEGD | SEGE | SEGF)//"(".
#define  R_PAREN  (UINT8)(SEGA | SEGD | SEGB | SEGC)//")".
#define  STAR     (UINT8)(SEGC | SEGF | SEGG | SEGB | SEGE) //"*".
#define  PLUS     (0) //"+", also called the "quadrathorpe".
#define  COMMA    (UINT8)(SEGE ) //","
#define  MINUS    (UINT8)(SEGG)//"-".
#define  DOT      (0)//".", 
#define  SLASH    (0)//"/"

// Numeric numbers.
#define  ZERO     (UINT8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF)
#define  ONE      (UINT8)(SEGB | SEGC)
#define  TWO      (UINT8)(SEGA | SEGB | SEGD | SEGE | SEGG)
#define  THREE    (UINT8)(SEGA | SEGB | SEGC | SEGD | SEGG)
#define  FOUR     (UINT8)(SEGB | SEGC | SEGF | SEGG)
#define  FIVE     (UINT8)(SEGA | SEGC | SEGD | SEGF | SEGG)
#define  SIX      (UINT8)(SEGA | SEGC | SEGD | SEGE | SEGF | SEGG)
#define  SEVEN    (UINT8)(SEGA | SEGB | SEGC)
#define  EIGHT    (UINT8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG)
#define  NINE     (UINT8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG) 
#define COLON     (UINT8)(SEGF | SEGE )//":".
#define SEMICOLON (UINT8)(SEGB | SEGE )//";"
#define LESS_THEN (UINT8)(SEGA )//"<".
#define EQUAL     (UINT8)(SEGD | SEGG) //"=".
#define GREATER_THEN (UINT8)(SEGD) //">".
#define QUERY     (UINT8)(SEGA | SEGB | SEGE | SEGG)//"?".
#define AT_SIGN   (UINT8)(SEGA | SEGG | SEGD )//"@"

//Uppercase Alphabets
#define  XAX     (UINT8)(SEGA | SEGB | SEGC | SEGE | SEGF | SEGG)
#define  XBX     (UINT8)(SEGC | SEGD | SEGE | SEGF | SEGG)
#define  XCX     (UINT8)(SEGA | SEGD | SEGE | SEGF)
#define  XDX     (UINT8)(SEGB | SEGC | SEGD | SEGE | SEGG)
#define  XEX     (UINT8)(SEGA | SEGD | SEGE | SEGF | SEGG)
#define  XFX     (UINT8)(SEGA | SEGE | SEGF | SEGG)
#define  XGX     (UINT8)(SEGA | SEGC | SEGD | SEGE | SEGF | SEGG)
#define  XHX     (UINT8)(SEGB | SEGC | SEGE | SEGF | SEGG)
#define  XIX     (UINT8)(SEGB | SEGC)
#define  XJX     (UINT8)(SEGB | SEGC | SEGD | SEGE)
#define  XKX     (UINT8)(SEGE | SEGF | SEGG)
#define  XLX     (UINT8)(SEGD | SEGE | SEGF)
#define  XMX     (UINT8)(SEGA | SEGD | SEGG)
#define  XNX     (UINT8)(SEGA | SEGB | SEGC | SEGE | SEGF)
#define  XOX     (UINT8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF)
#define  XPX     (UINT8)(SEGA | SEGB | SEGE | SEGF | SEGG)
#define  XQX     (UINT8)(SEGA | SEGB | SEGD | SEGE | SEGF)
#define  XRX     (UINT8)(SEGA | SEGE | SEGF)
#define  XSX     (UINT8)(SEGA | SEGC | SEGD | SEGF | SEGG)
#define  XTX     (UINT8)(SEGD | SEGE | SEGF | SEGG)
#define  XUX     (UINT8)(SEGB | SEGC | SEGD | SEGE | SEGF)
#define  XVX     (UINT8)(SEGB | SEGF)
#define  XWX     (UINT8)(SEGB | SEGD | SEGF)
#define  XXX     (UINT8)(SEGC | SEGF | SEGG)
#define  XYX     (UINT8)(SEGB | SEGC | SEGD | SEGF | SEGG )
#define  XZX     (UINT8)(SEGA | SEGD)  
    
#define L_BRACKET (UINT8)(SEGA | SEGD | SEGE | SEGF)//"[".
#define BACKSLASH (UINT8)(SEGC | SEGF )//"\".
#define R_BRACKET (UINT8)(SEGA | SEGD | SEGB | SEGC)//"]".
#define CIRCUMFLEX (UINT8)(SEGA | SEGB | SEGF |SEGG) //"^".
#define UNDERSCORE (UINT8)(SEGD)//"_".
#define REVERSE_QUOTE (UINT8)(SEGF)//"`". 
//Lower case alphabets.
#define  YaY     (UINT8)(SEGA | SEGB | SEGC | SEGE | SEGF | SEGG)
#define  YbY     (UINT8)(SEGC | SEGD | SEGE | SEGF | SEGG)
#define  YcY     (UINT8)(SEGD | SEGE | SEGG)
#define  YdY     (UINT8)(SEGB | SEGC | SEGD | SEGE | SEGG)
#define  YeY     (UINT8)(SEGA | SEGD | SEGE | SEGF | SEGG)
#define  YfY     (UINT8)(SEGA | SEGE | SEGF | SEGG)
#define  YgY     (UINT8)(SEGA | SEGC | SEGD | SEGE | SEGF | SEGG)
#define  YhY     (UINT8)(SEGC | SEGE | SEGF | SEGG)
#define  YiY     (UINT8)(SEGC)
#define  YjY     (UINT8)(SEGB | SEGC | SEGD | SEGE)
#define  YkY     (UINT8)(SEGE | SEGF | SEGG)
#define  YlY     (UINT8)(SEGB | SEGC)
#define  YmY     (UINT8)(SEGA | SEGD | SEGG)
#define  YnY     (UINT8)(SEGC | SEGE | SEGG)
#define  YoY     (UINT8)(SEGC | SEGD | SEGE | SEGG)
#define  YpY     (UINT8)(SEGA | SEGB | SEGE | SEGF | SEGG)
#define  YqY     (UINT8)(SEGA | SEGB | SEGD | SEGE | SEGF)
#define  YrY     (UINT8)(SEGE | SEGG)
#define  YsY     (UINT8)(SEGA | SEGC | SEGD | SEGF | SEGG)
#define  YtY     (UINT8)(SEGD | SEGE | SEGF | SEGG)
#define  YuY     (UINT8)(SEGC | SEGD | SEGE)
#define  YvY     (UINT8)(SEGB | SEGF)
#define  YwY     (UINT8)(SEGB | SEGD | SEGF)
#define  YxY     (UINT8)(SEGC | SEGF | SEGG)
#define  YyY     (UINT8)(SEGB | SEGC | SEGD | SEGF | SEGG )
#define  YzY     (UINT8)(SEGA | SEGD)   
#define L_BRACES (UINT8)(SEGA | SEGD | SEGE | SEGF)//"{".
#define BAR      (UINT8)(SEGB | SEGC)//"|".
#define R_BRACES (UINT8)(SEGA | SEGD | SEGB | SEGC)//"}".
#define TILDE    (UINT8)(SEGB | SEGG | SEGE )//"~". 

#define ON_ICON         BIT0
#define MICROTIP_I      BIT1
#define OFTIME          BIT2
#define CLOCK           BIT3
#define TEMPE           BIT4
#define PERCENT_I       BIT5
#define PULSE           BIT6
#define SECONDS         BIT7
#define AMPLITUDE_I     BIT8
#define AUTO            BIT9
#define ALARM_I         BIT10
#define SONICS_I        BIT11
#define JOULES          BIT12
#define DOT_I           BIT13
#define COLON_I         BIT14
#define TOTTIME         BIT15
#define PRESET_I        BIT16
#define LINE_2          BIT17
#define LINE_3          BIT18
#define LINE_4          BIT19
#define WATTAGE         BIT20
#define HRMINSEC_I      BIT21
#define OFF_ICON        BIT22
#define MAX				BIT23
#define PULSE2			BIT24
#define PROGRESS		BIT25


#define PERCENT_SEG     53
#define SECOND_SEG      55 
#define ALARM_SEG       58
#define SONIC_SEG       59
#define WATTAGE_SEG     68

#define Zero_Char   0
#define One_Char    1
#define Two_Char    2
#define Three_Char  3
#define Four_Char   4
#define Five_Char   5
#define Six_Char    6

#define Two_degrees_F    20
#define Three_degrees_F  30
#define Four_degrees_F  40
#define One_degree_C     18


#define MAX_TEMPE_LIMIT 1040


#define First_Block   1
#define Second_Block  2
#define Third_Block   3

#define ThirtyFive_degrees_F 350
#define ThirtyTwo_degrees_F  320
#define Two_degrees_C       356
#define Tree_degrees_C      374
#define TwoHundNine_deg_F 2090
#define HIGH_VALUE_C     1010
#define HIGH_VALUE_F     2130
#define MAX_NUMBER_FORMAT_MS 3599999
/*Define Binking icons on each screen*/

#define BLK_TIME2			HRMINSEC_I | COLON_I  
#define BLK_TIME			LINE_2 | LINE_3 | CLOCK | HRMINSEC_I | COLON_I		
#define BLK_ENERGY			LINE_2 | LINE_3 | JOULES			
#define BLK_ONTIME			LINE_4 | LINE_3 | ON_ICON | SECONDS | OFTIME | COLON_I			
#define BLK_OFFTIME			LINE_4 | LINE_3 | OFF_ICON| SECONDS | OFTIME | COLON_I		
#define BLK_ONENERGY		LINE_4 | LINE_3 | ON_ICON | OFTIME		
#define BLK_TOTALONTIME		LINE_4 | LINE_3 | TOTTIME | HRMINSEC_I | COLON_I			
#define BLK_TOTALONENERGY	LINE_4 | LINE_3 | TOTTIME 
#define BLK_PULSETEMPE		LINE_2 | LINE_3 | PULSE2 | TEMPE | DOT_I	
#define BLK_MAXTEMPE		LINE_2 | LINE_3 | MAX |	TEMPE | DOT_I
#define BLK_TEMPE		    TEMPE | DOT_I
#define BLK_AMPLITUDE		LINE_4 | LINE_3 | AMPLITUDE_I |	PERCENT_I						
#define BLK_CALIBRATION		LINE_2 | LINE_3 | TEMP			
#define NO_BLINK            0
#define BLINK_OFF_STRING    28    //Number to select the Off string and make blink it (011100)


/*Define Solid icons on Result screens.*/

#define SLD_TOTALTIME		CLOCK | HRMINSEC_I | COLON_I	
#define SLD_TOTALENERGY		JOULES
#define SLD_AMPLITUDE		AMPLITUDE_I | PERCENT_I
#define SLD_MAXTEMP			MAX |	TEMPE | DOT_I
#define SLD_ENDTEMP			TEMPE | DOT_I
#define SLD_PEAKPOWER		WATTAGE
#define SLD_ON_TIME			ON_ICON | SECONDS | OFTIME | COLON_I 	
#define SLD_ON_ENERGY		ON_ICON | OFTIME | JOULES
#define SLD_OFF_TIME		OFF_ICON| SECONDS | OFTIME | COLON_I	
#define SLD_TOTAL_ON_ENERGY	TOTTIME | JOULES
#define SLD_TOTAL_ON_TIME	TOTTIME | HRMINSEC_I | COLON_I
#define SLD_PROGRESS		PROGRESS | PERCENT_I




#define NUMERIC_MAXSIZE       sizeof(NUMERIC)



//extern UINT8 displayNum;
extern UINT8 UpdateLiveReady;
extern enum eScreens screen;
extern UINT8 DisplayCodes[NUM_7SEGMENT_DIGITS+1];
extern const STR  StrZero[NUM_7SEGMENT_DIGITS+1];
extern const STR  MinLimitC[NUM_7SEGMENT_DIGITS+1];
extern const STR  MaxLimitC[NUM_7SEGMENT_DIGITS+1];
extern const STR  OffTempC[NUM_7SEGMENT_DIGITS+1];
extern const STR  OffTempF[NUM_7SEGMENT_DIGITS+1];
extern const STR  MaxLimAmp_Micro[NUM_7SEGMENT_DIGITS+1];

extern bool ContinuousMode;
extern UINT8 ModeSet;       //Modes for Time,Temp and Energy..
extern bool ModeSet2 ;
extern UINT8  ValueOkay;
extern BOOLEAN UpdateTimer;
extern UINT16 BlinkTimer;
extern enum Params CurrentParam;
extern enum Params AuxCurrentParam;
extern UINT32 StorePassword;
extern UINT8 BlockSelected;
extern SINT32 Increment;
extern bool ShowDefaults;
extern bool LoadPreset;

struct Fn_Scrn {
   UINT8 screenNumber;
   UINT8 displayValue;
};



enum eWattage{ 
   PWR_150 =0,
   PWR_250,
   PWR_550,
};

struct Wattage {
   UINT8 WattageNumber;
   UINT16 displayValue;
};

typedef struct Numeric {
   UINT32 Value;
   UINT8 displayValue[NUM_7SEGMENT_DIGITS];
}NUMERIC;

typedef union Numeric_Data {
   UINT8 dat[NUMERIC_MAXSIZE];
   NUMERIC NumericData;
}NUMERIC_DATA;



typedef struct DispFormat {
   enum Params  Format;        /* Defines decimal location and units of parameter */     
   UINT8   *Ptr;               /* Pointer to place to put or get parameter value */
   UINT8   *Pointercurrent;
   
} DISP_FORMAT;



const typedef struct DispValue{
   enum Params  Format;        /* Defines decimal location and units of parameter */   
   SINT32  Default;            /* Default value when control level is set or NVRAM fails */ 
   UINT8   Min1;               /* Minimum ascii value for digit 1 */
   UINT8   Min2;               /* Minimum ascii value for digit 2 */
   UINT8   Min3;               /* Minimum ascii value for digit 3 */
   UINT8   Min4;               /* Minimum ascii value for digit 4 */
   UINT8   Min5;
   UINT8   Min6;
   UINT8   Max1;               /* Maximum ascii value for digit 1 */
   UINT8   Max2;               /* Maximum ascii value for digit 2 */
   UINT8   Max3;               /* Maximum ascii value for digit 3 */
   UINT8   Max4; 
   UINT8   Max5;  
   UINT8   Max6;
   UINT8   QtyG1;
   UINT8   QtyG2;
   UINT8   QtyG3;/* Maximum ascii value for digit 4 */
   UINT8   GroupDefault;
   UINT32  BlinkIcons;	
   UINT8   VarSize;
   UINT8   Offset;
}DisplayVal;

struct LCDData
{ 
	UINT32	TotalTime;
	UINT32	TotalEnergy;
	UINT8 	AmplitudeValue;
	UINT8	Progress;
	UINT16	MaxTemp;
	UINT16	LastTemp;
	UINT16	CurrentTemp;
	UINT16	PeakPower;
	UINT16	CurrentPower;
	UINT32  OnTime;
	UINT16	OnEnergy;
	UINT32	OffTime;
	UINT32	TotalOnTime;
	enum IdsModes  IDWeldMode; 
	UINT8	HandHeld;
	UINT8	ValidResult;

   //UINT16 TestPower;
};

typedef struct Result {
	enum IdsModes IDMODES;
	UINT8	length;
	UINT8	ResultValue[TOTAL_RESULTS];
}RESULT;
	

#define NODIGITS 	0
#define DIGIT1   	1
#define DIGIT2   	2
#define DIGIT3    	4
#define DIGIT4    	8
#define DIGIT5    	16
#define DIGIT6    	32
#define ALLDIGITS 	63

                              /* 7 segment display values*/
typedef struct dig{ 
   UINT8 digit;
   UINT8 Blinking;
}CURRENTDIGIT;

extern UINT8 BlinkDigitAux;
extern UINT8 PresetDisplay;
extern RESULT Results[ID_PS_INFINITE_ENERGY_B+1];
extern const struct Wattage MenuWattage[PWR_550+1];
extern bool EscKeyPressed;
extern NUMERIC_DATA Num_Current;
extern DISP_FORMAT Format[];
extern const DisplayVal Parameters[]; 
extern struct LCDData currentLCDData;
extern CURRENTDIGIT CurrentDigit;
extern UINT8 CurrFnScreen;
extern UINT32 FixedIcons;           
extern UINT8 PresetNo;
extern UINT8 ResultIndex;
extern UINT8 Weldmode;
extern const UINT8 GDisplay_Remap[];
extern UINT8 DataEntryError;
extern UINT16 tempr;
extern SINT16 DigMinVal;
extern UINT16 DigMaxVal;
extern UINT8  DigMax5Val;
void LCD_Init(void);
void UpdateLive(void);
void SwitchScreen(UINT16 NextScreen);
void TurnONorOFFLCDSegment(UINT8,BOOLEAN);
void ALLIconsOFF(void);
void SwitchFunctionScreen(void);
void SwitchFunctionScreenMsg(void);
void Numout(UINT32, UINT8 , UINT8 , UINT8 * , UINT8 );
void UpdateDisplayValue(void);

SINT32 CalcDisplayValue(UINT8 First,UINT8 Last);
STR* TemperatureOFF(void);
UINT32 GetParamNumber(void);
void PerformAction(void);
void VerifyTempVSCurret(void);
void VerifyOFF(void);
void UpdateParamVal(bool);
void SelectDigitGroup(bool);
void GetLimits(void);
void ChangeResult(bool);
void DisplayResult(void);
void DisplaySegments(void);
void UpFnScreen(void);
void DownFnScreen(void);
void AcceptValue(void);
void IncValue(void);
void DecValue(void);
void NextLEFTDigit(void);
void NextRIGHTDigit(void);
void DisplayAllIcons(void);
void PreviousScreen(void);
void DownPresetNum(void);
void UpPresetNum(void);
void WeldMode_Icon(UINT16 WeldMode);
void SetDigitBlink (void);
void IncAmplitudeValue(void);
void NextLeftRunScreen(void);
void NextRightRunScreen(void);
void IncAmplitudeValue(void);
void DecAmplitudeValue(void);
UINT16 ConvertTempToF(UINT16);
void DisplayLastWeldDataLeft(void);
void DigMinMaxVal(void);
#endif
