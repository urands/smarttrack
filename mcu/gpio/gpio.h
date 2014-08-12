

#ifndef __GPIO_H
#define __GPIO_H

#include "GPIO_STM32F10x.h"


#define pinGSM_PWRKEY 2

#define gpioSet(GPPIOx, gpioPin ) GPPIOx->ODR |= ( 1 << gpioPin)
#define gpioClr(GPPIOx, gpioPin ) GPPIOx->ODR &= ~(1 << gpioPin )


void gpioInit( void );


void ctrlSwitchGSMPwrKey( void );


#endif
