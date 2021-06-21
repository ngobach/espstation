#pragma once
#include <Arduino.h>
#include "screen.h"
#include "sensor.h"

class WeatherScreen : public Screen
{
private:
  SensorRead value;
  uint32_t last_read = 0;
public:
  void onTick()
  {
    if (millis() * 1ULL - last_read >= 1000ULL) {
      value = MySensor.get();
      setDirty(true);
      last_read = millis();
    }
  }

  void draw(Adafruit_SSD1306 *d) {
    char buffer[100];

    d->drawRect(0, 0, d->width(), d->height(), SSD1306_WHITE);
    d->setTextSize(2);
    sprintf(buffer, "Temp. : %02d", (int)roundf(value.temperature));
    d->setCursor(4, 12);
    d->print(buffer);
    sprintf(buffer, "Humid.: %02d", (int)roundf(value.relative_humidity));
    d->setCursor(4, 64 - 12 - 14 - 1);
    d->print(buffer);
  }
};
