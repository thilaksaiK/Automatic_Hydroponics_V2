  #include<Wire.h>
#include "I2C_eeprom.h"
#include "RTClib.h"

RTC_DS1307 rtc;
#define Slave_address 0x2F

#define Serial_debug
//#define Set_Time

byte ReceivedData[3];

bool WaterLevel[2][3];

bool LightPin = 0, MistMotor = 0, PlantMotor = 0, BucketMotor = 1;

int Temp, Hum, Level;

int Day, Hour, Minute;

byte StoreArray[4];
I2C_eeprom ee(0x51, I2C_DEVICESIZE_24LC512);

void setup() {
  Wire.begin();
  //#ifdef Serial_debug
  Serial.begin(115200);
  //#endif
  #ifdef Set_Time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  #endif
  //resetAddress();
  setOutputs();
  for(int i = 0; i<4; i++)
  storeData();
  for(int i = 0; i < 20; i++){
    readData(5*i+2);
  }
  
//  getLevel();
//  delay(2000);
//  setOutputs();
//  delay(2000);
//  Serial.println(getTemp());
//  delay(2000);
//  Serial.println(getHum());
//  delay(2000);
  
  

}

byte readI2Csensor(byte type){
  long entry = millis();
  Wire.requestFrom(Slave_address, 2);
  while(Wire.available() == 0 &&(millis() - entry) < 100){
    #ifdef Serial_debug
      Serial.println("Waiting...");
    #endif
  }
  if(millis() - entry < 100)
  return Wire.read();
}
void loop() {
  
}

byte getLevel(){
  #ifdef Serial_debug
  Serial.print("**********Level Mode************");
  #endif
  
  Wire.beginTransmission(Slave_address); // transmit to device #4
  Wire.write(0xAA);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  delay(1000);
  for(int i = 0; i < 3; i++){
    ReceivedData[i] = readI2Csensor(Slave_address);
    #ifdef Serial_debug
    Serial.print(ReceivedData[i], HEX);
    Serial.print("\t");
    #endif
  }
  #ifdef Serial_debug
  Serial.println();
  #endif
  
  if(ReceivedData[0] != 0xAA | ReceivedData[1] != ReceivedData[2])
  getLevel();
  #ifdef Serial_debug
   Serial.print("Decoded level is ");
  #endif
  byte returnVal = ReceivedData[1];
  for(int i = 0; i < 3; i++){
    WaterLevel[1][i] = (ReceivedData[1]>>i) & 0x01;
    #ifdef Serial_debug
    Serial.print(WaterLevel[1][i]);
    Serial.print("\t");
    #endif
  }
  for(int i = 0; i < 3; i++){
    WaterLevel[2][i] = (ReceivedData[1]>>(i+4)) & 0x01;
    #ifdef Serial_debug
    Serial.print(WaterLevel[2][i]);
    Serial.print("\t");
    #endif
  }
  #ifdef Serial_debug
  Serial.println();
  Serial.print("Return value = ");
  Serial.println(returnVal);
  #endif
  return returnVal;
}

void setOutputs(){
  #ifdef Serial_debug
  Serial.println("***************Output Mode***************");
  #endif
  byte sendByte = BucketMotor | PlantMotor<<1 | MistMotor<<2 | LightPin<<3;
  sendByte |= 0x60;
  Wire.beginTransmission(Slave_address);
  Wire.write(sendByte); 
  Wire.endTransmission();
  delay(500);
  #ifdef Serial_debug
  Serial.print("Sent byte = ");
  Serial.println(sendByte, HEX);
  Serial.println("ReceivedData = ");
  #endif
  for(int i = 0; i < 3; i++){
    ReceivedData[i] = readI2Csensor(Slave_address);
    #ifdef Serial_debug
    Serial.print(ReceivedData[i], HEX);
    Serial.print("\t");
    #endif
  }
  #ifdef Serial_debug
  Serial.println();
  #endif
  if(ReceivedData[0] != sendByte) setOutputs();  
}
 
int getTemp(){
  #ifdef Serial_debug
  Serial.println("********************Temperature Mode***************");
  #endif
  delay(500);
  Wire.beginTransmission(Slave_address);
  Wire.write(0xCA); 
  Wire.endTransmission();
  delay(500);
  #ifdef Serial_debug
  Serial.println("ReceivedData = ");
  #endif
  for(int i = 0; i < 3; i++){
    ReceivedData[i] = readI2Csensor(Slave_address);
    #ifdef Serial_debug
    Serial.print(ReceivedData[i], HEX);
    Serial.print("\t");
    #endif
  }
  #ifdef Serial_debug
  Serial.println();
  #endif
  if(ReceivedData[0] != 0xCA | ReceivedData[1] != ReceivedData[2])
  getTemp();
  return map(ReceivedData[1], 100, 200, 0, 50);
}

int getHum(){
  #ifdef Serial_debug
  Serial.println("*******************Humidity Mode*******************");
  #endif
  Wire.beginTransmission(Slave_address);
  Wire.write(0x5C); 
  Wire.endTransmission();
  delay(500);
  #ifdef Serial_debug
  Serial.println("ReceivedData = ");
  #endif
  for(int i = 0; i < 3; i++){
    ReceivedData[i] = readI2Csensor(Slave_address);
    #ifdef Serial_debug
    Serial.print(ReceivedData[i], HEX);
    Serial.print("\t");
    #endif
  }
  #ifdef Serial_debug
  Serial.println();
  #endif
  if(ReceivedData[0] != 0x5C | ReceivedData[1] != ReceivedData[2])
  getHum();
  return map(ReceivedData[1], 100, 220, 20, 80);
}

void storeData(){
  int address = 0;
    address |= ee.readByte(0)<<8;
    address |= ee.readByte(1);
  
  Serial.print("Current address = ");
  Serial.print(address);
  UpdateTime();
  int state_store = Day*1440 + Hour*60+ Minute;
  StoreArray[0] = highByte(state_store);
  StoreArray[1] = lowByte(state_store);
  StoreArray[2] = map(getTemp(), 0, 50, 100, 200);
  delay(1000);
  StoreArray[3] = map(getHum(), 20, 80, 100, 220);
  delay(1000);
  StoreArray[4] = getLevel();
  for( int i = 0; i < 5; i++){
    ee.writeByte(address, StoreArray[i]);
    address++;
  }
  ee.writeByte(0, highByte(address));
  ee.writeByte(1, lowByte(address));
  
}

void resetAddress(){
  ee.writeByte(0, 0);
  ee.writeByte(1, 2);
}

void readData(int address){
  byte AccessData[5];
  Serial.print("Reading Data at ");
  Serial.println(address);
  for(int i = 0; i<5; i++){
    AccessData[i] = ee.readByte(address++);
  Serial.print(AccessData[i]);
  Serial.print("\t");
  }
  Serial.println();
  Serial.print("Decoded Output: ");
  Serial.print("Day : ");
  Serial.print((int)(AccessData[0]<<8 | AccessData[1]) /1440);
  Serial.print("\t");
  Serial.print("Hour : ");
  Serial.print(((int)(AccessData[0]<<8 | AccessData[1]) %1440)/24);
  Serial.print("\t");
  Serial.print("Minutes : ");
  Serial.println((((int)(AccessData[0]<<8 | AccessData[1]) %1440)%24)/60);

}
void UpdateTime(){
  DateTime now = rtc.now();
  Day = now.day();
  Hour = now.hour();
  Minute = now.minute();
  Serial.print("Minute : ");
  Serial.print(Minute);
  Serial.print("\t");
  Serial.print("Hour : ");
  Serial.print(Hour);
  Serial.print("\t");
  Serial.print("Day : ");
  Serial.print(Day);
  Serial.print("\t");
  Serial.print("Compute state: ");
  Serial.println(Day*1440 + Hour*60+ Minute, HEX);
}
