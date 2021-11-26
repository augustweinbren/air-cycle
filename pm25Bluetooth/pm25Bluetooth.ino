/* Test sketch for Adafruit PM2.5 sensor with UART or I2C */
/*BLE code adapted from ButtonLED example*/
#include "Adafruit_PM25AQI.h"
#include <ArduinoBLE.h>

// If your PM2.5 is UART only, for UNO and others (without hardware serial) 
// we must use software serial...
// pin #2 is IN from sensor ("RX" on breakout), leave pin #3 disconnected
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial pmSerial(2, 3); //(resp.) RX, TX

BLEService pmService("c867682e-d8aa-4bf3-ba43-aa2ed0a24abf");
BLEByteCharacteristic pm10Characteristic(
  "d5212717-dd8e-4710-a332-ab35a282ba7c", BLERead | BLENotify);
BLEByteCharacteristic pm25Characteristic(
  "486c7b93-7fd5-43cc-8a17-b32a6cf4d8ab", BLERead | BLENotify);
BLEByteCharacteristic pm100Characteristic(
  "6f440878-5a1a-4a76-819a-3e64f73098dd", BLERead | BLENotify);

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

void setup() {
  // Wait for serial monitor to open
  Serial.begin(115200);
  while (!Serial) delay(10);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while(1);
  }
  Serial.println("Starting BLE Succeeded");

  BLE.setLocalName("PMSensor");
  BLE.setAdvertisedService(pmService);

  pmService.addCharacteristic(pm10Characteristic);
  pmService.addCharacteristic(pm25Characteristic);
  pmService.addCharacteristic(pm100Characteristic);


  BLE.addService(pmService);

  pm10Characteristic.writeValue(0);
  pm25Characteristic.writeValue(0);
  pm100Characteristic.writeValue(0);
  BLE.advertise();
  Serial.println("Bluetooth active and waiting for connection");

  Serial.println("Adafruit PMSA003I Air Quality Sensor");

  // Wait one second for sensor to boot up!
  delay(1000);

  // If using serial, initialize it and set baudrate before starting!
  // Uncomment one of the following
//  Serial1.begin(9600);
  pmSerial.begin(9600);

  // There are 3 options for connectivity!
  //if (! aqi.begin_I2C()) {      // connect to the sensor over I2C
//  if (! aqi.begin_UART(&Serial1)) { // connect to the sensor over hardware serial
  if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }

  Serial.println("PM25 found!");
}

void loop() {
  BLE.poll();
  
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    return;
  }
  Serial.println("AQI reading success");

  Serial.println();
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  pm10Characteristic.writeValue(data.pm10_standard);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  pm25Characteristic.writeValue(data.pm25_standard);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
  pm100Characteristic.writeValue(data.pm100_standard);
  Serial.println("BLUETOOTH WRITTEN");
  Serial.println(F("Concentration Units (environmental)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  Serial.println(F("---------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  Serial.println(F("---------------------------------------"));
  

  delay(1000);
}
