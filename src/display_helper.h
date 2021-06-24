#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class DisplayHelper
{
private:
  Adafruit_SSD1306 *display;
public:
  DisplayHelper(Adafruit_SSD1306 *d): display(d) {}

  uint16_t measure_width(const char* text) {
    int16_t t;
    uint16_t w, h;
    display->getTextBounds(text, (int16_t)0, (int16_t)0, &t, &t, &w, &h);

    return w;
  }

  void draw_text_centered(const char *text, int16_t y) {
    uint16_t w = measure_width(text);
    display->setCursor((display->width() - w) / 2, y);
    display->print(text);
  }
};
