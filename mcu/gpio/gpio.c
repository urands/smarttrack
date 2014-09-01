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
	
	//configure NVIC
		NVIC_InitTypeDef NVIC_InitStructure;
    
//		USART_ClockInitTypeDef  USART_ClockInitStructure;
//		USART_InitTypeDef USART_InitStructure;
	
//		USART_ClockInit(USART2, &USART_ClockInitStructure);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  	
		

     //Set USART2 Tx (PA2) as AF push-pull

     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA, &GPIO_InitStructure);

     //Set USART2 Rx (PA3) as input floating

		 
/*		 
	  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(USART2, &USART_ClockInitStructure);
		 
		 
		USART_InitStructure.USART_BaudRate = 19200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // Enable Tx and Rx
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		
		
		USART_Init(USART2, &USART_InitStructure);
		//Enable USART2
		USART_Cmd(USART2, ENABLE);
	*/	
/*		//select NVIC channel to configure
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		//set priority to lowest
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
		//set subpriority to lowest
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
		//enable IRQ channel
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		//update NVIC registers
		NVIC_Init(&NVIC_InitStructure);
		
		*/
		//disable Transmit Data Register empty interrupt
//		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
		//enable Receive Data register not empty interrupt
//		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	
		//GSM_PWRKEY init
    GPIO_StructInit(&port);
    port.GPIO_Mode = GPIO_Mode_Out_PP;
    port.GPIO_Pin = GPIO_Pin_2;
    port.GPIO_Speed = GPIO_Speed_2MHz;  
    GPIO_Init(GPIOC, &port);    
	
		//GPIO_PinConfigure( GPIOC, pinGSM_PWRKEY, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT2MHZ);
}


//Pulse 1 sec for enable
void ctrlSwitchGSMPwrKey( void ){
	
	  GPIO_PinWrite(GPIOC, pinGSM_PWRKEY, 1);
		//GPIO_SetBits(GPIOC, GPIO_Pin_2);
		osDelay(1000);
		GPIO_PinWrite(GPIOC, pinGSM_PWRKEY, 0);
		//GPIO_ResetBits(GPIOC, GPIO_Pin_2);
}


void ctrlGSM_PWRKEY(int state){
	if ( state) GPIO_SetBits(GPIOC, GPIO_Pin_2);
	else GPIO_ResetBits(GPIOC, GPIO_Pin_2);
	// GPIO_PinWrite(GPIOC, pinGSM_PWRKEY, state);
}

