#pragma once

#include <vector>

#include "IntegralScreenCoordinates.hpp"

namespace WayoutPlayer {
template <typename T>
std::vector<IntegralScreenCoordinates> getNeighbors(const T &t, const IntegralScreenCoordinates coordinates) {
  std::vector<IntegralScreenCoordinates> neighbors;
  const auto i = coordinates.getI();
  const auto j = coordinates.getJ();
  if (i > 0) {
    neighbors.emplace_back(i - 1, j);
  }
  if (j > 0) {
    neighbors.emplace_back(i, j - 1);
  }
  if (i + 1 < t.getHeight()) {
    neighbors.emplace_back(i + 1, j);
  }
  if (j + 1 < t.getWidth()) {
    neighbors.emplace_back(i, j + 1);
  }
  return neighbors;
}
}; // namespace WayoutPlayer