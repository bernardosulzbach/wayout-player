#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Types.hpp"

namespace WayoutPlayer {
class Board {
  std::vector<std::vector<bool>> matrix;

  [[nodiscard]] S32 getRowCount() const;

  [[nodiscard]] S32 getColumnCount() const;

  void safeInvert(S32 i, S32 j);

public:
  explicit Board(std::vector<std::vector<bool>> booleanMatrix);

  void activate(S32 i, S32 j);

  [[nodiscard]] std::string toString() const;

  static Board fromString(const std::string &string);
};
} // namespace WayoutPlayer
