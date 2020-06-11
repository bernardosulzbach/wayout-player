#pragma once

#include <cmath>
#include <stdexcept>
#include <string>

#include "Types.hpp"

namespace WayoutPlayer {
/**
 * Tests if value is in the range [low, high].
 */
template <typename T> inline constexpr bool inRange(const T low, const T value, const T high) noexcept {
  return low <= value && value <= high;
}

/**
 * Computes the absolute difference between two unsigned values.
 */
template <typename T> inline constexpr T absoluteDifference(const T a, const T b) noexcept {
  static_assert(std::is_unsigned<T>::value);
  return std::max(a, b) - std::min(a, b);
}

template <typename T> inline void assertInRange(const T low, const T value, const T high) {
  if (!inRange(low, value, high)) {
    const auto valueString = std::to_string(value);
    const auto rangeString = "[" + std::to_string(low) + ", " + std::to_string(high) + "]";
    throw std::runtime_error("Value " + valueString + " is outside of expected range " + rangeString + ".");
  }
}

/**
 * Rounds the input to the nearest integer.
 *
 * Throws an exception if the rounding required is above the specified threshold.
 *
 * This function template is meant to be used for safely converting near-integers into integers.
 */
template <typename T> inline constexpr T roundWithin(const T value, const T threshold) {
  static_assert(std::is_same<T, F32>::value || std::is_same<T, F64>::value);
  assertInRange(0.0, threshold, 0.5);
  const auto rounded = std::round(value);
  const auto distanceToInteger = std::abs(value - rounded);
  if (distanceToInteger > threshold) {
    const auto dS = std::to_string(distanceToInteger);
    const auto tS = std::to_string(threshold);
    throw std::runtime_error("Distance to nearest integer (" + dS + ") is greater than the threshold (" + tS + ").");
  }
  return rounded;
}
} // namespace WayoutPlayer
