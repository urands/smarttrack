/*
*  Project: Smarttrack
*  Author: uran
*  Description: AT Proto
*  History:
*				12.08.14 create
*/
#include "cmsis_os.h"		/* RTL OS declaration*/
#include "at_proto.h"
#include "at_serial.h"
#include <string.h>

#ifdef SMART_TESTUNIT
#include <stdio.h>
#endif




/*
* Write data
* return: size write data
*/
unsigned int atWrite(const char* buffer, unsigned int size, const unsigned int timeout){
#ifdef SMART_TESTUNIT
	unsigned int i;
	for (i = 0; i < size; ++i){
			printf("%c",buffer[i]);
	}
	return size;
#else
	return atSerWrite( (char*) buffer, size, timeout );
#endif
	
}

/*
* Reader response binary
* return: size response data
*/
unsigned int atRead(char* buffer, unsigned int size, const unsigned int timeout){

#ifdef SMART_TESTUNIT
	scanf_s("%s", buffer);
	return strlen(buffer);
	
#else
	return atSerRead( buffer, size, timeout );
#endif
}

/*NRD */
unsigned int atNumResponse(){
	return 0;
}



/* AT Base function  (level 1)*/

/*
* Write command to UART:
* with terminate string
*	cmd<CR>
* return true if success
*/
AT_RESPONSE atCommand(const char* cmd){
	char symbol = ATCR;
	atWrite(cmd, strlen(cmd), AT_DEFAULT_TIMEOUT);
	osDelay(50);
	atWrite(&symbol, 1, AT_DEFAULT_TIMEOUT);
	osDelay(50);
	return AT_OK;
}

/*
* Reader response without
* omitted <CR><LF>
* return: AT_RESPONSE enum
*/
AT_RESPONSE atReponse(char* response, unsigned int* psize, const unsigned int timeout){
	int i;
	*psize = atSerReponse( response, *psize, timeout );
	if ( *psize == 0 ) return AT_TIMEOUT;
	if ( *psize > 4 ){
		for (i=2;i< (*psize)-2;i++) response[i-2] = response[i];
		response[i-2] = 0;
		*psize = i-2;
		if ( strcmp("OK", response) == 0 ) return AT_OK;
		if ( strcmp("ERROR", response) == 0 ) return AT_ERROR;
		return AT_DATA;
	}
	return AT_ABNORMAL;
}



/* AT API (level 2)*/
char int_cmd[100];
char int_str[255];
AT_RESPONSE atExchange(const char* cmd, char* response, unsigned int*psize, const unsigned int timeout){
	
	AT_RESPONSE resp = AT_ABNORMAL;
	int it = 0;
	unsigned int sz = sizeof(int_str);
	strcpy(int_cmd,cmd);
	if ( atCommand( cmd ) == AT_OK){
		while( (resp = atReponse( int_str, &sz, timeout )) == AT_DATA ){
			if ( response != 0 ){
				if ( it + sz > *psize ) sz = *psize - it;
				if ( sz == 0 ) return AT_ABNORMAL;
				if (it > 0 ) response[it++] = '\n';
				memcpy( response + it, int_str, sz + 1); it+=sz;
			}
			sz = sizeof(int_str);
		}
	}
	return resp;
}


/* AT API (level 3)*/


AT_RESPONSE atExtention(const char* cmd, char* response, unsigned int size, const unsigned int timeout){
	AT_RESPONSE resp = AT_ABNORMAL;
	int i, pos =0;
	if ( atWrite("AT",2,1000) > 0 ){
			resp =  atExchange(cmd, response, &size, timeout);
			if ( resp == AT_OK){
				for (i = 0; i< strlen( cmd ); i++){
					if ( response[i] == ':' ) {
						pos = i + 1;
						if ( response[i+1] == ' ' ) pos++;
						break;
					}
					if ( response[i] != cmd[i] ){
						pos = -1;
						break;
					}
				}
				if (pos <=0 ) return AT_ABNORMAL;
					for (i = pos; i< size + 1; i++){
							response[i-pos] = response[i];
					}
			}
	}
	return resp;
	
}


/*
* Test command on implementations
* Using: atExtTest("+FTPEXTPUT");
* return true if command exists and response if exists
*/
AT_RESPONSE atExtTest(const char* cmd, char* response, unsigned int size, const unsigned int timeout){
	char cmdext[16];
	strcpy(cmdext,cmd);
	strcat(cmdext,"=?");
	return atExtention(cmdext,response,size,timeout);
}

AT_RESPONSE atExtRead(const char* cmd, char* response,unsigned int size, const unsigned int timeout){
	char cmdext[16];
	strcpy(cmdext,cmd);
	strcat(cmdext,"?");
	return atExtention(cmdext,response,size,timeout);
}

AT_RESPONSE atExtWrite(const char* cmd, const char* value, const unsigned int timeout){
	char cmdext[100];
	AT_RESPONSE resp = AT_ABNORMAL;
	strcpy(cmdext,"AT");
	strcat(cmdext,cmd);
	strcat(cmdext,"=");
	strcat(cmdext,value);
	resp =  atExchange(cmdext, 0, 0, timeout);
	return resp;
	
}

AT_RESPONSE atExtExec(const char* cmd, char* response, unsigned int size, const unsigned int timeout){
	char cmdext[30];
	AT_RESPONSE resp = AT_ABNORMAL;
	strcpy(cmdext,"AT");
	strcat(cmdext,cmd);
	resp =  atExchange(cmdext, response, &size, timeout);
	return resp;
}


AT_RESPONSE atSend(char* data, unsigned int size, const unsigned int timeout){
	/* send activate */
	char ansver[16];
	unsigned int sz = sizeof(ansver);
	osDelay(100);
	if ( atWrite("AT+CIPSEND\r",11,timeout) > 0 ){
		ansver[0] = 0;
		while (ansver[0] != '>') if ( atSerRead( ansver, 1, timeout ) == 0 ){ return AT_TIMEOUT ; };
			
			//send console active
			if ( ansver[0] == '>'){
				atSerFlushRx();
				osDelay(50);
				atSerFlushRx();
				//sending data
				if ( atWrite(data,size,timeout) > 0 ){
					ansver[0] = 0x0D;
					ansver[1] = 0x0A;
					ansver[2] = 0x1A;
 				 //close sending console
				if ( atWrite(ansver,3,timeout) > 0 ){ }
					  sz = sizeof(ansver);
						if ( atReponse(ansver,&sz, timeout) == AT_DATA){
								if ( strcmp("SEND OK", ansver) == 0) return AT_OK;
						}
						return AT_TIMEOUT;
				}
			}
			return AT_ABNORMAL;
	}
	return AT_ERROR;
}

AT_RESPONSE atRecv(char* data, unsigned int* size, const unsigned int timeout){
	
	*size = atSerRecvLine ( data, *size, timeout );
	if (*size > 2 ) {
		if ( data[ (*size) -1 ] == '\n' && data[ (*size) - 2 ] == '\r' )	return AT_OK;
		return AT_ERROR;
	}
	return AT_TIMEOUT;
}



