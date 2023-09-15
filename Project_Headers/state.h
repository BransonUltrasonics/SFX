/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/state.h_v   1.4.3.0   15 Dec 2015 16:15:00   hasanchez  $
*/  
/****************************************************************************/ 
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 1996-2007            */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.            */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------*/
/*                                                                          */
/* Module name: STATE                                                       */
/*                                                                          */
/* Filename:    State.h                                                     */
/*                                                                          */
/* This module is used to control weld sequence and user interface states   */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*------------------------------ REVISIONS ---------------------------------*/
/*                                                                          */
/* $Log::   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Project_Headers/state.h_v  $                                                                 
 * 
 *    Rev 1.4.3.0   15 Dec 2015 16:15:00   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.1   25 Aug 2015 12:42:28   hasanchez
 * Added new functions related to the navigation style.
 * 
 *    Rev 1.0   06 Aug 2012 05:28:08   akaushal
 * Initial revision.
 * 
 *    Rev 1.0   22 May 2012 12:01:36   ygupta
 * Initial revision.
                                                                          
*****************************************************************************/

 
#ifndef _STATE_H
#define _STATE_H

#include <stddef.h>
/*--------------------------- FUNCTION PROTOTYPES ----------------------------*/

void StateMachine(void);
void NaviMenuEnter(void);
void NaviMenuLeave(void);
/*------------------------------ EXTERNAL DATA -------------------------------*/
extern UINT16 CurrentState; 
extern UINT8  IndexMenu;
#endif /* STATE_H  */
