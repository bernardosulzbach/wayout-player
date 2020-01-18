#pragma once

#include "Types.hpp"

#include <array>
#include <stdexcept>

namespace WayoutPlayer {
enum class TileType : U8 { Default, Horizontal, Vertical, Tap, Blocked, Chain, Twin };

constexpr std::array<TileType, 7> TileTypes = {TileType::Default, TileType::Horizontal, TileType::Vertical,
                                               TileType::Tap,     TileType::Blocked,    TileType::Chain,
                                               TileType::Twin};

U32 tileTypeToInteger(TileType tileType);

TileType tileTypeFromInteger(U32 integer);

char tileTypeToCharacter(TileType tileType);

TileType tileTypeFromCharacter(char character);
} // namespace WayoutPlayer
