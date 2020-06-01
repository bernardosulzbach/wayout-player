#include "FloatingPointScreenCoordinates.hpp"

#include <cmath>

namespace WayoutPlayer {
FloatingPointScreenCoordinates::FloatingPointScreenCoordinates(const F32 screenI, const F32 screenJ)
    : i(screenI), j(screenJ) {
}

FloatingPointScreenCoordinates FloatingPointScreenCoordinates::mix(const FloatingPointScreenCoordinates other,
                                                                   const F64 weight) const noexcept {
  const auto mixedI = getI() * (1.0 - weight) + other.getI() * weight;
  const auto mixedJ = getJ() * (1.0 - weight) + other.getJ() * weight;
  return FloatingPointScreenCoordinates(mixedI, mixedJ);
}

F32 FloatingPointScreenCoordinates::getI() const noexcept {
  return i;
}

F32 FloatingPointScreenCoordinates::getJ() const noexcept {
  return j;
}

IntegralScreenCoordinates FloatingPointScreenCoordinates::roundToIntegralScreenCoordinates() const noexcept {
  return IntegralScreenCoordinates(std::round(getI()), std::round(getJ()));
}
} // namespace WayoutPlayer
