/*----------------------------------------------------------------------------
 * Name:    gpio.h
 * Purpose: LPC1768 Low level serial functions
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H



extern void GPIO_init(void);

extern void GPIO_led(unsigned int set);


extern void GPIO_gsm_pwron(int set);

extern void GPIO_gsm_pwrkey(int set);

extern void GPIO_gsm_reset(int set);

extern int GPIO_gsm_status(void);


extern int GPIO_gps_wakeup(void);

extern void GPIO_gps_reset(int set);

extern void GPIO_gps_pwrkey(int set);

extern void GPIO_gps_pwr(int set);




/* Define led color light */
#define LED_NONE  0
#define LED_GREEN 1
#define LED_RED	  2
#define LED_NONE_BLINK  10
#define LED_GREEN_BLINK 11
#define LED_RED_BLINK 	12

/* Define state key */
#define OFF		  0
#define ON		  1

	

#endif
