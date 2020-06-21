#pragma once

#include "Types.hpp"

#include <algorithm>
#include <string>

namespace WayoutPlayer {
/**
 * A range of the form [Low, High].
 */
template <typename DataType> class Range {
  DataType minimum{};
  DataType maximum{};

public:
  Range() = default;

  Range(const DataType rangeMinimum, const DataType rangeMaximum) : minimum(rangeMinimum), maximum(rangeMaximum) {
  }

  [[nodiscard]] Range<DataType> include(const DataType value) const noexcept {
    return Range<DataType>(std::min(minimum, value), std::max(maximum, value));
  }

  [[nodiscard]] DataType getLow() const noexcept {
    return minimum;
  }

  [[nodiscard]] DataType getHigh() const noexcept {
    return maximum;
  }
};
} // namespace WayoutPlayer
