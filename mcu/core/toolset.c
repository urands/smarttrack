/*----------------------------------------------------------------------------
 * Name:    toolset.c
 * Purpose: LPC1768 Low level tool functions
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/


#include "inc/toolset.h"
//#include "gpio.h"


/*---------------------------------------------------------------------------
  Delay function
  delays number of tick Systicks (happens every 10 ms)
 *---------------------------------------------------------------------------*/
void TOOL_delay(unsigned int ms ){
//	GPIO_led(LED_NONE);
	TOOL_delay_nl(ms);
//	GPIO_led(LED_RED);
}


void TOOL_delay_nl(unsigned int ms ){
	//unsigned int i;
	//for (i=0;i<ms*100000;++i);
//	os_dly_wait(ms);
	osDelay(ms);
}

void TOOL_error(unsigned int err_code){
  switch (err_code){
   case ERROR_SIM:
   	while(1){
	   TOOL_delay(50);
	   TOOL_delay_nl(50);
	}



  }


}


int g_reset_device;


void TOOL_reset(void){
  g_reset_device = 1;
}

void TOOL_wdt_init(int time_wdt){
  g_reset_device = 0;

 // WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_RESET);

 // WDT_Start(time_wdt);

}

void TOOL_wdt(void){
  if (g_reset_device == 0 ){
    tsk_lock();
//	WDT_Feed();
	tsk_unlock();
  }


}


void NVIC_interrups_init(void){
	/* 2 bits for pre-emption priority and 2 bits for subpriority */
//	NVIC_SetPriorityGrouping(4);

	/* Set USART1 interrupt preemption priority to 1 */
//	NVIC_SetPriority(UART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));

	/* Set SysTick interrupt preemption priority to 3 */
//	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0));
}
