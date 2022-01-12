**Table of Contents**
- [Description <a name="description"></a>](#description-)
- [Items Needed <a name="itemsNeeded"></a>](#items-needed-)
- [Installations and Software Setups <a name="installation"></a>](#installations-and-software-setups-)
- [Recommended order of Arduino deployments <a name="recommended order"></a>](#recommended-order-of-arduino-deployments-)
- [Enclosure <a name="enclosure"></a>](#enclosure-)
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

## Recommended order of Arduino deployments <a name="recommended order"></a>
The following order of software uploads via the Arduino IDE is recommended:
1. `pm25Bluetooth`
2. `Blink_ESP32`
3. `pm25_neopixel_esp32`
4. `pm25_neopixel_esp32_eeprom`
5. `pm25_neopixel_esp32_bt_eeprom`

## Enclosure <a name="enclosure"></a>
Note: it is not suggested to create an enclosure until a stripboard prototype is tried and tested. A step-by-step guide can be found in the [pm25_neopixel_esp32 directory](https://github.com/augustweinbren/air-cycle/tree/master/pm25_neopixel_esp32).

1. Plug the prototype into the power bank using a USB cable, and confirm that the NeoPixel LEDs are at least partially lit up.
2. Position the power bank atop the PMS5003.
3. Measure the width, height, and depth of the prototype + power bank, ideally with a digital caliper for added precision.
4. Add 3 mm to each dimension to ensure adequate space is available and note these dimensions for the inside of a 3d-printed box. Add another 2 mm for the dimensions of the outside of the box.
5. [Design a box in Fusion 360](https://www.youtube.com/watch?v=HDJ2g19SlCI)
6. [Make one side meshed to allow air to flow into the Plantower](https://www.youtube.com/watch?v=Kj6h6ARtITE&t=176s). This side will be faced towards the cyclist to reduce distortion induced by the PMS5003's fan.
7. 3D Print this box, and ensure that your powered device fits inside of it with the power bank closest to the opening.
8. If the device fits in the box, take measurements for your mount. The mount I printed in this project has three points of support: one at the middle of the stem, and two on the handlebar. A gap needs to be left for the stem's handlebar mount. Take the following measurements:
   * Length and width of the stem's handlebar mount
   * Height difference between the top of the handlebar mount and the tops of both stem and the handlebar at its lowest point (if your handlebar has curvature). Note down whichever height difference is greater.
   * Width of the stem at the approximate point of support
   * Width of the handlebar at the approximate points of support
9.  Return to your 3D Modelling software and produce the base with the following considerations:
    * To fit the boxed device securely, the base will need a .3 mm larger width and length than that of the outer dimensions of the box
    * Vault the edges of the supports slightly for added stability
    * Create a hole within each of the three supports to fit cable ties to create three supports in two axes of rotation
10. Once the base is fully printed, it can be mounted on the bicycle with the cable ties.
11. Carefully place the box upside-down on the mount, preventing the contents from falling out
12. Rubber bands were additionally used during test rides to secure the mount to the base.
