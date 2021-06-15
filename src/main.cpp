#include <Arduino.h>
#include <Wire.h>
#include "common.h"
#include "sensor.h"
#include "display.h"
#include "screens/splash.h"

#define BUZZER_PAD GPIO_NUM_23

void setup() {
  Serial.begin(9600);
  MySensor.begin();
  MyDisplay.begin();
  Screen *splash = new SplashScreen();
  MyDisplay.switch_screen(splash);
}

void read_sensor() {
  auto value = MySensor.get();
  Serial.printf("Temp: %.0f, Humidity: %.0f\n", value.temperature, value.relative_humidity);
}

void loop() {
  read_sensor();
  delay(1000);
}