/*
 * DS3231.c
 *
 *  Created on: Feb 6, 2023
 *      Author: Nasrul
 */

#include "DS3231.h"

// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}

// function to set time

void ds3231_SetTime (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
	uint8_t set_time[7];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);
	set_time[3] = decToBcd(dow);
	set_time[4] = decToBcd(dom);
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);

	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, 1, set_time, 7, 100);
}

void ds3231_SetTimeByTimeStruct (TIME time)
{
	ds3231_SetTime (time.seconds, time.minutes, time.hour, time.dayofweek, time.dayofmonth, time.month, time.year);
}

void ds3231_GetTime (TIME* time)
{
	uint8_t get_time[7];
	if(HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, 1, get_time, 7, 100)==HAL_OK)
	{
		time->seconds = bcdToDec(get_time[0]);
		time->minutes = bcdToDec(get_time[1]);
		time->hour = bcdToDec(get_time[2]);
		time->dayofweek = bcdToDec(get_time[3]);
		time->dayofmonth = bcdToDec(get_time[4]);
		time->month = bcdToDec(get_time[5]);
		time->year = bcdToDec(get_time[6]);
	}
}

float ds3231_GetTemp (void)
{
	uint8_t temp[2];

	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x11, 1, temp, 2, 100);
	return ((temp[0])+(temp[1]>>6)/4.0);
}

void ds3231_ForceTempConv (void)
{
	uint8_t status=0;
	uint8_t control=0;
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0F, 1, &status, 1, 100);  // read status register
	if (!(status&0x04))
	{
		HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &control, 1, 100);  // read control register
		HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, 1, (uint8_t *)(control|(0x20)), 1, 100);
	}
}
