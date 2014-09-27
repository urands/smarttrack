#include "gsm_control.h"
#include "../hal/at_serial.h"
#include "../gpio/gpio.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>


/* Wakeup GSM modem */
void gsmPowerOn( void ){
	char buf[9];

	//check power on (echo AT)
	if ( atExchange("AT", 0, 0, GSM_TIMEOUT) != AT_OK ){
		ctrlGsmPowerKey(1);
		osDelay(1000);
		ctrlGsmPowerKey(0);
		osDelay(1000);
		atSerFlushRx();
	  //load boudrate autocalibration test
		buf[0] = 0;	buf[1] = 0x49;	buf[2] = 0x49;	buf[3] = 0x49;	buf[4] = 0x49;	buf[5] = 0xFF;	buf[6] = 0xFF;	buf[7] = 0xFF;	buf[8] = 0xFF;	
		atSerWrite(buf,9,GSM_TIMEOUT);
		
		atSerFlushRx();
	}
	osDelay(1000);
	atSerFlushRx();
}

/* Suspend GSM modem */
void gsmPowerOff( void ){
	//check power on (echo AT)
	if ( atExchange("AT", 0, 0, GSM_TIMEOUT) == AT_OK ){
			ctrlGsmPowerKey(1);
			osDelay(GSM_TIMEOUT);
			ctrlGsmPowerKey(0);
			osDelay(GSM_TIMEOUT);
			//atSerFlushRx();
	}
	osDelay(1000);
	atSerFlushRx();
}

/*************************************************************************
*  AT Init modem
*  Check simcard
*************************************************************************/
AT_RESPONSE gsmInitModem( void ){
	char response[100];
	int it = 0;
	/* Simple test UART connection */
	if ( atExchange("AT", 0, 0, GSM_TIMEOUT) != AT_OK ) return AT_ERROR;
	
	/* Enable hardware flow control */
//	if ( atExtWrite("+IFC","2,2", GSM_TIMEOUT) != AT_OK ) {
		/* Enable software flow control */
//		if ( atExtWrite("+IFC","1,1", GSM_TIMEOUT) != AT_OK ) {
//			return AT_ERROR;	
///		}
//	}
	
	/* Disable echo (acselerate calculation) */
	if ( atExchange("ATE0", 0, 0, GSM_TIMEOUT) != AT_OK ) return AT_ERROR;
	
	/* Simple test UART connection again after new UART settings*/
	if ( atExchange("AT", 0, 0, GSM_TIMEOUT) != AT_OK ) return AT_ERROR;
	
	/* Check SIM PIN or PUK */
	it = 0;
	while( atExtRead("+CPIN",response,sizeof(response), GSM_TIMEOUT) != AT_OK ){				
		// Animate SIM CARD ERROR (inser valid simcart)
		osDelay(100);
		it++;
		if ( it > 100 ) return AT_ERROR;
	}	
	
	if ( strcmp( response, "READY")!= 0 ) {
		//Animate
		return AT_ERROR; //Need enter SIM PIN OR PUK
	}
	
	/* Get IMEI of phone */
	if ( atExtExec("+GSN",response,sizeof(response), GSM_TIMEOUT) != AT_OK ) return AT_ERROR;
	
	//TODO:save them
	

	
	
	/* Check registration in home network */
	for(it=0; it<100; ++it) { // 10sec wait
		while( atExtRead("+CREG",response,sizeof(response), GSM_TIMEOUT) != AT_OK ){				
			// Animate Network ERROR (go to home)
			osDelay(100);
		}	
		if ( strcmp(response, "0,1") == 0 ) break;
		osDelay(100);
	}
	if ( strcmp(response, "0,1") != 0 ) return AT_ERROR;
	
	
	/* Check functionality of phone */
	if ( atExtRead("+CFUN",response,sizeof(response), GSM_TIMEOUT) != AT_OK ) return AT_ERROR;
	if ( strcmp(response, "1") != 0 ) return AT_ERROR;;
	
	/* Get CIMI of mobile operator phone */
	if ( atExtExec("+CIMI",response,sizeof(response), GSM_TIMEOUT) != AT_OK ) return AT_ERROR;
	//and save them
	
	/* Get strenge GSM signal for test */
	if ( atExtExec("+CSQ",response,sizeof(response), GSM_TIMEOUT) != AT_OK ) {
		return AT_ERROR;
	}

	
	return AT_OK;
}

/****************************************************************************
*
*
*****************************************************************************/
AT_RESPONSE gsmInitGPRS( void ){
	char response[100];
	unsigned int it = 0;
	AT_RESPONSE resp;

	/* Wait GPRS registration */
	for(it=0; it<1000; ++it) { // 100sec wait
		if( atExtRead("+CGATT",response,sizeof(response), GSM_TIMEOUT) != AT_OK ){				
			return AT_ERROR; // Animate Network ERROR (go to home)
		}	
		if ( strcmp(response, "1") == 0 ) break;
		osDelay(100);
	}
	if ( strcmp(response, "1") != 0 )	return AT_ERROR;
	
	
	/* Read phone activity status */
	if ( atExtExec("+CPAS",response,sizeof(response), GSM_TIMEOUT) != AT_OK ) {
		return AT_ERROR;
	}
	//4 -call in progress 3 - ringing
	if ( strcmp(response, "+CPAS: 0") != 0 )	return AT_ERROR;
	
	/* Reset PDP context (deactivate gprs) wiat*/
	if ( gsmResetGPRS() != AT_OK ) {
		return AT_ERROR;
	}
	
	/* Read APN configuration */
	if( atExtRead("+CSTT",response,sizeof(response), GSM_TIMEOUT) != AT_OK ){				
			return AT_ERROR; // Animate Network ERROR (go to home)
	}	
	/* Setup APN configuration */
	if ( gsmSetupAPN() != AT_OK ) {
		return AT_ERROR;
	}
	
	/* Read APN configuration */
	if( atExtRead("+CSTT",response,sizeof(response), GSM_TIMEOUT) != AT_OK ){				
			return AT_ERROR; // Animate Network ERROR (go to home)
	}	
	
	
		/* Read APN configuration */
	if( atExtRead("+CIPCSGP",response,sizeof(response), GSM_TIMEOUT) != AT_OK ){				
			return AT_ERROR; // Animate Network ERROR (go to home)
	}	
	
			/* Read APN configuration */
	if( atExtRead("+CIPSPRT",response,sizeof(response), GSM_TIMEOUT) != AT_OK ){				
			return AT_ERROR; // Animate Network ERROR (go to home)
	}	
	
	
	/* Status connection */
	if ( gsmNetworkStatus() == AT_STATE_IP_START ) {
		/* Bring up wireless connection */
		if ( ( resp = atExtExec("+CIICR",response,sizeof(response), GSM_TIMEOUT * 10)) != AT_OK ) {
			if ( resp == AT_ERROR ){
					gsmPowerOff();
			}
			return AT_ERROR;
		}
		/* Get IP address */
		if ( atExtExec("+CIFSR",response,sizeof(response), GSM_TIMEOUT) != AT_OK ) {
			//return AT_ERROR;
		}
		
		return  gsmNetworkStatus();
	
}
	return AT_ERROR;
}


AT_RESPONSE gsmResetGPRS( void ){
	char ansver[15];
	AT_RESPONSE resp = AT_ABNORMAL;
	unsigned int sz = sizeof(ansver);
	strcpy(ansver,"AT+CIPSHUT");
	osDelay(10);
	if ( atCommand( ansver ) == AT_OK){
		if( (resp = atReponse( ansver, &sz, GSM_TIMEOUT )) == AT_DATA ){
			if ( sz > 0 ){
				if ( strcmp(ansver, "SHUT OK" ) == 0 ) return AT_OK;
			}
			return AT_ERROR;
		}
	}
	return resp;
}


AT_RESPONSE gsmNetworkStatus( void ){
	char ansver[40];
	AT_RESPONSE resp = AT_ABNORMAL;
	unsigned int sz = sizeof(ansver);
	if ( atCommand( "AT+CIPSTATUS" ) == AT_OK){
		if( (resp = atReponse( ansver, &sz, GSM_TIMEOUT )) == AT_OK ){
			sz = sizeof(ansver);
			if( (resp = atReponse( ansver, &sz, GSM_TIMEOUT )) == AT_DATA ){
				if ( strcmp("STATE: IP INITIAL", ansver) == 0 ) return AT_STATE_IP_INIT;
				if ( strcmp("STATE: IP START", ansver) == 0 ) 	return AT_STATE_IP_START;
				if ( strcmp("STATE: IP CONFIG", ansver) == 0 ) 	return AT_STATE_IP_CONFIG;
				if ( strcmp("STATE: IP GPRSACT", ansver) == 0 ) return AT_STATE_IP_GPRSACT;
				if ( strcmp("STATE: IP STATUS", ansver) == 0 ) 	return AT_STATE_IP_STATUS;
				if ( strcmp("STATE: TCP CONNECTING", ansver) == 0 ) return AT_STATE_IP_TCP_CONNECTING;
				if ( strcmp("STATE: CONNECT OK", ansver) == 0 ) return AT_STATE_IP_CONNECT;
				if ( strcmp("STATE: TCP CLOSING", ansver) == 0 ) return AT_STATE_IP_TCP_CLOSING;
				if ( strcmp("STATE: TCP CLOSED", ansver) == 0 ) return AT_STATE_IP_TCP_CLOSED;
				if ( strcmp("STATE: PDP DEACT", ansver) == 0 ) 	return AT_STATE_IP_PDP_DEACT;
				return AT_ABNORMAL;
			}
		}
	}
	return resp;
}

AT_RESPONSE gsmSetupAPN( void ){
	//Read CIMI
	char response[100];
	char value[100];
	unsigned int it = 0;
	
	//if MTS
	//strcpy(value,"\"internet.mts.ru\",\"mts\",\"mts\"");
	strcpy(value,"\"internet.beeline.ru\",\"beeline\",\"beeline\"");
	return atExtWrite("+CSTT",value, GSM_TIMEOUT);
	
}


AT_RESPONSE gsmConnect(char* host, int port ){
	AT_RESPONSE resp;
	char value[40];
	unsigned int sz = sizeof(value);

	resp = gsmNetworkStatus();
	
	if ( resp == AT_STATE_IP_STATUS || resp == AT_STATE_IP_TCP_CLOSED ){
			  sprintf(value,"\"TCP\",\"%s\",\"%i\"", host, port);
				//Connecting....
				if ( atExtWrite("+CIPSTART",value, GSM_TIMEOUT) == AT_OK ){
					//Wait connection
					if( (resp = atReponse( value, &sz, GSM_TIMEOUT*3 )) == AT_DATA ){
							if ( strcmp("CONNECT OK", value) == 0 ) return AT_STATE_IP_CONNECT;
							if ( strcmp("ALREADY CONNECT", value) == 0 ) return AT_STATE_IP_CONNECT;
					}
				}
			
				return gsmNetworkStatus();
	}
	while ( (resp = gsmNetworkStatus()) == AT_STATE_IP_TCP_CONNECTING ) osDelay(1000);
	return resp;
}

AT_RESPONSE gsmSend(char* data, int sz, const unsigned int timeout ){
	AT_RESPONSE resp = gsmNetworkStatus();
	if ( resp == AT_STATE_IP_CONNECT ){
			return atSend(data, sz, timeout );
	}
	return resp;
}





