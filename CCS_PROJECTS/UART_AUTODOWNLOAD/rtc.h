/*
 * rtc.h
 *
 *  Created on: Dec 19, 2016
 *      Author: gunnarpope
 */

#include <stdint.h>

#ifndef RTC_H_
#define RTC_H_

typedef struct Calendar
{
    //! Seconds of minute between 0-59
    uint8_t Seconds;
    //! Minutes of hour between 0-59
    uint8_t Minutes;
    //! Hour of day between 0-23
    uint8_t Hours;
    //! Day of week between 0-6
    uint8_t DayOfWeek;
    //! Day of month between 1-31
    uint8_t DayOfMonth;
    //! Month between 0-11
    uint8_t Month;
    //! Year between 0-4095
    uint16_t Year;
} Calendar;



static void RTC_B_initCalendar(Calendar *);
static void convert_ascii2bcd();
void printTime();
void printTimeStamp(unsigned int, unsigned int);

uint8_t update_RTC(char);
void rtc_init(void);



#endif /* RTC_H_ */
