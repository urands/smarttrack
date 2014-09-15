#ifndef __NMEA_PROTO_H__
#define __NMEA_PROTO_H__

#define NMEAMSG_MAX_BUF 2

typedef struct{
	
	/* Date and time in UTC format */
	unsigned int time;
	char				 date[7];
	/* Indicators:
	*	 0 bit: 0-N=north, 1-S=south
	*	 1 bit: 0-E=east, 1-W=west
	*	 2 bit: 0- GP (GPS), 1-GN (GLONASS)
	*	 3 bit: 0-Autonomous, 1-DGPS
	*
	*/
	unsigned char			indicators; 

	/* latitude */
	double latitude;
	
	/* longitude */
	double longitude;
	
	/* speed in knots */
	float speedkn;
	
	/* speed in knots */
	float speedkm;


	/* course over ground */
	float course;
	
	/* horizontal dilution of precision */
	float hdop;
	
	/* altitude (vertical height) */
	float altitude;
	
	/* geoid separation (vertical height) */
	float geoid;

	/* count satelites */
	unsigned char			satelite; 

	
	
} T_NMEA_PACKED;


typedef struct{
	
	/* Date */
	char date[6];
	char time[6];
	char indicatorNS;
	char indicatorEW;
	char longitude[10];
	char latitude[9];
	/*
	*	 0 bit: 0-N=north, 1-S=south
	*	 1 bit: 0-E=east, 1-W=west
	*	 1 bit: 0-E=east, 1-W=west
	*
	*/
	unsigned char			indicators; 
	
	
	
} T_NMEA_ORIGIN;


enum nmeaSignals{
	nmeaNoValid = 0,
	nmeaGGA = 0x1,
	nmeaGLL = 0x2,
	nmeaGSA = 0x4,
	nmeaRMC = 0x8,
	nmeaVTG = 0x10,
	nmeaZDA = 0x20,
	
};


/* Get param wrom nmeamsg
*  return 0 or sizeof param string
*/
int nmeaGetParam(char* param, char* nmeamsg, unsigned char size, unsigned char numparam);


/* Get param wrom nmeamsg
*  return 0 or sizeof param string
*/
enum nmeaSignals nmeaUpdate(char* nmeamsg, unsigned char size, T_NMEA_PACKED* nmea_data);


/* Convertion function */
unsigned long int nmeaTimeStrToUTC(const char* timestr);
unsigned long int nmeaDateStrToUTC(const char* datestr);

/* Message exchange buffers */
int nmeaSendData(T_NMEA_PACKED* data, unsigned int timeout );
int nmeaRecvData(T_NMEA_PACKED* data, unsigned int timeout );

/* Get param wrom nmeamsg
*  return 0 or sizeof param string
*/
int nmeaToWialon(char* wialonmsg, char* nmeamsg, unsigned char size );


#endif

