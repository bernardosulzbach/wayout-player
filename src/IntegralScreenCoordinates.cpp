#include "IntegralScreenCoordinates.hpp"

namespace WayoutPlayer {
IntegralScreenCoordinates::IntegralScreenCoordinates(U32 screenI, U32 screenJ) : i(screenI), j(screenJ) {
}

U32 IntegralScreenCoordinates::getI() const {
  return i;
}

U32 IntegralScreenCoordinates::getJ() const {
  return j;
}

bool IntegralScreenCoordinates::operator==(const IntegralScreenCoordinates &rhs) const {
  return i == rhs.i && j == rhs.j;
}

bool IntegralScreenCoordinates::operator!=(const IntegralScreenCoordinates &rhs) const {
  return !(rhs == *this);
}
} // namespace WayoutPlayer
