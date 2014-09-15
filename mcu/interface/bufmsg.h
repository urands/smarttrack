#ifndef __BUFMSG_H__
#define __BUFMSG_H__


#define BUFMSG_MAX_BUF 3
#define BUFMSG_MAX_DATA 255


typedef struct { // Message object structure
	#ifdef __BUFMSG_ID_ENABLED_	
		unsigned long data_id;
	#endif
		char data[BUFMSG_MAX_DATA];	
} T_BUFMSG;



/*
* Initialize buffers
* return: size write data
*/
void bufInit( void );

/*
* Send data via GPRS and write to flash
* return: size of writen data (if 0 error or data not writen (lost))
*/
int bufSendData(char* data, unsigned int sz,  unsigned int timeout );

/*
* Recv data
* return: size recv or -1 if timeout 
*/
int bufRecvData(char* data, unsigned int sz, unsigned int timeout );



#ifdef __BUFMSGTEST__

/* Unit test */
void bufUnitTest( void );

#endif




#endif
