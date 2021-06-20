#ifndef CLOCK_SCREEN_H
#define CLOCK_SCREEN_H

#include <Arduino.h>
#include <TimeLib.h>
#include "screen.h"

class ClockScreen : virtual public Screen
{
private:
  void get_time_str(char *buffer)
  {
    time_t n = now();
    sprintf(buffer, "NOW: %02d:%02d:%02d", hour(n), minute(n), second(n));
  }

public:
  void onTick()
  {
    setDirty(true);
  }

  void draw(Adafruit_SSD1306 *display)
  {
    char str[100];
    get_time_str(str);
    display->println(str);
  }
};
#endif