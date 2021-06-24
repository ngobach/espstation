#pragma once
#include <Arduino.h>
#include "screen.h"
#include "display_helper.h"
#include "../ir.h"

class IrScreen: public Screen
{
private:
  uint32_t value = 0;

public:
  void onTick() {
    if (MyIR.get_last_value() > 0) {
      value = MyIR.get_last_value();
      set_dirty(true);
    }
  }

  void draw(Adafruit_SSD1306 *d) {
    char buffer[128];
    DisplayHelper dh(d);

    d->drawRect(0, 0, d->width(), d->height(), SSD1306_WHITE);
    d->setTextSize(2);
    dh.draw_text_centered("IR Tester", 12);
    sprintf(buffer, "Key: %u", value);
    dh.draw_text_centered(buffer, 36);
  }
};