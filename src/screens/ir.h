#pragma once
#include <Arduino.h>
#include "screen.h"
#include "../ir.h"

class IrScreen: public Screen
{
private:
  uint32_t value = 0;

public:
  void onTick() {
    if (MyIR.get_last_value() > 0) {
      value = MyIR.get_last_value();
      setDirty(true);
    }
  }

  void draw(Adafruit_SSD1306 *d) {
    d->drawRect(0, 0, d->width(), d->height(), SSD1306_WHITE);
    d->setTextSize(2);
    d->setCursor(4, 4);
    d->print("IR Test");
    d->setCursor(4, 4 + 18);
    d->printf("Key: %u", value);
  }
};