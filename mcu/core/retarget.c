/* 
*  Project: Smarttrack
*  Author: uran
*  Description: Retarget stdio 
*  History:
*  						12.08.14 create
*/

#include <stdio.h>
#include <stm32f10x.h>
#include "cmsis_os.h"		/* RTL OS declaration*/

struct __FILE { int handle; };
FILE __stdout;
FILE __stdin;

int fputc(int c, FILE *f)
{
	//TODO: retarget to USB connections
	//ITM_SendChar(c); //Use trace SVO
	return (c);
}
