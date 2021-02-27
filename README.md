Automatic_Hydroponics_V2
==========================
## Using `I2CDB` library.

This arduino compatible library is made to simplify the communication definition that happens between master *esp32* and slave *arduino nano*.

The slave arduino nano does the following:
- controls 4 relays named as

1. Bucket Motor connected to pin 10
2. Plant Motor  connected to pin 9
3. Mist Motor   connected to pin 7
4. Light Pin    connected to pin 6

- Is connected to a DHT11 Temperature and Humidity sensor on pin 4.
- Has two sets of water level sensors with each water level sensor can detect 3 distinct points in the used bucket.

The Master ESP32 can access these function using the `I2CDB` Library. As the name suggests the communication happens in I2C protocol. 

## How to add the library?

To use the library just navigate to [I2CDB](Arduino_sketches/Library/I2CDB/) and download it as zip file. Later in arduino IDE add this library using library manager and get started with the library using examples.

## How to use the functions in the library?

1. First instantiate the class I2CDB with a slave address
```
#define Slave_address 0x2F
I2CDB DB1(Slave_address);
```
2. Initialize the I2C communication by calling `Wire.begin()`

3. The library has following functions

| Function name       | What is returned | return type | Arguments |
|---------------------|------------------|-------------|-----------|
| 1. isConnected()    |true if slave is connected | bool| no|
| 2. getLevel()       |Level encoded in single byte| byte| no|
| 3. getTemperature() |Temperature       | int  | no|
| 4. getHumidity()    |Humidity          | int  | no|
| 5. setOutputs()     |`True` if the outputs are set correctly `False` if fails to set| bool | 4 bool variables|

The library also can monitor communication error or Miscommunication took place. This is how:

1. For `isConnected()` function the master sends a request to get water level. If there is no error in the received information then the function returns `True`. In the event of death of slave or the slave is not connected or there is a communication error the function returns `False`.
2. For `getLevel()` function the return type is byte and the encoded level information in the byte is as follows `x LA2 LA1 LA0 0 LB2 LB2 LB0` Here LA2,1,0 are points in sensor A and LB2,1,0 are points in Level B sensor. The `x` bit carries the error information. If there is no error in the communication then `x = 1 ` else `x = 0`.
3. For `getTemperature()` function thenreturn type is integer. And the DHT11 sensor can measure the temperatures between 0 to 60degC with a resolution of 1degC. So the information we get out of the sensor can never hit negative temperature. So the function returns -10 if there is any error and returns value between 0 and 60 if there is no error.
4. Similarly for `getHumidity()` function, the return type is int and has values between 20 to 80. so if there is any error then the returned value will be -10.
5. For setOutputs() the return value is `True` if there is no error in communication as well as the outputs are set as expected. 
 The arguments in `setOutputs()` function is in this order `setOutputs(BucketMotor, PlantMotor, MistMotor, LightPin)`
