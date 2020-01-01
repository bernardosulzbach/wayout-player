#pragma once

#include <string>

#include "TileType.hpp"

namespace WayoutPlayer {
class Tile {
public:
  bool up = false;
  TileType type = TileType::Normal;

  Tile(bool up, TileType type);
  [[nodiscard]] std::string toString() const;

  bool operator==(const Tile &rhs) const;

  bool operator!=(const Tile &rhs) const;
};
} // namespace WayoutPlayer