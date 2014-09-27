#include "manager.h"

#include "../gps/nmea_task.h"
#include "../gps/nmea_proto.h"

#include "../gsm/gsm_task.h"
#include "../interface/bufmsg.h"
#include "../interface/adc.h"

#include "cmsis_os.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern osThreadId gps_thread_id;

/* Define low priority mngTask */
osThreadDef(mngTask,osPriorityNormal,1,0);

void Timer1_Callback (void const *arg); // prototypes for timer callback function
osTimerDef (Timer1, Timer1_Callback); // define timers
osTimerId timerid; 

unsigned int ltime = 0;

void Timer1_Callback (void const *arg) { // timer callback function
// arg contains &exec
// called every second after osTimerStart
	ltime++;
}
osTimerDef (Timer, Timer1_Callback); // define timer
uint32_t exec; // argument for the timer call back function



/* Initialize manager */
void mngInit( void ){
	bufInit();
	//nmeaInit();
	gsmInit();
	adcInit();
	
	timerid = osTimerCreate (osTimer(Timer1), osTimerPeriodic, 0);
	
	if ( timerid != 0 ){
		osTimerStart (timerid, 1000); // start timer
	}
	
	
	
	osThreadCreate(osThread(mngTask),0);
}


#ifdef WIALON_TRACKER			

/* Core task */
void mngTask(void const *arg){
	T_NMEA_PACKED nmea;
	char wialon[BUFMSG_MAX_DATA];
	unsigned int lasttime = 0;
	unsigned int flcourse = 0;
	unsigned int steptime = 12;
	float lastcourse = 0.0;
	
	while(1){
		
		if ( nmeaRecvData(&nmea, 1000 ) > 0 ){
			
			if ( nmea.speedkm > 3.0){
				if ( abs(nmea.course - lastcourse) >=7.0 ) {
					flcourse = 1;
				}
				
			}
			
			if ( nmea.speedkm > 4.0){
				if ( (nmea.course - lastcourse) >=3.0 ) steptime--;
			}
			
			if ( steptime < 3) steptime = 3;
			if ( steptime > 40) steptime = 40;
			
			if ( nmea.time != 0){
					memset(wialon,0,sizeof(wialon));
					nmeaToWialon(wialon, &nmea);
				
				if ( ((nmea.time - lasttime) >= steptime) || ( flcourse &&  (nmea.time - lasttime) >= 1 )){
					if ( bufSendData (wialon, strlen(wialon), 1000) > 0 ){
						 if (lasttime !=0 )osSignalSet (gps_thread_id, 0x00000001); 
						 lasttime = nmea.time;
						 flcourse = 0;
						 lastcourse = nmea.course;
						 steptime++;
						 
						


					}
				}
			}
		}else{
			osSignalClear (gps_thread_id, 0x01);
		}
	}
	
}

#else

void mngTask(void const *arg){
	char params[BUFMSG_MAX_DATA];
	
	unsigned int vmin, vmax = 0;
	unsigned int ap = 0;
	unsigned int indent = 0;
	unsigned int calibrate = 1;
	unsigned int summ = 0;
	unsigned int iter = 0;
	unsigned int lastvalue = 0;
	unsigned int lasttick = 0;
	unsigned int worktime = 0;
	unsigned int bytime = 0;
	unsigned int type = 0;
	unsigned int total = 0;
	unsigned int fixval = 0;
	unsigned int gtotal = 0;
	unsigned int gtlast = 0;
	
	//sprintf(gsm,"GET /dev.php?devid=1 HTTP/1.1\r\nHost: miss-n.ru\r\nConnection: keep-alive\r\n");
	
		unsigned int value = 0;
	
	
		//send on start
		indent++;
		while(1){
			osDelay(10);
			value = adcGet(0);
			
			
			if ( calibrate == 1){
				summ+= value;
				if ( vmax == 0 ) vmin = vmax = value;
				if ( value > vmax ) vmax = value;
				if ( value < vmin ) vmin = value;	
				
				
				iter++;
				///GET dev.php?devid=1&
				if ( iter == 300){
					calibrate = 0;
					summ = summ / 300;
				}
			}else{
	
			//	ap += summ;
			//	ap = ap >> 1;
				bytime = ltime - lasttick;
				if ( lastvalue > (summ + 200 ) || lastvalue < (summ - 200 ) || bytime > 59 || type == 0){
						if ( type != 0  && (lastvalue > (summ + 200 ) || lastvalue < (summ - 200 )) ) type = 2; else if (type!=0) type = 3;
						if ( fixval == 0 && indent > 2 ) {
									fixval = summ;
									gtlast = 0;
						}
						
						worktime = ltime - lasttick;
					
						if ((fixval > (summ + 200 ) || fixval < (summ - 200 )) && fixval!= 0){
								gtotal+= worktime;
							
						}
					
						lastvalue = summ;		
						
						lasttick = ltime;
					//summ*0.0008, ltime, indent,
						sprintf(params,"GET dev.php?i=%d&did=1&t=%d&v=%d&wt=%d&uv=%f&type=%d&tt=%d&tgt=%d HTTP/1.1\r\nHost: miss-n.ru\r\n",indent, ltime, summ, worktime, summ*0.0008, type, ltime - total, gtotal  );
						if ( bufSendData (params, strlen(params), 1000) > 0 ){
							indent++;
							calibrate = 1;
							vmax = 0;
							iter = 0;
							summ = 0;
							ap = 0;
							if ( type == 2 || type == 0 ) total = ltime;
							type = 1;
						}
				}else{
					
						calibrate = 1;
						calibrate = 1;
						vmax = 0;
						iter = 0;
						summ = 0;
						ap = 0;
						type = 1;
				}
				
				
				
			}
				
	
			
			
			
		}
	
}



#endif