#include <Arduino.h>
#include <Wire.h>

#include "display.h"

#define BUZZER_PAD GPIO_NUM_23

DHT_Unified dht(DHT_PAD, DHT_TYPE);
Display display;
Blink blink(BUILTIN_LED, 500);

void dead_loop(const char *msg) {
  while (1) {
    Serial.printf("Deadloop: %s\n", msg);
    delay(1000); 
  }
}

bool read_sensors(float *temp, float *humidity) {
  sensors_event_t event;
  bool success = false;
  success = dht.temperature().getEvent(&event);
  if (!success) {
    return false;
  }
  *temp = event.temperature;
  success = dht.humidity().getEvent(&event);
  if (!success) {
    return false;
  }
  *humidity = event.relative_humidity;
  return true;
}


void setup() {
  Serial.begin(9600);
  dht.begin();
  if (false
    || !display.begin()
    || !blink.begin()
  ) {
    dead_loop("Unable to initialize the device");
  }
  delay(500);
}

void loop() {
  display.tick();
  blink.tick();
  delay(16);
}