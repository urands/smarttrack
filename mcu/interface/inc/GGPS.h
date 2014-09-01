/******************************************************************************/
/* GLCD.h: Graphic LCD function prototypes and defines                        */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2011 Keil - An ARM Company. All rights reserved.        */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#ifndef _GLCD_H
#define _GLCD_H


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





extern void GLCD_WrCmd          (unsigned char cmd);
extern void GLCD_WrReg          (unsigned char reg, unsigned short val); 

extern int GGPS_RdDat(char* string); 

#define GPS_MAX_LINE_SIZE 127

#endif /* _GLCD_H */
