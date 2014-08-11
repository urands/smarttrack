
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stm32f10x.h>

#include "cmsis_os.h"		/* RTL OS declaration*/

struct __FILE { int handle; };
FILE __stdout;
FILE __stdin;

int fputc(int c, FILE *f)
{
	ITM_SendChar(c);
	//c=c+1;
	return (c);
}


void init (void const *argument){

	while(1){
	  printf("Start SMARTTRACKER programm from task\n");
		osDelay(100);
	}	 
	 
 }
 
 osThreadDef(init,osPriorityNormal,1,0);


int main(void){
	
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

