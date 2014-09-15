/*----------------------------------------------------------------------------
 * Name:    buffer
 * Purpose: LPC1768 tasks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
#include "cmsis_os.h"		/* RTL OS declaration*/
#include <stdio.h>
#include "inc/buffer.h"
//#include "tasks.h"
//#include "gsm-task.h"
#include "../core/inc/toolset.h"
//#include <string.h>
//#include "GGPS.h"
//#include "accel.h"

char g_gps_sat_number[6];
char g_gps_height[6];
char g_gps_hdop[6];
int	g_buffer_msg;
int	g_buffer_file;

int g_cnt_reopen;

//os_mbx_declare (g_buffer_base,GSM_BUF_MAX_COUNT);           /* Declare an RTX mailbox              */
//_declare_box (mpool,sizeof(GSM_Packet2),GSM_BUF_MAX_COUNT);		/* Dynamic memory pool                */


osPoolDef(mpool, GSM_BUF_MAX_COUNT, GSM_Packet2); // Define memory pool
osPoolId mpool;
osMessageQDef(MsgBox, GSM_BUF_MAX_COUNT, GSM_Packet2); // Define message queue
osMessageQId MsgBox;



int gps_read_from_buffer(char* gsm_line){
// 	int result;
	GSM_Packet2* ptr_pack;
	GSM_Packet2  pack;
	 osEvent evt;

//	g_buffer_file = getWriteNum();
	
	if ( g_buffer_msg > 0 || ( g_buffer_file == 0) ){
//		if ( os_mbx_wait (g_buffer_base, (void **)&ptr_pack, 6000)== OS_R_TMO){ /* wait for the message    */
			//Time out
			//return NOT;
		evt = osMessageGet(MsgBox, 120000); // wait for message
		if (evt.status == osEventMessage) {
				ptr_pack = evt.value.p;
		   
		}else{
			return NOT;
		}
			
		//}
		g_buffer_msg--;
//		GPRS_update_parameters(&g_gsm_param);
		if (GGPS_Unpack_Wialon2_full(ptr_pack, gsm_line) == OK ){
//			_free_box (mpool, ptr_pack);           /* free memory allocated for message  */
				osPoolFree(mpool, ptr_pack); // free memory allocated for message
			return OK;
		}
//		_free_box (mpool, ptr_pack);           /* free memory allocated for message  */
		osPoolFree(mpool, ptr_pack); // free memory allocated for message

	}else{

	   if ( g_buffer_file > 0 ){
	   		memset(&pack,0, sizeof(GSM_Packet2));
		//    requestFromBuf((char*)&pack,sizeof(GSM_Packet2), PACK);
			  //g_buffer_file--;
			  if (pack.date == 0 ){
					TOOL_delay_nl(10);
					return NOT;
				}
			
	///		GPRS_update_parameters(&g_gsm_param);
			if (GGPS_Unpack_Wialon2_full(&pack, gsm_line) == OK ){
				return OK;
			}
	   }
	}
  
	return NOT;


}

//GSM_BUF_MAX_COUNT

int gps_write_to_buffer(char* gps_line){
	GSM_Packet2* ptr_pack;

//	ptr_pack = _alloc_box (mpool); 
	ptr_pack = osPoolAlloc(mpool); 	
	
	if ( ptr_pack == 0) return 0;

	if (GGPS_Pack_NMEA2(gps_line, ptr_pack ) == OK ){
		/*
	  if ( g_buffer_msg+1 >= GSM_BUF_MAX_COUNT ){
	//	  sendToBuf( (char*) ptr_pack, sizeof(GSM_Packet2));
		  //g_buffer_file++;
			g_cnt_reopen++;
			if (g_cnt_reopen > 10 ){
		//			reopenStream();
					g_cnt_reopen = 0;
				
			}
//		  _free_box (mpool, ptr_pack);  
			 osPoolFree(mpool, ptr_pack); 
		  return OK;
	  }

	
//	  os_mbx_send (g_buffer_base, ptr_pack, 0xffff); 
		*/
		
		  g_buffer_msg++;
		 osMessagePut(MsgBox, (uint32_t)ptr_pack, 100); // Send Message

	}else{
		osPoolFree(mpool, ptr_pack); 
//		_free_box (mpool, ptr_pack);           /* free memory allocated for message  */
	}
   return OK;
}



int bufferinit(void){
//	_init_box (mpool, sizeof(mpool),    /* initialize the 'mpool' memory for   */
//              sizeof(GSM_Packet2));        /* the membox dynamic allocation       */
//	os_mbx_init (&g_buffer_base, sizeof(g_buffer_base) );/* initialize the mailbox             */

	g_buffer_msg = 0;
	//g_buffer_file = getWriteNum();
	g_cnt_reopen = 0;
	
	 mpool = osPoolCreate(osPool(mpool)); // create memory pool
	MsgBox = osMessageCreate(osMessageQ(MsgBox), NULL); // create msg queue
	

	return OK;
}


/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int GGPS_Unpack_Wialon2( GSM_Packet2* pack, char* w_msg){
	
   sprintf(w_msg,"#SD#%06i;%06i;%04i.%04i;%c;%05i.%04i;%c;%i;%i;%i;%i",
   	pack->date,
	pack->time,
	pack->longitude_h,
	pack->longitude_l,
	pack->longitude_d,
	pack->lotitude_h,
	pack->lotitude_l,
	pack->lotitude_d,
	pack->speed,
	pack->course,
	pack->height,
	pack->sat
   );
   return OK;
}
//#D#date;time;lat1;lat2;lon1;lon2;speed;course;height;sats;hdop;inputs;outputs;adc;ibutton;params\r\n
int GGPS_Unpack_Wialon2_full( GSM_Packet2* pack, char* w_msg){
  // t_accel_out accel;

 //  get_accel(&accel);

   sprintf(w_msg,"#D#%06i;%06i;%04i.%04i;%c;%05i.%04i;%c;%i;%i;%i;%i;%f;NA;NA;;NA;ANG:2:%f,AMP:2:%f,CSQ:1:%i",
   	pack->date,
	pack->time,
	pack->longitude_h,
	pack->longitude_l,
	pack->longitude_d,
	pack->lotitude_h,
	pack->lotitude_l,
	pack->lotitude_d,
	pack->speed,
	pack->course,
	pack->height,
	pack->sat,
	pack->hdop,
	1,
	1,
	1	
	//CSQ param
//	accel.Ang,
//	accel.Amp,
//	g_gsm_param.csq
	
   );
	
   return OK;
}




/*******************************************************************************
* Write a value into LCD controller register                                   *
*   Parameter:      reg:      lcd register address                             *
*                   val:      value to write into reg                          *
*   Return:                                                                    *
*******************************************************************************/
int GGPS_Pack_NMEA2(char* nmea_rmc_msg, GSM_Packet2* pk){
   char param[15];
   char* pparam;
   int i;
	 int value;
   float speed;
   if (GGPS_param(nmea_rmc_msg, param, 9 ) == 0 ) return NOT;
   pparam = param;  while(*pparam == '0') pparam++;
   if (*pparam == 0) pk->date = 0; else sscanf(pparam, "%i", &pk->date);
   if (GGPS_param(nmea_rmc_msg, param, 1 ) == 0 ) return  NOT;
   pparam = param;  while(*pparam == '0') pparam++;
   if (*pparam == 0) pk->time = 0; else sscanf(pparam, "%i", &pk->time);

   if (GGPS_param(nmea_rmc_msg, param, 3 ) == 0 ) return  NOT;
   for (i=0;i<strlen(param);++i) 
  	if (param[i]=='.') {   
		pparam =  param+i+1;  while(*pparam == '0') pparam++;
		if (*pparam == 0) value = 0; else sscanf(pparam, "%i",(int*) &value);
		pk->longitude_l = (uint16_t) value;
		param[i] = 0; 
		pparam =  param;  while(*pparam == '0') pparam++;
		if (*pparam == 0) value = 0; else  sscanf(pparam, "%i",(int*) &value);
		pk->longitude_h = (uint16_t) value;
   } 
   if (GGPS_param(nmea_rmc_msg, param, 4 ) == 0 ) return  NOT;
   pk->longitude_d = param[0];


   if (GGPS_param(nmea_rmc_msg, param, 5 ) == 0 ) return  NOT;
   for (i=0;i<strlen(param);++i) if (param[i]=='.') {   
   			pparam =  param+i+1;  while(*pparam == '0') pparam++;
			if (*pparam == 0) value = 0; else sscanf(pparam, "%i",(int*) &value);
		    pk->lotitude_l = (uint16_t) value;
			param[i] = 0; 
			pparam =  param;  while(*pparam == '0') pparam++;
			if (*pparam == 0) value = 0; else  sscanf(pparam, "%i",(int*) &value);
	  	    pk->lotitude_h = (uint16_t) value;
   } 

   if (GGPS_param(nmea_rmc_msg, param, 6 ) == 0 ) return  NOT;
   pk->lotitude_d = param[0];

   //Speed
   if (GGPS_param(nmea_rmc_msg, param, 7 ) == 0 ) {
   		pk->speed = 0;
		return NOT;
   }else{
     	//Need mul on 1.852 knot to km/h
   		sscanf(param, "%f", &speed);
		speed*=1.852;
		pk->speed = speed;
	}
   //Course
   pk->course = 0;
   if (GGPS_param(nmea_rmc_msg, param, 8 ) == 0 ){
	  pk->course = 0;
	  return NOT;
   }else{
	   	for (i=0;i<strlen(param);++i) if (param[i]=='.') { param[i] = 0; break;} 
		sscanf(param, "%i", (int*)&value);
		pk->course  = value;
	}
   for (i=0;i<strlen(g_gps_height);++i) if (g_gps_height[i]=='.') { g_gps_height[i] = 0; break;} 
   sscanf(g_gps_height, "%i", (int*)&value); 	
	 pk->height = (uint16_t) value;
   if ( g_gps_sat_number[0] == '0' && g_gps_sat_number[1] != 0){
   		 pk->sat = g_gps_sat_number[1] - '0';
   }else{
	   sscanf(g_gps_sat_number, "%i", (int*)&value); 	
	   pk->sat = (uint8_t) value;
	}
   sscanf(g_gps_hdop, "%f", &speed);
	 pk->hdop = speed;
   return OK;
}
