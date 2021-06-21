#ifndef DISPLAY_H
#define DISPLAY_H
#include <Ticker.h>
#include <Adafruit_SSD1306.h>
#include "common.h"
#include "screen.h"

class Display
{
private:
  const uint32_t TICKER_INTERVAL = 200;
  Adafruit_SSD1306 d;
  Screen *screen;
public:
  Display();
  void begin();
  void tick();
  void switch_screen(Screen*);
};

const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;
const uint8_t SCREEN_I2C_ADDRESS = 0x3C;
const uint8_t RESET_PIN = -1;

Display::Display():
  d(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET_PIN),
  screen(nullptr) {}

void Display::begin() {
  if (!this->d.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS)) {
    panic("Unable to initialize the display");
  }

  this->d.clearDisplay();
  this->d.display();
}

void Display::tick() {
  if (screen != nullptr) {
    screen->onTick();
  }
  if (screen != nullptr && screen->isDirty()) {
    d.clearDisplay();
    d.setFont();
    d.setCursor(0, 0);
    d.setTextColor(SSD1306_WHITE);
    screen->draw(&d);
    screen->setDirty(false);
    d.display();
  }
}

void Display::switch_screen(Screen *new_screen) {
  if (screen != nullptr) {
    screen->onUnmount();
    delete screen;
  }

  screen = new_screen;
  screen->onMount();
}

Display MyDisplay;
#endif
