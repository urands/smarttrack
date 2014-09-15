/* 
*  Project: Smarttrack
*  Author: uran
*  Description: Retarget stdio 
*  History:
*  						12.08.14 create
*/
#ifndef __LEGACY_H__
#define __LEGACY_H__

#include <stm32f10x.h>


#define SER_MAX_WAIT_RECV_TIME 		20000000
#define SER_MAX_LINE_SIZE	 		255	

#define SERIAL_DELAY 0x10000


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



void tsk_lock(void);
void tsk_unlock(void);

int GGPS_param(char* instr, char* outstr, int paramindex);




#endif
