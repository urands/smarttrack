#include "gsm_task.h"
#include "gsm_control.h"

#include "../hal/at_serial.h"
#include "../hal/at_proto.h"
#include "../core/settings.h"

#include "../interface/bufmsg.h"

#include "wialon.h"

#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

osThreadId gps_thread_id;

/* Define low priority nmeaTask */
osThreadDef(gsmTask,osPriorityNormal,1,0);

/* GSM Task init */
void gsmInit( void ){
	
	atSerInit(174336); //38200 ~ speed
	gps_thread_id = osThreadCreate(osThread(gsmTask),0);
}


#ifdef WIALON_TRACKER			
int gsmWork( int flag ){
	int res = 0;
	char gsm[200];
	res = bufRecvData(gsm, sizeof(gsm), 20000 );
	if ( res > 0 ){
		  osDelay(50);
			//"#D#160914;151022;5000.6025;N;03769.6834;E;10;100;250;3;8.5;05;6;12.25,78.54;DRIVERCODE;TEST:1:5,TEXT:3:Test text string"
			if ( wialonSend(gsm) == WN_DATAOK ){
				return flag;
			}else{
				return -1;
			}
	}else{
		//Ping pong
		//Wialon ping #P#;
		return 8; //all ok but gps not;
	}
	//return flag; //no path this
}


int gsmSendStatus( int flag ){
	char gsm[30];
	if (flag == 1 ) return 1;
	strcpy(gsm, "#M#Something wrong");
	if (flag == 3 ) strcpy(gsm, "#M#Device reloaded");
	if (flag == 2 ) strcpy(gsm, "#M#Device reconnected");
	if (flag == 4 ) strcpy(gsm, "#M#Hundreds points send to server. All ok");
	if (flag == 0 ) strcpy(gsm, "#M#Device started. Wait GPS.");
	if (flag == 5 ) strcpy(gsm, "#M#No GPS...Waiting that...");
	if (flag == 6 ) strcpy(gsm, "#M#Restart device");
	if (flag == 7 ) strcpy(gsm, "#M#Coords not found. Device underground. Sleep...");
	
	if ( wialonSend(gsm) == WN_DATAOK ){
			//Go to sleep
			if ( flag == 7 ){	gsmPowerOff();	return -1;}
		
			flag = 1;
			return flag;
	}
	return -1;
}

#else

int gsmSendStatus( int flag ){
	return flag;
}

int gsmWork( int flag ){
	
	
		char gsm[255];
		unsigned int sz = sizeof(gsm);
	/*	osDelay(3000);
		sprintf(gsm,"GET /dev.php?devid=1 HTTP/1.1\r\nHost: miss-n.ru\r\nConnection: keep-alive\r\n");
		//strcpy(gsm,"GET /dev.php HTTP/1.1\r\nhost: miss-n.ru\r\nConnection: keep-alive\r\n" );
		if ( gsmSend(gsm, strlen(gsm), 10000 ) != AT_OK  ) return AT_ERROR;
		if ( atRead(gsm, sz, 10000) > 0 ){
				return AT_OK;
	
	 }
		*/
  int res = 0;
	res = bufRecvData(gsm, sizeof(gsm), 20000 );
	if ( res > 0 ){
		  osDelay(50);
			//"#D#160914;151022;5000.6025;N;03769.6834;E;10;100;250;3;8.5;05;6;12.25,78.54;DRIVERCODE;TEST:1:5,TEXT:3:Test text string"
			if ( gsmSend(gsm, strlen(gsm), 10000 ) != AT_OK  ) {
				bufSendData (gsm, strlen(gsm), 1000);
				return AT_ERROR;
			}
			if ( atRead(gsm, sz, 10000) > 0 ){
				return AT_OK;
	
			}
			return -1; //all ok but gps not;
	}else{
		//Ping pong
		//Wialon ping #P#;
		return AT_TIMEOUT; //all ok but gps not;
	}
	//return flag; //no path this
	 
	 
	 
	 
		
	 
	 osDelay(100000);

		return -1;
}



#endif







/*
* Task running
*/
void gsmTask(void const *arg){
	
	int i;
	unsigned int reloadcnt = 0;
	int flag = 0;
	int cnt = 0;
	int gps_fail = 0;
	
	osDelay(3000);
	//osSignalWait (0x01, osWaitForever);
	
	while(1){
		if (reloadcnt > 3 ){
			reloadcnt = 0;
			osDelay(1000);
			gsmPowerOff();
			flag = 6;
			//osSignalWait (0x01, osWaitForever);
		}
		
			//gsmPowerOff();

	gsmPowerOn();
		reloadcnt++;
	
	if ( gsmInitModem() == AT_OK ){
		osDelay(1000);
		if ( gsmInitGPRS() == AT_STATE_IP_STATUS ){
			while(1){
			osDelay(100);
			if ( gsmConnect(GSM_SERVER_IP,GSM_SERVER_PORT) == AT_STATE_IP_CONNECT ){
				  osDelay(100);
				#ifdef WIALON_TRACKER	
					if ( wialonLogin (GSM_SERVER_IMEI, GSM_SERVER_PASSWORD ) == WN_LOGIN ){
				#endif
						
				
						flag = gsmSendStatus( flag );
						if ( flag < 0 ) break;
						
						flag = gsmWork( flag );
						if ( flag < 0 ) break;

#ifdef WIALON_TRACKER						
						 
						if ( flag == 8 ) gps_fail++;	else { gps_fail = 0; cnt++; }		//No GPS? add that
						if (cnt > 100 ) {flag = 4; cnt = 0;}	//Some message 100point
						if (cnt == 0 && flag == 1 ) flag = 5;	//No GPS	
						if (gps_fail > 50 ) flag = 7;						
						
#endif
								
			//		}		
#ifdef WIALON_TRACKER	
					flag = 2;
				}
#endif
			}
		}
		}
	}
	flag = 3;
	//gsm[254] = 0;
	}
	
	
}
