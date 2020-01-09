#pragma once

#include "Types.hpp"
#include <stdexcept>

namespace WayoutPlayer {
enum class TileType { Default, Horizontal, Vertical, Tap, Blocked, Chain };

U32 tileTypeToInteger(TileType tileType);

TileType tileTypeFromInteger(U32 integer);

char tileTypeToCharacter(TileType tileType);

TileType tileTypeFromCharacter(char character);
} // namespace WayoutPlayer
