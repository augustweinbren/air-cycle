/* Test sketch for Adafruit PM2.5 sensor with UART or I2C */
#include "Adafruit_PM25AQI.h"
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define PIXELPIN 4
#define NUMPIXELS 8

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
Adafruit_NeoPixel pixels(NUMPIXELS, PIXELPIN);
float elapsedSecondsSinceLastWrite = 0;
int days = 0;
int hours = 0;
int tenMinuteInts = 0;
int minutes;
void setup() {
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
  while (!Serial) { delay(10); elapsedSecondsSinceLastWrite += .01; }
  while (!Serial2) { delay(10); elapsedSecondsSinceLastWrite += .01; }

  if (days != 255) { //
  Serial.println("Total uptime [Days/Hours/Minutes]: ");
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
  elapsedSecondsSinceLastWrite++;

  // If using serial, initialize it and set baudrate before starting!
  // Uncomment one of the following
//  Serial1.begin(9600);
//  pmSerial.begin(9600);

  // There are 3 options for connectivity!
  //if (! aqi.begin_I2C()) {      // connect to the sensor over I2C
  if (! aqi.begin_UART(&Serial2)) { // connect to the sensor over hardware serial
//  if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial 
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) { delay(10); elapsedSecondsSinceLastWrite += .01; }
  }

  Serial.println("PM25 found!");
}

int i = 0;
void loop() {
//  BLE.poll();
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
        elapsedSecondsSinceLastWrite += .0033; //eeprom write takes 3.3ms (https://www.arduino.cc/en/Reference/EEPROMWrite)
      }
      EEPROM.write(1, hours);
      elapsedSecondsSinceLastWrite += .0033;
    }
    EEPROM.write(2, tenMinuteInts);
    elapsedSecondsSinceLastWrite += .0033;
  }
  pixels.clear();

  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(1000);  // try again in a bit!
    elapsedSecondsSinceLastWrite++;
    return;
  }
  Serial.println("AQI reading success");
  uint32_t color;
  int bitCount;
  if (data.pm25_standard <= 50) {
    color = 0x00FF00; // green
    bitCount = map(data.pm25_standard, 0, 50, 1, 8);
  } else if (data.pm25_standard <= 100) {
    color = 0xFFFF00; //yellow
    bitCount = map(data.pm25_standard, 51, 100, 1, 8);
  } else if (data.pm25_standard <= 150) {
    color = 0xFF8000; // orange
    bitCount = map(data.pm25_standard, 101, 150, 1, 8);
  } else if (data.pm25_standard <= 200) {
    color = 0xFF0000; // red
    bitCount = map(data.pm25_standard, 151, 200, 1, 8);
  } else {
    color = 0x7F00FF; //Purple
    bitCount = map(data.pm25_standard, 201, 300, 1, 8);
  }
  pixels.fill(color, 0, bitCount);  
  pixels.show();
  Serial.print("NEOPIXEL pixel count: ");
  Serial.println(i);
  if (i == NUMPIXELS - 1) {
    i = 0;
  } else {
    i++;
  }
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
  

  delay(10000);
  elapsedSecondsSinceLastWrite += 10.0;
}
