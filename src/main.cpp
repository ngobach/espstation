#include <Arduino.h>
#include <Wire.h>
#include <Ticker.h>
#include "common.h"
#include "sensor.h"
#include "display.h"
#include "screen_loader.h"
#include "buttons.h"
#include "wifinit.h"
#include "ir.h"

const uint8_t BUTTON_PAD = GPIO_NUM_0;
// const uint8_t BUZZER_PAD = GPIO_NUM_23;
const uint8_t LED_PAD = BUILTIN_LED;
const uint16_t FRAME_TIME = 100;
Button button(BUTTON_PAD);
ScreenNames current_screen, next_screen;

ScreenNames next_screen_of(ScreenNames current) {
  static ScreenNames screen_list[] = {
    ScreenNames::splash,
    ScreenNames::clock,
    ScreenNames::weather,
    ScreenNames::ir_tool,
  };
  static uint8_t screen_list_len = sizeof(screen_list) / sizeof(screen_list[0]);

  for (uint8_t i= 0; i < screen_list_len; i++) {
    if (current == screen_list[i]) {
      return screen_list[(i + 1) % screen_list_len];
    }
  }

  return screen_list[0];
}

void setup() {
  Serial.begin(9600);
  MySensor.begin();
  MyDisplay.begin();
  current_screen = ScreenNames::splash;
  MyDisplay.switch_screen(load_screen(current_screen));
  // pinMode(BUZZER_PAD, OUTPUT);
  pinMode(LED_PAD, OUTPUT);
  // TODO: Brownout detected :(
  // wifi_init();
  MyIR.begin();
  MyIR.push_callback([] (uint8_t code) {
    if (code == MyIR.hash) {
      next_screen = next_screen_of(current_screen);
    }
  });
}

void loop() {
  auto start = millis();
  button.update();
  if (button.is_up()) {
    next_screen = next_screen_of(current_screen);
  }
  if (current_screen != next_screen) {
    MyDisplay.switch_screen(load_screen(next_screen));
    current_screen = next_screen;
  }
  digitalWrite(LED_PAD, button.get_current() ? LOW : HIGH);
  MyDisplay.tick();
  MyIR.tick();
  auto now = millis();
  delay(max(0UL, FRAME_TIME - (now - start)));
}

