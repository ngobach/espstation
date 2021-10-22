#pragma once

#include "base.hpp"
#include "ESP8266WiFi.h"
#include "ntptime.hpp"

namespace appui {
  class LoadingScreen : public Screen
  {
  private:
    bool connected = false;
    char time_now[32], date_now[32];
    uint32_t last_check = 0;
    int page = 0;

  public:
    void paint(U8G2 *g) override
    {
      if (!connected)
      {
        draw_title(g, "Connecting...");
        g->setFont(get_font_icon_21());
        g->drawGlyph((g->getWidth() - 21) / 2, (g->getHeight() - 15 + 21) / 2, 0x01fd);
      }
      else
      {
        draw_title(g, "Connected");
        if (page == 0) {
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
        } else if (page == 1) {
          g->setCursor(0, g->getMaxCharHeight());
          g->printf("Free heap: %d", system_get_free_heap_size());
          g->setCursor(0, 2 * g->getMaxCharHeight());
          g->printf("CPU freq : %d", system_get_cpu_freq());
        }
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
        switch_screen(ScreenName::Menu);
        return;
      }

      if (!is_long && kc == KeyCode::Main) {
        update([this] {
          page = (page + 1) % 2;
        });
      }
    }
  };
}