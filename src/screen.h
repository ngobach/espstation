#ifndef SCREEN_H
#define SCREEN_H
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "common.h"

class Screen {
private:
  bool dirty;
  bool mounted;
public:
  Screen();
  virtual ~Screen() {}
  bool isDirty();
  bool isMounted();
  void setDirty(bool);
  virtual void onMount();
  virtual void onUnmount();
  virtual void onTick() {}
  virtual void draw(Adafruit_SSD1306*) = 0;
};

Screen::Screen(): dirty(false), mounted(false) {}

void Screen::setDirty(bool d) {
  this->dirty = d;
}

bool Screen::isDirty() {
  return this->dirty;
}

void Screen::onMount() {
  dirty = true;
  mounted = true;
}

void Screen::onUnmount() {
  mounted = false;
}

enum ScreenNames {
  splash,
  weather,
  internet_weather
};

#include "screens/splash.h"


Screen* load_screen(ScreenNames screen_name) {
  switch (screen_name) {
    case ScreenNames::splash:
      return new SplashScreen;
    default:
      panic("Invalid screen name");
  }
}
#endif
