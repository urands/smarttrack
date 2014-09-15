/***************************************************
*
*
****************************************************/

#include "bufmsg.h"
#include <stdio.h>
#include "cmsis_os.h"
#include <string.h>




osPoolDef(bufpool, BUFMSG_MAX_BUF + 1, T_BUFMSG); // Define memory pool
osPoolId bufpool;
osMessageQDef(MsgBuf, BUFMSG_MAX_BUF, T_BUFMSG); // Define message queue
osMessageQId MsgBuf;

#ifdef __BUFMSG_ID_ENABLED_
/* Data identifiction number */
unsigned long bufDataId = 0;
#endif



void bufInit( void ){
	 bufpool = osPoolCreate(osPool(bufpool)); // create memory pool
	 MsgBuf = osMessageCreate(osMessageQ(MsgBuf), NULL); // create msg queue
}


/*
* Send data via GPRS and write to flash
* timeout osWaitForever
* return: size of writen data (if 0 error or data not writen (lost))
*/
int bufSendData(char* data, unsigned int sz,  unsigned int timeout ){
	T_BUFMSG *mptr;
	osStatus res;

	if ( sz > BUFMSG_MAX_DATA ) sz = BUFMSG_MAX_DATA;

  mptr = osPoolAlloc(bufpool); // Allocate memory for the message
	
	if ( mptr == 0) { //Resource unavailable
		//Burn data to flash
			return -1; 
	}

#ifdef __BUFMSG_ID_ENABLED_	
	mptr->data_id = bufDataId++;
#endif
	
	memcpy( mptr->data, data, sz );
	
	res = osMessagePut(MsgBuf, (uint32_t)mptr, timeout);
	
	if ( res == osOK) return (int)sz; // Send Message
	
	return -1;

}

/*
* Recv data
* return: size recv or -1 if timeout 
*/
int bufRecvData(char* data, unsigned int sz, unsigned int timeout ){
	osEvent evt;
	if ( sz > BUFMSG_MAX_DATA ) sz = BUFMSG_MAX_DATA;
	
  evt = osMessageGet(MsgBuf, timeout); // wait for message
	
	if (evt.status == osEventMessage) {
			memcpy( data, evt.value.p, sz );
			osPoolFree(bufpool, evt.value.p); // free memory allocated for message
			return sz;
	}
	return -1;
}

#ifdef __BUFMSGTEST__

void send_bufthread (void const *argument); // forward reference
void recv_bufthread (void const *argument); // forward reference

// Thread definitions
osThreadDef(send_bufthread, osPriorityLow, 1, 0);
osThreadDef(recv_bufthread, osPriorityNormal, 1, 0);

/*
Thread 1: Send thread
*/
void send_bufthread (void const *argument) {
	char sampledata[100];
	int id = 0;
	
	
	while (1){
		id++;
		sprintf(sampledata,"Sample data for write: %i", id);
		
		if ( bufSendData (sampledata, strlen(sampledata), 1000) < 0 ){
			
			//	assert_param(1);
			id = 25;
		}
	}
}


/*
Thread 2: Receive thread
*/
void recv_bufthread (void const *argument){
	char samplercvdata[100];
	int sz = 0;
	
	while(1){
		sz = bufRecvData(samplercvdata, sizeof(samplercvdata), 1000 );
		
		if (sz <= 0 ) {
			
			sz = 1;
			osThreadYield();
			//osDelay(1000);
			break;
		}
		
		
	}
	
	
}



/* Unit test */
void bufUnitTest( void ){
	
	bufInit();
	
	osThreadCreate(osThread(send_bufthread), NULL);
	osThreadCreate(osThread(recv_bufthread), NULL);
}

#endif
