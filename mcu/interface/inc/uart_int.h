/*----------------------------------------------------------------------------
 * Name:    uart_int.h
 * Purpose: LPC1768 Low level serial functions
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
#ifndef __UART_INIT_H
#define __UART_INIT_H

#include <LPC17xx.H>     /* LPC17xx definitions    */
#include "lpc_types.h"


//Setup settings for uart 
int uart1_cmsis_init(void);

//Send data
uint32_t UARTSend(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);

//Receive data
uint32_t UARTReceive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);



 /************************** PRIVATE DEFINTIONS *************************/
// buffer size definition

#endif //__UART_INIT_H
