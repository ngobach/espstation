#pragma once

#include "vlist.hpp"
#include "widgets.hpp"

namespace appui {
  class TestScreen : public Vlist {
    public:
      TestScreen(): Vlist() {
        for (int i = 0; i < 10; i++) {
          auto tw = new TextWidget();
          tw->text = String() + "Item: " + i;
          if (i % 2 == 1) {
            tw->sub = String() + "Description " + i;
          }
          items.emplace_back(tw);
        }
      }

      void key_pressed(KeyCode kc, bool is_long) override {
        if (is_long && kc == KeyCode::Sub) {
          switch_screen(ScreenName::Menu);
          return;
        }

        Vlist::key_pressed(kc, is_long);
      }
  };
}