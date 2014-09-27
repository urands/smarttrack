
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_usart.h>

#include "cmsis_os.h"		/* RTL OS declaration*/

#include "../gpio/gpio.h"
#include "../gpio/adc.h"
 #include "../gsm/inc/gsm-task.h"
#include "../gps/inc/gps-task.h"
 #include "../gps/nmea_task.h"
 
 #include "../hal/at_serial.h"
 #include "../hal/nmea_serial.h"
//#include "../gpio/ctrl.h"
#include "../interface/bufmsg.h"
#include "../interface/manager.h"
#include "../interface/NOR_SST25VF016.h"
#include "rl_fs.h"                      /* FileSystem definitions             */

void test1(int a){
	
	
}

void init (void const *argument){
	 fsStatus stat;
	int a =0;
	FILE* f;
	stat = finit ("F0:");
  if (stat == fsOK) {
    stat = fmount ("F0:");
    if (stat == fsOK) {
			a=5;
      //printf ("Drive M0 ready!\n");
    }
    else if (stat == fsNoFileSystem) {
      /* Format the drive */
      //printf ("Drive M0 not formatted!\n");
      //cmd_format ("M0:");
				if (fformat ("F0:", "") == fsOK) {
    //  printf ("Format completed.\n");
    //  if (fvol ("F1", label, NULL) == 0) {
					a = 1;
         // printf ("Volume label is \"%s\"\n", label);
     // }
    }
    else {
     // printf ("Formatting failed.\n");
			a = 2;
    }
			
			
			a=6;
    }
    else {
			a=7;
      //printf ("Drive M0 mount failed with error code %d\n", stat);
    }
  }
  else {
    a=8;
		//printf ("Drive M0 initialization failed!\n");
  }
  
  //printf ("\nDone!\n");
	
	
	
   /* Format the drive */

  
	
	
	test1(a);
	/* flash  test init */
	
	f = fopen ("test.txt", "w"); /* open a file for writing            */
	if (f == NULL) {
    //printf ("\nCan not open file!\n");  /* error when trying to open file     */
    return;
  }
	fclose (f);         
	
	
	
	
	
#ifdef __BUFMSGTEST__	
	/*  Buffer Unit test */ 
	bufUnitTest();
#endif
 
}

 osThreadDef(init,osPriorityNormal,1,0);
 
 //osThreadDef(gsm_thread,osPriorityNormal,1,0);

int main(void){
	
	//unsigned int a;
	gpioInit();
 
	if (osKernelInitialize () != osOK) { // check osStatus for other possible valid values
				// exit with an error message
	}
	mngInit(); //Initialize and registry tasks
	

	//osThreadCreate(osThread(init),0);
	if (!osKernelRunning ()) { // is the kernel running ?
			if (osKernelStart () != osOK) { // start the kernel
										// kernel could not be started
			}
	}
	return 0;
}


 void assert_failed(uint8_t* file, uint32_t line){
	 //printf("assert_failed\n");
 }

