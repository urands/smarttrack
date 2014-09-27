#include "nmea_proto.h"
#include <string.h>
#include <stdio.h>

/* Get param wrom nmeamsg
*  return 0 or sizeof param string
*/
int nmeaGetParam(char* param, char* nmeamsg, unsigned char size, unsigned char paramindex){
	int i, index = 0, wi =0;
	for (i =0 ; i< size; ++i ){
   if (nmeamsg[i] == ',' ) {
  	index++;
		continue;
		}
		if ( index == paramindex ){
			param[wi++] = nmeamsg[i];
		}
	}
	if ( index < paramindex ) return -1;
	param[wi] = 0;
  return wi;
}

/* Parse data from nmea protocol
*	 return signal
*/
enum nmeaSignals nmeaUpdate(char* nmeamsg, unsigned char size, T_NMEA_PACKED* nmeadata){
	char nmeacmd[8];
	char nmeastr[12];
	int  numval;
	// Test on valid data
	if ( nmeaGetParam(nmeacmd, nmeamsg, size, 0 ) <= 0 ) return nmeaNoValid;
	
	/* RMC data parse Recommended minimum specific GNSS data */
	if ( strcmp( "$GPRMC", nmeacmd) == 0 || strcmp( "$GNRMC", nmeacmd) == 0){
		/* Test on valid data */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 2 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] == 'V' )  return nmeaNoValid;
		
		/* Data conversions */
		//memset(nmeadata, 0, sizeof(T_NMEA_PACKED));
		//nmeadata->indicators = 0;
		
		if ( strcmp( "$GNRMC", nmeacmd) == 0 )   { nmeadata->indicators|= 0x4;} else {nmeadata->indicators&= ~0x4;}
		/* latitude */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 3 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%lf", &nmeadata->latitude);

		/* longitude */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 5 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%lf", &nmeadata->longitude);
		
		/* N/S indicator */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 4 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] == 'S' )  { nmeadata->indicators|= 0x1;} else {nmeadata->indicators&= ~0x1;}

		/* E/W indicator */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 6 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] == 'W' )  { nmeadata->indicators|= 0x2;} else {nmeadata->indicators&= ~0x2;}
		
		/* Mode */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 12 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] == 'D' ) { nmeadata->indicators|= 0x8;}else{ nmeadata->indicators&= ~0x8;}

		/* speed knots */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 7 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%f", &nmeadata->speedkn);
		nmeadata->speedkm = nmeadata->speedkn * 1.852;
			
		/* course */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 8 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%f", &nmeadata->course);
		
		/* timeconversion */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 1 ) <= 0 ) return nmeaNoValid;
		nmeadata->time = nmeaTimeStrToUTC(nmeastr);
		memcpy(nmeadata->times, nmeastr, 6);

		if ( nmeaGetParam(nmeastr, nmeamsg, size, 9 ) <= 0 ) return nmeaNoValid;
		memcpy(nmeadata->date, nmeastr, 6);
		//nmeadata->time+= nmeaDateStrToUTC(nmeastr);
		return nmeaRMC;
	}
	
	/* GPGGA data parse Fixed Data */
	if ( strcmp( "$GPGGA", nmeacmd) == 0 || strcmp( "$GNGGA", nmeacmd) == 0  ){
		/* Test on valid data */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 2 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] == '0' )  return nmeaNoValid;
		
		/* Data conversions */
		//memset(nmeadata, 0, sizeof(T_NMEA_PACKED));
		if ( strcmp( "$GNGGA", nmeacmd) == 0 )  { nmeadata->indicators|= 0x4;} else {nmeadata->indicators&= ~0x4;}
		
		/* latitude */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 2 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%lf", &nmeadata->latitude);

		/* longitude */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 4 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%lf", &nmeadata->longitude);
		
		/* N/S indicator */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 3 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] == 'S' )  { nmeadata->indicators|= 0x1;} else {nmeadata->indicators&= ~0x1;}

		/* E/W indicator */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 5 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] == 'W' )  { nmeadata->indicators|= 0x2;} else {nmeadata->indicators&= ~0x2;}
		
		/* Satilites */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 7 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%i", &numval); nmeadata->satelite = (unsigned char) numval;

		/* hdop */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 8 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%f", &nmeadata->hdop);
			
		/* altitude */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 9 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%f", &nmeadata->altitude);
		
		/* geoid */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 11 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%f", &nmeadata->geoid);
		
		/* timeconversion */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 1 ) <= 0 ) return nmeaNoValid;
		nmeadata->time = nmeaTimeStrToUTC(nmeastr);
		memcpy(nmeadata->times, nmeastr, 6);

		return nmeaGGA;
		
		
	}
	
		/* GPGGA data parse Fixed Data */
	if ( strcmp( "$GPVTG", nmeacmd) == 0 || strcmp( "$GNVTG", nmeacmd) == 0  ){
		/* Test on valid data */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 2 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] != 'T' )  return nmeaNoValid;
		
			
		/* Data conversions */
		if ( strcmp( "$GNVTG", nmeacmd) == 0 )   { nmeadata->indicators|= 0x4;} else {nmeadata->indicators&= ~0x4;}
		
		/* course */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 1 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%f", &nmeadata->course);

		/* speed */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 5 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%f", &nmeadata->speedkn);
		
		/* speed */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 7 ) <= 0 ) return nmeaNoValid;
		sscanf(nmeastr,"%f", &nmeadata->speedkm);
		
		/* Mode */
		if ( nmeaGetParam(nmeastr, nmeamsg, size, 9 ) <= 0 ) return nmeaNoValid;
		if ( nmeastr[0] == 'D' ) { nmeadata->indicators|= 0x8;}else{ nmeadata->indicators&= ~0x8;}


		return nmeaVTG;
		
		
	}
	
	

	
	return nmeaNoValid;
}


/* Convertion function (need valid data)  hhmmss.sss */
unsigned long int nmeaTimeStrToUTC(const char* timestr){
	unsigned char num = 0;
	unsigned long int	time = 0;
	
	/* sec */
	num = timestr[5] - '0';
	num+= (timestr[4] - '0')*10;
	time = num;
	
	/* min */
	num = timestr[3] - '0';
	num+= (timestr[2] - '0')*10;
	time+= num * 60;
	
	/* hour */
	num = timestr[1] - '0';
	num+= (timestr[0] - '0')*10;
	time+= num * 60 * 60;
	return time;
}

int nmeaToWialon(char* wialonmsg, T_NMEA_PACKED* nmeamsg){
	//"#D#160914;151022;5000.6025;N;03769.6834;E;10;100;250;3;8.5;05;6;12.25,78.54;DRIVERCODE;TEST:1:5,TEXT:3:Test text string"
	
	sprintf(wialonmsg,"#D#%s;%s;%09.4f;%c;%010.4f;%c;%d;%d;%d;%d;%03.2f;0;0;0,0;DCODE;TEST:1:5",
		nmeamsg->date,
		nmeamsg->times,
		nmeamsg->latitude,
		(nmeamsg->indicators&0x1)?'S':'N',
		nmeamsg->longitude,
		(nmeamsg->indicators&0x2)?'W':'E',
		(int)nmeamsg->speedkm,
		(int)nmeamsg->course,
		(int)nmeamsg->altitude,
		(int)nmeamsg->satelite,
		nmeamsg->hdop
	);
	
	return 0;
}
