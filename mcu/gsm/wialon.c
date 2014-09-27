#include "wialon.h"
#include "gsm_control.h"
#include <stdio.h>
#include <string.h>


WN_RESPONSE wialonLogin(const char* user,const char* passwd){
	char cmd[50];
	char ansver[100];
	unsigned int sz = sizeof(ansver); 
  sprintf(cmd,"#L#%s;%s", user, passwd); 

	if ( gsmSend( cmd, strlen(cmd), 15000 ) == AT_OK){
			if ( atRecv(ansver, &sz, 15000) == AT_OK ){
				
				return wialonCheck( ansver, sz );
			}
	}
	return WN_ERROR;
}


WN_RESPONSE wialonCheck(char* ansver, const unsigned int size){
	
	ansver[size] = 0;
	
	if ( strcmp("#AL#1\r\n",ansver) == 0 ) return WN_LOGIN;
	if ( strcmp("#ASD#1\r\n",ansver) == 0 ) return WN_DATAOK;
	if ( strcmp("#AD#1\r\n",ansver) == 0 ) return WN_DATAOK;
	if ( strcmp("#AM#1\r\n",ansver) == 0 ) return WN_DATAOK;
	return WN_ERROR;
}

WN_RESPONSE wialonSend(char* wialonstr){
	char ansver[100];
	AT_RESPONSE resp;
	unsigned int sz = sizeof(ansver); 
	if ( (resp = gsmSend( wialonstr, strlen(wialonstr), 10000 )) == AT_OK){
			if ( atRecv(ansver, &sz, 10000) == AT_OK ){
				return wialonCheck( ansver, sz );
			}
			return WN_ERROR;
	}
	return WN_ERROR;
}
