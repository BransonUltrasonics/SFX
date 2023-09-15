/* $Header:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/state.c_v   1.5.3.5   20 Jul 2016 09:06:12   hasanchez  $
*/   
/****************************************************************************/ 
/*                                                                          */
/*                                                                          */
/*      Copyright (c) Branson Ultrasonics Corporation, 1996-2007            */
/*     This program is the property of Branson Ultrasonics Corporation      */
/*   Copying of this software is expressly forbidden, without the prior     */
/*   written consent of Branson Ultrasonics Corporation.                    */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*************************                         **************************/
/*--------------------------- MODULE DESCRIPTION ---------------------------*/
/*                                                                          */
/* Module name: STATE                                                       */
/*                                                                          */
/* Filename:    State.c                                                     */
/*                                                                          */
/* This module is used to control weld sequence and user   interface states */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*------------------------------ REVISIONS ---------------------------------*/
/*                                                                          */
/* $Log:   D:/databases/VMdb/archives/SFX/SFX CodeWarrior10/SFX_CW_10/SFXProject/Sources/state.c_v  $
 * 
 *    Rev 1.5.3.5   20 Jul 2016 09:06:12   hasanchez
 * Removed unnecesary comments.
 * 
 *    Rev 1.5.3.4   14 Jul 2016 12:11:34   hasanchez
 * Updated the State Machine to fix issue of first blink fast.
 * 
 *    Rev 1.5.3.3   13 Jun 2016 12:30:28   hasanchez
 * The state machine is updated to take care of the test and seek functions with the Hand Held.
 * 
 *    Rev 1.5.3.2   18 May 2016 13:09:58   hasanchez
 * Deleted debug function
 * 
 *    Rev 1.5.3.1   13 May 2016 12:51:20   hasanchez
 * Added new states to handle the verification of Handheld when running Test or Seek.
 * 
 *    Rev 1.5.3.0   15 Dec 2015 16:15:04   hasanchez
 * Fixes and changes done due the comments on the level 2 units.
 * 
 *    Rev 1.2   25 Aug 2015 11:58:34   hasanchez
 * Added new Navigation Style on the preset menu.
 * 
 *    Rev 1.1   06 Nov 2012 08:45:18   akaushal
 * Menu navigation, Code cleanup and Code formatting
 * 
 *    Rev 1.0   06 Aug 2012 05:31:42   akaushal
 * Initial revision.
 * 
 *    Rev 1.3   27 Jun 2012 07:51:30   akaushal
 * Preset ERROR  updated and extstart corrected
 * 
 *    Rev 1.2   21 Jun 2012 07:34:16   ygupta
 * Weldmode  Bug Fixed and EEPROM code Changed
 * 
 *    Rev 1.1   12 Jun 2012 09:48:10   ygupta
 * Updated Support for Keyboard and Alarms
 * 
 *    Rev 1.0   22 May 2012 12:01:32   ygupta
 * Initial revision.

*****************************************************************************/


/*----------------------------- DESCRIPTION --------------------------------*/

/***************************************************************************/
/*                                                                         */
/*       The following symbols are used in this set of structures:         */
/*                                                                         */
/*       Rdy = ready state              Top = first entry in state         */
/*       Nrdy= not ready state          sts = start flag                   */
/*       Tst = test state               Rst = reset key                    */
/*       Mde = mode change state        OL  = overload                     */
/*       Par = parameter change state   Lck = lock                         */
/*       On  = sonics on state          Lft = left key                     */
/*       Off = sonics off state         Up  = up key                       */
/*       Exp = experiment state         Ent = enter key                    */
/*       GD  = ground detect            Sek = Seek                         */
/*                                                                         */
/*       The meaning of the following would be:                            */
/*                                                                         */
/*       S_Rdy_Par   In ready state checking Parameter select key          */
/*       S_Tst_OL    In test state checking for overload                   */
/*       S_Par_Ent   In parameter change state dealing with enter key      */
/*                                                                         */
/***************************************************************************/



/*------------------------------ INCLUDES ----------------------------------*/

#include "portable.h"
#include "p_inputs.h"
#include "p_output.h"
#include "weldhold.h"
#include "util.h"
#include "global.h"
#include "LCD.h"
#include "MC9S08LG32.h"
#include "state.h"
#include "onems.h"
#include "string.h"
#include "print.h"

#include "eeprom.h"


/*---------------------------- LOCAL_EQUATES -------------------------------*/
typedef void (*StateFunc)(void);
UINT8  IndexMenu = 0;    //To track the current state on the Menu
typedef const struct substate {
    UINT8                  *FlagPtr;        /* Pointer to current flag status      */
    StateFunc              FuncPtr;         /* Pointer to executable function      */
    const struct substate  *TruePtr;        /* Pointer to next state if flag true  */
    const struct substate  *FalsePtr;       /* Pointer to next state if flag false */
    UINT16                 Id;              /* Unique # assigned to each entry     */   
} STATE;

typedef struct NAVI_MENU {
	enum MENUDESITION 	IdKey;
	enum Params  		Screen;
	enum Params  		Parameter;
	UINT32	     		Value_to_Compare;
	UINT8	     		TrueJump;
	UINT16       		SumTrue;
	UINT8	     		FalseJump;
	UINT16       		SumFalse;
}NAVI_MENU;

/*---------------------------- State prototypes-----------------------------*/
extern STATE S_Rdy_Test;
extern STATE S_Rdy_Top,  S_Rdy_Mde,  S_Rdy_Par,  S_Rdy_Sts,   S_Rdy_Rst, S_Rdy_Sek;
extern STATE S_Rst_Top,  S_Rst_001,  S_Sek_Top,  S_Sek_001,   S_Sek_002, S_Sek_003;
extern STATE S_Sek_004,  S_Sek_005;
extern STATE S_Tst_Top,  S_Tst_001,  S_Tst_OL,   S_Tst_Sp1,   S_Tst_Sp2;
extern STATE S_Tst_OL2,  S_Tst_OL3,  S_Tst_OL4;
extern STATE S_Nrdy_Top, S_Nrdy_Rst, S_Nrdy_Lck, S_Nrdy_Rst1;
extern STATE S_Prdy_Top, S_Prdy_001, S_Prdy_002,  S_Prdy_003;
extern STATE S_Mde_Top,  S_Mde_Rst,  S_Mde_Rst1;
extern STATE S_Par_Top,  S_Par_001,  S_Par_002,  S_Par_003,   S_Par_004, S_Par_005;
extern STATE S_Par_Lft,  S_Par_Up,   S_Par_Ent,  S_Par_Par,   S_Par_Rst;
extern STATE S_On_Top,   S_On_001,   S_On_003,    S_On_003A, S_On_004,  S_On_005;
//extern STATE S_On_Top,   S_On_001,   S_On_002,   S_On_003,    S_On_003A, S_On_004,  S_On_005;
extern STATE S_On_006,   S_On_Ent,   S_On_GD , S_Pause_Top;
//extern STATE S_Off_Top,  S_Off_001,  S_Off_002,  S_Off_003,   S_Off_004, S_Off_Ent;
extern STATE S_Off_Top,  S_Off_001,  S_Off_003,   S_Off_004, S_Off_Ent;
extern STATE S_Exp_Top,  S_Exp_001;
extern STATE S_Exp_002,  S_Exp_003,  S_Exp_Ent,S_Pause_001,S_Pause_002,S_Pause_003,S_Off_003A;
extern STATE S_HH_Top, S_HH_Verif1, S_HH_Verif2, S_HH_Verif3, S_HH_Verif4, S_HH_Verif5, S_HH_FoundC;
extern STATE S_Sek_HH1, S_Sek_HH2, S_Sek_HH3, S_HH_NFound, S_HH_NFound1, S_HH_NFound2, S_HH_NFound3;
extern STATE S_HH_FoundT, S_HH_Cycle2, S_HH_Cycle, S_HH_Alarm, S_HH_Test, S_HH_Test2, S_HH_FoundT2, S_HH_FoundT3;
extern STATE S_HH_FoundT4, S_Tst_001H, S_Tst_002H;
/*----------------------------- GLOBAL DATA --------------------------------*/

/*                  CURR COND    FUNC             TRUE         FALSE       UNIQUE  */
/*                  PTR          PTR              STATE        STATE         ID    */
/*                  ---------    -----            ----------   ---------   ------  */

/***   Ready state   ***/

STATE S_Rdy_Top  = {NULL,        EnterReady,      &S_Rdy_Test, &S_Rdy_Test, 99   };
STATE S_Rdy_Test = {&TestKey,    NULL,      	  &S_HH_Top,   &S_Rdy_Par,  100  };
STATE S_Rdy_Par  = {&SetKey,     NULL,            &S_Par_Top,  &S_Rdy_Sts,  101  };
STATE S_Rdy_Sts  = {&StartFlag,  InitWeldParam,   &S_HH_Top,   &S_Rdy_Rst,  102  };
STATE S_Rdy_Rst  = {&ResetKey,   NULL,            &S_Rst_Top,  &S_Rdy_Sek,  103  };
STATE S_Rdy_Sek  = {&SeekRequest,NULL,            &S_HH_Top,   &S_Rdy_Test, 104  };


/***   Reset state   ***/

STATE S_Rst_Top  = {&ResetKey,   ResetClearAlarm, &S_Rst_001,  &S_Rdy_Top,  110  };
STATE S_Rst_001  = {&ResetKey,   NULL,            &S_Rst_001,  &S_Rdy_Top,  111  };


/***   Seek state   ***/

STATE S_Sek_Top  = {NULL,        ClrReady,        &S_Sek_HH1,  &S_Sek_HH1, 120};
STATE S_Sek_HH1  = {NULL,        Start_HH_Timer2, &S_Sek_HH2,  &S_Sek_HH2, 121};
STATE S_Sek_HH2  = {&FlagTimer1, NULL,            &S_Sek_HH3,  &S_Sek_001, 122};
STATE S_Sek_HH3  = {&SeekRequest, NULL,           &S_Sek_HH2,  &S_Rdy_Top, 123};
STATE S_Sek_001  = {&SeekRequest,PSSeek,          &S_Sek_002,  &S_Rdy_Top,  124  };
STATE S_Sek_002  = {&SeekActive, NULL,            &S_Sek_004,  &S_Sek_003,  125  }; /* Power supply is slow to respond so */
STATE S_Sek_003  = {&SeekRequest,NULL,            &S_Sek_002,  &S_Rdy_Top,  126  }; /* Wait until active, or request gone */
STATE S_Sek_004  = {NULL,        RemovePSSeek,    &S_Sek_005,  &S_Sek_005,  127  };
STATE S_Sek_005  = {&SeekRequest,NULL,            &S_Sek_005,  &S_Rdy_Top,  128  }; /* Wait until request removed         */


/***   Test state   ***/

STATE S_Tst_Top  = {NULL,            EnterTest,  &S_Tst_001H,  &S_Tst_001H,  1000 };
STATE S_Tst_001H = {&HandHeld,       CalcPower,  &S_Tst_002H, &S_Tst_001,  1001 };
STATE S_Tst_002H = {&Tst_N_ExtStart, NULL,       &S_Tst_OL,   &S_Tst_Sp2,  1002 };
STATE S_Tst_001  = {&TestKey,        NULL,       &S_Tst_OL,   &S_Tst_Sp2,  1003 };

STATE S_Tst_OL   = {&OLFlag,     NULL,            &S_Tst_Sp1,  &S_Tst_001H,  1004 };
STATE S_Tst_Sp1  = {&Conf.ConfigData.AutoReset, FinalPower,    &S_Tst_OL2,  &S_Tst_OL3,  1005 };
STATE S_Tst_Sp2  = {NULL,        FinalPower,      &S_Prdy_Top, &S_Prdy_Top,  1006 };
STATE S_Tst_OL2  = {&TestKey,    NULL,            &S_Tst_OL2,  &S_Tst_OL4,  1007 };  /* Wait til key released */
STATE S_Tst_OL3  = {&TestKey,    NULL,            &S_Tst_OL3,  &S_Nrdy_Top, 1008 };  /* Wait til key released */
STATE S_Tst_OL4  = {NULL,        ResetClearAlarm, &S_Prdy_Top, &S_Prdy_Top,  1009 };  /* Clear OL alarm        */


/***   Not Ready state   ***/

STATE S_Nrdy_Top = {&AlarmFlag,  ClrReady,        &S_Nrdy_Rst, &S_Nrdy_Lck, 2000 };
STATE S_Nrdy_Rst = {&ResetKey,   NULL,            &S_Nrdy_Rst1,&S_Nrdy_Lck, 2001 };
STATE S_Nrdy_Lck = {&LockItFlag, NULL,            &S_Nrdy_Rst, &S_Rdy_Top,  2002 };
STATE S_Nrdy_Rst1= {NULL,        ResetAlarm,      &S_Nrdy_Top, &S_Nrdy_Top, 2003 };


/***   Pre Ready state   ***/

STATE S_Prdy_Top = {NULL,        PreReady,        &S_Prdy_001, &S_Prdy_001, 5000 };
STATE S_Prdy_001 = {NULL,        CheckPreReady,   &S_Prdy_002, &S_Prdy_002, 5001 };
STATE S_Prdy_002 = {&AlarmFlag,  NULL,            &S_Nrdy_Top, &S_Prdy_003, 5002 };
STATE S_Prdy_003 = {&OkToAdvance,NULL,            &S_Rdy_Top,  &S_Prdy_001, 5003 };


/***   Parameter Change state   ***/

STATE S_Par_Top  = {NULL,        ClrReady,        &S_Par_001,  &S_Par_001,  4000 };
STATE S_Par_001  = {&SetKey,     NULL,            &S_Par_001,  &S_Prdy_Top,  4001 };
/***   Running Sonics On state   ***/

STATE S_On_Top   = {&StopFlag,  StartSonicsTimer, &S_Prdy_Top,   &S_On_001, 200 };
STATE S_On_001   = {&StopFlag,       CheckOnParam,&S_Prdy_Top, &S_On_003,   201 };
STATE S_On_003   = {&TimeFlag,   NULL,            &S_Off_Top,  &S_On_003A,  202 };
STATE S_On_003A  = {&PauseFlag,  NULL,            &S_Pause_Top,&S_On_004,   203 };
STATE S_On_004   = {&OLFlag,     NULL,            &S_Exp_Top,  &S_On_001,   204 };

/***   Pause State  ***/

STATE S_Pause_Top	= {NULL,        PauseTimers,	&S_Pause_001,	&S_Pause_001,	500 };
STATE S_Pause_001   = {&StopFlag,   NULL,           &S_Prdy_Top,	&S_Pause_002,	501 };
STATE S_Pause_002   = {&PauseFlag,  NULL,			&S_Pause_001,	&S_Pause_003,	502 };
STATE S_Pause_003   = {&ONorOFF,	ResumeTimers,	&S_On_001,		&S_Off_001,		503 };

/***   Running Sonics Off state   ***/

STATE S_Off_Top  = {NULL,        StartHoldTimer,  &S_Off_001,   &S_Off_001,   300 };
STATE S_Off_001  = {&StopFlag,   CheckOffParam,   &S_Prdy_Top,  &S_Off_003,   301 };
STATE S_Off_003  = {&TimeFlag,   NULL,            &S_Exp_Top,   &S_Off_003A,  302 };
STATE S_Off_003A = {&PauseFlag,  NULL,            &S_Pause_Top, &S_Off_001,   303 };

/*** Experiment state   ***/

STATE S_Exp_Top  = {&OLFlag,     NULL,     		  &S_Prdy_Top,  &S_Exp_001,  400 };
STATE S_Exp_001  = {&ExperFlag,  CheckExperiment, &S_Exp_002,  &S_Prdy_Top, 401 };  /* Experiment on ? */
STATE S_Exp_002  = {&StopFlag,   NULL,            &S_Prdy_Top,  &S_On_Top,   402 };

/*** Hand Held state   ***/

STATE S_HH_Top     = {NULL,          Start_HH_Timer,   &S_HH_Verif1, &S_HH_Verif1, 700};
STATE S_HH_Verif1  = {&FlagTimer1,             NULL,   &S_HH_Verif2, &S_HH_Cycle2, 701};
STATE S_HH_Verif2  = {&SeekRequest,            NULL,   &S_HH_Verif3, &S_HH_Verif1, 702};
STATE S_HH_Verif3  = {NULL,          Start_HH_Timer2,  &S_HH_Verif4, &S_HH_Verif4, 703};
STATE S_HH_Verif4  = {&FlagTimer1,             NULL,   &S_HH_Verif5, &S_HH_Cycle2, 704};
STATE S_HH_Verif5  = {&SeekRequest,            NULL,   &S_HH_Verif4, &S_HH_Cycle,  705};


STATE S_HH_Cycle   = {&CycleReqFlag,     NULL,          &S_HH_FoundC, &S_HH_Test,   706};
STATE S_HH_Test    = {&TestReqFlag,      NULL,          &S_HH_FoundT, &S_Prdy_Top,  707};
STATE S_HH_Cycle2   = {&CycleReqFlag,    NULL,          &S_HH_NFound, &S_HH_Test2,  708};
STATE S_HH_Test2    = {&TestReqFlag,     NULL,          &S_Tst_Top,   &S_Sek_Top,   709};

STATE S_HH_FoundT  = {NULL,            SetHHT,        &S_HH_Alarm,   &S_HH_Alarm,   710};
STATE S_HH_Alarm   = {&AlarmFlag,        NULL,        &S_Nrdy_Top,   &S_HH_FoundT2, 711};
STATE S_HH_FoundT2 = {&Tst_N_ExtStart,   NULL,        &S_HH_FoundT3, &S_HH_FoundT4, 712};
STATE S_HH_FoundT3 = {NULL,        SetUserOut,        &S_Tst_Top,    &S_Tst_Top,    713};
STATE S_HH_FoundT4 = {&TestKey,          NULL,        &S_HH_FoundT2, &S_Prdy_Top,   714};

STATE S_HH_FoundC  = {&AlarmFlag,       SetHH,         &S_Nrdy_Top,     &S_On_Top,     715};

STATE S_HH_NFound  = {&Conf.ConfigData.PanelTrig,NULL,&S_HH_NFound1, &S_HH_NFound2, 716};
STATE S_HH_NFound1 = {&StartByPT,             NULL,   &S_On_Top,     &S_HH_NFound3, 717};
STATE S_HH_NFound2 = {&StartByPT,             NULL,   &S_Rdy_Top,    &S_On_Top,     718};
STATE S_HH_NFound3 = {NULL,             SetStartFlag, &S_Prdy_Top,   &S_Prdy_Top,   719};
/*--------------------------  Global Variables  ----------------------------*/

UINT16   CurrentState, LastState;

/*Structure of Navigation to handle the decisions while a preset is being configured*/

const NAVI_MENU TABLEMENU[TOTAL_DECISIONS] = {
		{ 	MENU_MODE, 			CONF_MODE	 ,	WELDMODE		,	(CONFIG_CONT)					,	CON_SONIC		,	CONFIG_CONT			,	PUL_SUNIC			,	CONFIG_PULSE	}	,
		{	CON_SONIC, 			CONF_CONTROL ,	WELDMODE		,	(CONFIG_CONT | CONFIG_TIME)		,	CON_TIME		,	CONFIG_TIME			,	CON_SONIC2			,	0	}	,
		{	CON_SONIC2,			FNEND		 ,	WELDMODE		,	(CONFIG_CONT | CONFIG_ENERGY)	,	CON_ENERGY		,	CONFIG_ENERGY		,	CON_MAX				,	CONFIG_TEMP	}	,
		{	PUL_SUNIC, 			CONF_CONTROL ,	WELDMODE		,	(CONFIG_PULSE | CONFIG_TIME)	,	PUL_TIME_ON		,	CONFIG_TIME			,	PUL_SONIC2			,	0	}	,
		{	PUL_SONIC2, 		FNEND		 ,	WELDMODE		,	(CONFIG_PULSE | CONFIG_ENERGY)	,	PUL_ENER_ON		,	CONFIG_ENERGY		,	PUL_TEM				,	CONFIG_TEMP	}	,
		{	CON_TIME,  			TIME		 ,	TIME			,	0								,	MENU_AMPLITUDE	,	SET_TIME_ZERO		,	CON_TIME_MTEMP		,	SET_TIME_NOT_ZERO	}	,
		{	CON_TIME_MTEMP,		MAXTEMPE	 ,	MAXTEMPE		,	0								,	MENU_AMPLITUDE	,	SET_MAXTEMP_ZERO	,	CON_TIME_PULSE		,	SET_MAXTEMP_NOT_ZERO	}	,
		{	MENU_AMPLITUDE, 	AMPLITUDE	 ,	0			    ,	0								,	49				,	0					,	49					,	0	}		,
		{	CON_TIME_PULSE,		PULSETEMPE	 ,	PULSETEMPE		,	0								,	MENU_AMPLITUDE	,	SET_PUL_TEMP_ZERO	,	MENU_AMPLITUDE		,	SET_PUL_TEMP_NO_ZERO	}	,
		{	CON_ENERGY,			ENERGY		 ,	FNEND			,	0								,	CON_ENER_MTEMP	,	0					,	CON_ENER_MTEMP		,	0	}	,
		{	CON_ENER_MTEMP, 	MAXTEMPE	 ,	MAXTEMPE		,	0								,	MENU_AMPLITUDE	,	SET_MAXTEMP_ZERO	,	CON_ENER_PULSE		,	SET_MAXTEMP_NOT_ZERO	}	,
		{	CON_ENER_PULSE,		PULSETEMPE	 ,	PULSETEMPE		,	0								,	MENU_AMPLITUDE	,	SET_PUL_TEMP_ZERO	,	MENU_AMPLITUDE		,	SET_PUL_TEMP_NO_ZERO	}	,
		{	CON_MAX,  			MAXTEMPE	 ,	FNEND			,	0								,	MENU_AMPLITUDE	,	0					,	MENU_AMPLITUDE		,	0	}	,
		{	PUL_TIME_ON,		ONTIME		 ,	FNEND			,	0								,	PUL_TIME_OFF	,	0					,	PUL_TIME_OFF		,	0	}	,
		{	PUL_TIME_OFF, 		OFFTIME		 ,	FNEND			,	0								,	PUL_TIME_TT		,	0					,	PUL_TIME_TT			,	0	}	,
		{	PUL_TIME_TT, 		TOTALONTIME	 ,	TOTALONTIME		,	0								,	MENU_AMPLITUDE	,	SET_ONTIME_ZERO		,	PUL_TIME_MTEMP		,	SET_ONTIME_NOT_ZERO	}	,
		{	PUL_TIME_MTEMP, 	MAXTEMPE	 ,	MAXTEMPE		,	0								,	MENU_AMPLITUDE	,	SET_MAXTEMP_ZERO	,	PUL_TIME_PULSE		,	SET_MAXTEMP_NOT_ZERO	}	,
		{	PUL_TIME_PULSE, 	PULSETEMPE	 ,	PULSETEMPE		,	0								,	MENU_AMPLITUDE	,	SET_PUL_TEMP_ZERO	,	MENU_AMPLITUDE		,	SET_PUL_TEMP_NO_ZERO	}	,
		{	PUL_ENER_ON, 		ONENERGY	 ,	FNEND			,	0								,	PUL_ENER_OFF	,	0					,	PUL_ENER_OFF		,	0	}	,
		{	PUL_ENER_OFF, 		OFFTIME		 ,	FNEND			,	0								,	PUL_ENER_TE		,	0					,	PUL_ENER_TE			,	0	}	,
		{	PUL_ENER_TE,		TOTALONENERGY,	TOTALONENERGY	,	0								,	MENU_AMPLITUDE	,	SET_ONENERGY_ZERO 	,	PUL_ENER_MTEMP		,	SET_ONENERGY_NOT_ZERO	}	,
		{	PUL_ENER_MTEMP, 	MAXTEMPE	 ,	MAXTEMPE		,	0								,	MENU_AMPLITUDE	,	SET_MAXTEMP_ZERO	,	PUL_ENER_PULSE		,	SET_MAXTEMP_NOT_ZERO	}	,
		{	PUL_ENER_PULSE, 	PULSETEMPE	 ,	PULSETEMPE		,	0								,	MENU_AMPLITUDE	,	SET_PUL_TEMP_ZERO	,	MENU_AMPLITUDE		,	SET_PUL_TEMP_NO_ZERO	}	,
		{	PUL_TEM, 			MAXTEMPE	 ,	FNEND			,	0								,	PUL_TEM_CONROL	,	0					,	PUL_TEM_CONROL		,	0	}	,
		{	PUL_TEM_CONROL,		CONF_CONTROL2,	WELDMODE		,	(CONFIG_PULSE | CONFIG_TEMP | CONFIG_SUBTIME),	PUL_TEM_TIME_ON	,	CONFIG_SUBTIME	,	PUL_TEM_ENER_ON	,	CONFIG_SUBENERGY	}	,
		{	PUL_TEM_TIME_ON,	ONTIME		 ,	FNEND			,	0								,	PUL_TEM_TIME_OFF,	0					,	PUL_TEM_TIME_OFF	,	0	}	,
		{	PUL_TEM_TIME_OFF,	OFFTIME		 ,	FNEND			,	0								,	MENU_AMPLITUDE	,	0					,	MENU_AMPLITUDE		,	0	}	,
		{	PUL_TEM_ENER_ON,	ONENERGY	 ,	FNEND			,	0								,	PUL_TEM_ENER_OFF,	0					,	PUL_TEM_ENER_OFF	,	0	}	,
		{	PUL_TEM_ENER_OFF,	OFFTIME		 ,	FNEND			,	0								,	MENU_AMPLITUDE	,	0					,	MENU_AMPLITUDE		,	0	}	,
};
/*---------------------------- EXTERNAL DATA -------------------------------*/


/*-------------------------------- CODE ------------------------------------*/


void StateMachine(void)
/***************************************************************************/
/*                                                                         */
/*  This is the state machine driver.   This code is table driven.  For    */
/*  every state the following entries will exist:                          */
/*                                                                         */
/*       1) Pointer to UINT8 flag (FlagPtr) which contains TRUE or FALSE   */
/*       2) Pointer to next state in the event the flag is TRUE.           */
/*       3) Pointer to next state in the event the flag is FALSE.          */
/*       4) Pointer to routine to run for this substate                    */
/*       5) Unique ID for each state entry for debugging                   */
/*                                                                         */
/*  The following global variables will be used:                           */
/*                                                                         */
/*     CurrentState - pointer to the current state unique ID for debugging */
/*     LastState - pointer to the previous state unique ID for debugging   */
/*                                                                         */
/***************************************************************************/
{
   static STATE *StatePtr = &S_Prdy_Top;  /* Start off in Pre-ready state */
   
   if (StatePtr->FuncPtr !=NULL)
      (StatePtr->FuncPtr)();                    /* Go execute function     */
   if (StatePtr->FlagPtr == NULL) {
      StatePtr = (STATE*) StatePtr->TruePtr;
   }
   else if (*StatePtr->FlagPtr)
      StatePtr = (STATE*) StatePtr->TruePtr;  /* Set up for true state   */
   else
      StatePtr = (STATE*) StatePtr->FalsePtr; /* Set up for false state  */
   LastState = CurrentState;                   /* Save last state and then*/
   CurrentState = StatePtr->Id;                /* new state for debugging */
}


/***************************************************************************
* Function Name: NaviMenuEnter
*
* Arguments: N/A  
*
* Return Type: N/A
*
* Description:
*		Handle the Navigation when entering to a Navigation Menu (Preset 
*		Parameter). It handle if the screen should show the default value
*		or the value stored. It decide to skip the temp values related 
*		if the temp probe is not connected.
***************************************************************************/
void NaviMenuEnter(void)
{
	if (TABLEMENU[IndexMenu].Screen != FNEND )
	{
		CurrentParam = TABLEMENU[IndexMenu].Screen;
		
		if (EscKeyPressed==TRUE)
		{
			if (CurrentParam == CONF_CONTROL2)
				TempPreset.presetData.WeldMode |= (UINT16)(CONFIG_SUBTIME); 
			else
				AcceptValue();
			
			VerifyTempVSCurret();
			NaviMenuLeave();
		}
		else
		{	
			if (CurrentParam == CONF_MODE)  
			{
				if ((TempPreset.presetData.WeldMode & CONFIG_CONT ) == CONFIG_CONT) 
					ContinuousMode = TRUE;
				else
					ContinuousMode = FALSE;
				SwitchScreen(SCREEN_MODECONFIG);	
			}
			else
				if (CurrentParam == CONF_CONTROL)
				{
					if (ShowDefaults == TRUE)
						ModeSet = TIME_MODE;
					else
					{
						ModeSet = (CurrentPreset.presetData.WeldMode & CONFIG_TEMP) >> 9;  
					}
					SwitchScreen(SCREEN_SONICCONF);
				}
				else
					if (CurrentParam == CONF_CONTROL2)
					{
						if (ShowDefaults == TRUE)
							ModeSet2=TIME_MODE;
						else
							if ((CurrentPreset.presetData.WeldMode & ENERGY_MODE ) != ENERGY_MODE)
								ModeSet2=TIME_MODE;
							else
								ModeSet2=ENERGY_MODE;
						SwitchScreen(SCREEN_SONICCONF2);
					}
					else
						if (TempeProbePresent == FALSE  && (CurrentParam == PULSETEMPE  ||  CurrentParam == MAXTEMPE))
						{
							SetVar(Format[CurrentParam].Ptr , Parameters[CurrentParam].Default, Parameters[CurrentParam].VarSize);
							NaviMenuLeave();
						}
						else	
							SwitchScreen(SCREEN_PARAMETER);
		}
	}
	else
		NaviMenuLeave();
}


void NaviMenuLeave(void)
/**************************************************************************
 * NaviMenuLeave compares the captured value with the table, with this    *
 * the system will decide which is the new index and update the Weldmode  *
 * according the selection.                                               *
***************************************************************************/
{
	UINT32 TempValue;
	UINT8 *Value;
	if (TABLEMENU[IndexMenu].Parameter != 0 )  //Most change 0 to defined identifier
	{
		if (TABLEMENU[IndexMenu].Parameter != FNEND ) 
		{
			Value = Format[TABLEMENU[IndexMenu].Parameter ].Ptr;
			TempValue = GetVar(Value,Parameters[TABLEMENU[IndexMenu].Parameter].VarSize);
			if (TempValue == (TABLEMENU[IndexMenu].Value_to_Compare))
			{
				TempPreset.presetData.WeldMode |=  TABLEMENU[IndexMenu].SumTrue;
				IndexMenu = TABLEMENU[IndexMenu].TrueJump;
			}
			else
			{
				TempPreset.presetData.WeldMode |=  TABLEMENU[IndexMenu].SumFalse;
				IndexMenu = TABLEMENU[IndexMenu].FalseJump;
			}
		}
		else
			IndexMenu = TABLEMENU[IndexMenu].TrueJump;
		NaviMenuEnter();
	}
	else
	{
		if (TempPreset.presetData.WeldMode != CurrentPreset.presetData.WeldMode)
		{
			PresetDisplay=0;
			SavePresetSelected(PresetDisplay);
		}
			
		CurrentPreset = TempPreset; 
		EEPROM_SavePreset(0);
		SwitchScreen(SCREEN_RDY);
		 
	}	
}
