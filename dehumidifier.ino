/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in color mode and reads the RGB and clear values.

  Designed specifically to work with the Adafruit APDS9960 breakout
  ----> http://www.adafruit.com/products/3595

  These sensors use I2C to communicate. The device's I2C address is 0x39

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

/* modified from the source at: https://github.com/adafruit/Adafruit_APDS9960/blob/master/examples/color_sensor/color_sensor.ino */

#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;

#define PIN_TO_RELAY 6

void setup() {
  Serial.begin(115200);

  if(!apds.begin()) {
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else {
    // Assume success
    // Serial.println("Device initialized!");
  }

  apds.enableColor(true);
  pinMode(PIN_TO_RELAY, OUTPUT);
  // start with the relay "open"
  digitalWrite(PIN_TO_RELAY, LOW);
}

void loop() {
  // create some variables to store the color data in; mostly unused, except for 'red'
  uint16_t red, g, b, c;
  int relay_pin_state = 0;
  int debug = 0;
  
  // wait for color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }
  // get fresh data
  apds.getColorData(&red, &g, &b, &c);
  Serial.print("red value is:"); Serial.println(red);

  if (debug) {
    relay_pin_state = digitalRead(PIN_TO_RELAY);
    if (relay_pin_state == HIGH) {
      Serial.println("steadystate: Relay pin is HIGH");
    } else {
      Serial.println("steadystate: Relay pin is LOW");
    }
  }

  if(red > 50) {
    Serial.println("start the pump (for 5 sec)");
    digitalWrite(PIN_TO_RELAY, HIGH);
    if (debug) {
      relay_pin_state = digitalRead(PIN_TO_RELAY);
      if (relay_pin_state == HIGH) {
        Serial.println("pumping: Relay pin is HIGH");
      } else {
        Serial.println("pumping: Relay pin is LOW");
      }
    }
    delay(5 * 1000);  /* 5 sec in ms; TODO: eventually 4min */
    Serial.println("stop the pump");
    digitalWrite(PIN_TO_RELAY, LOW);
  }
  delay(1 * 1000);  /* 1 sec in ms; TODO: increase when ready */
}
