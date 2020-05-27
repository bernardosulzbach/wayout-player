#pragma once

#include "Board.hpp"
#include "Image.hpp"

namespace WayoutPlayer {
class BoardScanner {
public:
  BoardScanner();
  Board scan(const Image &image);
};
} // namespace WayoutPlayer