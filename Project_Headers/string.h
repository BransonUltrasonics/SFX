/*
$Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/string.h_v   1.1.1.3.3.0   15 Dec 2015 16:15:00   hasanchez  $
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

//This is the main module for controlling the string operations.

//Module name: STRING

//Filename:    string.h
/**************************REVISIONS******************************************************
$Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/string.h_v  $   
 * 
 *    Rev 1.1.1.3.3.0   15 Dec 2015 16:15:00   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.1.1.0   25 Aug 2015 12:46:12   hasanchez
 * Functions Memcpy, Strcpy, and Memset were updated. 
 * 
 *    Rev 1.1   06 Nov 2012 08:42:54   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.0   06 Aug 2012 05:28:16   akaushal
 * Initial revision.
 * 
 *    Rev 1.0   22 May 2012 12:01:48   ygupta
 * Initial revision.
******************************************************************************************/

/*--------------------------------------------------------------------------*/
#ifndef _STRING_H
   #define _STRING_H

#include "portable.h"

//Fuction Prototype//

void  Memcpy(void * dst, const void * src, SINT16 len);
void  Strcpy(SINT8 * dst, const SINT8 * src);
void  Memset(void * dst, const SINT8 dat,SINT8 size);

#endif
