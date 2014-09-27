
/*
*  Project: Smarttrack
*  Author: uran
*  Description: AT Proto
*  History:
*				12.08.14 create
*/
#include "Driver_UART.h"
#include "cmsis_os.h"		/* RTL OS declaration*/
#include <stm32f10x.h>
#include "GPIO_STM32F10x.h"
#include "nmea_serial.h"
#include <string.h>
#include <stdio.h>

extern ARM_DRIVER_UART Driver_UART1;



void nmeaSetEvent( ARM_UART_EVENT event ){
//	int a = event;
	
	if ( ARM_UART_EVENT_RX_OVERRUN == event ){
		//test(a);
	}
}


/*
* Ser init
* return: size write data
*/
unsigned int nmeaSerInit( unsigned int  baudrate){
	
	ARM_UART_STATUS status;	
	ARM_UART_CAPABILITIES cap;
	//GPIO_InitTypeDef GPIO_InitStructure;
	status = Driver_UART1.Initialize(nmeaSetEvent, 0xFFFF); 
	if (status != ARM_UART_OK ) { return 0; }
	status = Driver_UART1.PowerControl(ARM_POWER_FULL);
	if (status != ARM_UART_OK ) { return 0; }
  status =  Driver_UART1.Configure(baudrate, 8, ARM_UART_PARITY_NONE,
	ARM_UART_STOP_BITS_1, ARM_UART_FLOW_CONTROL_NONE);	
	if (status != ARM_UART_OK ) { return 0; }
	
	
	cap = Driver_UART1.GetCapabilities();
	
	
//	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		//enable Receive Data register not empty interrupt
//	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	
	
	//cap.cts = 1;
	
//	Driver_UART2.SetTxThreshold(1);
	
	Driver_UART1.FlushRxBuffer();
	return cap.dcd;
}


unsigned int nmeaSerWriteChar(char c ){
//	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)== RESET){}
//	USART_SendData(USART2, c);
	return 0;
}

/*
* Write data
* return: size write data
*/
unsigned int nmeaSerWrite(char* buffer, unsigned int size, const unsigned int timeout ){
//	int i;
//	for(i=0;i<size;i++) atSerWriteChar( buffer[i] );
//	return i;	

	int len =	Driver_UART1.WriteData((uint8_t*) buffer, size);
	while ( !Driver_UART1.TxDone() ) osDelay(1);
	return len;
	
}



unsigned int nmeaSerReadChar(){
	//if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) return 0;
	return 0;//USART_ReceiveData(USART1);
	
}

/*
* Reader response binary
* return: size response data
*/
unsigned int nmeaSerRead(char* buffer, unsigned int size, const unsigned int timeout ){
	int i;
	for (i=0;i<timeout;i++) {
		if ( Driver_UART1.DataAvailable() >= size ) break;
		osDelay(1);
	}
	i = Driver_UART1.ReadData((uint8_t*)buffer, size);
	
	
/*	//char c;
	//int it = 0;
	for (i=0;i<timeout;i++) {
		c = atSerReadChar();
		if (c!=0 ) buffer[it++] = c;
		if ( it >= size ) break;
		osDelay(1);
	}
*/
	return i;
}


unsigned int nmeaSerReadLine(char* buffer, unsigned int size, const unsigned int timeout ){
	int it = 0,time = 0;
	while (time < timeout) {
		while ( Driver_UART1.DataAvailable() == 0 ) {
			osDelay(1);
			time++;
			if ( time >= timeout) break;
		};
		if (Driver_UART1.ReadData((uint8_t*)(buffer+it), 1) == 0){
			return it;
		}
		
		if ( it > 1 && buffer[it] == '\n' ){
			if ( buffer[it - 1] == '\r' ){
				if (it < size ) buffer[it+1] = 0;
				return it + 1;
			}
		}
		if ( buffer[it] == '$' && it == 0 ){
			it++;
		}else{
			if ( it != 0 ) it++;
		}

		if ( it >= size ) return it;

	}
	return it;
}



unsigned int nmeaSerReadData(char* buffer, unsigned int size, const unsigned int timeout ){
		size = nmeaSerReadLine(buffer, size, timeout );
		if ( size > 0 ) {
			if ( nmeaCheckSum(buffer, size) ){
				return size;
			}
		}
		return 0;
}



unsigned int nmeaCheckSum(char* buffer, unsigned int size ){
 unsigned int i, len, rcrc, crc = 0;
	len = strlen(buffer);
	if ( len > size ) len = size;
	if (buffer[0] == '$'){
			for( i = 1; i< len ;++i ){
				if (buffer[i] != '*' ){
					crc^= buffer[i];
				}else{
					break;
			}
		}
		if ( len - i == 5 ){
			i++;
			sscanf(buffer+i,"%x",&rcrc);
			if ( rcrc == crc) return crc;
		}
  }
	return 0;
}


#ifndef __RELEASE__

/*
* unit test
* return: size response data
*/
void nmeaSerUnitTest(){
	
	char buf[255];
	int sz ,i;
	//load boudrate autocalibration test
	buf[0] = 0;	buf[1] = 0x49;	buf[2] = 0x49;	buf[3] = 0x49;	buf[4] = 0x49;	buf[5] = 0xFF;	buf[6] = 0xFF;	buf[7] = 0xFF;	buf[8] = 0xFF;	
	
	
	nmeaSerInit(43584);
	osDelay(100);
	osDelay(1000);	
	//sz = nmeaSerRead(buf,128,1000);
	
	
	
	//sz = atSerWrite(buf,9,100);
	//osDelay(100);
	//sz = nmeaSerWrite("PSIMIPR,T\r\n",11,1000);
	osDelay(1000);	
	sz = nmeaSerRead(buf,128,1000);
	
	for (i=0;i<sz;i++){ buf[i] = ~buf[i]; }
	
	
	//sz = atSerWrite("AT+IPR=19200\r",13,1000);
	osDelay(1000);	
	sz = nmeaSerRead(buf,128,1000);
	
	while(1){
	//	sz = nmeaSerWrite("AT+IFC?\r",8,1000);
		sz = nmeaSerReadLine(buf,255,500);
		//osDelay(1000);	
//		test(sz);
		
//		sz = nmeaSerRead(buf,128,1000);
//		osDelay(1000);	
//		test(sz);
		
	}
	
	
	
	
}

#endif

