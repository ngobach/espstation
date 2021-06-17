#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <Arduino.h>
#include <Ticker.h>
#include "screen.h"
#include "bitmaps.h"

class SplashScreen: virtual public Screen {
  private:
    Ticker::callback_t cb;
    unsigned int counter;
  public:
    void draw(Adafruit_SSD1306 *display) {
      display->drawBitmap(0, 0, bm_logo, 128, 64, SSD1306_WHITE);
      display->setCursor(0, 0);
      display->setTextColor(SSD1306_WHITE);
      display->printf("C: %d", counter);
    }

    void onTick() {
      counter++;
      setDirty(true);
    }
};
#endif