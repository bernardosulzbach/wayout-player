#pragma once

#include <string>

#include "TileType.hpp"

namespace WayoutPlayer {
class Tile {
public:
  bool up = false;
  TileType type = TileType::Default;

  Tile(bool tileUp, TileType tileType);
  [[nodiscard]] std::string toString() const;

  bool operator==(const Tile &rhs) const;

  bool operator!=(const Tile &rhs) const;
};
} // namespace WayoutPlayer