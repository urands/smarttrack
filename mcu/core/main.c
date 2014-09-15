
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_usart.h>

#include "cmsis_os.h"		/* RTL OS declaration*/

#include "../gpio/gpio.h"
#include "../gpio/adc.h"
 #include "../gsm/inc/gsm-task.h"
#include "../gps/inc/gps-task.h"
 #include "../gps/nmea_task.h"
 
 #include "../hal/at_serial.h"
 #include "../hal/nmea_serial.h"
//#include "../gpio/ctrl.h"
#include "../interface/bufmsg.h"
#include "../interface/manager.h"

void test1(int a){
	
	
}

void init (void const *argument){
	
#ifdef __BUFMSGTEST__	
	/*  Buffer Unit test */ 
	bufUnitTest();
#endif
 
}

 //osThreadDef(init,osPriorityNormal,1,0);
 
 osThreadDef(gsm_thread,osPriorityNormal,1,0);

int main(void){
	
	unsigned int a;
	gpioInit();
	
//	adc_init();
	
//	a = adc_get_value();

	 if (osKernelInitialize () != osOK) { // check osStatus for other possible valid values
				// exit with an error message
		}
		mngInit(); //Initialize and registry tasks

//		osThreadCreate(osThread(init),0);
		
//	osThreadCreate(osThread(gsm_thread),0);
//	printf("Start SMARTTRACKER programm\n");

		
		if (!osKernelRunning ()) { // is the kernel running ?
				if (osKernelStart () != osOK) { // start the kernel
											// kernel could not be started
				}
		}
	return 0;
}


 void assert_failed(uint8_t* file, uint32_t line){
	 //printf("assert_failed\n");
 }

