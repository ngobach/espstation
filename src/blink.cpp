#include <Arduino.h>
#include "blink.h"

Blink::Blink(int pin, int interval) {
  this->pin = pin;
  this->interval = interval;
  this->isOn = false;
}

Blink::~Blink() {}

bool Blink::begin() {
  pinMode(this->pin, OUTPUT);
  this->lastBlink = millis();
  return true;
}

bool Blink::tick() {
  long passed = millis() - this->lastBlink;
  if (passed > this->interval) {
    this->lastBlink += this->interval;
    if (this->isOn) {
      digitalWrite(this->pin, HIGH);
    } else {
      digitalWrite(this->pin, LOW);
    }
    this->isOn = !this->isOn;
  }
  return true;
}