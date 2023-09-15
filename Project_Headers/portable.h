/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/portable.h_v   1.2.3.0   15 Dec 2015 16:14:58   hasanchez  $
********************************************************************************* 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


--------------------------- MODULE DESCRIPTION ----------------------------------

This is used to make the Datatypes  and Bit Definitions for Masks portable

Module name: PORTABLE

Filename:    Portable.h
--------------------------------- TECHNICAL NOTES -------------------------------

This Header contains the declarations that makes the Code generic and Datatype Portable


------------------------------------- REVISIONS ---------------------------------

 * $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/portable.h_v  $
 * 
 *    Rev 1.2.3.0   15 Dec 2015 16:14:58   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.0   06 Aug 2012 05:27:36   akaushal
 * Initial revision.
 * 
 *    Rev 1.0   22 May 2012 12:00:54   ygupta
 * Initial revision.
------------------------------------- INCLUDES ----------------------------------*/

 #ifndef _PORTABLE_H
   #define _PORTABLE_H


/*-------------------------------------- DEFINES ----------------------------------*/

/* Emulator define */
#define EMULATOR 0

#define ON  ((BYTE)1)
#define OFF ((BYTE)0)
#define PAUSE ((BYTE)2)

#define BIT0    0x1
#define BIT1    0x2
#define BIT2    0x4
#define BIT3    0x8
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7    0x80
                /* WORDWISE or LONGWISE         */
#define BIT8    0x100
#define BIT9    0x200
#define BIT10   0x400
#define BIT11   0x800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

                /* LONGWISE             */
#define BIT16   0x10000 
#define BIT17   0x20000
#define BIT18   0x40000
#define BIT19   0x80000
#define BIT20   0x100000
#define BIT21   0x200000
#define BIT22   0x400000
#define BIT23   0x800000
#define BIT24   0x1000000
#define BIT25   0x2000000
#define BIT26   0x4000000
#define BIT27   0x8000000
#define BIT28   0x10000000
#define BIT29   0x20000000
#define BIT30   0x40000000
#define BIT31   0x80000000



/*-------------------------- TYPE DECLARATIONS ------------------------------*/

typedef  unsigned char       UINT8;           /* 8-bit  unsigned quantity  */
typedef  UINT8               BYTE;            /* 8-bit  unsigned quantity  */
typedef  unsigned       int  UINT16;          /* 16-bit unsigned quantity  */
typedef  unsigned long int   UINT32;          /* 32-bit unsigned quantity  */
typedef  char                SINT8;           /* 8-bit  signed quantity    */
typedef  signed       int    SINT16;          /* 16-bit signed quantity    */
typedef  signed long int     SINT32;          /* 32-bit signed quantity    */
typedef  unsigned char       STR;             /* byte value for text       */




typedef  enum boolean  
   {
      FALSE = 0,
      TRUE  = 1
   } BOOLEAN;                                /* system boolean definition */

#endif /* PORTABLE_H  */


/*-------------------------- FUNCTION PROTOTYPES ----------------------------*/



/*---------------------------- EXTERNAL DATA --------------------------------*/
