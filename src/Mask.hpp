#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "IntegralScreenCoordinates.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
/**
 * A Mask object represents an in-memory boolean matrix.
 */
class Mask {
  std::vector<std::vector<bool>> data;

public:
  Mask(U32 maskHeight, U32 maskWidth);

  [[nodiscard]] U32 getHeight() const noexcept;

  [[nodiscard]] U32 getWidth() const noexcept;

  [[nodiscard]] bool getValue(IntegralScreenCoordinates coordinates) const;

  void setValue(IntegralScreenCoordinates coordinates, bool value);

  void open(const std::function<bool(IntegralScreenCoordinates)> &predicate);
};
} // namespace WayoutPlayer