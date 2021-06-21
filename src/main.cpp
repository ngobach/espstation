#include <Arduino.h>
#include <Wire.h>
#include <Ticker.h>
#include "common.h"
#include "sensor.h"
#include "display.h"
#include "screen_loader.h"
#include "buttons.h"
#include "wifinit.h"

const uint8_t BUTTON_PAD = GPIO_NUM_0;
// const uint8_t BUZZER_PAD = GPIO_NUM_23;
const uint8_t LED_PAD = BUILTIN_LED;
const uint16_t FRAME_TIME = 100;
Button button(BUTTON_PAD);
ScreenNames current_screen;

ScreenNames next_screen_of(ScreenNames current) {
  switch (current) {
    case ScreenNames::splash:
      return ScreenNames::clock;
    default:
      return ScreenNames::splash;
  }
}

void setup() {
  Serial.begin(9600);
  MySensor.begin();
  MyDisplay.begin();
  current_screen = ScreenNames::splash;
  MyDisplay.switch_screen(load_screen(current_screen));
  // pinMode(BUZZER_PAD, OUTPUT);
  pinMode(LED_PAD, OUTPUT);
  // wifi_init();
}

void loop() {
  auto start = millis();
  button.update();
  if (button.is_up()) {
    current_screen = next_screen_of(current_screen);
    MyDisplay.switch_screen(load_screen(current_screen));
  }
  digitalWrite(LED_PAD, button.get_current() ? LOW : HIGH);
  MyDisplay.tick();
  auto now = millis();
  delay(max(0UL, FRAME_TIME - (now - start)));
}

