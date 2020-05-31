#pragma once

namespace WayoutPlayer {
/**
 * Tests if value is in the range [low, high].
 */
template <typename T> inline constexpr bool inRange(const T low, const T value, const T high) noexcept {
  return low <= value && value <= high;
}
} // namespace WayoutPlayer
