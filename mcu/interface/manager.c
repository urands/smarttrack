#include "manager.h"

#include "../gps/nmea_task.h"
#include "../gps/nmea_proto.h"

#include "cmsis_os.h"

/* Define low priority mngTask */
osThreadDef(mngTask,osPriorityNormal,1,0);

/* Initialize manager */
void mngInit( void ){
	
	nmeaInit();
	osThreadCreate(osThread(mngTask),0);
}

/* Core task */
void mngTask(void const *arg){
	T_NMEA_PACKED nmea;
	
	while(1){
		
		if ( nmeaRecvData(&nmea, 1000 ) > 0 ){
				nmea.hdop = 12;
			
		}
		
		
	}
	
	
	
}

