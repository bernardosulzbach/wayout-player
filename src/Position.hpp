#pragma once

#include "Types.hpp"

#include <string>

namespace WayoutPlayer {
class Position {
public:
  IndexType i = 0;
  IndexType j = 0;

  Position(IndexType i, IndexType j);

  bool operator==(const Position &rhs) const;

  bool operator!=(const Position &rhs) const;

  [[nodiscard]] std::string toString(std::size_t width = 0) const;
};
} // namespace WayoutPlayer
