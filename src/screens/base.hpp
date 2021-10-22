#pragma once

#include <memory>
#include <Arduino.h>
#include "ui.hpp"

namespace appui
{
  const uint8_t *get_font_default()
  {
    return u8g2_font_5x7_tf;
  }

  const uint8_t *get_font_small()
  {
    return u8g2_font_4x6_tf;
  }

  const uint8_t *get_font_icon_16()
  {
    return u8g2_font_open_iconic_all_1x_t;
  }

  const uint8_t *get_font_icon_21()
  {
    return u8g2_font_streamline_all_t;
  }

  enum ScreenName
  {
    Loading,
    Menu,
    Coins,
    CoinInfo,
    Test,
  };

  enum KeyCode
  {
    Main,
    Sub,
  };

  class Screen
  {
  protected:
    void update(std::function<void()> inner)
    {
      inner();
      is_dirty = true;
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

    virtual ~Screen(){};
    virtual void mount() {}
    virtual void unmount() {}
    virtual void paint(U8G2 *g) = 0;
    virtual void key_pressed(KeyCode kc, bool is_long) {}
    virtual void on_tick() {}
  };

  typedef std::unique_ptr<std::vector<std::pair<String, String>>> ScreenParams;
  void switch_screen(ScreenName name, ScreenParams = nullptr);

  struct Rect
  {
    int x, y, w, h;

    bool is_empty()
    {
      return x == -1 && y == -1 && w == -1 && h == -1;
    }

    static Rect empty()
    {
      return Rect{-1, -1, -1, -1};
    }

    static Rect zero()
    {
      return Rect{0, 0, 0, 0};
    }
  };

  class Widget
  {
  public:
    virtual ~Widget() {};
    virtual Rect measure(U8G2 *g) { return Rect::empty(); }
    virtual void paint(U8G2 *g, Rect r, bool focused = false) = 0;
    virtual void key_pressed(KeyCode kc, bool is_long){};
  };
}