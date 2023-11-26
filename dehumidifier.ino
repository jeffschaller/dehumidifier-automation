// dehumidifier pump controller; turns on (closes) an aquarium pump relay when the "bucket full" light turns on
// Jeff Schaller, Nov 2023
// copied & modified from the source at: https://github.com/adafruit/Adafruit_APDS9960/blob/master/examples/color_sensor/color_sensor.ino

#include "Adafruit_APDS9960.h"
// create a global color sensor object "apds"
Adafruit_APDS9960 apds;

// the pin on the feather that we're using to talk to the relay
// this hole is labeled "6" on the front but is really GPIO8
#define PIN_TO_RELAY 8

// experimentally-determined threshold
// tested with room lights on & off, and with
// the sensor positioned behind the dehumidifier light, 
// facing downward about 45 degrees, with the edges of the
// connectors touching the dehumidifier
#define BUCKET_FULL_RED 50

void setup() {
  // enable Serial communication
  Serial.begin(115200);

  // try to start the color sensor
  if(!apds.begin()) {
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else {
    // Assume success
    // Serial.println("Device initialized!");

    // we want color readings
    apds.enableColor(true);
  }

  // define the relay pin as an OUTPUT pin
  pinMode(PIN_TO_RELAY, OUTPUT);
  // start with the relay "open" AKA not running the pump
  digitalWrite(PIN_TO_RELAY, LOW);

  // use the built-in LED as an external signal for "pumping"
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // create some variables to store the color data in; unused, except for 'red'
  uint16_t red, green, blue, clear;

  // for checking the relay pin state
  int relay_pin_state = 0;

  // for extra printing
  int debug = 1;
  
  // wait for color data to be ready
  while (!apds.colorDataReady()) {
    delay(5);
  }
  // get fresh data; we only care about the red, but we have to read them all
  apds.getColorData(&red, &green, &blue, &clear);

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

  if (red > BUCKET_FULL_RED) {
    if (debug) {
      Serial.println("start the pump");
    }
    
    // light up the built-in LED
    digitalWrite(LED_BUILTIN, HIGH);
    // "close" the relay to turn on the pump
    digitalWrite(PIN_TO_RELAY, HIGH);

    if (debug > 2) {
      relay_pin_state = digitalRead(PIN_TO_RELAY);
      if (relay_pin_state == HIGH) {
        Serial.println("pumping: Relay pin #PIN_TO_RELAY is HIGH / enabled");
      } else {
        Serial.println("pumping: Relay pin #PIN_TO_RELAY is LOW / disabled");
      }
    }

    // wait for a while for the pump to do its work, in ms
    delay(4 * 60 * 1000);
    
    if (debug) {
      Serial.println("stop the pump");
    }

    // turn off the built-in LED
    digitalWrite(LED_BUILTIN, LOW);
    // "open" the relay to turn off the pump
    digitalWrite(PIN_TO_RELAY, LOW);
  }
  // no need to spazz the sensor reading; wait 1 sec in ms
  delay(1 * 1000);
}
