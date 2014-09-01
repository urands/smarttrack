/*
*  Project: Smarttrack
*  Author: uran
*  Description: AT Proto
*  History:
*				12.08.14 create
*/
#include "at_proto.h"
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
	
#endif
	return size;
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
	
#endif
	return 0;
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
	atWrite(&symbol, 1, AT_DEFAULT_TIMEOUT);
	return 0;
}

/*
* Reader response without
* omitted <CR><LF>
* return: AT_RESPONSE enum
*/
AT_RESPONSE atReponse(char* response, unsigned int* psize, const unsigned int timeout);



/* AT API (level 2)*/



/* AT API (level 3)*/


/*
* Test command on implementations
* Using: atExtTest("+FTPEXTPUT");
* return true if command exists and response if exists
*/
AT_RESPONSE atExtTest(const char* cmd, char* response);

AT_RESPONSE atExtRead(const char* cmd, unsigned int* psize, char* response);

AT_RESPONSE atExtWrite(const char* cmd, const char* value, unsigned int* psize, char* response);

AT_RESPONSE atExtExec(const char* cmd, const char* value, char* response);


