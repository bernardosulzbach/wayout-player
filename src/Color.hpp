#pragma once

#include "Types.hpp"

namespace WayoutPlayer {
class Color {
  U8 r{};
  U8 g{};
  U8 b{};

public:
  Color() = default;

  Color(U8 r, U8 g, U8 b);

  static Color fromHSV(F32 h, F32 s, F32 v);

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

  bool operator==(const Color &rhs) const;
  bool operator!=(const Color &rhs) const;
};
} // namespace WayoutPlayer
