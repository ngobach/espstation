#ifndef CLOCK_SCREEN_H
#define CLOCK_SCREEN_H

#include <Arduino.h>
#include <TimeLib.h>
#include "screen.h"
#include "bitmaps.h"

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
    char str[100];
    // draw border
    int w = display->width();
    int h = display->height();
    display->drawRect(0, 0, w, h, SSD1306_WHITE);
    display->drawBitmap(4, (64 - 16) / 2 , bm_clock_16, 16, 16, SSD1306_WHITE);
    get_time_str(str);
    display->setCursor(4 + 16 + 8, (64 - 14) / 2);
    display->setTextSize(2);
    display->print(str);
  }
};
#endif