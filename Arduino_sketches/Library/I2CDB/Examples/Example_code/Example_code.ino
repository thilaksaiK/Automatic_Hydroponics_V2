#include <Wire.h>
#include <I2CDB.h>

#define Slave_address 0x2F

I2CDB node1(Slave_address);

void setup(){
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Started");
}

void loop() {
  Serial.println(node1.isConnected());
  delay(2000);
  Serial.println(node1.getLevel(), BIN);
  delay(2000);
  Serial.println(node1.getTemperature());
  delay(2000);
  Serial.println(node1.getHumidity());
  delay(2000);
  Serial.println(node1.setOutputs(1, 1, 1, 1));
  delay(2000);
}
