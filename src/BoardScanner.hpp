#pragma once

#include "Board.hpp"
#include "Image.hpp"

namespace WayoutPlayer {
class BoardScanner {
  Board scan(const Image &image) {
    return Board(std::vector<std::vector<std::optional<Tile>>>{});
  }
};
} // namespace WayoutPlayer