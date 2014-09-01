/*----------------------------------------------------------------------------
 * Name:    gsm tasks
 * Purpose: LPC1768 gsm asks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
 
 
#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "lpc_types.h"

#define SER_MAX_WAIT_RECV_TIME 		20000000
#define SER_MAX_LINE_SIZE	 		255	

#define SERIAL_DELAY 0x1000000


void SER_init(void);


int SER_exchange(char* command, char* ansver, int ansvers);


int SER_getChar (int uart);

int SER_putChar (char c) ;

int SER_putStringbyChar(char* str);

int SER_getline(char* strline ,char* stransver, int lineindex);

int SER_sms_send(char* string, char* phone, char* ansver);

uint32_t SER_read_nb(char* str);



int SER_getline_simple(char* strline ,char* stransver, int lineindex);

int SER_gprs_send(char* data, char* ansver );

void SER_proccess_cmd(char* str);

int SER_readStringByLine(char* str);

 
#endif // __SERIAL_H_
