/* Test sketch for Adafruit PM2.5 sensor with  
  BLE code adapted from BLE_server example
  (https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/)
*/
#include "Adafruit_PM25AQI.h"
#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <EEPROM.h>

#define PIXELPIN 4
#define NUMPIXELS 8
#define SERVICE_UUID "c867682e-d8aa-4bf3-ba43-aa2ed0a24abf"
#define PM10_UUID "d5212717-dd8e-4710-a332-ab35a282ba7c"
#define PM25_UUID "486c7b93-7fd5-43cc-8a17-b32a6cf4d8ab"
#define PM100_UUID "6f440878-5a1a-4a76-819a-3e64f73098dd"

BLEServer* pServer = NULL;
BLECharacteristic* pm10Characteristic = NULL;
BLECharacteristic* pm25Characteristic = NULL;
BLECharacteristic* pm100Characteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint16_t pm10Value = 0;
uint16_t pm25Value = 0;
uint16_t pm100Value = 0;

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
Adafruit_NeoPixel pixels(NUMPIXELS, PIXELPIN);
float elapsedSecondsSinceLastWrite = 0;
int days = 0;
int hours = 0;
int tenMinuteInts = 0;
int minutes;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  long startOfSetup = millis();

  pixels.begin();
  days = EEPROM.read(0);
  hours = EEPROM.read(1);
  tenMinuteInts = EEPROM.read(2);
  minutes = tenMinuteInts * 10;
  EEPROM.write(0, 0);
  EEPROM.write(1, 0);
  EEPROM.write(2, 0);
  // Wait for serial monitor to open
  Serial.begin(115200);
 
  
  Serial2.begin(9600);
  while (!Serial) { delay(10); }
  while (!Serial2) { delay(10); }

  if (days != 255) { // 255 means the eeprom memory is empty
    Serial.println("Total previous uptime [Days/Hours/Minutes]: ");
    Serial.println(days);
    Serial.println(hours);
    Serial.println(minutes);
  }
  days = 0;
  hours = 0;
  tenMinuteInts = 0;
  Serial.println("Adafruit PMSA003I Air Quality Sensor");

  // Wait one second for sensor to boot up!
  delay(1000);

  if (! aqi.begin_UART(&Serial2)) { // connect to the sensor over hardware serial
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) { delay(10); }
  }

  Serial.println("PM25 found!");
  Serial.println("Setting up BLE...");
  BLEDevice::init("ESP32 PM Sensor");
  
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  pm10Characteristic = pService->createCharacteristic(
    PM10_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY);
    //notify used because speed more important than reliability (UDB vs TCP)

  pm10Characteristic -> addDescriptor(new BLE2902()); //  allow server-initiated updates (https://www.oreilly.com/library/view/getting-started-with/9781491900550/ch04.html)
  
  pm25Characteristic = pService->createCharacteristic(
    PM25_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY);
  pm25Characteristic -> addDescriptor(new BLE2902()); //allow server-initiated updates (https://www.oreilly.com/library/view/getting-started-with/9781491900550/ch04.html)
  
  pm100Characteristic = pService->createCharacteristic(
    PM100_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY);
  pm100Characteristic -> addDescriptor(new BLE2902()); //allow server-initiated updates (https://www.oreilly.com/library/view/getting-started-with/9781491900550/ch04.html)
  
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMaxPreferred(0x12); // See "advertising interval" for BLE
  BLEDevice::startAdvertising();
  Serial.println("Waiting for a client connection to notify...");
  
  long endOfSetup = millis();
  int setupMillis = endOfSetup - startOfSetup;
  elapsedSecondsSinceLastWrite = setupMillis / 1000.0;
}

void loop() {
  long startOfLoop = millis();

  if (elapsedSecondsSinceLastWrite >= 600.0) {
    tenMinuteInts++;
    elapsedSecondsSinceLastWrite -= 600.0;
    if (tenMinuteInts == 6) {
      hours++;
      tenMinuteInts = 0;
      if (hours == 24) {
        days++;
        hours = 0;
        EEPROM.write(0, days);
      }
      EEPROM.write(1, hours);
    }
    EEPROM.write(2, tenMinuteInts);
  }
  
  pixels.clear();

  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(1000);  // try again in a bit!
    long endOfLoop = millis();
    int loopMillis = endOfLoop - startOfLoop;
    elapsedSecondsSinceLastWrite += (loopMillis / 1000.0);
    return;
  }
  Serial.println("AQI reading success");
  uint32_t color;
  int bitCount;
  if (data.pm25_standard <= 50) {
    color = 0x00FF00; // green
    bitCount = map(data.pm25_standard, 0, 50, 1, NUMPIXELS + 1);
  } else if (data.pm25_standard <= 100) {
    color = 0xFFFF00; //yellow
    bitCount = map(data.pm25_standard, 51, 100, 1, NUMPIXELS + 1);
  } else if (data.pm25_standard <= 150) {
    color = 0xFF8000; // orange
    bitCount = map(data.pm25_standard, 101, 150, 1, NUMPIXELS + 1);
  } else if (data.pm25_standard <= 200) {
    color = 0xFF0000; // red
    bitCount = map(data.pm25_standard, 151, 200, 1, NUMPIXELS + 1);
  } else {
    color = 0x7F00FF; //Purple
    bitCount = map(data.pm25_standard, 201, 300, 1, NUMPIXELS + 1);
  }

  // bitCount shown has a slight possibility of being larger than the number of pixels,
  // so needs to be capped
  Serial.print("NEOPIXEL pixel count: ");
  Serial.println(bitCount);
  if (bitCount >= NUMPIXELS) {
    pixels.fill(color);
  } else {
    pixels.fill(color, 0, bitCount);
  }
  pixels.show();


  Serial.println();
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));
  Serial.println(F("---------------------------------------"));
  Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
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

  if (deviceConnected) {
    Serial.println("Client connected");
    pm10Value = data.pm10_standard;
    pm10Characteristic -> setValue((uint8_t*)&pm10Value, 2);
    pm10Characteristic -> notify();
    pm25Value = data.pm25_standard;
    pm25Characteristic -> setValue((uint8_t*)&pm25Value, 2);
    pm25Characteristic -> notify();
    pm100Value = data.pm100_standard;
    pm100Characteristic -> setValue((uint8_t*)&pm100Value, 2);
    pm100Characteristic -> notify();
    delay(10000);
  }

  if (!deviceConnected && oldDeviceConnected) {
    Serial.println("Client disconnected");
    delay(500);
    pServer->startAdvertising();
    Serial.println("Advertising again");
    oldDeviceConnected = false;
  }

  if (deviceConnected && !oldDeviceConnected) {
    Serial.println("Client connected");
    oldDeviceConnected = true;
  }


  long endOfLoop = millis();
  int loopMillis = endOfLoop - startOfLoop;
  elapsedSecondsSinceLastWrite += (loopMillis / 1000.0);
}
