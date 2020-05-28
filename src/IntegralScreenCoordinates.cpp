#include "IntegralScreenCoordinates.hpp"

namespace WayoutPlayer {
IntegralScreenCoordinates::IntegralScreenCoordinates(U32 i, U32 j) : i(i), j(j) {
}

U32 IntegralScreenCoordinates::getI() const {
  return i;
}

U32 IntegralScreenCoordinates::getJ() const {
  return j;
}
} // namespace WayoutPlayer
