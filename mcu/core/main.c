
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stm32f10x.h>


#include "cmsis_os.h"		/* RTL OS declaration*/

#include "../gpio/gpio.h"



void init (void const *argument){

	
	
	osDelay(1000); //dly 1 sec
	
	
	ctrlSwitchGSMPwrKey();
	
	while(1){
		
	//	GPIOC->BSRR = GPIO_BSRR_BR2;
	  //GPIO_PinWrite( GPIOC, pinGSM_PWRKEY, 0 );
	//	GPIO_SetBits(GPIOC, GPIO_Pin_2);
		//osDelay(100);
		//GPIOC->BSRR = GPIO_BSRR_BS2;
		 //GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		//GPIO_PinWrite( GPIOC, pinGSM_PWRKEY, 1 );
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
	

	return 0;
}


 void assert_failed(uint8_t* file, uint32_t line){
	 printf("assert_failed\n");
 }

