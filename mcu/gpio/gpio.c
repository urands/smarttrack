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
	
		GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

     //Set USART2 Tx (PA2) as AF push-pull

     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;

     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

     GPIO_Init(GPIOA, &GPIO_InitStructure);

     //Set USART2 Rx (PA3) as input floating

     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;

     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

     GPIO_Init(GPIOA, &GPIO_InitStructure);

	
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

