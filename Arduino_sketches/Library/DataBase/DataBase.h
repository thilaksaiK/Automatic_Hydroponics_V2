/* 
	I2CDB - I2C Daughter board
	Library for I2C communication between daughter board
	Created by K S Sai Thilak, February 27, 2021.
*/

#ifndef DataBase_h
#define DataBase_h

#include "Arduino.h"
#include "Wire.h"
#include "I2C_eeprom.h"

#define DD 	1
#define HoD	2
#define MD	3
#define TD	4
#define HuD	5
#define LD  	6


class DataBase
{		public:
		storeAt(I2C_eeprom ee, int Address, int Day, int Hour, int Minute, int Temperature, int Humidity, byte LevelArray);
		int getNextAddress(I2C_eeprom ee); 
		resetAddress(I2C_eeprom ee);
 		int getDataAt(I2C_eeprom ee, int Address, byte Data);
};



#endif