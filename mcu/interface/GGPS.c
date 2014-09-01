/******************************************************************************/
/* GLCD_SPI_LPC1700.c: LPC1700 low level Graphic LCD (240x320 pixels) driven  */
/*                     with SPI functions                                     */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2011 Keil - An ARM Company. All rights reserved.        */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/


#include <lpc17xx.h>
#include <lpc17xx_clkpwr.h>
#include "GGPS.h"
#include "gpio.h"
#include "toolset.h"
#include <string.h>
#include <stdio.h>
#include "buffer.h"
#include "gps-control.h"


/************************** Orientation  configuration ************************/

#define LANDSCAPE   1                   /* 1 for landscape, 0 for portrait    */
#define ROTATE180   0                   /* 1 to rotate the screen for 180 deg */

/*********************** Hardware specific configuration **********************/

/* SPI Interface: SPI3
   
   PINS: 
   - CS     = P0.6 (GPIO pin)
   - RS     = GND
   - WR/SCK = P0.7 (SCK1)
   - RD     = GND
   - SDO    = P0.8 (MISO1)
   - SDI    = P0.9 (MOSI1)                                                    */

#define PIN_CS      (1 << 6)
#define PIN_CLK     (1 << 7)
#define PIN_DAT     (1 << 9)

#define IN          0x00
#define OUT         0x01

/* SPI_SR - bit definitions                                                   */
#define TFE         0x01
#define RNE         0x04
#define BSY         0x10

/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N    18

/*---------------------- Graphic LCD size definitions ------------------------*/

#if (LANDSCAPE == 1)
#define WIDTH       320                 /* Screen Width (in pixels)           */
#define HEIGHT      240                 /* Screen Hight (in pixels)           */
#else
#define WIDTH       240                 /* Screen Width (in pixels)           */
#define HEIGHT      320                 /* Screen Hight (in pixels)           */
#endif
#define BPP         16                  /* Bits per pixel                     */
#define BYPP        ((BPP+7)/8)         /* Bytes per pixel                    */

/*--------------- Graphic LCD interface hardware definitions -----------------*/

/* Pin CS setting to 0 or 1                                                   */
#define LCD_CS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_CS)    : (LPC_GPIO0->FIOCLR = PIN_CS))
#define LCD_CLK(x)  ((x) ? (LPC_GPIO0->FIOSET = PIN_CLK)   : (LPC_GPIO0->FIOCLR = PIN_CLK))
#define LCD_DAT(x)  ((x) ? (LPC_GPIO0->FIOSET = PIN_DAT)   : (LPC_GPIO0->FIOCLR = PIN_DAT))

#define DAT_MODE(x) ((x == OUT) ? (LPC_GPIO0->FIODIR |= PIN_DAT) : (LPC_GPIO0->FIODIR &= ~PIN_DAT))
#define BUS_VAL()                ((LPC_GPIO0->FIOPIN  & PIN_DAT) != 0)


#define SPI_START   (0x70)              /* Start byte for SPI transfer        */
#define SPI_RD      (0x01)              /* WR bit 1 within start              */
#define SPI_WR      (0x00)              /* WR bit 0 within start              */
#define SPI_DATA    (0x02)              /* RS bit 1 within start byte         */
#define SPI_INDEX   (0x00)              /* RS bit 0 within start byte         */

#define BG_COLOR  0                     /* Background color                   */
#define TXT_COLOR 1                     /* Text color                         */



 

int  g_gps_sec;


/*******************************************************************************
* Transfer 1 byte over the serial communication                                *
*   Parameter:    byte:   byte to be sent                                      *
*   Return:               byte read while sending                              *
*******************************************************************************/

static __inline unsigned char spi_tran (unsigned char byte) {

  LPC_SSP1->DR = byte;
  while (!(LPC_SSP1->SR & RNE));        /* Wait for send to finish            */
  return (LPC_SSP1->DR);
}











/************************ Exported functions **********************************/

/*******************************************************************************
* Initialize the Graphic LCD controller                                        *
*   Parameter:                                                                 *
*   Return:                                                                    *
*******************************************************************************/

void GGPS_init (void) {

  /* Enable clock for SSP1, clock = CCLK / 2                                  */
  LPC_SC->PCONP       |= 0x00000400;
  LPC_SC->PCLKSEL0    |= 0x00200000;

  /* SSEL1 is GPIO output set to high                                         */
  LPC_GPIO0->FIODIR   |= 0x00000040;
  LPC_GPIO0->FIOSET    = 0x00000040;
  LPC_PINCON->PINSEL0 &= 0xFFF03FFF;
  LPC_PINCON->PINSEL0 |= 0x000A8000;

  /* Enable SPI in Master Mode, CPOL=0, CPHA=1                                */
  /* Max. 12.5 MBit used for Data Transfer @ 100MHz                           */

  //can speedly 
  LPC_SSP1->CR0        = 0xFF87;
  LPC_SSP1->CPSR       = 0x04;
  LPC_SSP1->CR1        = 0x02;

  GPS_init();


  g_gps_sec = 12;


  return ;
}

void GGPS_deinit(){
	
//	SPI_DeInit(LPC_SPI);
//	GPIO_gps_pwrkey(OFF);
	LCD_CS(1);
	GPIO_gps_pwrkey(OFF);  
	GPIO_gps_reset(OFF);

	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSPI, DISABLE);

	TOOL_delay_nl(200);
	GPIO_gps_reset(ON);
 	GPIO_gps_pwrkey(ON);  
	TOOL_delay_nl(100);

}







/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int GGPS_RdDat(char* string){
    unsigned char val=0;
	int i=0;
	int b=0;
	unsigned char pattern = 0xA7;
	LCD_CS(0);
	while ( (val&0xFF) != '\n' ){
		val   = spi_tran(pattern);  
		if (pattern  == 0xA7 ) pattern = 0xB4; else  pattern = 0xA7;
		if (val == 0xA7 || val == 0xB4 ) {
			b++;
			val   = spi_tran(pattern);  
			if ( b > 1000){

				GGPS_send("$PSRF101,0,0,0,0,0,0,12,4*10\r\n");
				TOOL_delay_nl(500);
				b=0;
				i=0;
			}
				TOOL_delay_nl(1);
				continue;                           
		}else{
			b=0;
		}
		if ( val == 0 ){
			TOOL_delay_nl(20);
			continue;	
		}
		string[i++] = val;
		
		if (i >= GPS_MAX_LINE_SIZE){
			LCD_CS(1);
			GGPS_deinit();
			GGPS_enable();
			LCD_CS(0);
			i=0;
		}
	}
    LCD_CS(1);
	return i;
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
	if ( strcmp("$GPGGA", cmd) == 0 ) {
	   if (GGPS_param(string, g_gps_sat_number, 7)==0) strcpy(g_gps_sat_number, "0");
	   if (GGPS_param(string, g_gps_height, 9) == 0)   strcpy(g_gps_height, "0");
	   if (GGPS_param(string, g_gps_hdop, 8) == 0)   strcpy(g_gps_hdop, "0");
	}

  	if ( strcmp("$GPRMC", cmd) == 0 ) break;
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
	GPIO_gps_reset(ON);
	TOOL_delay_nl(10);
   	GPIO_gps_reset(OFF);
	TOOL_delay_nl(20);
	GPIO_gps_reset(ON);
	TOOL_delay_nl(10);
	return 1;
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
	GGPS_init();					//Setup SPI0 (SSP0) port configuration		GPIO_gps_pwrkey(OFF);
	GPIO_gps_reset(ON);
 	TOOL_delay(20);


	while ( !GPIO_gps_wakeup() ){
		GPIO_gps_pwrkey(ON);
		TOOL_delay(20);

		TOOL_delay_nl(150);
	}
	TOOL_delay_nl(100);
	//GPS ready for work
	return 1;
}

int GGPS_send(char* str){
	int i;
 	for ( i = 0; i< strlen(str); ++i){
	   spi_tran(str[i]);

	}
	return 0;

}

int GGPS_check(void){
    unsigned short val=0;
	int i=0;
	int b=0;
	unsigned char pattern = 0xA7;
	LCD_CS(0);
	while ( (val&0xFF) != '\n' ){
		i++;
	    val   = spi_tran(pattern);  
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
			LCD_CS(1);
			GGPS_deinit();
			GGPS_enable();
			LCD_CS(0);
			i=0;
		}
	}
    LCD_CS(1);
	return 1;
}
