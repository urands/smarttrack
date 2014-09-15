/*----------------------------------------------------------------------------
 * Name:    tasks
 * Purpose: LPC1768 tasks
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/
 
 #ifndef __BUFFER_H_
 #define __BUFFER_H_

#include <stm32f10x.h>
 
 
#ifdef FLASH 
 #define GSM_BUF_MAX_COUNT 1
#else
 #define GSM_BUF_MAX_COUNT 5
#endif
/*
	Buffer header
*/
typedef struct {
  uint32_t date;
  uint32_t time;
  uint16_t longitude_h;
  uint16_t longitude_l;
  char longitude_d;
  uint16_t lotitude_h;
  uint16_t lotitude_l;
  char lotitude_d;
  uint16_t course;
  uint16_t speed;
  uint16_t height;
  uint8_t sat;
  float hdop;
} GSM_Packet2;




int bufferinit(void);

int gps_read_from_buffer(char* gsm_line);
int gps_write_to_buffer(char* gps_line);
 
 
int GGPS_Pack_NMEA2(char* nmea_rmc_msg, GSM_Packet2* pk);
 
int GGPS_Unpack_Wialon2( GSM_Packet2* pack, char* w_msg);
int GGPS_Unpack_Wialon2_full( GSM_Packet2* pack, char* w_msg);

extern char g_gps_sat_number[6];
extern char g_gps_height[6];
extern char g_gps_hdop[6];
extern int	g_buffer_msg;
extern int	g_buffer_file;
 
 
 #endif //__BUFFER_H_
