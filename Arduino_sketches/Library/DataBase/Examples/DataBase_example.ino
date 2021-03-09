#include <DataBase.h>
#include <I2C_eeprom.h>

I2C_eeprom ee(0x50, I2C_DEVICESIZE_24LC512);

DataBase DB;

void setup() {
  ee.begin();
  Serial.begin(9600);
  delay(100);
  //DB.resetAddress(ee);
  //DB.storeAt(ee,DB.getNextAddress(ee), 28, 23, 30, 25, 80, 0x11);
  for(int i=0; i<10; i++){
  Serial.print(ee.readByte(i));
  Serial.print("\t");
  }
  Serial.println();
  Serial.print("Day = ");
  Serial.print(DB.getDataAt(ee, 2, DD));
  Serial.print("\t");
  Serial.print("Hour = ");
  Serial.print(DB.getDataAt(ee, 2, HoD));
  Serial.print("\t");
  Serial.print("Minute = ");
  Serial.print(DB.getDataAt(ee, 2, MD));
  Serial.print("\t");
  Serial.print("Temp = ");
  Serial.print(DB.getDataAt(ee, 2, TD));
  Serial.print("\t");
  Serial.print("Hum = ");
  Serial.print(DB.getDataAt(ee, 2, HuD));
  Serial.print("\t");
  Serial.print("Lev = ");
  Serial.print(DB.getDataAt(ee, 2, LD));
  Serial.print("\t");
}

void loop() {
  
}
