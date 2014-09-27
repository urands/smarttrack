#ifndef __GSM_CONTROL_EXT_H__
#define __GSM_CONTROL_EXT_H__

#include "../hal/at_proto.h"


#define GSM_TIMEOUT 5000

/* Wakeup GSM modem */
void gsmPowerOn( void );

/* Suspend GSM modem */
void gsmPowerOff( void );


AT_RESPONSE gsmInitModem( void );


AT_RESPONSE gsmInitGPRS( void );

AT_RESPONSE gsmResetGPRS( void );

AT_RESPONSE gsmSetupAPN( void );

AT_RESPONSE gsmNetworkStatus( void );

AT_RESPONSE gsmConnect(char* host, int port );


AT_RESPONSE gsmSend(char* data, int sz, const unsigned int timeout );






#endif
