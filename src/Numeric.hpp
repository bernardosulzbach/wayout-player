#pragma once

#include <stdexcept>
#include <string>

namespace WayoutPlayer {
/**
 * Tests if value is in the range [low, high].
 */
template <typename T> inline constexpr bool inRange(const T low, const T value, const T high) noexcept {
  return low <= value && value <= high;
}

template <typename T> inline void assertInRange(const T low, const T value, const T high) {
  if (inRange(low, value, high)) {
    const auto valueString = std::to_string(value);
    const auto rangeString = "[" + std::to_string(low) + ", " + std::to_string(high) + "]";
    throw std::runtime_error("Value " + valueString + " is outside of expected range " + rangeString + ".");
  }
}
} // namespace WayoutPlayer
