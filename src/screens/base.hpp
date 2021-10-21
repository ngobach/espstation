#pragma once

#include <memory>
#include <Arduino.h>
#include "ui.hpp"

namespace appui
{
  enum ScreenName
  {
    Loading,
    Menu,
    Coins,
  };

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
    bool is_dirty = true;

    virtual ~Screen() {};
    virtual void mount() {}
    virtual void unmount() {}
    virtual void paint(U8G2 *g) = 0;
    virtual void key_pressed(KeyCode kc, bool is_long){}
    virtual void on_tick() {}
  };

  void switch_screen(ScreenName name);
}