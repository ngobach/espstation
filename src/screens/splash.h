#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "screen.h"

class SplashScreen: virtual public Screen {
  public:
    void draw(Adafruit_SSD1306*);
};

#include <Arduino.h>
#include "bitmaps.h"

void SplashScreen::draw(Adafruit_SSD1306 *display) {
  display->clearDisplay();
  display->drawBitmap(0, 0, bm_logo, 128, 64, SSD1306_WHITE);
}
#endif