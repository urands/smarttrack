#include "nmea_task.h"
#include "nmea_proto.h"
#include "../hal/nmea_serial.h"

#include "cmsis_os.h"
#include <string.h>



/* Define messages from GPS */
osPoolDef(nmeapool, NMEAMSG_MAX_BUF + 1, T_NMEA_PACKED); // Define memory pool
osPoolId nmeapool;
osMessageQDef(MsgNmea, NMEAMSG_MAX_BUF, T_NMEA_PACKED); // Define message queue
osMessageQId MsgNmea;


/* Define low priority nmeaTask */
osThreadDef(nmeaTask,osPriorityNormal,1,0);

/*
* Init and run task
*/
void nmeaInit(void){
	nmeaSerInit(43584); //9600
  nmeapool = osPoolCreate(osPool(nmeapool)); // create memory pool
	MsgNmea = osMessageCreate(osMessageQ(MsgNmea), NULL); // create msg queue

	osThreadCreate(osThread(nmeaTask),0);
}


/*
* Task running
*/
void nmeaTask(void const *arg){
	char nmea_str[255];	
	int  nmea_size = 0;
	T_NMEA_PACKED nmeaPack;
	enum nmeaSignals nmeaSig;
	while( 1 ){
			nmea_size = nmeaSerReadData(nmea_str,sizeof(nmea_str),1000);
			if ( nmea_size > 0 ){
				nmeaSig = nmeaUpdate( nmea_str, nmea_size, &nmeaPack);
				if ( nmeaSig != nmeaNoValid ){
						nmeaSendData( &nmeaPack, 1000);
				}
			}
	}
}


/*
* Send data via GPRS and write to flash
* timeout osWaitForever
* return: size of writen data (if 0 error or data not writen (lost))
*/
int nmeaSendData(T_NMEA_PACKED* data, unsigned int timeout ){
	T_NMEA_PACKED *mptr;
	osStatus res;
  mptr = osPoolAlloc(nmeapool); // Allocate memory for the message
	
	if ( mptr == 0) { //Resource unavailable
		//Burn data to flash
			return -1; 
	}
	memcpy( mptr, data, sizeof(T_NMEA_PACKED) );
	
	res = osMessagePut(MsgNmea, (uint32_t)mptr, timeout);
	
	if ( res == osOK) return (int)sizeof(T_NMEA_PACKED); // Send Message
	
	return -1;

}

/*
* Recv data
* return: size recv or -1 if timeout 
*/
int nmeaRecvData(T_NMEA_PACKED* data, unsigned int timeout ){
	osEvent evt;
  evt = osMessageGet(MsgNmea, timeout); // wait for message
	if (evt.status == osEventMessage) {
			memcpy( data, evt.value.p, sizeof(T_NMEA_PACKED) );
			osPoolFree(nmeapool, evt.value.p); // free memory allocated for message
			return sizeof(T_NMEA_PACKED);
	}
	return -1;
}

