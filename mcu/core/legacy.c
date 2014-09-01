/* 
*  Project: Smarttrack
*  Author: uran
*  Description: Retarget stdio 
*  History:
*  						12.08.14 create
*/


#include "legacy.h"
 #include <string.h>
 #include <stdio.h>
#include "inc/toolset.h"
#include "cmsis_os.h"		/* RTL OS declaration*/
#include "Driver_UART.h"
#include "../hal/at_serial.h"


extern ARM_DRIVER_UART Driver_UART2;

 char internal_buffer[SER_MAX_LINE_SIZE];
 
 void SER_wait_transmite(void){
	while (!Driver_UART2.TxDone()){
		 TOOL_delay_nl(1);
	}
	//while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1) == SET){
	//	 TOOL_delay_nl(1);
	// }
}
 
 void tsk_lock(void){
	 
 }
void tsk_unlock(void){
	
}
 
 void SER_init(void){
	 //  uart1_cmsis_init();
	// atSerInit(43584);
	  atSerInit(174336);
}

void SER_deinit(void){
//	  while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1) == SET);
//		UART_DeInit((LPC_UART_TypeDef *)LPC_UART1);
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putChar (char c) {
	
	 atSerWrite(&c, 1, 100);
	osDelay(100);
	return 1;
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putString (char* str) {
	//	return UARTSend((LPC_UART_TypeDef *)LPC_UART1, (uint8_t*)str, strlen(str) );
	return  atSerWrite(str, strlen(str), 100);
	//Driver_UART2.FlushRxBuffer();
 	SER_wait_transmite();
 // return len;
}

int SER_putStringbyChar(char* str){
 	int i=0;
	for (i=0;i<strlen(str);++i) SER_putChar(str[i]);
	return i;
}

int SER_readStringByChar(char* str, int sz_read){
	return atSerRead(str,sz_read, 800);
//	return Driver_UART2.ReadData(str,sz_read);
/*	 int len = 0;
	 int c_idx = 0;
	 unsigned int i = 0;
  	while (1){
   	 len = 0;
	   while (len == 0 ){
				//len = UARTReceive((LPC_UART_TypeDef *)LPC_UART1, (uint8_t*)(str+c_idx), 1);
				 ++i;
			    if ( i >= SERIAL_DELAY ) return c_idx; 
	   }
	   c_idx+=len;
	   if (c_idx >= sz_read || SER_MAX_LINE_SIZE <= c_idx) return c_idx;
	 }
	*/
//	 return 0;
}


int SER_readStringByLine(char* str){
	 int len = 0;
	 int c_idx = 0;
	 unsigned int i = 0;
  	while (1){
   	 len = 0;
		 i = 0;
	   while (len == 0 ){
			     len =  atSerRead((char*)(str+c_idx),1, 300);
					//len = Driver_UART2.ReadData( , 1);
			//	len = UARTReceive((LPC_UART_TypeDef *)LPC_UART1, (uint8_t*)(str+c_idx), 1);
			  ++i;
			  if ( i >= SERIAL_DELAY ) return c_idx; 
	   }
		 
	   c_idx+=len;
	   if ( c_idx > 1) {
			 if ((*(str+c_idx-2) == CR) && (*(str+c_idx-1) == LF)) return c_idx;
		 }
	 }
//	 return 0;
}




int SER_sms_send(char* string, char* phone, char* ansver){


	//int len = 0;

	SER_exchange("AT+CMGF=1\r",	ansver, 1);	 //OK

	sprintf(ansver,"AT+CMGS=\"%s\"\r", phone);

	SER_putString(ansver);  

	//while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1) == SET);

 //   TOOL_delay_nl(50);		  	   // 4 read
  // 	if (SER_readStringByChar(internal_buffer, 4) == 4 ){
		  //if (internal_buffer[13] != '>' ) return NOT;
	// }


	SER_putString(string);
	SER_putChar(0x1A);

//	while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1) == SET);

 //   TOOL_delay_nl(50);
 //   if (SER_readStringByLine(internal_buffer) > 0) return OK;
 	return NOT;
}




int SER_gprs_send(char* data, char* ansver ){
	 
	 int len = 0;
//     int c_len = 0;
//	 int c_idx = 0;
//	 int flag;
     TOOL_delay_nl(10);
	   memset(internal_buffer, 0, sizeof(internal_buffer));
     len = SER_read_nb(internal_buffer);
	   internal_buffer[len] = 0;
	   if ( len > 0 ){
			 SER_proccess_cmd(internal_buffer);
			 
		 }
	
	
	
	 tsk_lock();

	 SER_putString("AT+CIPSEND\r");

	 if (SER_readStringByChar(internal_buffer, 4) == 4 ){
		  //if (internal_buffer[13] != '>' ) return NOT;
	 }
	 tsk_unlock();

	 TOOL_delay_nl(40);

	 len = strlen(data);

	 tsk_lock();

     SER_putString(data);

	 SER_putChar(0x0D);
 	 SER_putChar(0x0A);
	 SER_putChar(0x1A);	
	 
	 len = SER_readStringByLine(internal_buffer);

	 if (len != 2 ) {
	 	tsk_unlock();
	 	return NOT;
	}

	 len = SER_readStringByLine(internal_buffer);

	 if (len != 9 ){
	 	 tsk_unlock();
	 	 return NOT;
	  }
	 internal_buffer[len] = 0;
	 if (strcmp("SEND OK\r\n", internal_buffer) != 0 ){
	 	 tsk_unlock();
	 	 return NOT;
	 }
	 
	 //Read ansver from site
	 len = SER_readStringByLine(ansver);
	 ansver[len] = 0;

	 //Cancel operation //need ?
	 tsk_unlock();
	 TOOL_delay_nl(10); 
	 SER_putChar(0x1B);
	 TOOL_delay_nl(30); 
	 return OK;
}


int SER_exchange(char* command, char* ansver, int ansvers ){

   int c_len = 0; 
   int c_idx = 0; 
   int len = 0;
   int watch_dog;
   int i;
   int idx;
   uint8_t c_word = 0;

   //simple test
   TOOL_delay(10);
   memset(internal_buffer, 0, sizeof(internal_buffer));
   len = SER_read_nb(internal_buffer);
	 internal_buffer[len] = 0;
	 if ( len > 0 ){
			SER_proccess_cmd(internal_buffer);
		 }
   memset(internal_buffer,0, sizeof(internal_buffer));
		 
	SER_wait_transmite();
//  if (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1) == SET) return NOT;
   tsk_lock();
   c_len = strlen (command);
		 
			Driver_UART2.FlushRxBuffer();
		 
	len = Driver_UART2.WriteData((uint8_t*) command, c_len);
//   len = UARTSend((LPC_UART_TypeDef *)LPC_UART1, (uint8_t*) command, c_len );
   if ( c_len != len ){
   		tsk_unlock();
    	return NOT;
	}

   //no need
   *(internal_buffer) = 0;

//   while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1) == SET);
	
//	Driver_UART2.FlushTxBuffer();
	
	SER_wait_transmite();

   
   while (1){
	 watch_dog = 0;
	 len = 0;
     while (len == 0 ){
					osDelay(800);
			    //Driver_UART2.FlushRxBuffer();
					len = Driver_UART2.ReadData((uint8_t*)(internal_buffer+c_idx), SER_MAX_LINE_SIZE);
		//		len = UARTReceive((LPC_UART_TypeDef *)LPC_UART1, (uint8_t*)(internal_buffer+c_idx), SER_MAX_LINE_SIZE);
			    if ( (++watch_dog) >= SERIAL_DELAY ){
					 tsk_unlock();
				 	return c_idx; 
				}
     }
	 c_idx+=len;
	 *(internal_buffer+c_idx) = 0;
	 idx =0;
	 c_word = 0;
	 for (i=0;i<c_idx-1;++i){
	 	
		if ( (*(internal_buffer+i) == CR) && (*(internal_buffer+i+1) == LF) ){
			 c_word++;
			  if ( i == 0 ) i++;
			 continue;
		}else{
			*(ansver+idx) = *(internal_buffer+i);
			idx++;
		}
		
	 }			    
	 *(ansver+idx) = LF;
	 *(ansver+idx+1) = 0;
	 if (( c_word/2) >= ansvers ) {
	 		tsk_unlock();
	 		return OK;
	 }
	 if ( c_word == 2){
	 	if ( strcmp(ansver,"\nERROR\n") == 0 ) {
			tsk_unlock();
			return NOT;
		}
	 }
   }
}


uint32_t SER_read_nb(char* str){
//	 return UARTReceive((LPC_UART_TypeDef *)LPC_UART1, (uint8_t*)str, SER_MAX_LINE_SIZE);
	return Driver_UART2.ReadData(str,SER_MAX_LINE_SIZE);
	return 0;
}

int SER_getChar (int uart) {
	char c = 0;
	while( 0 == Driver_UART2.ReadData(&c,1));
	return c;
 // LPC_UART_TypeDef *pUart;

 // pUart = (uart == 0) ? (LPC_UART_TypeDef *)LPC_UART0 : (LPC_UART_TypeDef *)LPC_UART1;
 // while (!(pUart->LSR & 0x01));
//  return (pUart->RBR);
	return 0;
}


int SER_getline_simple(char* strline ,char* stransver, int lineindex){

 	int i = 0;
	int wi = 0;
	int windex = 0;
	int szw = 0;
	strline[0] = 0;
	szw = strlen(stransver);
	for( i = 0; i< szw; ++i){
		if ( stransver[i] == LF ){
		   windex++;
		   continue;
		}
	    if ( lineindex == (windex/2) ) {
		   strline[wi++] = stransver[i];
	 	}
		if ( lineindex < (windex/2) ) {
			break;
		}

	}
   	strline[wi] = 0;
	return wi;
}


int SER_getline(char* strline ,char* stransver, int lineindex){
	int i = 0;
	int wi = 0;
	int windex = 0;
	int szw = 0;
	strline[0] = 0;
	szw = strlen(stransver)-1;
	if ( szw < 2  ) return 0;
	
	for( i = 0; i< szw; ++i){
	 if ( stransver[i] == '\r' && stransver[i+1] == '\n' ){
	  	i++;
		windex++;
	 }else{
		 if ( lineindex == windex ) {
		   strline[wi++] = stransver[i];
	 	}
	 }
	}
	strline[wi] = 0;
	return wi;
}

void SER_proccess_cmd(char* str){
	
	int len;
	
	len = strlen(str);
	
	if (strlen(str) == 0) return;
	
	if ( len >=2 && str[len-2] == 0x0D && str[len-1] == 0x0A){
		
		len = strlen(str);
		
	}else{
	  SER_readStringByLine(str);	
	}
	
	

}



