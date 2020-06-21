#pragma once

#include "IntegralScreenCoordinates.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
class FloatingPointScreenCoordinates {
  F32 i{};
  F32 j{};

public:
  FloatingPointScreenCoordinates() = default;

  FloatingPointScreenCoordinates(F32 screenI, F32 screenJ);

  [[nodiscard]] FloatingPointScreenCoordinates mix(FloatingPointScreenCoordinates other, F64 weight) const noexcept;

  [[nodiscard]] F32 getI() const noexcept;

  [[nodiscard]] F32 getJ() const noexcept;

  [[nodiscard]] F32 distanceTo(const FloatingPointScreenCoordinates &rhs) const;

  [[nodiscard]] IntegralScreenCoordinates roundToIntegralScreenCoordinates() const noexcept;
};
} // namespace WayoutPlayer
