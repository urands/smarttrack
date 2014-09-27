#ifndef __WIALON_H__
#define __WIALON_H__

#include "../hal/at_proto.h"
//#include "../gps/nmea_proto.h"


typedef enum{
	WN_OK  = 0,
	WN_ERROR = 1,
	WN_LOGIN = 2,
	WN_SEND = 3,
	WN_DATAOK = 4,
}  WN_RESPONSE;


WN_RESPONSE wialonLogin(const char* user,const char* passwd);

WN_RESPONSE wialonSend(char* string);

WN_RESPONSE wialonCheck(char* ansver, const unsigned int size);


//WN_RESPONSE wialonRender(T_NMEA_PACKED* pack, char* dataout)





#endif
