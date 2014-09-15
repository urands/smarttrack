/*----------------------------------------------------------------------------
 * Name:    settings.h
 * Purpose: Settings for device
 * Note(s):
 *----------------------------------------------------------------------------
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
#ifndef __GSM_SETTINGS_H
#define __GSM_SETTINGS_H

/********************************************* SERVERS ***************************************************/

/* Wialon vidsboku.ru    */
//#define GSM_SERVER_IP 			"92.243.174.83"
//#define GSM_SERVER_PORT			1220

/* Wialon http://orange.gps-trace.com    */

/*	Defaul settings */

// Phonebook: Name: server Phone: 193193165166
#define GSM_SERVER_IP 			"193.193.165.166"

// Phonebook: Name: port Phone: 20332
#define GSM_SERVER_PORT			20332

/* Phone for default sending messages */
// Phonebook: Name: phone Phone: +79060795144
#define GSM_PHONE			"+79060795144"



/********************************************* DEVICE ****************************************************/


// Phonebook: Name: password Phone: 87654

/* Device test3	 				*/
//#define GSM_SERVER_PASSWORD		"12345"		  // by Orange GPS

/* Device test2	 				*/
#define GSM_SERVER_PASSWORD		"87654"

/* Device test1	 				*/
//#define GSM_SERVER_PASSWORD		"99887"

//012207002485735




/* Defined from device */
#define GSM_SERVER_IMEI 		"001245789865" 
//g_imei	

typedef struct t_device_settings{
	 char password[10];
	 char server[15];
	 char phone[15];
	 unsigned int port;
	 
 }t_device_settings;

 


#endif

