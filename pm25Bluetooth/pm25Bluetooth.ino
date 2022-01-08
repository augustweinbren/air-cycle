/* 
 * This sketch collects PM measurements and sends them via the Arduino WiFi Rev2
 * using Bluetooth Low Energy. BLE Code is adapted from ButtonLED example.
 * 
 * \note Please generate randomised values for the service and characteristic keys */
#include "Adafruit_PM25AQI.h"
#include <ArduinoBLE.h>

// If your PM2.5 is UART only, for UNO and others (without hardware serial) 
// we must use software serial...
// pin #2 is IN from sensor ("RX" on breakout), leave pin #3 disconnected

// Using software serial to transmit PM measurements for convenience
#include <SoftwareSerial.h>
SoftwareSerial pmSerial(2, 3); //(resp.) RX, TX

// Initialising service and characteristics. As mentioned above, please randomise the keys
BLEService pmService("c867682e-d8aa-4bf3-ba43-aa2ed0a24abf");
BLEByteCharacteristic pm10Characteristic(
  "d5212717-dd8e-4710-a332-ab35a282ba7c", BLERead | BLENotify);
BLEByteCharacteristic pm25Characteristic(
  "486c7b93-7fd5-43cc-8a17-b32a6cf4d8ab", BLERead | BLENotify);
BLEByteCharacteristic pm100Characteristic(
  "6f440878-5a1a-4a76-819a-3e64f73098dd", BLERead | BLENotify);

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

// Used to time the delay between measurements
long prevMillis = 0;

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

  pmSerial.begin(9600);

  if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }

  Serial.println("PM25 found!");
}
void loop() {
  // Check if a device has been connected
  BLE.poll();
  // Query the central device (in this case the smartphone)
  BLEDevice central = BLE.central();
  

  // If there is a central device connected
  if (central) {
    Serial.println("Central Address");
    Serial.println(central.address());
    // Query the central device again and return true if still connected
    while (central.connected()) {
      long currentMillis = millis();
      // only update data at max every 3 seconds
      if (currentMillis - prevMillis >= 3000) {
        prevMillis = currentMillis;
        updatePM();
      }
    }
  }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());

}

void updatePM() {
    PM25_AQI_Data data;
    // if unable to read data from aqi
    if (! aqi.read(&data)) {
      // query connection status to prevent timing out
      BLE.central().connected();
      Serial.println("Could not read from AQI");
      return; // try again next cycle!
    }
    // if the smartphone is no longer connected over ble
    if(!((BLE.central()).connected())) {
      return;
    }
    Serial.println("AQI reading success");
  
    Serial.println();
    Serial.println(F("---------------------------------------"));
    Serial.println(F("Concentration Units (standard)"));
    Serial.println(F("---------------------------------------"));
    Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
    // query to avoid timing out
    BLE.central().connected();
    pm10Characteristic.writeValue(data.pm10_standard);
    Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
    // query to avoid timing out
    BLE.central().connected();
    pm25Characteristic.writeValue(data.pm25_standard);
    Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
    // query to avoid timing out
    BLE.central().connected();
    pm100Characteristic.writeValue(data.pm100_standard);
    // query to avoid timing out
    BLE.central().connected();
    Serial.println("BLUETOOTH WRITTEN");
}
