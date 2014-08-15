
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_usart.h>

#include "Driver_UART.h"


#include "cmsis_os.h"		/* RTL OS declaration*/

#include "../gpio/gpio.h"
#include "../gpio/ctrl.h"

extern ARM_DRIVER_UART Driver_UART2;


void Usart2_Send_symbol(uint8_t data)
{
  Driver_UART2.WriteData(&data,1);
}

void Usart2_Send_String(char* str)
{
  uint8_t i=0;
  while(str[i])
  {
    Usart2_Send_symbol(str[i]);
    i++;
  }
  Usart2_Send_symbol('\n');
  Usart2_Send_symbol('\r');
}


void init (void const *argument){
	char c;
	int32_t av = 0;
	USART_InitTypeDef    USART_InitStruct;
	   USART_InitTypeDef USART_InitStructure;
	
	
	osDelay(1000); //dly 1 sec
  ctrlGSM_PWRKEY(1);
	osDelay(2000);
	ctrlGSM_PWRKEY(0);
	osDelay(100);
	
	
	  




     //enable bus clocks

 
     USART_InitStructure.USART_BaudRate = 9600;

     USART_InitStructure.USART_WordLength = USART_WordLength_8b;

     USART_InitStructure.USART_StopBits = USART_StopBits_1;

     USART_InitStructure.USART_Parity = USART_Parity_No ;

     USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;



    USART_Init(USART2, &USART_InitStructure);
		
		USART_Cmd(USART2, ENABLE);

    //Enable USART2
		
		Driver_UART2.Initialize(NULL, 0); Driver_UART2.PowerControl(ARM_POWER_FULL);
		Driver_UART2.Configure(9600, 8, ARM_UART_PARITY_NONE, ARM_UART_STOP_BITS_1, ARM_UART_FLOW_CONTROL_NONE);





	while(1){
			Usart2_Send_String("AT");
//			c = 'A';
//			USART_SendData(USART2, 'A');
//		 Driver_UART2.WriteData(&c,1);
		
		while (c = Driver_UART2.DataAvailable()){
			Driver_UART2.ReadData(&c,1);
		}
		
		
		
		osDelay(200);
	};
	 
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

