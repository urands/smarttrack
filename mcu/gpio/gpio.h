/* 
*  Project: Smarttrack
*  Author: uran
*  Description: Retarget stdio 
*  History:
*  						12.08.14 create
*/
#ifndef __GPIO_H
#define __GPIO_H

#include "GPIO_STM32F10x.h"
#include "stm32f10x_rcc.h"

/*
* PORT DEFINITIONS
*/

//PORT C

//PC2 GSM_PWRKEY 
#define pinGSM_PWRKEY 2



void gpioInit( void );


void ctrlGSM_PWRKEY(int state);


#endif
