/*----------------------------------------------------------------------------
 * Name:    gsm tasks
 * Purpose: LPC1768 gsm asks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/ 
 
 
 #ifndef __GSM_THREAD_H
 #define __GSM_THREAD_H

//#include "tasks.h"
#include "gsm-control.h"

void gsm_thread (void);
 

extern char g_imei[16];
extern char g_apn[25];
extern char g_apn_usr[10];
extern char g_apn_pwd[10];
extern char g_gsm_local[200];
extern char g_gsm_work[255];

extern char gsm_str[255];
extern GSM_Parameters g_gsm_param;

 
 #endif //__GSM_THREAD_H
