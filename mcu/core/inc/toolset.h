/*----------------------------------------------------------------------------
 * Name:    toolset.h
 * Purpose: LPC1768 Low level tool functions
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/

#ifndef __TOOLSET_H
#define __TOOLSET_H

#include "cmsis_os.h"		/* RTL OS declaration*/
//#include <rtl.h>

//Error codes

#define ERROR_SIM 0

void TOOL_delay(unsigned int ms);
void TOOL_delay_nl(unsigned int ms);

void TOOL_error(unsigned int err_code);

void NVIC_interrups_init(void);

void TOOL_wdt(void);
void TOOL_reset(void);
void TOOL_wdt_init(int time_wdt);


#define CNTLQ      0x11
#define CNTLS      0x13
#define DEL        0x7F
#define BACKSPACE  0x08
#define CR         0x0D
#define LF         0x0A


#define ON			1
#define OFF			0

#define OK			1
#define NOT			0



#endif
