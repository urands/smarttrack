
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_usart.h>

#include "cmsis_os.h"		/* RTL OS declaration*/

#include "../gpio/gpio.h"
 #include "../gsm/inc/gsm-task.h"
 #include "../gps/inc/gps-task.h"
 
 #include "../hal/at_serial.h"
//#include "../gpio/ctrl.h"




void Usart2_Send_symbol(uint8_t data)
{
//  Driver_UART2.WriteData(&data,1);
	SER_wait_transmite();
}

void Usart2_Send_String(char* str)
{
  uint8_t i=0;
  while(str[i])
  {
    Usart2_Send_symbol(str[i]);
			SER_wait_transmite();
    i++;
  }
  Usart2_Send_symbol('\r');
}


void send_Uart(USART_TypeDef* USARTx, unsigned char c) // ????????? ????
{
	Usart2_Send_symbol(c);
	return;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)== RESET){}
	USART_SendData(USARTx, c);
}
 
unsigned char getch_Uart(USART_TypeDef* USARTx)  //  ???????? ????
{
	char c = 0;
//	if ( Driver_UART2.DataAvailable() )		Driver_UART2.WriteData(&c,1) ;
	return c;
	
	if (USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) == RESET){
		return USART_ReceiveData(USARTx);
	}else{
		return 0;
	}
}



void init (void const *argument){
	char c;
	char buff[255];
	int read;
	int32_t av = 0;
	int it = 0;
	USART_InitTypeDef    USART_InitStruct;
	USART_InitTypeDef USART_InitStructure;

	
	
/*
	osDelay(1000); //dly 1 sec
  ctrlGSM_PWRKEY(1);
	osDelay(1000);
	ctrlGSM_PWRKEY(0);
	osDelay(1000);
	*/
//	atSerUnitTest();
	nmeaSerUnitTest();
	
	
	return;
	

	/*
	
	USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
*/
	
	//GSM_establish_connection();
	
	//return;

	send_Uart(USART2,0x0 );
	send_Uart(USART2,0x49 );
	send_Uart(USART2,0x49 );
	send_Uart(USART2,0x49 );
	send_Uart(USART2,0x49 );
	send_Uart(USART2,0xFF );
	send_Uart(USART2,0xFF );
	send_Uart(USART2,0xFF );
	send_Uart(USART2,0xFF );



	while(1){
			send_Uart(USART2,'A' );
			send_Uart(USART2,'T' );
			send_Uart(USART2,'\r' );
			//Usart2_Send_String("AT");
		c = 0;
		//	USART_SendData(USART2, 'A');
		// Driver_UART2.WriteData(&c,1);
		  osDelay(1000);
		c = getch_Uart(USART2);
		it = 0;
		if (c!=0 ) buff[it++] = c;
		while( c!= 0){
				c = getch_Uart(USART2);
				buff[0] = c;
			it++;
		}
		
		if (it > 0 ){
			it = buff[0];
			it = 0;
		}

/*		
		while (c = Driver_UART2.DataAvailable()){
			read = Driver_UART2.ReadData(buff,255);
			buff[read+1] = 0;
		//	printf("read=%i %s",read, buff);
		}
	*/	
		
		
		osDelay(200);
	};
 
 }

 osThreadDef(init,osPriorityNormal,1,0);
 
 osThreadDef(gsm_thread,osPriorityNormal,1,0);
 osThreadDef(gps_thread,osPriorityNormal,1,0);


int main(void){
	
	gpioInit();
	
	
	
	osKernelInitialize();
	
	osThreadCreate(osThread(gps_thread),0);
	osThreadCreate(osThread(gsm_thread),0);
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

