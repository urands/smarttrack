/*----------------------------------------------------------------------------
 * Name:    gpscntrl
 * Purpose: LPC1768 Low level tool functions
 * Note(s):
 *----------------------------------------------------------------------------
 * Author Blokhin Y
 * Copyright (c) Systelen 2012
 *----------------------------------------------------------------------------*/


#ifndef __GPS_CONTROL_H__
#define __GPS_CONTROL_H__

void GPS_init (void);
void GPS_deinit (void);

#define GPS_MAX_LINE_SIZE 255




extern int GGPS_GPRMC(char *string );
extern int GGPS_param(char* instr, char* outstr, int paramindex);
extern int GGPS_Build_Wialon_Packet(char* nmea_rmc_msg, char* wialan_msg);
extern int GGPS_wait_sec(int sec, char* str);
extern int GPRS_test_wialon(char* ansver);

extern void GGPS_init (void);
extern int GGPS_enable (void);
extern int GGPS_reset( void );
extern int GGPS_check(void);

int GGPS_send(char* str);

extern char g_gps_sat_number[6];
extern char g_gps_height[6];

extern int GGPS_RdDat(char* string); 





#endif //__GPS_CONTROL_H__
