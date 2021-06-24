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
  bool is_dirty();
  bool isMounted();
  void set_dirty(bool);
  virtual void onMount();
  virtual void onUnmount();
  virtual void onTick() {}
  virtual void draw(Adafruit_SSD1306*) = 0;
};

Screen::Screen(): dirty(false), mounted(false) {}

void Screen::set_dirty(bool d) {
  this->dirty = d;
}

bool Screen::is_dirty() {
  return this->dirty;
}

void Screen::onMount() {
  dirty = true;
  mounted = true;
}

void Screen::onUnmount() {
  mounted = false;
}

#endif
