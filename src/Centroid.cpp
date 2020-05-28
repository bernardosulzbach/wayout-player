#include "Centroid.hpp"

namespace WayoutPlayer {
void Centroid::add(const U32 newI, const U32 newJ) noexcept {
  const auto weight = n / (n + 1.0f);
  i = weight * i + (1.0f - weight) * newI;
  j = weight * j + (1.0f - weight) * newJ;
  n++;
}

U32 Centroid::getPointCount() const noexcept {
  return n;
}

IntegralScreenCoordinates Centroid::roundToIntegralScreenCoordinates() const noexcept {
  return IntegralScreenCoordinates(std::round(i), std::round(j));
}
} // namespace WayoutPlayer