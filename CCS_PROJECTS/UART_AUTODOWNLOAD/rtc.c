/*
 * rtc.c
 *
 *  Created on: Dec 19, 2016
 *      Author: gunnarpope
 */

#include "msp430fr5969.h"
#include "rtc.h"
#include "uart_driver.h"
#include <stdio.h>

Calendar currentTime;

extern size_t strlen(const char *);

static volatile char index = 0;
static char ascii_rtc[16] = {};
static uint8_t updatingRTC = 0;
static char str[50];

uint8_t update_RTC(char x){
	//SEND A '~' character to start the upload sequence, then send all the Time values in ASCII, BCD format, (2016 = '2','0','1','16')

	if (x == '~'){
    		updatingRTC = 1;
    		sprintf(str, "\r\nEnter the Year: 'YYYY'\r\n");
    		PutData(str,strlen(str));
	}
    else if (updatingRTC == 1){
    		ascii_rtc[index++] = x - '0';
    		switch(index){
    		case 4:
    			sprintf(str, "\r\nEnter the Month: 'MM'\r\n");
    		    	PutData(str,strlen(str));
    		    	break;
    		case 6:
    			sprintf(str, "\r\nEnter the Day of the Month: 'DD'\r\n");
			PutData(str,strlen(str));
			break;
    		case 8:
			sprintf(str, "\r\nEnter the Day of the Week: '00' = Sunday\r\n");
			PutData(str,strlen(str));
			break;

    		case 10:
			sprintf(str, "\r\nEnter the Hour (24h): 'HH'\r\n");
			PutData(str,strlen(str));
			break;

    		case 12:
			sprintf(str, "\r\nEnter the Minute: 'MM'\r\n");
			PutData(str,strlen(str));
			break;

    		case 14:
			sprintf(str, "\r\nEnter the Second: 'SS'\r\n");
			PutData(str,strlen(str));
			break;
    		}

    }


    if (index > 15 ){
    		updatingRTC = 0;
    		index = 0;
    		convert_ascii2bcd();
    		RTC_B_initCalendar(&currentTime);
    }

    return (updatingRTC);
}




static void convert_ascii2bcd(){
	currentTime.Year = (ascii_rtc[0] << 12 | ascii_rtc[1] << 8 | ascii_rtc[2] << 4 | ascii_rtc[3] );
	currentTime.Month 		= 	(ascii_rtc[4] 	<< 4 | ascii_rtc[5]);
	currentTime.DayOfMonth 	= 	(ascii_rtc[6] 	<< 4 | ascii_rtc[7]);
	currentTime.DayOfWeek	=	(ascii_rtc[8] 	<< 4 | ascii_rtc[9]);
	currentTime.Hours		= 	(ascii_rtc[10] 	<< 4 | ascii_rtc[11]);
	currentTime.Minutes		= 	(ascii_rtc[12] 	<< 4 | ascii_rtc[13]);
	currentTime.Seconds		= 	(ascii_rtc[14] 	<< 4 | ascii_rtc[15]);
}

void rtc_init(){

	//hold the RTC time, BCD format , the rest are defaults with no interrupts
	RTCCTL01 |= RTCRDYIE;

	//use BCD format
	RTCCTL01 |= RTCBCD;
	RTCCTL01 &= ~RTCHOLD;
//******************************** This sections works and has been tested for fsamp = 8.0000Hz
//	//prescale the 1 sec clock by 8, to get an 8Hz sample frequency
//	//32768/(256*16) = 8 Hz
	RTCPS0CTL |= ( 0x1C ); //this works for a 8Hz sample frequency (divide by 256)
	RTCPS1CTL |= ( 0xC | RT1PSIE);  //divide by 16
//******************************** This sections works and has been tested for fsamp = 8.0000Hz

	//******************************** This sections works and has been tested for fsamp = 2.0000Hz
	//prescale the 1 sec clock by 8, to get an 8Hz sample frequency
	//32768/(256*64) = 2 Hz
//	RTCPS0CTL |= ( 0x1C ); //this works for a 8Hz sample frequency (divide by 256)   //the two below were previously enabled
//	RTCPS1CTL |= ( 0x14 | RT1PSIE);  //divide by 64


	//******************************** This sections works and has been tested for fsamp = 2.0000Hz

	//setup the RTC to trigger interrupts on the minute
	RTCCTL01 |= RTCTEVIE; //this should automatically trigger on the minute (RTCTEVx = 00)

}

static void RTC_B_initCalendar(Calendar *time){
	//stop the clock
	RTCCTL01 &= ~RTCHOLD;

	RTCSEC  = time->Seconds;
	RTCMIN  = time->Minutes;
	RTCHOUR = time->Hours;
	RTCDOW  = time->DayOfWeek;
	RTCDATE = time->DayOfMonth;
	RTCMON  = time->Month;
	RTCYEAR = time->Year;

	//star the clock
	RTCCTL01 &= ~RTCHOLD;

	printTime();
}

void printTime(){
	while(!(RTCCTL01 & RTCRDY)){}

	sprintf(str, "\r\nThe Time is: ");
	PutData(str,strlen(str));

	char h,l = 0;
	h = (((RTCHOUR >> 4) & 0x0F) + '0');
	l = (RTCHOUR &  0x0F) + '0';
	sprintf(str, "%c%c:",h,l);
	PutData(str,strlen(str));

	h = (((RTCMIN >> 4) & 0xF) + '0');
	l = (RTCMIN &  0xF) + '0';
	sprintf(str, "%c%c:",h,l);
	PutData(str,strlen(str));

	h = (((RTCSEC >> 4) & 0xF) + '0');
	l = (RTCSEC &  0xF) + '0';
	sprintf(str, "%c%c\r\n",h,l);
	PutData(str,strlen(str));

}

void printTimeStamp(unsigned int dowhr, unsigned int minsec){

	char h,l,dow,hr,min,sec = 0;
	dow = (dowhr >> 8) & 0x00FF;
	hr  = (dowhr & 0x00FF);
	min = (minsec >> 8) & 0x00FF;
	sec = (minsec & 0x00FF);

	//print the dow
	h = (((dow >> 4) & 0x0F) + '0');
	l = (dow &  0x0F) + '0';
	sprintf(str, "%c%c",h,l);
	PutData(str,strlen(str));


	//print the hour
	h = (((hr >> 4) & 0x0F) + '0');
	l = (hr &  0x0F) + '0';
	sprintf(str, "%c%c",h,l);
	PutData(str,strlen(str));

	//print the min
	h = (((min >> 4) & 0xF) + '0');
	l = (min &  0xF) + '0';
	sprintf(str, "%c%c",h,l);
	PutData(str,strlen(str));

	//print the sec
	h = (((sec >> 4) & 0xF) + '0');
	l = (sec &  0xF) + '0';
	sprintf(str, "%c%c",h,l);
	PutData(str,strlen(str));

}
