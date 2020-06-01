#pragma once

#include "Types.hpp"

#include <string>

namespace WayoutPlayer {
class Color {
  U8 r{};
  U8 g{};
  U8 b{};

public:
  constexpr Color() = default;

  constexpr Color(U8 rChannel, U8 gChannel, U8 bChannel) : r(rChannel), g(gChannel), b(bChannel) {
  }

  static Color fromHSV(F32 h, F32 s, F32 v);

  [[nodiscard]] Color getHighContrastGrey() const;

  [[nodiscard]] U8 getR() const;
  U8 &getR();
  [[nodiscard]] U8 getG() const;
  U8 &getG();
  [[nodiscard]] U8 getB() const;
  U8 &getB();

  [[nodiscard]] Color mix(Color other, F64 weight) const noexcept;

  /**
   * Returns the CIELAB (CIE L*a*b*) lightness of this color.
   *
   * This value is guaranteed to be in the [0.0, 100.0] range.
   */
  [[nodiscard]] F32 getLightness() const noexcept;

  /**
   * Returns the HSV saturation of this color.
   *
   * This value is guaranteed to be in the [0.0, 1.0] range.
   */
  [[nodiscard]] F32 getSaturation() const noexcept;

  [[nodiscard]] std::string toString() const;

  bool operator==(const Color &rhs) const;
  bool operator!=(const Color &rhs) const;
};
} // namespace WayoutPlayer
