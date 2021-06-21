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
    auto tmp = MyIR.read_freq();
    if (tmp > 0) {
      value = tmp;
      setDirty(true);
    }
  }

  void draw(Adafruit_SSD1306 *d) {
    d->printf("Value: %u", value);
  }
};