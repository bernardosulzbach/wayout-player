#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
/**
 * A Mask object represents an in-memory boolean matrix.
 */
class Mask {
  std::vector<std::vector<bool>> data;

public:
  Mask(U32 maskHeight, U32 maskWidth);

  [[nodiscard]] U32 getHeight() const;

  [[nodiscard]] U32 getWidth() const;

  [[nodiscard]] bool getValue(U32 i, U32 j) const;

  void open(const std::function<bool(U32, U32)> &predicate);

  void setValue(U32 i, U32 j, bool value);
};
} // namespace WayoutPlayer