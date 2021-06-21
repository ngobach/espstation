#ifndef CLOCK_SCREEN_H
#define CLOCK_SCREEN_H

#include <Arduino.h>
#include <TimeLib.h>
#include "screen.h"
#include "bitmaps.h"
#include "font.h"

class ClockScreen : virtual public Screen
{
private:
  int8_t last_second = -1;
  void get_time_str(char *buffer)
  {
    time_t n = now();
    sprintf(buffer, "%02d:%02d:%02d", hour(n), minute(n), second(n));
  }

public:
  void onTick()
  {
    if (second() != last_second) {
      setDirty(true);
      last_second = second();
    }
  }

  void draw(Adafruit_SSD1306 *display)
  {
    // draw border
    int w = display->width();
    int h = display->height();
    display->drawRect(0, 0, w - 1, h - 1, SSD1306_WHITE);
    display->drawBitmap(4, 20, bm_clock_16, 16, 16, SSD1306_WHITE);
    char str[100];
    get_time_str(str);
    display->setFont(&Fonts::FreeMono9pt7b);
    display->setCursor(24, 33);
    display->println(str);
  }
};
#endif