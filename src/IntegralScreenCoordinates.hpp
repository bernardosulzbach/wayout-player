#pragma once

#include "Types.hpp"

namespace WayoutPlayer {
class IntegralScreenCoordinates {
  U32 i{};
  U32 j{};

public:
  IntegralScreenCoordinates(U32 screenI, U32 screenJ);

  [[nodiscard]] U32 getI() const;

  [[nodiscard]] U32 getJ() const;
};
} // namespace WayoutPlayer
