#pragma once

#include "Types.hpp"

#include <string>

namespace WayoutPlayer {
class Position {
public:
  S32 i = 0;
  S32 j = 0;

  bool operator==(const Position &rhs) const;

  bool operator!=(const Position &rhs) const;

  [[nodiscard]] std::string toString(std::size_t width = 0) const;
};
} // namespace WayoutPlayer
