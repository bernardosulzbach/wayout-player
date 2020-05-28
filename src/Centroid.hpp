#pragma once

#include "IntegralScreenCoordinates.hpp"
#include "Types.hpp"

#include <cmath>

namespace WayoutPlayer {
class Centroid {
  F32 i{};
  F32 j{};
  U32 n{};

public:
  void add(U32 newI, U32 newJ) noexcept;

  [[nodiscard]] U32 getPointCount() const noexcept;

  [[nodiscard]] IntegralScreenCoordinates roundToIntegralScreenCoordinates() const noexcept;
};
} // namespace WayoutPlayer
