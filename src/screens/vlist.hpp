#pragma once

#include "base.hpp"

namespace appui
{
  class Vlist : public Screen
  {
  protected:
    std::vector<std::unique_ptr<Widget>> items;
    size_t focused_index = 0;

  public:
    void paint(U8G2 *g) override
    {
      if (items.size() > 0) {
        std::vector<std::pair<int, int>> sizes;
        sizes.reserve(items.size());
        auto rect = Rect::zero();

        for (auto &it : items) {
          rect = it->measure(g);
          auto offset = sizes.empty() ? 0 : sizes.back().first + sizes.back().second;
          sizes.emplace_back(std::make_pair(offset, rect.h));
        }

        if (focused_index >= items.size()) {
          focused_index = items.size() - 1;
        }

        int offset = g->getHeight() / 2 - sizes[focused_index].second / 2 - sizes[focused_index].first;
        if (offset < g->getHeight() - sizes.back().first - sizes.back().second) {
          offset = g->getHeight() - sizes.back().first - sizes.back().second;
        }
        if (offset > 0) {
          offset = 0;
        }
        if (offset + sizes[focused_index].first < 0) {
          offset = -sizes[focused_index].first;
        }
        if (offset + sizes[focused_index].first + sizes[focused_index].second > g->getHeight()) {
          offset = g->getHeight() - sizes[focused_index].first - sizes[focused_index].second;
        }

        rect = Rect::zero();
        rect.w = g->getWidth();
        for (size_t i = 0; i < items.size(); i++) {
          rect.y = offset + sizes[i].first;
          rect.h = sizes[i].second;
          items[i]->paint(g, rect, i == focused_index);
        }
      }
    }

    void key_pressed(KeyCode kc, bool is_long) override {
      if (!is_long) {
        if (kc == KeyCode::Main && focused_index > 0) {
          update([this] {
            focused_index--;
          });
        } else if (kc == KeyCode::Sub && focused_index < items.size() - 1) {
          update([this] {
            focused_index++;
          });
        }
      }
    }
  };
}
