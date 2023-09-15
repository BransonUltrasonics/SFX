/*
$Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/print.h_v   1.3.3.0   15 Dec 2015 16:14:58   hasanchez  $
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

//This is the main module for controlling the Serial.

//Module name: PRINT

//Filename:    print.h
/**************************REVISIONS******************************************************
$Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/print.h_v  $
 * 
 *    Rev 1.3.3.0   15 Dec 2015 16:14:58   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.1   10 Sep 2013 03:26:44   akaushal
 * Updated after CodeOptimization, handled seek signal, changed the default value of reg 50, pause issue.
 * 
 *    Rev 1.0   06 Aug 2012 05:27:44   akaushal
 * Initial revision.
 * 
 *    Rev 1.1   12 Jun 2012 09:47:58   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:01:04   ygupta
 * Initial revision.
 
******************************************************************************************/

/*--------------------------------------------------------------------------*/

#ifndef _PRINT_H
#define _PRINT_H

//#define DEBUG_ENABLE
#define TXBUFSIZE 255
#define RXBUFSIZE 255

#include "portable.h"
#define HIBYTEMASK      0xFF00
#define LOBYTEMASK      0x00FF
#define LONIBBLEMASK    0X0F
 

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
void Print8(UINT8 character);

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
void Print16(UINT16);

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
 void PrintStr(SINT8* string);

 
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
void SCI_Init(void);


/****************************************************************************
 * Function Name: SCI_SendCommand
 *
 * Agruments:
 * UINT8* buffer: Data bytes to be sent.
 * UINT16 buflen: data length.
 *
 * Return Type: VOID
 *
 * Description:
 *    It sends the MsgBuffer over the Serial Port.
 *
 ****************************************************************************/

void PrintHex8(UINT8 character);
 
void Print32(UINT32 number);
 
void Print8(UINT8 character);


void TERMIO_PutChar(UINT8 character);

 
#endif
