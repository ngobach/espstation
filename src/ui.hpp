#pragma once

#include <functional>
#include <Arduino.h>
#include <Ticker.h>
#include <U8g2lib.h>
#include "extra_fonts.h"
#include "conf.hpp"

namespace appui
{
  enum KeyCode
  {
    Main,
    Sub,
  };

  class Screen
  {
  protected:
    void update(std::function<void()> inner) {
      inner();
      is_dirty = true;
    }

    const uint8_t* get_font_default() {
      return u8g2_font_5x7_tf;
    }

    const uint8_t* get_font_small() {
      return u8g2_font_4x6_tf;
    }

    const uint8_t* get_font_icon_16() {
      return u8g2_font_open_iconic_all_1x_t;
    }

    const uint8_t* get_font_icon_21() {
      return u8g2_font_streamline_all_t;
    }
    
    void draw_title(U8G2 *g, String title)
    {
      g->drawBox(0, g->getHeight() - 10, g->getWidth(), 10);
      g->setDrawColor(0);
      g->drawStr(
          (g->getWidth() - g->getStrWidth(title.c_str())) / 2,
          g->getHeight() - 2,
          title.c_str());
      g->setDrawColor(1);
    }

  public:
    virtual ~Screen() {};

    bool is_dirty = true;
    virtual void mount() {}
    virtual void unmount() {}
    virtual void paint(U8G2 *g) = 0;
    virtual void key_pressed(KeyCode kc, bool is_long){}
    virtual void on_tick() {}
  };

  std::unique_ptr<Screen> get_initial_screen();

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
    Ticker ticker;
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
      screen = get_initial_screen();
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

      ticker.attach_ms(100, [this]
                       { this->__tick(); });
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
