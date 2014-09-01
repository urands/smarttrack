/*
*  Project: Smarttrack
*  Author: uran
*  Description: AT Proto
*  History:
*				12.08.14 create
*/
#ifndef __AT_PROTO_H__
#define __AT_PROTO_H__


/* Parameters */

// 0 - limitless timeout
#define AT_DEFAULT_TIMEOUT 0

#define ATCR 0x0D
#define ATLF 0x0A



/* Hardware implementation base function (level 0 (AL))*/

/*
* Write data
* return: size write data
*/
unsigned int atWrite(const char* buffer, unsigned int size, const unsigned int timeout );

/*
* Reader response binary
* return: size response data
*/
unsigned int atRead(char* buffer, unsigned int size, const unsigned int timeout );

/*NRD */
//unsigned int atNumResponse();


/* AT Base function  (level 1)*/

typedef enum 
{
	AT_DATA = 0,
	AT_OK = 1,
	AT_CME_ERROR = 2,
	AT_CMS_ERROR = 3,
	AT_TIMEOUT = 4,
	AT_ERROR = 5,
} AT_RESPONSE;

/*
* Write command to UART:
* with terminate string
*	cmd<CR>
* return true if success
*/
AT_RESPONSE atCommand(const char* cmd);

/*
* Reader response without
* omitted <CR><LF>
* return: AT_RESPONSE enum
*/
AT_RESPONSE atReponse(char* response, unsigned int* psize, const unsigned int timeout );



/* AT API (level 2)*/



/* AT API (level 3)*/


/*
* Test command on implementations
* Using: atExtTest("+FTPEXTPUT");
* return true if command exists and response if exists
*/
AT_RESPONSE atExtTest(const char* cmd, char* response );

AT_RESPONSE atExtRead(const char* cmd, unsigned int* psize, char* response );

AT_RESPONSE atExtWrite(const char* cmd, const char* value, unsigned int* psize, char* response );

AT_RESPONSE atExtExec(const char* cmd, const char* value, char* response );



#endif // !__AT_PROTO_H__
