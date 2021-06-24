#pragma once

#include "screen.h"
#include "bitmaps.h"

class SplashScreen: virtual public Screen {
  public:
    void draw(Adafruit_SSD1306 *d) {
      d->drawBitmap(0, 0, bm_logo, 128, 64, SSD1306_WHITE);
    }
};
