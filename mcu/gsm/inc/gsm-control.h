/*----------------------------------------------------------------------------
 * Name:    gsm tasks
 * Purpose: LPC1768 gsm asks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
 
 #ifndef __GSM_CONTROL_H
 #define __GSM_CONTROL_H
 
// #include "lpc_types.h"

#include <stm32f10x.h>
 
 
typedef struct GSM_Parameters{
	 uint8_t csq;
	 
	 
	 
 }GSM_Parameters;

 void GPRS_enable(int on_off);


 int GSM_setup(void);

 int GPRS_init(void);


 int GPRS_gprs_attach(void);
 
 int GPRS_gsm_attach(void);
 
 int GPRS_connect(char* host, int port, char* ansver );

 int GPRS_close(char* ansver);
 
 int GPRS_state(char* command, uint8_t line_ansvers, char* ansver);
 
 int GPRS_wait_state(char* command, uint8_t line_ansvers, char* ansver);
 
 int GPRS_test_wialon(char* ansver);
 
 int GPRS_Wialon_login(char* login, char* passwd, char* ansver);
 
 int GPRS_send(char* string, char* ansver);
 
 int GPRS_update_parameters(GSM_Parameters* param);
 
 int GPRS_getstatus(char * state);
 
 int GPRS_getstatus_wait(char* state);
 
 int GSM_readsettings(void);
 
 int GSM_update_sms( void );



 
 #endif // __GSM_CONTROL_H
