/*----------------------------------------------------------------------------
 * Name:    gps.c
 * Purpose: LPC1768 Low level tool functions
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/

 
#include "inc/gps-task.h"
#include "inc/gps-control.h"
#include "../interface/inc/buffer.h"
#include "../core/inc/toolset.h"

 
#include <string.h>


char gps_str[GPS_MAX_LINE_SIZE];

unsigned int g_gps_delay;


/*----------------------------------------------------------------------------
 *        Task 5 'it_gps_recv': Serial read from gsm controller
 *---------------------------------------------------------------------------*/
void gps_thread(void){
	int val = 0;
	GSM_Packet2 pack;
//	t_gps = os_tsk_self();
//   GSM_Packet pack;
	memset((void*)&pack,0,sizeof(GSM_Packet2));
  memset(gps_str,0,sizeof(gps_str));
  gps_str[0] = 0; //Clear string
  g_gps_delay = 12;

	
	GGPS_init();
	
	while(1){
		//GPIO_led(LED_NONE);
	//	GSM_gps_write_sms();
  //	TOOL_wdt();
		TOOL_delay_nl(50);
	   	if (GGPS_wait_sec(g_gps_delay, gps_str) == 0 ) {
			if ( GGPS_GPRMC(gps_str) == 0) {
					// Counter read GPS and reset GPS
					GGPS_reset();
					GGPS_enable();
			}
			//GPIO_led(LED_RED);
//			if (g_gsm_connection == 0 )GPIO_led(LED_RED);
//			if (g_gsm_connection == 1 )GPIO_led(LED_RED);
			TOOL_delay_nl(80);
//			if (g_gsm_connection == 1 ) GPIO_led(LED_NONE);
	//		g_gsm_param.status = 0;
			continue;
		}
		//Read Packet.....
		// Add mutex

//		os_mut_wait (m_write_gsm, 0xffff);
 //   GPIO_led(LED_GREEN);
		gps_write_to_buffer(gps_str);

//		os_evt_set (0x0001, t_gsm_thread);
		//if (g_gsm_connection == 1 ) GPIO_led(LED_NONE);
		//};
//		os_mut_release (m_write_gsm);
	}
}
