/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Direct copy of the example provided at the following link:
  https://circuitdigest.com/microcontroller-projects/getting-started-with-esp32-with-arduino-ide
*/
int LED_BUILTIN = 2;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}
///AW Notes: Using ESP32 Dev Module; Need to press and hold for ~1 second on BOOT when console says "Connecting.."
// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
