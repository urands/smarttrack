/**********************************************************************
* $Id$		uart_fullmodem			2010-05-21
*//**
* @file		uart_fullmodem
* @brief	This example describes how to use UART1 full-modem function
* @version	2.0
* @date		21. May. 2010
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2010, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "uart_int.h"
#include <rtl.h>

/* Example group ----------------------------------------------------------- */
/** @defgroup UART_UART1_FullModem	UART1_FullModem
 * @ingroup UART_Examples
 * @{
 */


#define UART_RING_BUFSIZE 256

/************************** PRIVATE DEFINITIONS *************************/
#define MCB_LPC_1768
//#define IAR_LPC_1768

// buffer size definition
/* Auto RTS and Auto CTS definition:
 * - 1: Enable Auto RTS and CTS function
 * - 0: Disable this function, in this case, handle manually
 * modem functionality */
#define AUTO_RTS_CTS_USE	1

#define ENABLE_FULL_CTRL    0

/* Buf mask */
#define __BUF_MASK (UART_RING_BUFSIZE-1)
/* Check buf is full or not */
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
/* Check buf will be full in next receiving or not */
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
/* Check buf is empty */
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
/* Reset buf */
#define __BUF_RESET(bufidx)	(bufidx=0)
#define __BUF_INCR(bufidx)	(bufidx=(bufidx+1)&__BUF_MASK)


/************************** PRIVATE TYPES *************************/
/** @brief UART Ring buffer structure */
typedef struct
{
    __IO uint32_t tx_head;                /*!< UART Tx ring buffer head index */
    __IO uint32_t tx_tail;                /*!< UART Tx ring buffer tail index */
    __IO uint32_t rx_head;                /*!< UART Rx ring buffer head index */
    __IO uint32_t rx_tail;                /*!< UART Rx ring buffer tail index */
    __IO uint8_t  tx[UART_RING_BUFSIZE];  /*!< UART Tx data ring buffer */
    __IO uint8_t  rx[UART_RING_BUFSIZE];  /*!< UART Rx data ring buffer */
} UART_RING_BUFFER_T;


/************************** PRIVATE VARIABLES *************************/

// UART Ring buffer
UART_RING_BUFFER_T rb;

// RTS State
__IO int32_t RTS_State;

// Current Tx Interrupt enable state
__IO FlagStatus TxIntStat;


/************************** PRIVATE FUNCTIONS *************************/
/* Interrupt service routines */
void UART1_IRQHandler(void);
void UART1_IntTransmit(void);
void UART1_IntReceive(void);
void UART1_IntErr(uint8_t bLSErrType);

uint32_t UARTReceive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen);
uint32_t UARTSend(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen);
void print_menu(void);


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		UART1 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART1_IRQHandler(void)
{
#if (AUTO_RTS_CTS_USE == 0)
	uint8_t modemsts;
#endif
	uint32_t intsrc, tmp, tmp1;
//	tsk_lock();
	/* Determine the interrupt source */
	intsrc = UART_GetIntId((LPC_UART_TypeDef *)LPC_UART1);
	tmp = intsrc & UART_IIR_INTID_MASK;

	/*
	 * In case of using UART1 with full modem,
	 * interrupt ID = 0 that means modem status interrupt has been detected
	 */

	if (tmp == 0){
		// Check Modem status
		#if (AUTO_RTS_CTS_USE == 0)
		modemsts = UART_FullModemGetStatus(LPC_UART1);
			// Check CTS status change flag
			if (modemsts & UART1_MODEM_STAT_DELTA_CTS) {
				// if CTS status is active, continue to send data
				if (modemsts & UART1_MODEM_STAT_CTS) {
					// Re-Enable Tx
					UART_TxCmd((LPC_UART_TypeDef *)LPC_UART1, ENABLE);
				}
				// Otherwise, Stop current transmission immediately
				else{
					// Disable Tx
					UART_TxCmd((LPC_UART_TypeDef *)LPC_UART1, DISABLE);
				}
			}
		#endif
	}

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = UART_GetLineStatus((LPC_UART_TypeDef *)LPC_UART1);
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {
			UART1_IntErr(tmp1);
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
		UART1_IntReceive();
	}

	// Transmit Holding Empty
	if (tmp == UART_IIR_INTID_THRE){
		UART1_IntTransmit();
	}

//	tsk_unlock();
}

/********************************************************************//**
 * @brief 		UART1 receive function (ring buffer used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART1_IntReceive(void)
{
	uint8_t tmpc;
	uint32_t rLen;

	while (1){
		// Call UART read function in UART driver
		rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART1, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){

			/* If buffer will be full and RTS is driven manually,
			 * RTS pin should be forced into INACTIVE state
			 */
#if (AUTO_RTS_CTS_USE == 0)
			if (__BUF_WILL_FULL(rb.rx_head, rb.rx_tail))
			{
				if (RTS_State == ACTIVE)
				{
					// Disable request to send through RTS line
					UART_FullModemForcePinState(LPC_UART1, UART1_MODEM_PIN_RTS, \
							INACTIVE);
					RTS_State = INACTIVE;
				}
			}
#endif

			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
			if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
				rb.rx[rb.rx_head] = tmpc;
				__BUF_INCR(rb.rx_head);
			}
		}
		// no more data
		else {
			break;
		}
	}
}


/********************************************************************//**
 * @brief 		UART1 transmit function (ring buffer used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART1_IntTransmit(void)
{
    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, DISABLE);

	/* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
	 * of data or break whenever ring buffers are empty */
	/* Wait until THR empty */
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART1) == SET);

	while (!__BUF_IS_EMPTY(rb.tx_head,rb.tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
    	if (UART_Send((LPC_UART_TypeDef *)LPC_UART1, (uint8_t *)&rb.tx[rb.tx_tail], \
			1, NONE_BLOCKING)){
        /* Update transmit ring FIFO tail pointer */
        __BUF_INCR(rb.tx_tail);
    	} else {
    		break;
    	}
    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
	if (__BUF_IS_EMPTY(rb.tx_head, rb.tx_tail)) {
    	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, DISABLE);
    	// Reset Tx Interrupt state
    	TxIntStat = RESET;
    }
    else{
      	// Set Tx Interrupt state
		TxIntStat = SET;
    	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, ENABLE);
    }
}


/*********************************************************************//**
 * @brief		UART Line Status Error
 * @param[in]	bLSErrType	UART Line Status Error Type
 * @return		None
 **********************************************************************/
void UART1_IntErr(uint8_t bLSErrType)
{
	//uint8_t test;
	// Loop forever
//	while (1){
		// For testing purpose
	//	test = bLSErrType;
//	}
}

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		UART transmit function for interrupt mode (using ring buffers)
 * @param[in]	UARTPort	Selected UART peripheral used to send data,
 * 				should be UART1.
 * @param[out]	txbuf Pointer to Transmit buffer
 * @param[in]	buflen Length of Transmit buffer
 * @return 		Number of bytes actually sent to the ring buffer
 **********************************************************************/
uint32_t UARTSend(LPC_UART_TypeDef *UARTPort, uint8_t *txbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) txbuf;
    uint32_t bytes = 0;


	/* Temporarily lock out UART transmit interrupts during this
	   read so the UART transmit interrupt won't cause problems
	   with the index values */
	UART_IntConfig(UARTPort, UART_INTCFG_THRE, DISABLE);

	/* Loop until transmit run buffer is full or until n_bytes
	   expires */
	while ((buflen > 0) && (!__BUF_IS_FULL(rb.tx_head, rb.tx_tail)))
	{
		/* Write data from buffer into ring buffer */
		rb.tx[rb.tx_head] = *data;
		data++;

		/* Increment head pointer */
		__BUF_INCR(rb.tx_head);

		/* Increment data count and decrement buffer size count */
		bytes++;
		buflen--;
	}

	/*
	 * Check if current Tx interrupt enable is reset,
	 * that means the Tx interrupt must be re-enabled
	 * due to call UART_IntTransmit() function to trigger
	 * this interrupt type
	 */
	if (TxIntStat == RESET) {
		UART1_IntTransmit();
	}
	/*
	 * Otherwise, re-enables Tx Interrupt
	 */
	else {
		UART_IntConfig(UARTPort, UART_INTCFG_THRE, ENABLE);
	}

    return bytes;
}

/*********************************************************************//**
 * @brief		UART read function for interrupt mode (using ring buffers)
 * @param[in]	UARTPort	Selected UART peripheral used to send data,
 * 				should be UART1.
 * @param[out]	rxbuf Pointer to Received buffer
 * @param[in]	buflen Length of Received buffer
 * @return 		Number of bytes actually read from the ring buffer
 **********************************************************************/
uint32_t UARTReceive(LPC_UART_TypeDef *UARTPort, uint8_t *rxbuf, uint8_t buflen)
{
    uint8_t *data = (uint8_t *) rxbuf;
    uint32_t bytes = 0;

	/* Temporarily lock out UART receive interrupts during this
	   read so the UART receive interrupt won't cause problems
	   with the index values */
	UART_IntConfig(UARTPort, UART_INTCFG_RBR, DISABLE);

	/* Loop until receive buffer ring is empty or
		until max_bytes expires */
	while ((buflen > 0) && (!(__BUF_IS_EMPTY(rb.rx_head, rb.rx_tail))))
	{
		/* Read data from ring buffer into user buffer */
		*data = rb.rx[rb.rx_tail];
		data++;

		/* Update tail pointer */
		__BUF_INCR(rb.rx_tail);

		/* Increment data count and decrement buffer size count */
		bytes++;
		buflen--;

#if (AUTO_RTS_CTS_USE == 0)
		/* In case of driving RTS manually, this pin should be
		 * release into ACTIVE state if buffer is free
		 */
		if (RTS_State == INACTIVE)
		{
			if (!__BUF_WILL_FULL(rb.rx_head, rb.rx_tail))
			{
				// Disable request to send through RTS line
				UART_FullModemForcePinState(LPC_UART1, UART1_MODEM_PIN_RTS, \
						ACTIVE);
				RTS_State = ACTIVE;
			}
		}
#endif
	}

	/* Re-enable UART interrupts */
	UART_IntConfig(UARTPort, UART_INTCFG_RBR, ENABLE);

    return bytes;
}

//void __swi(8)  inc_5bit (U32 *cp);

/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief		c_entry: Main UART-FULLMODEM program body
 * @param[in]	None
 * @return 		int
 **********************************************************************/
int uart1_cmsis_init(void)
{
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	// Pin configuration for UART1
	//PINSEL_CFG_Type PinCfg;
	//uint32_t len;
	//uint32_t  idx;
	__IO FlagStatus exitflag;

	/*
	 * Initialize UART1 pin connect
	 * If using MCB1700 eval board, assign pin P2.0 - P2.7
	 * If using IAR 1768 KS board, assign pin P0.7 - P0.15
	 */
  LPC_PINCON->PINSEL4 |= (2 << 0);             /* Pin P2.0 used as TXD1 (Com1) */
  LPC_PINCON->PINSEL4 |= (2 << 2);             /* Pin P2.1 used as RXD1 (Com1) */
//Full control
	LPC_PINCON->PINSEL4 |= (2 << 4);             /* Pin P2.2 used as CTS1 (Com1) */
	LPC_PINCON->PINSEL4 |= (2 << 6);             /* Pin P2.3 used as DCD1 (Com1) */
	LPC_PINCON->PINSEL4 |= (2 << 8);             /* Pin P2.4 used as DSR1 (Com1) */
	LPC_PINCON->PINSEL4 |= (2 << 10);            /* Pin P2.5 used as DTR1 (Com1) */
	LPC_PINCON->PINSEL4 |= (2 << 12);            /* Pin P2.6 used as RI1 (Com1) */
	LPC_PINCON->PINSEL4 |= (2 << 14);            /* Pin P2.7 used as RTS1 (Com1) */

/*
#ifdef MCB_LPC_1768
	PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 2;
	for (idx = 0; idx <= 7; idx++){
		PinCfg.Pinnum = idx;
		PINSEL_ConfigPin(&PinCfg);
	}
#elif defined(IAR_LPC_1768)
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	for (idx = 15; idx <= 22; idx++){
		PinCfg.Pinnum = idx;
		PINSEL_ConfigPin(&PinCfg);
	}
#endif

*/

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = 19200;
//	UARTConfigStruct.Baud_rate = 2400;
//	UARTConfigStruct.Databits = UART_DATABIT_8;
//	UARTConfigStruct.Parity = UART_PARITY_NONE;
//	UARTConfigStruct.Stopbits = UART_STOPBIT_1;

	// Initialize UART1 peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)LPC_UART1, &UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
//	UARTFIFOConfigStruct.FIFO_DMAMode = DISABLE;
//	UARTFIFOConfigStruct.FIFO_Level = UART_FIFO_TRGLEV0;
//	UARTFIFOConfigStruct.FIFO_ResetRxBuf = ENABLE;
//	UARTFIFOConfigStruct.FIFO_ResetTxBuf = ENABLE;

	// Initialize FIFO for UART1 peripheral
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART1, &UARTFIFOConfigStruct);


#if (AUTO_RTS_CTS_USE==0)
	/*
	 * Determine current state of CTS pin to enable Tx
	 * activity
	 */
	if (UART_FullModemGetStatus(LPC_UART1) & UART1_MODEM_STAT_CTS) {
		// Enable UART Transmit
		UART_TxCmd((LPC_UART_TypeDef *)LPC_UART1, ENABLE);
	}
#else
	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART1, ENABLE);
#endif

	// Reset ring buf head and tail idx
	__BUF_RESET(rb.rx_head);
	__BUF_RESET(rb.rx_tail);
	__BUF_RESET(rb.tx_head);
	__BUF_RESET(rb.tx_tail);

#if AUTO_RTS_CTS_USE
//	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_RTS, ENABLE);
//	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_CTS, ENABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART1_INTCFG_CTS, ENABLE);
#else
	// Enable Modem status interrupt
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART1_INTCFG_MS, ENABLE);
	// Enable CTS1 signal transition interrupt
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART1_INTCFG_CTS, ENABLE);
	// Force RTS pin state to ACTIVE
//	UART_FullModemForcePinState(LPC_UART1, UART1_MODEM_PIN_RTS, ACTIVE);
	//RESET RTS State flag
	RTS_State = ACTIVE;
#endif


  /* Enable UART Rx interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RLS, ENABLE);

	/*
	 * Do not enable transmit interrupt here, since it is handled by
	 * UART_Send() function, just to reset Tx Interrupt state for the
	 * first time
	 */
	TxIntStat = RESET;

  /* preemption = 1, sub-priority = 1 */
  //NVIC_SetPriority(UART1_IRQn, 1); //((0x01<<3)|0x01)
	/* Enable Interrupt for UART1 channel */
    NVIC_EnableIRQ(UART1_IRQn);
	return 1;
}


/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}

/*
 * @}
 */
