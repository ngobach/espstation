#include <Arduino.h>
#include <Wire.h>
#include <Ticker.h>
#include "common.h"
#include "sensor.h"
#include "display.h"
#include "screens/splash.h"
#include "buttons.h"

const uint8_t BUTTON_PAD = GPIO_NUM_0;
const uint8_t BUZZER_PAD = GPIO_NUM_23;
const uint8_t LED_PAD = BUILTIN_LED;
const uint16_t FRAME_TIME = 100;
Button button(BUTTON_PAD);

void setup() {
  Serial.begin(9600);
  MySensor.begin();
  MyDisplay.begin();
  MyDisplay.switch_screen(load_screen(ScreenNames::splash));
  pinMode(BUZZER_PAD, OUTPUT);
  pinMode(LED_PAD, OUTPUT);
}

void loop() {
  static bool is_playing = false;
  auto start = millis();
  button.update();
  if (button.is_up()) {
    is_playing = !is_playing;
  }
  digitalWrite(BUZZER_PAD, is_playing ? LOW : HIGH);
  digitalWrite(LED_PAD, is_playing ? HIGH : LOW);
  MyDisplay.tick();
  auto now = millis();
  delay(max(0UL, FRAME_TIME - (now - start)));
}

