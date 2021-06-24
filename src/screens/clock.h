#pragma once

#include <Arduino.h>
#include <TimeLib.h>
#include "../ir.h"
#include "screen.h"
#include "bitmaps.h"
#include "display_helper.h"

class ClockScreen : virtual public Screen
{
private:
  int8_t last_second = -1;
  uint16_t ir_callback_key;
  uint16_t setting_values;
  int8_t setting_values_focus = -1;
  const uint16_t multiplier[3] = { 3600, 60, 1 };

  void get_time_str(char *buffer)
  {
    time_t n = now();
    sprintf(buffer, "%02d:%02d:%02d", hour(n), minute(n), second(n));
  }

public:
  bool is_setting_mode() {
    return setting_values_focus >= 0;
  }

  void toggle_setting() {
    if (setting_values_focus >= 0) {
      setting_values_focus = -1;
    } else {
      setting_values_focus = 0;
      setting_values = 0;
    }
    set_dirty(true);
  }

  void setting_next_value() {
    setting_values_focus++;
    if (setting_values_focus > 3) {
      setting_values_focus -= 3;
    }
    set_dirty(true);
  }

  void setting_prev_value() {
    setting_values_focus--;
    if (setting_values_focus < 0) {
      setting_values_focus += 3;
    }
    set_dirty(true);
  }

  void setting_change_value(int8_t amount) {
    if (setting_values_focus < 0) {
      return;
    }

    setting_values += amount * multiplier[setting_values_focus];
    set_dirty(true);
  }

  void onMount() {
    ir_callback_key = MyIR.push_callback([this] (uint16_t code) {
      if (code == MyIR.ok) {
        toggle_setting();
        return;
      }

      if (code == MyIR.right) {
        setting_next_value();
        return;
      }

      if (code == MyIR.left) {
        setting_prev_value();
        return;
      }

      if (code == MyIR.up) {
        setting_change_value(1);
        return;
      }

      if (code == MyIR.down) {
        setting_change_value(-1);
        return;
      }
    });
  }

  void onUnmount() {
    MyIR.remove_callback(ir_callback_key);
  }

  void onTick()
  {
    if (second() != last_second) {
      last_second = second();
      set_dirty(true);
    }
  }

  void draw(Adafruit_SSD1306 *display)
  {
    char str[100];
    DisplayHelper dh(display);
    // draw border
    display->drawRect(0, 0, display->width(), display->height(), SSD1306_WHITE);
    if (setting_values_focus < 0) {
      display->drawBitmap(4, (64 - 16) / 2 , bm_clock_16, 16, 16, SSD1306_WHITE);
      get_time_str(str);
      display->setCursor(4 + 16 + 8, (64 - 14) / 2);
      display->setTextSize(2);
      display->print(str);
    } else {
      uint16_t h, m, s;
      s = setting_values % 60;
      m = setting_values / 60;
      h = m / 60;
      m = m % 60;
      sprintf(str, "%02u:%02u:%02u", h, m, s);
      display->setTextSize(2);
      dh.draw_text_centered(str, 20);
    }
  }
};
