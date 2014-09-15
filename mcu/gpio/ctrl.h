/* 
*  Project: Smarttrack
*  Author: uran
*  Description: Retarget stdio 
*  History:
*  						12.08.14 create
*/

#ifndef __CTRL_H_
#define __CTRL_H_

#include "GPIO_STM32F10x.h"
#include "gpio.h"


__FORCE_INLINE void ctrlGSM_PWRKEY( uint32_t val ){ GPIO_PinWrite(GPIOC, pinGSM_PWRKEY, val); }

#endif
