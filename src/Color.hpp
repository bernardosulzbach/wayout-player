#pragma once

#include "Numeric.hpp"
#include "Types.hpp"

#include <opencv2/imgproc.hpp>
#include <string>

namespace WayoutPlayer {
/**
 * A color represented as three channels (RGB) in the range [0, 255].
 */
template <typename DataType> class Color {
  // No need to allow for more than bytes and 32-bit floating points.
  static_assert(std::is_same<DataType, U8>::value || std::is_same<DataType, F32>::value);

  DataType r{};
  DataType g{};
  DataType b{};

public:
  constexpr Color() = default;

  constexpr Color(DataType rChannel, DataType gChannel, DataType bChannel) : r(rChannel), g(gChannel), b(bChannel) {
  }

  static Color<DataType> fromHSV(const F32 h, const F32 s, const F32 v) {
    assertInRange<F32>(0.0f, h, 360.0f);
    assertInRange<F32>(0.0f, s, 1.0f);
    assertInRange<F32>(0.0f, v, 1.0f);
    const auto source = cv::Mat3f(cv::Vec3f(h, s, v));
    auto destination = cv::Mat3f(1, 1);
    cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_HSV2RGB);
    auto result = destination.at<cv::Vec3f>(0, 0) * 255.0f;
    if constexpr (std::is_same<DataType, U8>::value) {
      for (U32 i = 0; i < 3; i++) {
        result[i] = std::clamp(std::round(result[i]), 0.0f, 255.0f);
      }
    }
    return Color<DataType>(result[0], result[1], result[2]);
  }

  [[nodiscard]] Color<DataType> getHighContrastGrey() const {
    if (getLightness() >= 50.0) {
      return Color<DataType>(31, 31, 31);
    } else {
      return Color<DataType>(224, 224, 224);
    }
  }

  [[nodiscard]] DataType getR() const noexcept {
    return r;
  }

  DataType &getR() noexcept {
    return r;
  }

  [[nodiscard]] DataType getG() const noexcept {
    return g;
  }

  DataType &getG() noexcept {
    return g;
  }

  [[nodiscard]] DataType getB() const noexcept {
    return b;
  }

  DataType &getB() noexcept {
    return b;
  }

  [[nodiscard]] Color<DataType> mix(Color<DataType> other, F32 weight) const noexcept {
    const auto mixedR = getR() * (1.0f - weight) + other.getR() * weight;
    const auto mixedG = getG() * (1.0f - weight) + other.getG() * weight;
    const auto mixedB = getB() * (1.0f - weight) + other.getB() * weight;
    return Color<DataType>(mixedR, mixedG, mixedB);
  }

  /**
   * Returns the CIELAB (CIE L*a*b*) lightness of this color.
   *
   * This value is guaranteed to be in the [0.0, 100.0] range.
   */
  [[nodiscard]] F32 getLightness() const noexcept {
    const auto source = cv::Mat3f(cv::Vec3f(getR(), getG(), getB()) / 255.0f);
    auto destination = cv::Mat3f(1, 1);
    cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_RGB2Lab);
    return destination.at<cv::Vec3f>(0, 0)[0];
  }

  /**
   * Returns the HSV saturation of this color.
   *
   * This value is guaranteed to be in the [0.0, 1.0] range.
   */
  [[nodiscard]] F32 getSaturation() const noexcept {
    const auto source = cv::Mat3f(cv::Vec3f(getR(), getG(), getB()) / 255.0f);
    auto destination = cv::Mat3f(1, 1);
    cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_RGB2HSV);
    return destination.at<cv::Vec3f>(0, 0)[1];
  }

  [[nodiscard]] std::string toString() const {
    const auto rString = std::to_string(static_cast<U32>(getR()));
    const auto gString = std::to_string(static_cast<U32>(getG()));
    const auto bString = std::to_string(static_cast<U32>(getB()));
    return "(" + rString + ", " + gString + ", " + bString + ")";
  }

  bool operator==(const Color &rhs) const {
    // Doesn't make sense when using F32.
    static_assert(std::is_same<DataType, U8>::value);
    return r == rhs.r && g == rhs.g && b == rhs.b;
  }

  bool operator!=(const Color &rhs) const {
    return !(rhs == *this);
  }
};
} // namespace WayoutPlayer
