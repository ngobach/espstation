#include <Wire.h>
#include <Arduino.h>
#include "display.h"
#include "bitmaps.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_I2C_ADDRESS 0x3C
#define SCREN_NO_RESET -1

Display::Display() {
  this->d = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREN_NO_RESET);
  this->lastDisplay = 0;
  this->startAt = millis();
}

Display::~Display()
{
  delete this->d;
}

bool Display::begin() {
  if (!this->d->begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS)) {
    return false;
  }

  this->d->clearDisplay();
  this->d->drawBitmap(0, 0, bm_logo, 128, 64, SSD1306_WHITE);
  delay(200);
  this->d->display();
  delay(1800);
  return true;
}

bool Display::tick() {
  unsigned long now = millis();
  if (now - this->lastDisplay < 1000) {
    // Just skip
    return true;
  }

  auto d = this->d;
  d->clearDisplay();
  d->setTextColor(SSD1306_WHITE);
  d->setCursor(0, 0);
  d->printf("Hello ESP32 %lu\n", (now - startAt) / 1000);
  delay(10);
  d->display();
  this->lastDisplay = now;
  return true;
}