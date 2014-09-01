/*----------------------------------------------------------------------------
 * Name:    tasks
 * Purpose: LPC1768 tasks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
 
 #include "tasks.h"
 
 #include "gpio.h"
 #include "gsm-task.h"
 #include "gps-task.h"
 #include "toolset.h"
 #include "lpc17xx_uart.h"
 #include "buffer.h"
 #include "ggps.h"
 #include "definitions.h"

 #include "accel.h"
 
 OS_TID	t_gsm_thread;
 OS_TID	t_gps;
 OS_TID t_init_task;
 
 
OS_MUT m_write_gsm;
OS_MUT m_write_buffer;

 __task void init (void){
	
	t_init_task = os_tsk_self();
	GPIO_led( LED_GREEN );  			//Startup LED green lighting

	TOOL_delay_nl(50);	
 	GPIO_led( LED_RED );  			//Startup LED green lighting

	GGPS_enable();					//Setup SPI0 (SSP0) port configuration
/*	while (GGPS_check() == 0 ){
		TOOL_delay_nl(100);
		GGPS_enable();
	}
  */
	GPIO_led( LED_GREEN ); 
	os_mut_init (m_write_gsm);
	os_mut_init (m_write_buffer);

	bufferinit();
	os_tsk_create (manageTask, 102);    /* start task GSM  */
	 
	os_evt_wait_or(FINISH, 0xFFFF);
	t_gps = os_tsk_create (gps_thread, 101);    /* start task GSM  */
	t_gsm_thread = os_tsk_create (gsm_thread, 101);    /* start task GSM  */
	
	
	
	
	

	os_tsk_delete_self ();                 /* STOP init task (no longer needed)*/
 }
