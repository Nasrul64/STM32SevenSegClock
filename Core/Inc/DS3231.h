/*
 * DS3231.h
 *
 *  Created on: Feb 6, 2023
 *      Author: Nasrul
 */

#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#include "i2c.h"

#define DS3231_ADDRESS 0xD0

typedef struct TIME_S{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} TIME;

void ds3231_SetTime (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);
void ds3231_SetTimeByTimeStruct (TIME time);
void ds3231_GetTime (TIME* time);
float ds3231_GetTemp (void);
void ds3231_ForceTempConv (void);

#endif /* INC_DS3231_H_ */
