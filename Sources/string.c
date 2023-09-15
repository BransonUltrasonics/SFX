/*
$Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/string.c_v   1.1.1.2.3.0   15 Dec 2015 16:15:04   hasanchez  $
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

//This is the main module for the String operations.

//Module name: STRING

//Filename:    string.c
/**************************REVISIONS******************************************************
$Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/string.c_v  $
 * 
 *    Rev 1.1.1.2.3.0   15 Dec 2015 16:15:04   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.1.1.0   25 Aug 2015 12:02:04   hasanchez
 * Update on the functions to do not return any value on memcpy, strcpy,  and memset operations.
 * 
 *    Rev 1.1   06 Nov 2012 08:45:32   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.0   06 Aug 2012 05:31:50   akaushal
 * Initial revision.
 * 
 *    Rev 1.0   22 May 2012 12:01:42   ygupta
 * Initial revision.
*****************************************************************************************/

/*--------------------------------------------------------------------------*/
#include "string.h"

/**
 * Memcpy: Memory copy. Function.
 * @param dst Destination address
 * @param src Source address
 * @param len Data length
 * @return void* By convention, destination pointer is returned.
 */
void Memcpy(void * dst, const void * src, SINT16 len)
{
	SINT8 * cd = (SINT8 *) dst;
	const SINT8 * cs = (const SINT8 *) src;
	while (len--)
		*cd++ = *cs++;
}

/**
 * Strcpy: Copy a string.
 * @param dst Destination string.
 * @param src Source string.
 * @return char* Destination string.
 */
void Strcpy(SINT8 * dst, const SINT8 * src)
{
	SINT8 * dst1 = dst;
	while (*src)
		*dst1++ = *src++;
	*dst1 = 0;
} 

/**
 * Memset: Copy a byte to whole string a string.
 * @param dst Destination string.
 * @param dat Byte to be filled.
 * @param size Number of bytes to be copied.
 * @return char* Destination string.
 */
void Memset(void * dst, const SINT8 dat,SINT8 size)
{
   SINT8 * dst1 = (SINT8*)dst;
   while(size--)   
      *dst1++ = (SINT8)dat;
}
