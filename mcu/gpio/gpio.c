/* 
*  Project: Smarttrack
*  Author: uran
*  Description: Retarget stdio 
*  History:
*  						12.08.14 create
*/
#include "gpio.h"
#include "cmsis_os.h"		/* RTL OS declaration*/


void gpioInit( void ){
	
    GPIO_InitTypeDef port;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
		//GSM_PWRKEY init
    //GPIO_StructInit(&port);
    //port.GPIO_Mode = GPIO_Mode_Out_PP;
    //port.GPIO_Pin = GPIO_Pin_2;
    //port.GPIO_Speed = GPIO_Speed_2MHz;  
    //GPIO_Init(GPIOC, &port);    
	
		GPIO_PinConfigure( GPIOC, pinGSM_PWRKEY, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT2MHZ);
}


//Pulse 1 sec for enable
void ctrlSwitchGSMPwrKey( void ){
	
	  GPIO_PinWrite(GPIOC, pinGSM_PWRKEY, 1);
		//GPIO_SetBits(GPIOC, GPIO_Pin_2);
		osDelay(1000);
		GPIO_PinWrite(GPIOC, pinGSM_PWRKEY, 0);
		//GPIO_ResetBits(GPIOC, GPIO_Pin_2);
}

