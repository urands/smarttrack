/*
*  Project: Smarttrack
*  Author: uran
*  Description: AT Proto
*  History:
*				12.08.14 create
*/
#ifndef __AT_SERIAL_H__
#define __AT_SERIAL_H__


/* Hardware implementation base function (level 0 (AL))*/


/*
* Write data
* return: size write data
*/
unsigned int atSerInit( unsigned int  baudrate);

/*
* Write data
* return: size write data
*/
unsigned int atSerWrite(char* buffer, unsigned int size, const unsigned int timeout );

/*
* Reader response binary
* return: size response data
*/
unsigned int atSerRead(char* buffer, unsigned int size, const unsigned int timeout );


#ifndef __RELEASE__
/* Unit test */
void atSerUnitTest( void );

#endif


#endif
