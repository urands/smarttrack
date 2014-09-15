/*----------------------------------------------------------------------------
 * Name:    gpscntrl
 * Purpose: LPC1768 Low level tool functions
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/


#include "inc/gps-control.h"

#include "../hal/nmea_serial.h"
#include "../interface/inc/buffer.h"
#include "inc/gps-control.h"
#include "../core/inc/toolset.h"


#include <string.h>
#include <stdio.h>


 

int  g_gps_sec;



/************************ Exported functions **********************************/

/*******************************************************************************
* Initialize the Graphic LCD controller                                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void GGPS_init (void) {
  g_gps_sec = 12;
	nmeaSerInit(43584);
	bufferinit();
	osDelay(100);
  return ;
}

void GGPS_deinit(){
	

}







/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int GGPS_RdDat(char* string){
 
	return 	nmeaSerReadLine(string,GPS_MAX_LINE_SIZE,500);;
}


/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int GGPS_param(char* instr, char* outstr, int paramindex){
 int i, index = 0, wi =0;
 int len = strlen(instr);
 for (i =0 ; i< len; ++i ){
   if (instr[i] == ',' ) {
   		index++;
		continue;
	}
   if ( index == paramindex ){
	  outstr[wi++] = instr[i];
   }
 }
 outstr[wi] = 0;
 return wi;
}


/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int GGPS_GPRMC(char *string ){
  char cmd[10];
  while(1){
  	GGPS_RdDat( string );
  	GGPS_param(string, cmd, 0);
	if (strlen(string) >= GPS_MAX_LINE_SIZE ) return 0;
	if ( strcmp("$GPGGA", cmd) == 0 || strcmp("$GNGGA", cmd) == 0 ) {
	   if (GGPS_param(string, g_gps_sat_number, 7)==0) strcpy(g_gps_sat_number, "0");
	   if (GGPS_param(string, g_gps_height, 9) == 0)   strcpy(g_gps_height, "0");
	   if (GGPS_param(string, g_gps_hdop, 8) == 0)   strcpy(g_gps_hdop, "0");
	}

  	if ( strcmp("$GPRMC", cmd) == 0 || strcmp("$GNRMC", cmd) == 0) break; 
	
	
	if ( strcmp("$GNGSV", cmd) == 0 || strcmp("$GPGSV", cmd) == 0) {
		cmd[0]=0;
	}
  }
  return 1;
}

/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int timetoi(char* param){
 int current=0;
 if ( strlen(param) != 6) return 0;

 current = param[5]-'0';
 current+= (param[4]-'0')*10;
 current+= (param[3]-'0')*60;
 current+= (param[2]-'0')*600;
 current+= (param[1]-'0')*3600;
 current+= (param[0]-'0')*36000;
 return current;

}

/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int GGPS_wait_sec(int sec, char* str){
 int current = 0;
 int last = 0;
 char param[15];
 float speed;
 //GGPS_GPRMC(str);
/* while ( GGPS_param(str, param, 1 ) == 0 ) {
 	if (strlen(str) == 0 ) 	if (GGPS_GPRMC(str) == 0 ) return 0;
 }
*/
 if (GGPS_param(str, param, 1 ) == 0 ) return 0;
 param[6] = 0;
 current = 	timetoi(param);

 while( last - current < g_gps_sec ){
	//GPIO_led(LED_NONE);
	TOOL_delay_nl(50);
	//GPIO_led(LED_GREEN);
    if (GGPS_GPRMC(str) == 0 ) return 0;
	if (GGPS_param(str, param, 1 ) == 0 ) return 2;
	param[6] = 0;
	last = timetoi(param);

	//speed
	if (GGPS_param(str, param, 7 ) == 0 ){ g_gps_sec = sec; } else{
	   	sscanf(param, "%f", &speed);
		speed*=1.852;
		if ( speed < 10 ) g_gps_sec = 20;
		if ( speed >= 10 && speed<40 ) g_gps_sec = 15;
		if ( speed >= 40 && speed<60 ) g_gps_sec = 12;
		if ( speed >= 60 && speed<90 ) g_gps_sec = 8;
		if ( speed >= 90 && speed<120 ) g_gps_sec =5;
		if ( speed >= 120 ) g_gps_sec = 2;

	}


 
	if (last < current) break;
}
return 1;
 
}


/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int GGPS_Build_Wialon_Packet(char* nmea_rmc_msg, char* wialan_msg){
   char param[15];
   float speed;
   int i;
   strcpy(wialan_msg,"#SD#"); //header
   wialan_msg+=4;
   if (GGPS_param(nmea_rmc_msg, param, 9 ) == 0 ) return 0;
   strcpy(wialan_msg, param); wialan_msg+=strlen(param); *wialan_msg++ = ';';

   if (GGPS_param(nmea_rmc_msg, param, 1 ) == 0 ) return 0;
   param[6] = 0;
   strcpy(wialan_msg, param); wialan_msg+=strlen(param); *wialan_msg++ = ';';

   if (GGPS_param(nmea_rmc_msg, param, 3 ) == 0 ) return 0;
   strcpy(wialan_msg, param); wialan_msg+=strlen(param); *wialan_msg++ = ';';

   if (GGPS_param(nmea_rmc_msg, param, 4 ) == 0 ) return 0;
   strcpy(wialan_msg, param); wialan_msg+=strlen(param); *wialan_msg++ = ';';

   if (GGPS_param(nmea_rmc_msg, param, 5 ) == 0 ) return 0;
   strcpy(wialan_msg, param); wialan_msg+=strlen(param); *wialan_msg++ = ';';

   if (GGPS_param(nmea_rmc_msg, param, 6 ) == 0 ) return 0;
   strcpy(wialan_msg, param); wialan_msg+=strlen(param); *wialan_msg++ = ';';

   //Speed
   if (GGPS_param(nmea_rmc_msg, param, 7 ) == 0 ) {
   	 	strcpy(wialan_msg, "0;");	
		wialan_msg+=2;
   }else{
   		//Need mul on 1.852 knot to km/h
   		sscanf(param, "%f", &speed);
		speed*=1.852;
		sprintf(param,"%i",(int)speed);
   		//for (i=0;i<strlen(param);++i) if (param[i]=='.') { param[i] = 0; break;} 
		   
		   
		   strcpy(wialan_msg, param);
		   wialan_msg+=strlen(param); *wialan_msg++ = ';';
	   }
   //Course
   if (GGPS_param(nmea_rmc_msg, param, 8 ) == 0 ){
	  	strcpy(wialan_msg, "0;");	
		wialan_msg+=2;
   }else{
	   	for (i=0;i<strlen(param);++i) if (param[i]=='.') { param[i] = 0; break;} 
	   	strcpy(wialan_msg, param); 
	   	wialan_msg+=strlen(param); *wialan_msg++ = ';';
	}

	//Height
	for (i=0;i<strlen(g_gps_height);++i) if (g_gps_height[i]=='.') { g_gps_height[i] = 0; break;} 
	strcpy(wialan_msg, g_gps_height); 
	wialan_msg+=strlen(g_gps_height); *wialan_msg++ = ';';

	//Sat
	strcpy(wialan_msg, g_gps_sat_number); 
	wialan_msg+=strlen(g_gps_sat_number);
    *wialan_msg=0;
   return 1;
}
/******************************************************************************/
int GGPS_reset(){
/*	GPIO_gps_reset(ON);
	TOOL_delay_nl(10);
   	GPIO_gps_reset(OFF);
	TOOL_delay_nl(20);
	GPIO_gps_reset(ON);
	TOOL_delay_nl(10);
	return 1;
	*/
}



int GGPS_enable(void){
	
	//Power on or reset GPS
	TOOL_delay_nl(100);
/*	if ( GPIO_gps_wakeup() ){
	   	GPIO_gps_pwrkey(ON);
		TOOL_delay(20);
		GPIO_gps_pwrkey(OFF);
		TOOL_delay_nl(500);
	}
  */
	/*
	//need shutdown? 
	while ( GPIO_gps_wakeup() ){
		GPIO_gps_pwrkey(ON);
		TOOL_delay(10);
		GPIO_gps_pwrkey(OFF);
		TOOL_delay(200);  
		if ( GPIO_gps_wakeup() ){
			GPIO_gps_reset(OFF);
			TOOL_delay_nl(100);
			GPIO_gps_reset(ON);
		}else{
		 	break;
		}
*/
		//TOOL_delay_nl(1000);
	//}
	//Out from reset
	//GGPS_init();					//Setup SPI0 (SSP0) port configuration		GPIO_gps_pwrkey(OFF);
	//GPIO_gps_reset(ON);
 	//TOOL_delay(20);
/*

	while ( !GPIO_gps_wakeup() ){
		GPIO_gps_pwrkey(ON);
		TOOL_delay(20);

		TOOL_delay_nl(150);
	}
	TOOL_delay_nl(100);
	//GPS ready for work
	*/
	return 1;
}

int GGPS_send(char* str){
	
	return nmeaSerWrite(str,strlen(str),100);
}

int GGPS_check(void){
    unsigned short val=0;
	int i=0;
	int b=0;
	unsigned char pattern = 0xA7;
	//LCD_CS(0);
	while ( (val&0xFF) != '\n' ){
		i++;
	//    val   = spi_tran(pattern);  
		if (pattern  == 0xA7 ) pattern = 0xB4; else  pattern = 0xA7;
		if (val == 0xA7 || val == 0xB4 ) {
				b++;
			if ( b > 2024){

				GGPS_send("$PSRF101,0,0,0,0,0,0,12,4*10\r\n");
				TOOL_delay_nl(200);

/*		  		LCD_CS(1);
				GGPS_deinit();
				TOOL_delay_nl(200);
				GGPS_enable();
				LCD_CS(0);
*/
				b=0;
			}
				continue;                           
		}
		
		if (i >= GPS_MAX_LINE_SIZE){
//			LCD_CS(1);
			GGPS_deinit();
			GGPS_enable();
	//		LCD_CS(0);
			i=0;
		}
	}
 //   LCD_CS(1);
	return 1;
}


