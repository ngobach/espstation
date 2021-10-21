#pragma once

#include <memory>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <cmath>
#include "ui.hpp"
#include "ntptime.hpp"
#include "corelib.hpp"
#include "scheduler.hpp"

namespace appui
{
  enum ScreenName
  {
    Loading,
    Menu,
    Coins,
  };
  std::unique_ptr<Screen> load_screen(ScreenName name);

  struct MenuScreenItem {
    const char* title;
    const char* icon;
    const ScreenName screen;
  };
  
  class MenuScreen : public Screen
  {
  private:
    const MenuScreenItem items[2] = {
      {
        .title = "Coins",
        .icon = "\u0229",
        .screen = ScreenName::Coins,
      },
      {
        .title = "System",
        .icon = "\u01fc",
        .screen = ScreenName::Loading,
      },
    };
    const size_t items_count = sizeof(items) / sizeof(MenuScreenItem);
    int current_index = 0;

  public:
    void paint(U8G2 *g) override
    {
      const MenuScreenItem &current = items[current_index];
      draw_title(g, current.title);
      char pos[20];
      sprintf(pos, "%d/%d", current_index + 1, items_count);
      g->setFont(get_font_small());
      g->drawStr((g->getWidth() - g->getStrWidth(pos)) / 2, 7, pos);
      g->setFont(get_font_icon_21());
      g->drawUTF8((g->getWidth() - 21) / 2, (g->getHeight() - 10 + 8 + 21) / 2, current.icon);
    }

    void key_pressed(KeyCode kc, bool is_long) override {
      if (is_long && kc == KeyCode::Main) {
        Serial.printf("Loading screen %d\n", kc);
        AppUI.switch_screen(load_screen(items[current_index].screen));
        return;
      }

      if (!is_long) {
        switch (kc) {
          case KeyCode::Main:
            update([this] {
              current_index = (current_index - 1 + items_count) % items_count;
            });
            break;
          case KeyCode::Sub:
            update([this] {
                current_index = (current_index + 1) % items_count;
            });
            break;
        }
      }
    }
  };

  class CoinsScreen : public Screen
  {
  private:
    const uint8_t task_fetch_id = 0;
    bool is_fetching = false;
    std::vector<core::Coin> coins;
    int page_offset = 0, next_page_offset = INT_MIN;
    size_t focused_index = 0;

  protected:
    void mount() override
    {
      TS.add(
          task_fetch_id, 5 * 1000, [this](void *_)
          {
            update([this]
                   {
                     is_fetching = true;
                     coins = CoreLib.get_coins();
                     is_fetching = false;
                   });
          },
          nullptr, true);
    }

    void unmount() override
    {
      TS.remove(task_fetch_id);
    }

    void on_tick() override
    {
      if (next_page_offset != INT_MIN)
      {
        update([this]
               {
                 page_offset = next_page_offset;
                 next_page_offset = INT_MIN;
               });
      }
    }

    int draw_coin(U8G2 *g, int y, core::Coin &coin, bool bordered)
    {
      char title[32], price[32];
      int yoffset;
      sprintf(title, "%s (%s)", coin.name, coin.symbol);
      sprintf(price, "%0.2f", coin.current_price);
      g->setFont(get_font_small());
      yoffset = y + 2 + g->getMaxCharHeight();
      g->drawStr(4, yoffset, title);
      g->setFont(get_font_default());
      yoffset += 1 + g->getMaxCharHeight();
      g->drawStr(4, yoffset, price);
      yoffset += 2;
      if (bordered)
      {
        g->drawRFrame(0, y, g->getDisplayWidth(), yoffset - y, 2);
      }
      return yoffset - y;
    }

    void paint(U8G2 *g) override
    {
      if (coins.empty())
      {
        draw_title(g, "Fetching...");
        return;
      }

      int offset = page_offset;
      for (size_t i = 0; i < coins.size(); i++)
      {
        int h = draw_coin(g, offset, coins[i], i == focused_index);

        if (i == focused_index)
        {
          if (offset < 0)
          {
            next_page_offset = page_offset + (-offset);
          }
          else if (offset + h + 2 > g->getDisplayHeight())
          {
            next_page_offset = page_offset - (offset + h + 2 - g->getDisplayHeight());
          }
        }
        offset += h + 2;
      }
    }

    void key_pressed(KeyCode kc, bool is_long) override
    {
      if (is_fetching)
      {
        return;
      }

      if (!is_long)
      {
        if (kc == KeyCode::Sub && focused_index != coins.size() - 1)
        {
          update([this]
                 { focused_index = focused_index + 1; });
        }
        else if (kc == KeyCode::Main && focused_index > 0)
        {
          update([this]
                 { focused_index = focused_index - 1; });
        }
      }
      else
      {
        if (kc == KeyCode::Sub)
        {
          AppUI.switch_screen(load_screen(ScreenName::Menu));
        }
      }
    }
  };

  class LoadingScreen : public Screen
  {
  private:
    bool connected = false;
    char time_now[32], date_now[32];
    uint32_t last_check = 0;

  public:
    void paint(U8G2 *g) override
    {
      if (!connected)
      {
        draw_title(g, "Connecting...");
        g->setFont(get_font_icon_21());
        g->drawUTF8((g->getWidth() - 21) / 2, (g->getHeight() - 15 + 21) / 2, "\u01fd");
      }
      else
      {
        draw_title(g, "Connected");
        g->setCursor(0, g->getMaxCharHeight());
        g->printf("WF:%s", WiFi.SSID().c_str());
        g->setCursor(0, 2 * g->getMaxCharHeight());
        g->printf("IP:%s", WiFi.localIP().toString().c_str());
        g->setCursor(0, 3 * g->getMaxCharHeight());
        g->printf("GW:%s", WiFi.gatewayIP().toString().c_str());
        g->setCursor(0, 4 * g->getMaxCharHeight());
        g->printf("T :%s", time_now);
        g->setCursor(0, 5 * g->getMaxCharHeight());
        g->printf("D :%s", date_now);
      }
    }

    void on_tick() override
    {
      if (!connected && WiFi.status() == WL_CONNECTED)
      {
        update([this]
               { connected = true; });
      }

      if (connected)
      {
        uint32_t now = millis();
        if (now - last_check >= 1000)
        {
          String time_str = NtpTime.get_time_string();
          String date_str = NtpTime.get_date_string();
          update([this, time_str, date_str, now]
                 {
                   last_check = now;
                   strcpy(time_now, time_str.c_str());
                   strcpy(date_now, date_str.c_str());
                 });
        }
      }
    }

    void key_pressed(KeyCode kc, bool is_long) override
    {
      if (WiFi.status() != WL_CONNECTED)
      {
        return;
      }

      if (is_long && kc == KeyCode::Sub)
      {
        AppUI.switch_screen(load_screen(ScreenName::Menu));
      }
    }
  };

  std::unique_ptr<Screen> load_screen(ScreenName name)
  {
    switch (name)
    {
    case ScreenName::Loading:
      return std::make_unique<LoadingScreen>();

    case ScreenName::Coins:
      return std::make_unique<CoinsScreen>();

    case ScreenName::Menu:
      return std::make_unique<MenuScreen>();

    default:
      return nullptr;
    }
  }

  /**
   * Default Screen
   */
  std::unique_ptr<Screen> get_initial_screen()
  {
    return load_screen(ScreenName::Loading);
  }
}