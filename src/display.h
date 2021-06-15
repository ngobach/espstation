#ifndef DISPLAY_H
#define DISPLAY_H
#include <Ticker.h>
#include <Adafruit_SSD1306.h>
#include "common.h"
#include "screen.h"

class Display
{
private:
  Adafruit_SSD1306 d;
  Ticker tkr;
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
const uint32_t TICKER_INTERVAL = 3000;

Display::Display():
  d(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, RESET_PIN),
  screen(nullptr) {}

void Display::begin() {
  if (!this->d.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS)) {
    panic("Unable to initialize the display");
  }

  this->d.clearDisplay();
  delay(50);
  this->d.display();

  this->tkr.attach_ms<Display*>(TICKER_INTERVAL, [](Display *self) {
    self->tick();
  }, this);
}

void Display::tick() {
  if (screen != nullptr && screen->isDirty()) {
    Serial.println("Redrawn");
    screen->draw(&d);
    screen->setDirty(false);
    delay(100);
    d.display();
  }
}

void Display::switch_screen(Screen *newScreen) {
  if (screen != nullptr) {
    screen->onUnmount();
    delete screen;
  }

  screen = newScreen;
  screen->onMount();
  this->tick();
}

Display MyDisplay;
#endif
