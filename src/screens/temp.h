#pragma once
#include <Arduino.h>
#include "screen.h"
#include "sensor.h"
#include "font.h"

class TempScreen : public Screen
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
    sprintf(buffer, "Temp: %02d\nHumid.: %02d", (int)value.temperature, (int)value.relative_humidity);
    d->setFont(&Fonts::FreeMono9pt7b);
    d->setCursor(0, 14);
    d->println(buffer);
  }
};
