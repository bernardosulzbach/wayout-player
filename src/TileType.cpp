#include "TileType.hpp"

namespace WayoutPlayer {
U32 tileTypeToInteger(TileType tileType) {
  return static_cast<U32>(tileType);
}

TileType tileTypeFromInteger(U32 integer) {
  for (const auto tileType : TileTypes) {
    if (tileTypeToInteger(tileType) == integer) {
      return tileType;
    }
  }
  throw std::invalid_argument("Invalid integer for tile type: " + std::to_string(integer) + ".");
}

char tileTypeToCharacter(TileType tileType) {
  switch (tileType) {
  case TileType::Default:
    return 'D';
  case TileType::Horizontal:
    return 'H';
  case TileType::Vertical:
    return 'V';
  case TileType::Tap:
    return 'T';
  case TileType::Blocked:
    return 'B';
  case TileType::Chain:
    return 'C';
  case TileType::Twin:
    return 'P';
  }
  throw std::invalid_argument("Should not be reachable.");
}

TileType tileTypeFromCharacter(char character) {
  for (const auto tileType : TileTypes) {
    if (tileTypeToCharacter(tileType) == character) {
      return tileType;
    }
  }
  throw std::invalid_argument("Invalid character for tile type: " + std::string(1, character) + ".");
}
} // namespace WayoutPlayer
