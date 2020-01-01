#pragma once

#include "Position.hpp"

#include <vector>

namespace WayoutPlayer {
class Solution {
  std::vector<Position> clicks;
  bool optimal;

public:
  Solution(std::vector<Position> clickVector, bool isOptimal);

  [[nodiscard]] const std::vector<Position> &getClicks() const;

  [[nodiscard]] bool isOptimal() const;

  bool operator==(const Solution &rhs) const;

  bool operator!=(const Solution &rhs) const;

  [[nodiscard]] std::string toString() const;
};
} // namespace WayoutPlayer