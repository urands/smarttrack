
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stm32f10x.h>


#include "cmsis_os.h"		/* RTL OS declaration*/

#include "../gpio/gpio.h"
#include "../gpio/ctrl.h"



void init (void const *argument){

	
	
	osDelay(1000); //dly 1 sec
	
	
	
	
	while(1){

		ctrlGSM_PWRKEY(1);
		osDelay(100);
		ctrlGSM_PWRKEY(0);
		osDelay(100);
	}	 
	 
 }
 
 osThreadDef(init,osPriorityNormal,1,0);


int main(void){
	
	gpioInit();
	
	
	
	osKernelInitialize();
	
	osThreadCreate(osThread(init),0);
//	printf("Start SMARTTRACKER programm\n");
	
	osKernelStart();
	
	
	while(1){
		osThreadYield();
	}
	

//	return 0;
}


 void assert_failed(uint8_t* file, uint32_t line){
	 printf("assert_failed\n");
 }

