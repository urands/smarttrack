/*----------------------------------------------------------------------------
 * Name:    gsm tasks
 * Purpose: LPC1768 gsm asks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
 
#include "inc/gsm-control.h"
#include "../gpio/gpio.h"
#include "../core/inc/toolset.h"
//#include "serial.h"
#include "inc/gsm-task.h"
//#include "lpc17xx_uart.h"
//#include "accel.h"
#include "../core/settings.h"
#include <string.h>
#include <stdio.h>


#include "../core/legacy.h"


char g_ansver_gsm[255];

t_device_settings g_device_settings;


/******************************************************************************
 @brief: Function: setup and test UART1 <-> GMS modem connection
 @return: OK if connection establish
******************************************************************************/
int GSM_establish_connection(void){

//RTX WAIT need here
	TOOL_delay_nl(200);
//  Wait incoming
	//len = SER_getChar_nbtm(1000000);

//Startup auto baudrate sent
	SER_putChar(0x00);
	SER_putChar(0x49);
	SER_putChar(0x49);
	SER_putChar(0x49);
	SER_putChar(0x49);
	SER_putChar(0xFF);
	SER_putChar(0xFF);
	SER_putChar(0xFF);
	SER_putChar(0xFF);
//Auto test speed
	TOOL_delay_nl(100);

	SER_putChar('A');
	SER_putChar('T');
	SER_putChar('E');
	SER_putChar('0');
	SER_putChar(0x0D);
	TOOL_delay_nl(100);
	SER_read_nb(g_ansver_gsm); //Flush buffer
	SER_read_nb(g_ansver_gsm); //Flush buffer
	SER_read_nb(g_ansver_gsm); //Flush buffer
   
	if ( SER_exchange("AT\r", g_ansver_gsm, 1) == OK ){
	//		GPIO_led(LED_GREEN); // AT test OK
	}else{
		//	GPIO_led(LED_RED);
	}
	
		if ( SER_exchange("AT\r", g_ansver_gsm, 1) == OK ){
	//		GPIO_led(LED_GREEN); // AT test OK
	}else{
		//	GPIO_led(LED_RED);
	}
	
	
		if ( SER_exchange("AT\r", g_ansver_gsm, 1) == OK ){
	//		GPIO_led(LED_GREEN); // AT test OK
	}else{
		//	GPIO_led(LED_RED);
	}
	
	
		if ( SER_exchange("AT\r", g_ansver_gsm, 1) == OK ){
	//		GPIO_led(LED_GREEN); // AT test OK
	}else{
		//	GPIO_led(LED_RED);
	}
	//Check state IFC (Modem full control IFC 2,2)
	if ( SER_exchange("AT+IFC?\r", g_ansver_gsm, 2) == OK ){
	//		GPIO_led(LED_GREEN);
	}else{
	//		GPIO_led(LED_RED);
	}
	
	//Setup IFC 2,2 (manualy)
/*	SER_putChar('A');
	SER_putChar('T');
	SER_putChar('+');
	SER_putChar('I');
	SER_putChar('F');
	SER_putChar('C');
	SER_putChar('=');
	SER_putChar('2');
	SER_putChar(',');
	SER_putChar('2');
	SER_putChar(0x0D);
	TOOL_delay_nl(10);
	*/
#if AUTO_RTS_CTS_USE
	//Full control mode setup for pin
	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_RTS, ENABLE);
	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_CTS, ENABLE);
#else
//	UART_FullModemForcePinState(LPC_UART1, UART1_MODEM_PIN_RTS, ACTIVE);
#endif

	TOOL_delay_nl(30);
	
	//Read ansver (manual)
  SER_read_nb(g_ansver_gsm);
	//Test AT with full control
	if ( SER_exchange("AT\r", g_ansver_gsm, 1) == OK ){
	//	GPIO_led(LED_GREEN);
		//If success go to
		if (g_ansver_gsm[0] == 'O' && g_ansver_gsm[1] == 'K') return OK; 
	}else{
	//	GPIO_led(LED_RED);
	}
	return NOT;
}


void GPRS_enable(int on_off){
	if ( on_off == ON ){
	//  GPIO_gsm_pwron(1);
		TOOL_delay(100);
	//   GPIO_gsm_pwrkey(1);
		TOOL_delay(10);
	//	while (GPIO_gsm_status() == 0);
		TOOL_delay(300);
		SER_init();						//Setup GSM<->CPU Serial port configuration
	//	GPIO_led(LED_RED);
		if (GSM_establish_connection() == OK ) return;
		TOOL_delay(10);

	}else{
	//	GPIO_gsm_pwrkey(0);
		TOOL_delay(300);
	//  GPIO_gsm_pwron(0);
		TOOL_delay_nl(1000);
		//SER_deinit ();

	}

	//g_gprs_establish_connection = 0;
}

/******************************************************************************
 @brief: Function: setup  test GMS modem and read parameters
 @return: OK if setup success
******************************************************************************/
int GSM_setup(){


   if ( GPRS_state ("AT+CPIN?\r", 2, "+CPIN: READY") == NOT ){
	   TOOL_error(ERROR_SIM); // block thread
       return NOT;
	}

 // GPIO_led(LED_GREEN);
   TOOL_delay_nl(10);

   if ( SER_exchange("AT+CSQ\r", g_ansver_gsm, 2) == OK ){
   		//+CSQ
			//OK
   }else{
		 return NOT;
	 }
 
  if ( SER_exchange("AT+GSN\r", g_ansver_gsm, 2) == NOT ) return NOT;

  if (SER_getline_simple(g_imei,g_ansver_gsm,0) == 0 ) return NOT;
	//g_imei[15] = 0;
	 
	 
	 osDelay(5000);

   if ( GPRS_wait_state("AT+CREG?\r",2, "+CREG: 0,1" ) == NOT ) return NOT;

   if ( GPRS_state ("AT+CFUN?\r", 2, "+CFUN: 1") == NOT ) return NOT;


   if ( SER_exchange("AT+CIMI\r", g_ansver_gsm, 2) == NOT ) return NOT;
   if (SER_getline_simple(g_gsm_local,g_ansver_gsm,0) == 0 ) return NOT;
	 
	 g_gsm_local[5] = 0;
	 if (strcmp (g_gsm_local, "25001") == 0){
		strcpy(g_apn,"internet.mts.ru");
		strcpy(g_apn_usr,"mts");
		strcpy(g_apn_pwd,"mts");
	 }
	 
	 if (strcmp (g_gsm_local, "25099") == 0){
		strcpy(g_apn,"internet.beeline.ru");
		strcpy(g_apn_usr,"beeline");
		strcpy(g_apn_pwd,"beeline");
		}

	 if (strcmp (g_gsm_local, "25002") == 0){
		strcpy(g_apn,"megafonpro.ru");
		strcpy(g_apn_usr,"internet");
		strcpy(g_apn_pwd,"");
	 }
	 
	 
	 
/*
Russia MTS Moscow 250 01
  
Russia North-West GSM 250 02 
 
Russia Siberian Cellular 250 05
   
Russia Zao Smarts 250 07  
 
Russia Don Telecom 250 10  
 
Russia New Telephone Company 250 12  
 
Russia Far-Eastern Cellular 250 12
   
Russia Kuban GSM 250 13

Russia Uratel 250 39
   
Russia North Caucasian GSM 250 44  

Russia KB Impuls BeeLine 250 99 
*/


	 //TODO: add SIM reading 
	if ( SER_exchange("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r", g_ansver_gsm, 1) != OK ) return NOT;
	sprintf(g_gsm_local,"AT+SAPBR=3,1,\"APN\",\"%s\"\r", g_apn); 
	if ( SER_exchange(g_gsm_local, g_ansver_gsm, 1) != OK ) return NOT;
	sprintf(g_gsm_local,"AT+SAPBR=3,1,\"USER\",\"%s\"\r", g_apn_usr); 
	if ( SER_exchange(g_gsm_local, g_ansver_gsm, 1) != OK ) return NOT;
	sprintf(g_gsm_local,"AT+SAPBR=3,1,\"PWD\",\"%s\"\r", g_apn_pwd); 
	if ( SER_exchange(g_gsm_local, g_ansver_gsm, 1) != OK ) return NOT;
	
	sprintf(g_gsm_local,"AT+CSTT=\"%s\",\"%s\",\"%s\"\r", g_apn, g_apn_usr, g_apn_pwd);
  if ( GPRS_state( g_gsm_local, 1, "OK") == NOT )  return NOT;



//sms settings
	if ( SER_exchange("AT+CSCA?\r", g_ansver_gsm, 2) == NOT ) return NOT;
	if ( SER_exchange("AT+CSCS?\r", g_ansver_gsm, 2) == NOT ) return NOT;
	if ( SER_exchange("AT+CSMP?\r", g_ansver_gsm, 2) == NOT ) return NOT;

	

	//SER_sms_send("Privet ot sim900", "+79064569144" g_ansver_gsm);
	
	GSM_readsettings();
	 
	 return OK;
}



int GPRS_location(void){
	 
	if ( SER_exchange("AT+SAPBR=1,1\r", g_ansver_gsm, 1) != OK ) return NOT;
	if ( SER_exchange("AT+SAPBR=2,1\r", g_ansver_gsm, 2) != OK ) return NOT;
		
	
	
	
	 if ( SER_exchange("AT+CIPGSMLOC=1,1\r", g_ansver_gsm, 2) == OK ){
   		//+CSQ
			//OK
   }else{
		 return NOT;
	 }
	 
	 if ( SER_exchange("AT+SAPBR=0,1\r", g_ansver_gsm, 1) != OK ) return NOT;
	
	return OK;
}


int GPRS_wait_state(char* command, uint8_t line_ansvers, char* ansver){
   while (GPRS_state(command, line_ansvers, ansver) == NOT){
   //  	GPIO_led(LED_GREEN);
	   	TOOL_delay_nl(100);
	//	GPIO_led(LED_RED);
	}
   return OK;
}

int GPRS_state(char* command, uint8_t line_ansvers, char* ansver){
   	if ( SER_exchange(command, g_ansver_gsm, line_ansvers) == NOT ) return NOT;
	if (SER_getline_simple(g_gsm_work,g_ansver_gsm,0) == 0 ) return NOT;
	if (strcmp (g_gsm_work, ansver) == 0){
		 return OK;
	}
	return NOT;


}

int GPRS_gprs_attach(void){
	return GPRS_state("AT+CGATT?\r", 2, "+CGATT: 1");
}

int GPRS_gsm_attach(void){
	return  GPRS_state("AT+CREG?\r",2, "+CREG: 0,1" );
}


int GPRS_init(void){
//  int i;
  //Wait while GPRS or EDGE has be avilable
  while (GPRS_gprs_attach() == NOT ){
 // 	GPIO_led(LED_GREEN);
   	TOOL_delay_nl(50);
//	GPIO_led(LED_RED);
  }
  TOOL_delay_nl(100);
 //Read GPRS status
  if (GPRS_state("AT+CPAS\r", 2, "+CPAS: 0") == NOT) return NOT;

  //Reset PDP
  if ( GPRS_state("AT+CIPSHUT\r",1, "SHUT OK" ) == NOT ) return NOT;



  if (GPRS_state("AT+CPAS\r", 2, "+CPAS: 0") == NOT) return NOT;

  if ( SER_exchange("AT+CSTT?\r", g_ansver_gsm, 2) == NOT )  return NOT;
 
  sprintf(g_gsm_local,"AT+CSTT=\"%s\",\"%s\",\"%s\"\r", g_apn, g_apn_usr, g_apn_pwd);
  if ( GPRS_state( g_gsm_local, 1, "OK") == NOT )  return NOT;

//  if ( SER_exchange("AT+CSTT?\r", g_ansver_gsm, 2) == NOT )  return NOT;
 // if ( SER_exchange("AT+CGDCONT?\r", g_ansver_gsm, 2) == NOT )  return NOT;
	//Define PDP content ?  DEL
	
	
  //if ( SER_exchange("AT+CGACT?\r", g_ansver_gsm, 2) == NOT )  return NOT;
	//Activate PDP DEL	 +CGACT 3,0 error
	
	
  if ( SER_exchange("AT+CIPCSGP?\r", g_ansver_gsm, 2) == NOT )  return NOT;
	//? DEL
	
  if ( SER_exchange("AT+CIPSPRT?\r", g_ansver_gsm, 2) == NOT )  return NOT;
	//? DE:
	
//  if ( SER_exchange("AT+CIPSPRT=0\r", g_ansver_gsm, 1) == NOT )  return NOT;
	//Set promt > without > (need)
	
	//Check status to IP START wait
	//TODO:
	GPRS_getstatus("");
   if( GPRS_getstatus_wait("STATE: IP START") == NOT ) return NOT;
	
	//Bring up wireless connection (IP CONFIG)
  if ( GPRS_state("AT+CIICR\r",1 , "OK" ) == NOT)  return NOT;

  if( GPRS_getstatus_wait("STATE: IP GPRSACT") == NOT ) return NOT;
	
	//Check status to IP CONFIG wait
	//TODO:
//	if ( SER_exchange("AT+CIPSTATUS?\r", g_ansver_gsm, 2) == NOT )  return NOT;
	
	
	
	//Check status to IP GPRSACT wait
	//TODO:
//	if ( SER_exchange("AT+CIPSTATUS?\r", g_ansver_gsm, 2) == NOT )  return NOT;
	
  
	//Read local IP (for test)
	if ( SER_exchange("AT+CIFSR\r", g_ansver_gsm, 1) == NOT )  return NOT;
	
	
	//GPRS ACTIVATE
	
  //GPRS_location(); Update SIM900

	return OK;

}

int GPRS_getstatus_wait(char* state) {
	  while (GPRS_getstatus (state) == NOT){TOOL_delay_nl(10);};
	  return OK;

}

int GPRS_getstatus(char * state){
		if ( SER_exchange("AT+CIPSTATUS\r", g_ansver_gsm, 2) == NOT )  return NOT;
		if (SER_getline_simple(g_gsm_work,g_ansver_gsm,1) == 0 ) return NOT;
		if ( strcmp (g_gsm_work, state) == 0 ) return OK;
	    return NOT;
	
}

int GPRS_connect(char* host, int port, char* ansver ){
	//Check status to IP CONNECT OK
		//TODO:
	if (GPRS_getstatus("STATE: IP STATUS") == NOT && GPRS_getstatus("STATE: TCP CLOSED") == NOT) return NOT;
	//	if ( GPRS_getstatus() != 6 )  return NOT;
	
  sprintf(ansver,"AT+CIPSTART=\"TCP\",\"%s\",\"%i\"\r", host, port);
	if ( SER_exchange(ansver,g_ansver_gsm, 2) == NOT)  return NOT;
	if (SER_getline_simple(g_gsm_work,g_ansver_gsm,1) == 0 ) return NOT;
	if (strcmp (g_gsm_work, "CONNECT OK") == 0){
		
		//Read status CONNECT OK 6
		if ( GPRS_getstatus("STATE: CONNECT OK") != OK )  return NOT;
		 return OK;
	}
	return NOT;
}




int GPRS_send(char* string, char* ansver){
		//Check status to IP CONNECT OK
		//TODO:
		if ( GPRS_getstatus("STATE: CONNECT OK") != OK )  return NOT;
	
		if ( SER_gprs_send(string, ansver ) == NOT ) return NOT;

		//Check status to IP CONNECT OK
		//TODO:
		if ( GPRS_getstatus("STATE: CONNECT OK") != OK )  return NOT;
	  return OK;
}

int GPRS_ping(char* ansver){
		//Check status to IP CONNECT OK
		//TODO:
		if ( GPRS_getstatus("STATE: CONNECT OK") != OK )  return NOT;
		if (SER_gprs_send("#P#", ansver) == OK ){
	   return GPRS_test_wialon(ansver);
	}
	return NOT;

}

int GPRS_test_wialon(char* ansver){
	//#ASD#1\r\n  - All ok
	if ( strcmp ("#AL#1\r\n", ansver) == 0 ) return OK;
	if ( strcmp ("#ASD#1\r\n", ansver) == 0 ) return OK;
	if ( strcmp ("#AD#1\r\n", ansver) == 0 ) return OK;
	if ( strcmp ("#AP#\r\n", ansver) == 0 ) return OK;
	
	return NOT;
}

int GPRS_Wialon_login(char* login, char* passwd, char* ansver){
	char cmd[50];
	memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"#L#%s;%s", login, passwd); 
	if ( GPRS_getstatus("STATE: CONNECT OK") != OK )  return NOT;
//	   GPRS_open_transmition();
	  if (GPRS_send(cmd, ansver) == OK ){
//	  		GPRS_close_transmition();
			if ( GPRS_test_wialon(ansver) == OK ) return OK;
 		}
//	GPRS_close_transmition();
	return NOT;
}

int GPRS_get_param(char* dst_param, char* src_param, int param_num){
	int len,i;
	int c_param = -1;
	len = strlen(src_param);
	for (i=0;i<len;++i){
		if (c_param == -1 && src_param[i] == ':'){
			i++;
			c_param++;
			continue;
		}
		
		if (src_param[i] == ','){
			c_param++;
			continue;
		}
		if ( c_param == param_num ) *dst_param++=src_param[i];
			
		
	}
	*dst_param = 0;
	return OK;
	
}

int GPRS_close(char* ansver){
	if ( GPRS_getstatus("STATE: CONNECT OK") != OK )  return OK;
	if ( SER_exchange("AT+CIPCLOSE\r", ansver, 1) == NOT ) return NOT;
	return OK;
}


int GPRS_update_parameters(GSM_Parameters* param){
	 int value;
	 
	 //CSQ UPDATE
	 if ( SER_exchange("AT+CSQ\r", g_ansver_gsm, 2) == NOT ) return NOT;
	 if (SER_getline_simple(g_gsm_local,g_ansver_gsm,0) == 0 ) return NOT;
	 if (GPRS_get_param(g_ansver_gsm,g_gsm_local,0) == NOT ) return NOT;
	
	 sscanf(g_ansver_gsm, "%i", &value);
   param->csq = (uint8_t) value;
	/////////////////////////////////////////////
	 return OK;
}

int GSM_readphonebook(char* find){
	
	int len;
	sprintf(g_gsm_local,"AT+CPBF=\"%s\"\r", find);
	if ( SER_exchange(g_gsm_local , g_ansver_gsm, 1) == NOT ) return NOT;
	if ( SER_getline_simple(g_gsm_local, g_ansver_gsm,0) == NOT ) return NOT;
	
	if ( strcmp (g_gsm_local, "OK" )  != 0 ) {
			len = SER_readStringByLine(g_ansver_gsm); //LF RL
		  len = SER_readStringByLine(g_ansver_gsm); //OK LF RL
		  if ( len > 0) {
				if (GPRS_get_param(g_ansver_gsm,g_gsm_local,1) == NOT ) return NOT;
				if (g_ansver_gsm[0] == '"' ){
					memcpy(g_gsm_local, g_ansver_gsm+1,strlen(g_ansver_gsm)-2);
					g_gsm_local[strlen(g_ansver_gsm)-2] = 0;
					strcpy(g_ansver_gsm,g_gsm_local);
					
					
				}
				return OK;
			}
	}
	return NOT;
}

int GSM_readsettings(void){
	
	
	//Setup default settings
	strcpy (g_device_settings.phone, GSM_PHONE );
	strcpy (g_device_settings.password, GSM_SERVER_PASSWORD );
	strcpy(g_device_settings.server, GSM_SERVER_IP);
	g_device_settings.port = GSM_SERVER_PORT;
	
	if ( SER_exchange("AT+CPBS=?\r", g_ansver_gsm, 2) == NOT ) return NOT;
	
	if ( SER_exchange("AT+CPBS=\"SM\"\r", g_ansver_gsm, 1) == NOT ) return NOT;
	
	if ( SER_exchange("AT+CPBR=?\r", g_ansver_gsm, 2) == NOT ) return NOT;
	
	if ( GSM_readphonebook("password") == OK ){
			strcpy(g_device_settings.password, g_ansver_gsm);
	}
	if ( GSM_readphonebook("phone") == OK ){
			strcpy(g_device_settings.phone, g_ansver_gsm);
	}
	
	if ( GSM_readphonebook("server") == OK ){
		g_gsm_local[0] = g_ansver_gsm[0];
		g_gsm_local[1] = g_ansver_gsm[1];
		g_gsm_local[2] = g_ansver_gsm[2];
		g_gsm_local[3] = '.';
		g_gsm_local[4] = g_ansver_gsm[3];
		g_gsm_local[5] = g_ansver_gsm[4];
		g_gsm_local[6] = g_ansver_gsm[5];
		g_gsm_local[7] = '.';
		g_gsm_local[8] = g_ansver_gsm[6];
		g_gsm_local[9] = g_ansver_gsm[7];
		g_gsm_local[10] = g_ansver_gsm[8];
	  g_gsm_local[11] = '.';		
		g_gsm_local[12] = g_ansver_gsm[9];
		g_gsm_local[13] = g_ansver_gsm[10];
		g_gsm_local[14] = g_ansver_gsm[11];
		g_gsm_local[15] = 0;

		strcpy(g_device_settings.server, g_gsm_local);
		
	}
	
	if ( GSM_readphonebook("port") == OK ){
			sscanf(g_ansver_gsm, "%i", &g_device_settings.port);
	}
	
	GSM_update_sms();

}


int GSM_update_sms( void ){
//	if ( GSM_readsms(gsm_str) == OK ){
		
		// send gsm_str update params
		SER_sms_send(gsm_str, g_device_settings.phone, g_ansver_gsm);
		return OK;
		
//	}
	return NOT;
	
}


int GSM_readsms(char* datastr){
	
	int len;
	char smsindex[5];
	int dismiss = 0;
	if ( GPRS_state("AT+CMGF=1\r",1 , "OK" ) == NOT)  return NOT;
	
	if ( SER_exchange("AT+CMGL=\"REC UNREAD\",1\r" , g_ansver_gsm, 1) == NOT ) return NOT;
	
	if ( SER_getline_simple(g_gsm_local, g_ansver_gsm,0) == NOT ) return NOT;
	
	while ( strcmp (g_gsm_local, "OK" )  != 0 && strcmp (g_gsm_local, "ERROR" )  != 0) {
		  if (GPRS_get_param(smsindex,g_gsm_local,0) == NOT ) return NOT;
   		len = SER_readStringByLine(g_ansver_gsm); //LF RL
		  if ( SER_getline(g_gsm_local, g_ansver_gsm,0) == NOT ) return NOT;
		  if ( dismiss == 0 ){
				strcpy(datastr, g_gsm_local);
				dismiss = 1;
			}
			//Read ok or other line
			len = SER_readStringByLine(g_ansver_gsm); //LF RL
			len = SER_readStringByLine(g_ansver_gsm); //OK LF RL
		  if ( SER_getline(g_gsm_local, g_ansver_gsm,0) == NOT ) return NOT;

			
		  
	}
	if ( dismiss == 1) {
		sprintf(g_gsm_local,"AT+CMGD=%s\r", smsindex);
		if ( SER_exchange(g_gsm_local , g_ansver_gsm, 1) == NOT ) return NOT;
		return OK;
	}
	return NOT;
	
}
