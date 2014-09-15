/*----------------------------------------------------------------------------
 * Name:    gsm tasks
 * Purpose: LPC1768 gsm asks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>
 #include "inc/gsm-task.h"
 #include "inc/gsm-control.h"
#include "../core/inc/toolset.h"
#include "../core/settings.h"
#include "../gpio/gpio.h"
// #include "buffer.h"
// #include "tasks.h"
//#include "accel.h"
//#include "serial.h"
#include <stdio.h>
#include <string.h>
#include "Driver_UART.h"


#include "cmsis_os.h"		/* RTL OS declaration*/


extern t_device_settings g_device_settings;
extern ARM_DRIVER_UART Driver_UART2;

 
 
char g_imei[16];
char g_apn[25];
char g_apn_usr[10];
char g_apn_pwd[10];

char g_gsm_local[200];


char g_gsm_work[255];

char gsm_cmd_str[255];

char gsm_str[255];

char wialon_str[255];

GSM_Parameters g_gsm_param;



 
 
void gsm_thread (void){
	int gprsinit_rep;
	int connection_rep;

	  
/*	USART_InitTypeDef    USART_InitStruct;
	USART_InitTypeDef USART_InitStructure;
	*/
	
	
	 
	
	osDelay(1000); //dly 1 sec
  ctrlGSM_PWRKEY(1);
	osDelay(2000);
	ctrlGSM_PWRKEY(0);
	osDelay(1000);
	
		
		GPRS_enable(ON);
		SER_read_nb(gsm_str);
		memset(gsm_str,0,sizeof(gsm_str));
		
		
	//	GSM_establish_connection();
	
	
		//while(1){} ; 

		if (GSM_setup() == OK ){
			
			while (1){
		//ok
			gprsinit_rep = 0;
			SER_read_nb(gsm_str);

		//	while ( g_buffer_file < 5 ) TOOL_delay(100);
			memset(gsm_str,0,sizeof(gsm_str));
			while (GPRS_init() == OK ) {
		  	//GPRS ok
				 connection_rep = 0;
		 		SER_read_nb(gsm_str);
				memset(gsm_str,0,sizeof(gsm_str));
			 	 while ( GPRS_connect(g_device_settings.server, g_device_settings.port, gsm_cmd_str) == OK ){
							
							if ( GPRS_Wialon_login(GSM_SERVER_IMEI,GSM_SERVER_PASSWORD,gsm_cmd_str) == OK ){ 
								 while (1){
										//	GPIO_led(LED_GREEN);
											TOOL_delay_nl(10);
//											if ( os_evt_wait_and(0x1, 6000) == OS_R_EVT){
											
   										memset(gsm_str,0,sizeof(gsm_str));
										  //GPRS_update_parameters(&g_gsm_param);
											GSM_update_sms();
											SER_read_nb(gsm_str);
											if ( 0 ) { //(gps_read_from_buffer(gsm_str) == OK ){
												//	os_mut_wait (m_write_gsm, 0xffff);
												//	os_evt_clr (0x1, t_gsm_thread);
												//	os_mut_release (m_write_gsm);
												
													//Add build to packed Heights and Sat
													/************** PART OF GSM THREAD ************************/
														if (GPRS_send(gsm_str,wialon_str) == OK ){ //auto establish connection
															//Test ansvers
															if (GPRS_test_wialon(wialon_str) == NOT ){
														
																 break;
															}
															TOOL_delay_nl(10);
											//				GPIO_led(LED_GREEN);
														}else{ 
														
															// GPRS send fail (auto close and establish )
															break;
														}
														TOOL_delay_nl(10);
													//} //gps_read_from_buffer
												} // os_evt_wait_and
												else{
													
													strcpy(gsm_str, "#D#010914;201020;5554.6025;N;03769.6834;E;10;100;250;3;8.5;05;6;12.25,78.54;DRIVERCODE;TEST:1:5,TEXT:3:Test text string\r\n") ;
													//strcpy(gsm_str, "#P#\r\n") ;
												  if (GPRS_send(gsm_str,wialon_str) == OK ){ //auto establish connection


													}else{
													 	break;
													}
													//osDelay( 100000 );

												}
											} //while (1)
																	
										
								
							} //GPRS_Wialon_login
					 connection_rep++;
					 GPRS_close(gsm_str);
					 if (  /*GPRS_gprs_attach() == NOT  ||*/ connection_rep > 5 ) break;
				 } //GPRS_connect
				gprsinit_rep++;
		    if (GPRS_gsm_attach() == NOT || gprsinit_rep > 5 ) break;
			} //while GPRS_init 
		} //if GSM_setup		
	
		GPRS_enable(OFF);
	}

 
		
}
