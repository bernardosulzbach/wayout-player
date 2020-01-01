#include "Tile.hpp"

namespace WayoutPlayer {
std::string WayoutPlayer::Tile::toString() const {
  std::string string;
  string += tileTypeToCharacter(type);
  string += (up ? '1' : '0');
  return string;
}

Tile::Tile(bool up, TileType type) : up(up), type(type) {
}

bool Tile::operator==(const Tile &rhs) const {
  return up == rhs.up && type == rhs.type;
}

bool Tile::operator!=(const Tile &rhs) const {
  return !(rhs == *this);
}
} // namespace WayoutPlayer