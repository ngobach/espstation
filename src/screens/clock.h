#ifndef CLOCK_SCREEN_H
#define CLOCK_SCREEN_H

#include <Arduino.h>
#include <TimeLib.h>
#include "screen.h"
#include "Fonts/FreeMono9pt7b.h"

class ClockScreen : virtual public Screen
{
private:
  void get_time_str(char *buffer)
  {
    time_t n = now();
    sprintf(buffer, "%02d:%02d:%02d", hour(n), minute(n), second(n));
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
    display->setFont(&FreeMono9pt7b);
    display->setCursor(0, 10);
    display->println(str);
  }
};
#endif