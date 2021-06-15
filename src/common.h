#include <Arduino.h>
void panic(const char *msg);

#ifndef COMMON_IMPLEMENTATION
#define COMMON_IMPLEMENTATION
void panic(const char *msg) {
  while (1) {
    if (Serial) {
      Serial.printf("Panicked: %s\n", msg);
    }
    delay(1000); 
  }
}
#endif