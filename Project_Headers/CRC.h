/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/CRC.h_v   1.2.3.0   15 Dec 2015 16:14:56   hasanchez  $
*/
/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2010

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.



--------------------------------- MODULE DESCRIPTION ---------------------------

This is the main module for CRC Calculation.
********************************************************************************************/
/*  Module name: CRC
  Filename:    CRC.h
  */
  
/* Log: $

   
********************************************************************************************/

#ifndef _CRC_H
#define _CRC_H

#include "global.h"
#include "portable.h"

//UINT16 CRC(UINT8 *Ptr, UINT32 Length);
UINT8  CRC(UINT8 *Ptr, UINT32 Length, BOOLEAN SecureByte, UINT8 CRC8) ;
UINT16 Ck16(UINT8 *Ptr, UINT32 Length, BOOLEAN SecureByte);
UINT32 CRC_Ck(UINT8 *Ptr, UINT32 Length);
void Calc_Flash_CRC_Ck(void);

extern UINT8 CRCLowByte;
extern UINT8 CRCMidByte;
extern UINT8 CRCHighByte;

#endif
