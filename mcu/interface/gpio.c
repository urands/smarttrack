/*----------------------------------------------------------------------------
 * Name:    gpio.c
 * Purpose: LPC1768 Low level serial functions
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/

#include <LPC17xx.H>                              /* LPC17xx definitions    */
#include "gpio.h"


/*----------------------------------------------------------------------------
  Initialize GPIO pins, LED
*----------------------------------------------------------------------------*/
void GPIO_init(void){

 //LPC_PINCON  pin select default in GPIO
 //LPC_PINCON->PINSEL2 = 0x00;
 //LPC_PINCON->PINSEL3 = 0x00;

 //P1.1 and P1.4 set as output
 LPC_GPIO1->FIODIR0= 0x12;

//P1.14 GPS ON_OFF
//P1.15 GPS nRESET
 //LPC_GPIO1->FIODIR1= 0xC0; 
 LPC_GPIO1->FIODIR1= 0x40; 
 
//P1.19 as output
 LPC_GPIO1->FIODIR2= 0x08;

 //Mask default 0
 LPC_GPIO1->FIOMASK = 0x00;


 LPC_GPIO2->FIODIR1= 0x20;


//P0.6  - GPS_CS
//P0.7 GPS_CLK/GPS_CS
//P0.8 MSIO
//P0.9 MOSI

 LPC_GPIO0->FIODIR0= 0xE0;

}

// 15 14 13 12 11 10 9 8 
//  7  6  5  4  3  2 1 0
//   0010    0000    

/*----------------------------------------------------------------------------
  Set GPIO pins, LED - 0 - no 1 - red 2 - green
*----------------------------------------------------------------------------*/
void GPIO_led(unsigned int set){

   //P1.1 = 0 and P1.4 = 0
   if ( set == LED_NONE ){
	 LPC_GPIO1->FIOCLR0 = 0x12;
   }

   //P1.1 = 1 and P1.4 = 0
   if (set == LED_GREEN ){
	   LPC_GPIO1->FIOCLR0 = 0x10;
	   LPC_GPIO1->FIOSET0 = 0x02;
   }
   //P1.1 = 0 and P1.4 = 1
   if (set == LED_RED ){
	   LPC_GPIO1->FIOCLR0 = 0x02;
	   LPC_GPIO1->FIOSET0 = 0x10;
   }
}



//P0.6  - GPS_CS
//P0.7 GPS_CLK/GPS_CS
//P0.8 MSIO
//P0.9 MOSI
//P1.14 GPS ON_OFF
//P1.15 GPS nRESET


/*----------------------------------------------------------------------------
  Set GPIO pins, P1.14 (ON_OFF GPS)	   
*----------------------------------------------------------------------------*/
void GPIO_gps_pwrkey(int set){
	if (set == OFF ){
	    LPC_GPIO1->FIOCLR1 = 0x40;
	}else{
   		LPC_GPIO1->FIOSET1 = 0x40;
	}
}

//P0.3
/*----------------------------------------------------------------------------
  Set GPIO pins, P1.15 (nRESET GPS)	   
*----------------------------------------------------------------------------*/
void GPIO_gps_pwr(int set){
	if (set == OFF ){
	    LPC_GPIO0->FIOCLR0 = 0x08;
	}else{
   		LPC_GPIO0->FIOSET0 = 0x08;
	}
}

/*----------------------------------------------------------------------------
  Set GPIO pins, P1.15 (nRESET GPS)	   
*----------------------------------------------------------------------------*/
void GPIO_gps_reset(int set){
	if (set == OFF ){
	    LPC_GPIO1->FIOCLR1 = 0x80;
	}else{
   		LPC_GPIO1->FIOSET1 = 0x80;
	}
}


int GPIO_gps_wakeup(){
	return ( LPC_GPIO2->FIOPIN1 & 0x10)?1:0;
}


int GPIO_gsm_status(){
	return ( LPC_GPIO0->FIOPIN0 & 0x10)?1:0;
}



//p2.13 50 key (power on) - 1 PWRK
//p0.5 reset
//p0.4 -  STATUS 81 

/*----------------------------------------------------------------------------
  Set GPIO pins, P2.13 (PWRKEY GSM)	   
*----------------------------------------------------------------------------*/
void GPIO_gsm_reset(int set){
	if (set ==0 ){
	    LPC_GPIO0->FIOCLR0 = 0x20;
	}else{
   		LPC_GPIO0->FIOSET0 = 0x20;
	}
}


/*----------------------------------------------------------------------------
  Set GPIO pins, P2.13 (PWRKEY GSM)	   
*----------------------------------------------------------------------------*/
void GPIO_gsm_pwrkey(int set){
	if (set == OFF ){
	    LPC_GPIO2->FIOCLR1 = 0x20;
	}else{
   		LPC_GPIO2->FIOSET1 = 0x20;
	}
}

/*----------------------------------------------------------------------------
  Set GPIO pins, P1.19 (enable GSM)	   
*----------------------------------------------------------------------------*/
void GPIO_gsm_pwron(int set){
	if (set == OFF ){
	    LPC_GPIO1->FIOCLR2 = 0x08;
	}else{
   		LPC_GPIO1->FIOSET2 = 0x08;
	}
}



