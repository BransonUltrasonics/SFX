/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/Cpu.c_v   1.3.1.2.3.0   15 Dec 2015 16:15:02   hasanchez  $
********************************************************************************* 

   Copyright (c) Branson Ultrasonics Corporation, 1996-2010

   This program is the property of Branson Ultrasonics Corporation
   Copying of this software is expressly forbidden, without the prior
   written consent of Branson Ultrasonics Corporation.


---------------------------------- MODULE DESCRIPTION ---------------------------

This is the main module for controlling the CPU initialisation

Module name: CPU

Filename:    Cpu.c
--------------------------------- TECHNICAL NOTES -------------------------------

Contains the low level register configurations and kicks off ADC, LCD, 
and timer inits 


------------------------------------- REVISIONS ---------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/Cpu.c_v  $
 * 
 *    Rev 1.3.1.2.3.0   15 Dec 2015 16:15:02   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.3.1.0   25 Aug 2015 11:30:54   hasanchez
 * Update on the microcontroller configuration.
 * 
 *    Rev 1.3   06 Nov 2012 08:43:06   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.2   16 Aug 2012 02:22:34   akaushal
 * Watchdog Timer Enabled
 * 
 *    Rev 1.1   16 Aug 2012 01:57:12   akaushal
 * SFX Code With External Crystal Oscillator and HW Changes
 * 
 *    Rev 1.0   06 Aug 2012 05:30:12   akaushal
 * Initial revision.
 * 
 *    Rev 1.4   06 Jul 2012 10:02:06   akaushal
 * Code Cleanup and Formatting.
 * 
 *    Rev 1.3   27 Jun 2012 07:52:38   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:30:14   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:44:02   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 11:59:16   ygupta
 * Initial revision.
--------------------------------------------------------------------------------*/

#pragma MESSAGE DISABLE C4002 /* WARNING C4002: Result not used is ignored */

#include "PE_Types.h"
#include "Cpu.h"
#include "LCD.h"
#include "MC9S08LG32.h"
#include "global.h"
#include "onems.h"
#include "print.h"



/* Global variables */
volatile byte CCR_reg;                 /* Current CCR register */
UINT8 ADC_Ch = 0;
UINT8 PWRCounter = 0;
UINT8 TempeCounter = 0;
UINT16 ADTempe[ADCTEMPECOUNT];
UINT16 ADPower[ADCPOWERCOUNT];
UINT16 DataHandle = 0;
ISR(Cpu_Interrupt)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        The method services unhandled interrupt vectors.
*                                                                         
***************************************************************************/
{
   asm(BGND);
}

extern void _Startup(void);            // Forward declaration of external startup function declared in file Start12.c
#pragma NO_FRAME
#pragma NO_EXIT
void _EntryPoint(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Initializes the whole system like timing and so on. At the end 
*        of this function, the C startup is invoked to initialize stack,
*        memory areas and so on.
*
***************************************************************************/
{
   /* ### MC9S08LG32_64 "Cpu" init code ... */

   /* Common initialization of the write once registers */
   /* SOPT1: COPE=1,COPT=1,STOPE=0,??=0,??=0,??=0,BKGDPE=1,RSTPE=0 */
   SOPT1 = 0xC3;
   /* SOPT2: COPCLKS=0,??=0,??=0,??=0,??=0,??=0,??=0,SPIFE=1 */
   setReg8(SOPT2, 0x01);     
   /* SPMSC1: LVWF=0,LVWACK=0,LVWIE=0,LVDRE=1,LVDSE=1,LVDE=1,??=0,BGBE=0 */
   SPMSC1 = 0x1C;                
   /* SPMSC2: ??=0,??=0,LVDV=0,LVWV=0,PPDF=0,PPDACK=0,??=0,PPDC=0 */
   SPMSC2 = 0x00;         

   //  System clock initialization
   ICSC1 = 0x00;     // bus freq controlled by output of FLL; external clk selected.
   ICSC1_RDIV = 0;   // No divider, external clock is already in range.
   // ICSC2: BDIV=0,RANGE=0,HGO=0,LP=0,EREFS=1,ERCLKEN=1,EREFSTEN=0
   ICSC2 = (ICSC2_ERCLKEN_MASK | ICSC2_EREFS_MASK); // Initialization of the ICS control register 2
   while (!ICSSC_OSCINIT); // Wait until the initialization of the external crystal oscillator is completed
   // ICSSC: DRST_DRS=0,DMX32=0
   ICSSC &= 0x1F;
   //ICSSC &= (UINT8)~(ICSSC_DRST_DRS1_MASK | ICSSC_DRST_DRS0_MASK | ICSSC_DMX32_MASK);//Initialization of the ICS status and control
   while ((ICSSC & ICSSC_DRST_DRS_MASK) != 0x00); // Wait until the FLL switches to Low range DCO mode

   __asm   jmp _Startup ;               // Jump to C startup code
   FeedTheDog();                              // Reset Watchdog timer
}


void LowLevelInit(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Initializes modules and provides common register initialization. 
*        The method is called automatically as a part of the 
*        application initialization code.
*
*   Entry condition:
*
*        All values are hard coded in function.                                                      
*
*   Exit condition:            
*                                                                         
***************************************************************************/
{
   SCGC1 = 0x51;  // Clock Gate Control 1
                  // 0:    SCI1		disabled
                  // 1:    SCI2		enable
                  // 2:    I2C		disabled
                  // 3:    MTIM		disabled
                  // 4:    ADC		enabled
                  // 5:    TPM1		disabled
                  // 6:    TPM2		enable
                  // 7:    RTC		disabled
                                    
   SCGC2 = 0xC2;  // Clock Gate Control 2
                  // 0:    SP		disabled
                  // 1:    LCD		enable
                  // 2:    -			disabled
                  // 3:    -			disabled
                  // 4:    KBI		disabled
                  // 5:    IRQ		disabled
                  // 6:    Flash Clock  enable
                  // 7:    DBG		 enable
                                
   PINPS3 = 0x30;
                  // 0:    SS pin pos     x
                  // 1:    SCK pin pos    x
                  // 2:    MOS1 pin pos   x
                  // 3:    MOS0 pin pos   x
                  // 4:    SDA pin pos    1 = PTI4
                  // 5:    SCL pin pos    1 = PTI5
                  // 6:    RX pin pos     x
                  // 7:    TX pin pos     x

   /* ### Shared modules init code ... */	 
   LCD_Init();		//initialise the LCD
   Init_Port_DS1804();
   SCI_Init();
   TI_Init();
   ADC_Init();  

   // GPIO Setup
   /* -----------
   Port A:
   **    Port:       Data     Direction      Slew     Pullup      Strength
   **    0: Col3     1        output         no       yes         Strong
   **    1: Col2     1        output         no       yes         Strong
   **    2: Col1     1        output         no       yes         Strong
   **    3: Col0     0        output         no       yes         Strong
   **    4: Row2     x        input          no       yes         Strong
   **    5: Row1     x        input          no       yes         Strong
   **    6: Row0     x        input          no       yes         Strong
   **    7: LCD16    0        x              x        x           x
   --------------*/
   PTAD  = 0x77;                             // Data -- COL0 = 0
   PTADD = 0x0F;                             // Data Direction
   PTASE = 0x00;                             // Slew Rate Enable
   PTAPE = 0x7F;                             // Pullup Enable
   PTADS = 0x0F;                             // Drive strength

   /* -----------
   Port B:
   **    Port:       Data     Direction      Slew     Pullup      Strength
   **    0: LCD3     x        x              x        x           x
   **    1: LCD4     x        x              x        x           x
   **    2: LCD5     x        x              x        x           x
   **    3: LCD6     x        x              x        x           x
   **    4: NA       x        x              x        x           x
   **    5: NA       x        x              x        x           x
   **    6: NA       x        x              x        x           x
   **    7: NA       x        x              x        x           x
   --------------*/
   PTBD  = 0x00;                             // Data
   PTBDD = 0x00;                             // Data Direction
   PTBSE = 0x00;                             // Slew Rate Enable
   PTBPE = 0x00;                             // Pullup Enable
   PTBDS = 0x00;                             // Drive strength   

   /* -----------
   Port C:
   **    Port:       Data     Direction      Slew     Pullup      Strength
   **    0: LCD23    x        x              x        x           x
   **    1: LCD22    x        x              x        x           x
   **    2: LCD21    x        x              x        x           x
   **    3: LCD20    x        x              x        x           x
   **    4: LCD19    x        x              x        x           x
   **    5: BKGD     x        x              x        x           x
   **    6: KB_RESET    x        x              x        x           x
   **    7: NA       x        x              x        x           x
   --------------*/
   PTCD  = 0x00;                             // Data
   PTCDD = 0x00;                             // Data Direction
   PTCSE = 0x00;                             // Slew Rate Enable
   PTCPE = 0x00;                             // Pullup Enable
   PTCDS = 0x00;                             // Drive strength   

   /* -----------
   Port D:
   **    Port:       Data     Direction      Slew     Pullup      Strength
   **    0: LCD1     x        x              x        x           x
   **    1: LCD2     x        x              x        x           x
   **    2: LCD7     x        x              x        x           x
   **    3: LCD8     x        x              x        x           x
   **    4: LCD30    x        x              x        x           x
   **    5: LCD29    x        x              x        x           x
   **    6: LCD28    x        x              x        x           x
   **    7: LCD27    x        x              x        x           x
   --------------*/
   PTDD  = 0x00;                             // Data
   PTDDD = 0x00;                             // Data Direction
   PTDSE = 0x00;                             // Slew Rate Enable
   PTDPE = 0x00;                             // Pullup Enable
   PTDDS = 0x00;                             // Drive strength 

   /* -----------
   Port E:
   **    Port:       Data     Direction      Slew     Pullup      Strength
   **    0: LCD9     x        x              x        x           x
   **    1: LCD10    x        x              x        x           x
   **    2: LCD11    x        x              x        x           x
   **    3: LCD12    x        x              x        x           x
   **    4: LCD13    x        x              x        x           x
   **    5: LCD14    x        x              x        x           x
   **    6: LCD15    x        x              x        x           x
   **    7: LCD24    x        x              x        x           x
   --------------*/
   PTED  = PORTE_DEFAULTOUT;							// Data
   PTEDD = 0x00;						// Data Direction
   PTESE = 0x00;							// Slew Rate Enable
   PTEPE = 0xff;						// Pullup Enable
   PTEDS = 0x00;							// Drive strength  

   /* -----------
   Port F:
   **    Port:       Data     Direction      Slew     Pullup      Strength
   **    0: TX       x        x              x        x           x
   **    1: RX       1        in             x        1           strong
   **    2: TC(a)    x        x              x        x           strong   //UPDATED 07/17/15
   **    3: PPOE     0        out            no       yes         strong
   **    4: DAC      0        out            no       yes         strong
   **    5: Sonics   1        out            no       yes         strong
   **    6: LowPwr   1        out            no       yes         strong
   **    7: empty    x        in             x        no          x
   --------------*/
   PTFD  = 0xE7;						// Data
   PTFDD = 0xBE;                           // Data Direction
   PTFSE = 0x00;                           // Slew Rate Enable
   PTFPE = 0x7A;                           // Pullup Enable
   PTFDS = 0x78;                           // Drive strength    74

   /* -----------
   Port G:
   **    Port:       Data     Direction      Slew     Pullup      Strength
   **    0: LCD26    x        x              x        x           x
   **    1: OUTPUT   x        x              x        x           x
   **    2: LCD18    x        x              x        x           x
   **    3: LCD17    x        x              x        x           x
   **    4: NA       x        x              x        x           x
   **    5: NA       x        x              x        x           x
   **    6: NA       x        x              x        x           x
   **    7: NA       x        x              x        x           x
   --------------*/
   PTGD  = 0x00;                             // Data
   PTGDD = 0x02;                             // Data Direction
   PTGSE = 0x00;                             // Slew Rate Enable
   PTGPE = 0x02;                             // Pullup Enable
   PTGDS = 0x02;                             // Drive strength     

   /* -----------
   Port H:
   **    Port:       Data     Direction      Slew     Pullup      Strength
   **    0: NA       x        x              x        x           x
   **    1: NA       x        x              x        x           x
   **    2: NA       x        x              x        x           x
   **    3: NA       x        x              x        x           x
   **    4: CS       1        out            no       yes         x
   **    5: TempADC  x        in             x        x           x
   **    6: Bcklight 1        out            no       x           Strong
   **    7: INPUT    1        out            no       no          x
   --------------*/
   PTHD  = 0xE0;                             // Data
   PTHDD = 0xE0;                             // Data Direction
   PTHSE = 0x00;                             // Slew Rate Enable
   PTHPE = 0xE0;                             // Pullup Enable
   PTHDS = 0xC0;                             // Drive strength     

   __EI();                              /* Enable interrupts */

   /* Initialise the OUTPUTS */
   PSReset();
   PSIdle();
   ClrReady();
   ClrAlarm();
   ClrBeep();
   
}

void ADC_Init(void) 
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Initializes the associated peripheral(s) for ADC. The method is called 
*        automatically as a part of the application initialization code.
*                                                                         
*   Entry condition:  
*
*   Exit condition: 
*                                                                         
***************************************************************************/
{
   /*Bit 7 ADLPC 1 Configures for low-power (lowers maximum clock speed)
   Bit 6:5 ADIV 00 Sets the ADCK to the input clock ÷ 1
   Bit 4 ADLSMP 1 Configures for long sample time
   Bit 3:2 MODE 10 Sets mode at 10-bit conversions
   Bit 1:0 ADICLK 01 Selects bus clock as half input clock source  */ 
                
   ADCCFG = 0x99;
   ADCSC2 = 0x00;
   ADCSC1 = 0x4B;
   APCTL1 = 0x00;
   APCTL2 = 0x48; //ADC14(POWER_ADC),ADC11(TEMP_ADC)

   PWRCounter = 0;
   TempeCounter = 0;
   ADC_Ch = ADC_TEMPECH;
}

void StartADCConversion(void)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        Start the ADC conversion for ch 14 i.e.POWER_ADC. Interrupt handler
*        will take care of other two ADC channels.Function will be called every ms.                                                                 
***************************************************************************/
{
   ADCSC1 = 0x4E;// (b01001110)  Enable conevrion for ch14.
    
   /*Bit 7 COCO 0 Read-only flag which is set when a conversion completes
   Bit 6 AIEN 1 Conversion complete interrupt enabled
   Bit 5 ADCO 0 One conversion only (continuous conversions disabled)
   Bit 4:0 ADCH 01110 Input channel 14 selected as ADC input channel */

   ADC_Ch = ADC_POWERCH;
}

ISR( ADC_Interrupt)
/***************************************************************************
*                                                                         
*   Purpose:
*
*        The method services ADC interrupt vectors and Start conversion for next ch.
*                                                                         
***************************************************************************/
{
   UINT8 Adh,Adl;
   UINT16 Temp;
   switch(ADC_Ch)
   {
      case ADC_POWERCH:
         Adh = ADCRH;
         Adl = ADCRL;
         Temp = Adh;
         Temp = Temp << 8;
         Temp |= Adl;
         ADPower[PWRCounter++] = Temp;
         if( PWRCounter == ADCPOWERCOUNT) 
         {
            PWRCounter = 0;
         }
         ADCSC1 = 0x4B; //Enable Next Channel conversion.
         ADC_Ch = ADC_TEMPECH;
      break;

      case ADC_TEMPECH:
         Adh = ADCRH;
         Adl = ADCRL;
         Temp = Adh;
         Temp = Temp << 8;
         Temp |= Adl;
         ADTempe[TempeCounter++] = Temp;
         if( TempeCounter == ADCTEMPECOUNT)
         {
            TempeCounter = 0;
         } 
         
         ADCSC1 = 0x0;  //Stop conversion.           
         ADC_Ch = ADC_POWERCH;       
      break;
   }
}

// Initialization of the CPU registers in FLASH

// NVPROT: FPS7=1,FPS6=1,FPS5=1,FPS4=1,FPS3=1,FPS2=1,FPS1=1,FPDIS=1
const UINT8 NVPROT_INIT @0x0000FFBD = 0xFF;

// NVOPT: KEYEN=0,FNORED=1,??=1,??=1,??=1,??=1,SEC01=1,SEC00=0
const UINT8 NVOPT_INIT @0x0000FFBF = 0x7E;
