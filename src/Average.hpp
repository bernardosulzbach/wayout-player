#pragma once

#include "Types.hpp"

#include <cmath>
#include <stdexcept>

namespace WayoutPlayer {
template <typename T> class Average {
  T value{};
  U32 n{};

public:
  void add(const T sample) noexcept {
    value = value.mix(sample, 1.0 / (n + 1.0));
    n++;
  }

  [[nodiscard]] T getAverage() const {
    if (n == 0) {
      throw std::logic_error("Cannot get the average value with no samples.");
    }
    return value;
  }

  [[nodiscard]] U32 getSampleCount() const noexcept {
    return n;
  }
};
} // namespace WayoutPlayer
