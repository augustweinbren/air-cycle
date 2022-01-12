**Table of Contents**
- [Description <a name="description"></a>](#description-)
- [Items Needed <a name="itemsNeeded"></a>](#items-needed-)
- [Installations and Software Setups <a name="installation"></a>](#installations-and-software-setups-)
## Description <a name="description"></a>
This directory contains everything needed to develop a BLE-enabled particulate matter monitor and 3D print a case and mount for a bicycle. 

## Items Needed <a name="itemsNeeded"></a>

*Electronic parts*
- [Arduino WiFi Rev2](http://store.arduino.cc/products/arduino-uno-wifi-rev2) (Optional)
- [ESP32](https://www.amazon.co.uk/gp/product/B071JR9WS9/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
- [NeoPixel Stick](https://www.adafruit.com/product/1426)
- [Plantower PMS5003](https://shop.pimoroni.com/products/pms5003-particulate-matter-sensor-with-cable?variant=29075640352851&currency=GBP&utm_source=google&utm_medium=cpc&utm_campaign=google+shopping?utm_source=google&utm_medium=surfaces&utm_campaign=shopping&gclid=Cj0KCQiA8vSOBhCkARIsAGdp6RSGTk4PbN6OXMfmVgTRnW0nTTmLnevmUaXB78eK67NjLtzWNMzlp0saApA0EALw_wcB)
- [Particulate Matter Sensor Breakout (for PMS5003)](https://shop.pimoroni.com/products/particulate-matter-sensor-breakout)
- Female-female jumper cables
- Header pins
- [Stripboard 64 x 95mm, 24 Tracks, 37 Holes](https://www.betterequipped.co.uk/stripboard-64-x-95mm-24-tracks-37-holes-4224)
- Power bank
- At least two breadboards (needed to fit ESP32 atop of)

*Hand Tools*
- [Stripboard Track Cutter](https://thepihut.com/products/stripboard-track-cutter?variant=37628041494723&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gclid=Cj0KCQiA8vSOBhCkARIsAGdp6RTHVP186qTaA273yeRDDO4YhkkDe5MsoM5w-hYyMir60p5wJHqxo-AaAh_cEALw_wcB)
- Soldering iron

*Enclosure*
- 3D Printer + PLA Filament
- 3 cable ties
- Rubber bands for additional support

## Installations and Software Setups <a name="installation"></a>
1.	Download and install [Arduino IDE](https://www.arduino.cc/en/software)
2.	[Set up a GitHub repo](https://docs.github.com/en/get-started/quickstart/create-a-repo) for the project
3. [Install the ESP32 module for Arduino IDE](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
4. Install the following Arduino libraries:
   * [Arduino BLE](https://www.arduino.cc/en/Reference/ArduinoBLE)
   * [Software Serial](https://www.arduino.cc/en/Reference/SoftwareSerial)
   * [PM 2.5 AQI](https://github.com/adafruit/Adafruit_PM25AQI)
   * [Adafruit NeoPixel](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation)
   * [EEPROM](https://www.arduino.cc/en/Reference/EEPROM)
5. Set up your 3D modelling and printing software--this will depend on the particular modelling and printing software you are using
6. [Download LightBlue](https://punchthrough.com/lightblue/)
7. [Set up Adafruit IO](https://learn.adafruit.com/welcome-to-adafruit-io/getting-started-with-adafruit-io)