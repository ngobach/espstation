#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "conf.hpp"
#include "ui.hpp"
#include "screens.hpp"
#include "ntptime.hpp"
#include "ts.h"

void setup() {
  Serial.begin(9600);
  WiFi.begin(AppConfig.WifiName, AppConfig.WifiPassword);
  NtpTime.begin();
  AppUI.begin();
}

void loop() {
  TS.update();
}
