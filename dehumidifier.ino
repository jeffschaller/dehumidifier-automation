// dehumidifier pump controller; turns on (closes) an aquarium pump relay when the "bucket full" light turns on
// Jeff Schaller, Nov 2023
// copied & modified from the source at: https://github.com/adafruit/Adafruit_APDS9960/blob/master/examples/color_sensor/color_sensor.ino

#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;

// the pin we're using to talk to the relay
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
  // start with the relay "open" AKA  not running the pump
  digitalWrite(PIN_TO_RELAY, LOW);
}

void loop() {
  // create some variables to store the color data in; mostly unused, except for 'red'
  uint16_t red, g, b, c;

  // for checking the relay pin state
  int relay_pin_state = 0;

  // for extra printing
  int debug = 1;
  
  // wait for color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }
  // get fresh data; we only care about the red, but we have to read them all
  apds.getColorData(&red, &g, &b, &c);

  if (debug) {
    Serial.print("current red value is:"); Serial.println(red);
  }

  if (debug > 2) {
    relay_pin_state = digitalRead(PIN_TO_RELAY);
    if (relay_pin_state == HIGH) {
      Serial.println("steadystate: Relay pin is HIGH / enabled");
    } else {
      Serial.println("steadystate: Relay pin is LOW / disabled");
    }
  }

  if(red > 50) {
    if (debug) {
      Serial.println("start the pump (for 5 sec)");
    }
    
    digitalWrite(PIN_TO_RELAY, HIGH);
    
    if (debug > 2) {
      relay_pin_state = digitalRead(PIN_TO_RELAY);
      if (relay_pin_state == HIGH) {
        Serial.println("pumping: Relay pin is HIGH / enabled");
      } else {
        Serial.println("pumping: Relay pin is LOW / disabled");
      }
    }
    // 5 sec in ms; TODO: eventually 4min 
    delay(5 * 1000);
    
    if (debug) {
      Serial.println("stop the pump");
    }

    digitalWrite(PIN_TO_RELAY, LOW);
  }
  // 1 sec in ms; TODO: increase when ready
  delay(1 * 1000);
}
