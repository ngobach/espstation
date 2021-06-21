#ifndef WIFINIT_H
#define WIFINIT_H
#include <WiFi.h>

void wifi_init() {
  WiFi.begin("@", "25052008");
  bool is_connected = WiFi.isConnected();
  Serial.println(is_connected ? "Connected" : "Not connected");
}
#endif