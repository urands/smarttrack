#ifndef __ADC_H___
#define __ADC_H___

/* GSM Task init */
void adcInit( void );


/*
* Task running
*/
void adcTask(void const *arg);


unsigned int adcGet(unsigned char channel);



#endif
