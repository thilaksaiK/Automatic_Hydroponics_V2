/* 
	I2CDB - I2C Daughter board
	Library for I2C communication between daughter board
	Created by K S Sai Thilak, February 25, 2021.
*/

#ifndef I2CSB_h
#define I2CSB_h

#include "Arduino.h"
#include "Wire.h"
class I2CSB
{
	public:
		I2CSB(byte address);
		byte getLevel();
		bool setOutputs(bool LightPin, bool MistMotor, bool PlantMotor, bool BucketMotor);
         	int getHumidity();
         	int getTemperature();
     		bool isConnected();
	private:
		byte readI2Csensor(byte _address);
		byte _address;
};

#endif
