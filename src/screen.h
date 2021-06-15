#include <Adafruit_SSD1306.h>

#ifndef SCREEN_H
#define SCREEN_H

class Screen {
private:
  bool dirty;

public:
  Screen();
  virtual ~Screen() {}
  bool isDirty();
  void setDirty(bool);
  virtual void onMount();
  virtual void onUnmount();
  virtual void draw(Adafruit_SSD1306*) = 0;
};

Screen::Screen() {
  this->dirty = false;
}

void Screen::setDirty(bool d) {
  this->dirty = d;
}

bool Screen::isDirty() {
  return this->dirty;
}

void Screen::onMount() {
  this->setDirty(true);
}

void Screen::onUnmount() {}
#endif
