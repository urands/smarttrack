
#include "gpio.h"

#include "stm32f10x_rcc.h"

#include "cmsis_os.h"		/* RTL OS declaration*/


void gpioInit( void ){
	
    GPIO_InitTypeDef port;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
		//GSM_PWRKEY init
    GPIO_StructInit(&port);
    port.GPIO_Mode = GPIO_Mode_Out_PP;
    port.GPIO_Pin = GPIO_Pin_2;
    port.GPIO_Speed = GPIO_Speed_2MHz;  
    GPIO_Init(GPIOC, &port);    
 
}


//Pulse 1 sec for enable
void ctrlSwitchGSMPwrKey( void ){
		GPIO_SetBits(GPIOC, GPIO_Pin_2);
		osDelay(1000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_2);
}

