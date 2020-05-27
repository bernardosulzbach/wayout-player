#pragma once

#include "Types.hpp"

namespace WayoutPlayer {
class Pixel {
  U8 r{};
  U8 g{};
  U8 b{};

public:
  Pixel() = default;

  Pixel(U8 r, U8 g, U8 b);

  [[nodiscard]] U8 getR() const;
  U8 &getR();
  [[nodiscard]] U8 getG() const;
  U8 &getG();
  [[nodiscard]] U8 getB() const;
  U8 &getB();

  /**
   * Returns the CIELAB (CIE L*a*b*) lightness of this pixel.
   *
   * This value is guaranteed to be in the [0.0, 100.0] range.
   */
  [[nodiscard]] F32 getLightness() const;

  bool operator==(const Pixel &rhs) const;
  bool operator!=(const Pixel &rhs) const;
};
} // namespace WayoutPlayer
