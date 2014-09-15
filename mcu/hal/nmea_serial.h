/*
*  Project: Smarttrack
*  Author: uran
*  Description: AT Proto
*  History:
*				12.08.14 create
*/
#ifndef __NMEA_SERIAL_H__
#define __NMEA_SERIAL_H__


/* Hardware implementation base function (level 0 (AL))*/


/*
* Write data
* return: size write data
*/
unsigned int nmeaSerInit( unsigned int  baudrate);

/*
* Write data
* return: size write data
*/
unsigned int nmeaSerWrite(char* buffer, unsigned int size, const unsigned int timeout );

/*
* Reader response binary
* return: size response data
*/
unsigned int nmeaSerRead(char* buffer, unsigned int size, const unsigned int timeout );


/*
* Reader response binary
* return: size response data
*/
unsigned int nmeaSerReadLine(char* buffer, unsigned int size, const unsigned int timeout );


/*
* Reader response binary
* return: size response data
*/
unsigned int nmeaSerReadData(char* buffer, unsigned int size, const unsigned int timeout );

/*
* Test Checksum
* return: size response data
*/
unsigned int nmeaCheckSum(char* buffer, unsigned int size );


#ifndef __RELEASE__
/* Unit test */
void nmeaSerUnitTest( void );

#endif


#endif
