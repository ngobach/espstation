#pragma once

#include <functional>
#include <Arduino.h>
#include <U8g2lib.h>
#include "extra_fonts.h"
#include "conf.hpp"
#include "scheduler.hpp"
#include "screens/screens.hpp"

namespace appui
{
  struct PinReadState
  {
    int8_t pin;
    KeyCode key_code;
    int32_t low_at = -1;
  };

  class AppUI_
  {
  private:
    bool initialized = false;
    U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2 = U8G2_PCD8544_84X48_F_4W_HW_SPI(U8G2_R2, D8, D0, D1);
    std::vector<PinReadState> pin_reads;
    std::unique_ptr<Screen> screen;

  public:
    AppUI_()
    {
      pin_reads.emplace_back(PinReadState{
          .pin = D2,
          .key_code = KeyCode::Sub,
      });
      pin_reads.emplace_back(PinReadState{
          .pin = D3,
          .key_code = KeyCode::Main,
      });
      screen = load_screen(ScreenName::Loading);
    }

    void begin()
    {
      if (initialized)
      {
        return;
      }

      for (auto &pr : pin_reads)
      {
        pinMode(pr.pin, INPUT_PULLUP);
      }

      u8g2.begin();

      MainLoops.emplace_back([this] { this->__tick(); });
      initialized = true;
    }

    void switch_screen(std::unique_ptr<Screen> next_screen) {
      if (screen) {
        screen->unmount();
      }

      if (next_screen) {
        next_screen->mount();
      }

      screen = std::move(next_screen);
    }

    void dispatch_key_pressed(KeyCode kc, bool is_long)
    {
      if (screen)
      {
        screen->key_pressed(kc, is_long);
      }
    }

  private:
    void __tick()
    {
      int32_t now = millis();
      for (auto &pr : pin_reads)
      {
        int8_t current = digitalRead(pr.pin);
        if (pr.low_at >= 0 && current == HIGH) 
        {
          dispatch_key_pressed(pr.key_code, false);
          pr.low_at = -2;
        } else if (pr.low_at >= 0 && current == LOW && now - pr.low_at >= 500) {
          dispatch_key_pressed(pr.key_code, true);
          pr.low_at = -2;
        } else if (pr.low_at == -1 && current == LOW) {
          pr.low_at = now;
        }
        if (current == HIGH) {
          pr.low_at = -1;
        }
      }

      if (screen) {
        screen->on_tick();
      }

      if (screen && screen->is_dirty) {
        u8g2.clearBuffer();
        u8g2.home();
        u8g2.setDrawColor(1);
        u8g2.setFont(u8g2_font_5x7_tf);
        screen->paint(&u8g2);
        u8g2.sendBuffer();
        screen->is_dirty = false;
      }
    }
  };
}

appui::AppUI_ AppUI;

void appui::switch_screen(ScreenName name, ScreenParams params) {
  AppUI.switch_screen(load_screen(name, std::move(params)));
}