/*----------------------------------------------------------------------------
 * Name:    tasks
 * Purpose: LPC1768 tasks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
 
#ifndef __TASKS_H
#define __TASKS_H

#include <rtl.h>		/* RTL OS declaration*/


//extern OS_TID	t_gsm_thread;				//Send to GSM

//extern OS_TID	t_gps_thread;				//Send to GSM
 
 
 
extern OS_MUT m_write_gsm;
extern OS_MUT m_write_buffer;
 

__task void init (void);

#endif //__TASKS_H
