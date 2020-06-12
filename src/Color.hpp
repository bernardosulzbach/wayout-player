#pragma once

#include "Numeric.hpp"
#include "Types.hpp"

#include <opencv2/imgproc.hpp>
#include <string>

namespace WayoutPlayer {
static constexpr auto HsvEpsilon = 1.0e-6;

static constexpr auto MaximumHue = 360.0F;
static constexpr auto MaximumLightness = 100.0F;
static constexpr auto LightnessMidpoint = 0.5F * MaximumLightness;

static constexpr auto DarkValueMultiple = 0.2F;
static constexpr auto LightValueMultiple = 0.8F;

/**
 * A color represented as three channels (RGB) in the range [0, 255].
 */
template <typename DataType> class Color {
  // No need to allow for more than bytes and 32-bit floating points.
  static_assert(std::is_same<DataType, U8>::value || std::is_same<DataType, F32>::value);

  constexpr DataType getMaximum() const {
    if constexpr (std::is_same<DataType, U8>::value) {
      return std::numeric_limits<DataType>::max();
    }
    if constexpr (std::is_same<DataType, F32>::value) {
      return 1.0F;
    }
    throw std::runtime_error("Should not be reachable.");
  }

  constexpr DataType getLightValue() const {
    return LightValueMultiple * getMaximum();
  }

  constexpr DataType getDarkValue() const {
    return DarkValueMultiple * getMaximum();
  }

  DataType r{};
  DataType g{};
  DataType b{};

public:
  constexpr Color() = default;

  constexpr Color(DataType rChannel, DataType gChannel, DataType bChannel) : r(rChannel), g(gChannel), b(bChannel) {
  }

  static Color<DataType> fromHSV(const F32 h, const F32 s, const F32 v) {
    assertInRange<F32>(0.0F - HsvEpsilon, h, MaximumHue + HsvEpsilon);
    assertInRange<F32>(0.0F - HsvEpsilon, s, 1.0F + HsvEpsilon);
    assertInRange<F32>(0.0F - HsvEpsilon, v, 1.0F + HsvEpsilon);
    const auto source = cv::Mat3f(cv::Vec3f(h, s, v));
    auto destination = cv::Mat3f(1, 1);
    cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_HSV2RGB);
    auto result = destination.at<cv::Vec3f>(0, 0);
    if constexpr (std::is_same<DataType, U8>::value) {
      result *= std::numeric_limits<U8>::max();
      for (U32 i = 0; i < 3; i++) {
        result[i] = std::clamp(std::round(result[i]), 0.0F, static_cast<F32>(std::numeric_limits<U8>::max()));
      }
    }
    return Color<DataType>(result[0], result[1], result[2]);
  }

  [[nodiscard]] Color<DataType> getHighContrastGrey() const {
    if (getLightness() >= LightnessMidpoint) {
      const auto lowValue = getDarkValue();
      return Color<DataType>(lowValue, lowValue, lowValue);
    }
    const auto highValue = getLightValue();
    return Color<DataType>(highValue, highValue, highValue);
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
    const auto mixedR = getR() * (1.0F - weight) + other.getR() * weight;
    const auto mixedG = getG() * (1.0F - weight) + other.getG() * weight;
    const auto mixedB = getB() * (1.0F - weight) + other.getB() * weight;
    return Color<DataType>(mixedR, mixedG, mixedB);
  }

  /**
   * Returns the CIELAB (CIE L*a*b*) lightness of this color.
   *
   * This value is guaranteed to be in the [0.0, 100.0] range.
   */
  [[nodiscard]] F32 getLightness() const {
    const auto maximum = static_cast<F32>(getMaximum());
    const auto source = cv::Mat3f(cv::Vec3f(getR() / maximum, getG() / maximum, getB() / maximum));
    auto destination = cv::Mat3f(1, 1);
    cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_RGB2Lab);
    return destination.at<cv::Vec3f>(0, 0)[0];
  }

  /**
   * Returns the HSV saturation of this color.
   *
   * This value is guaranteed to be in the [0.0, 1.0] range.
   */
  [[nodiscard]] F32 getSaturation() const {
    const auto maximum = static_cast<F32>(getMaximum());
    const auto source = cv::Mat3f(cv::Vec3f(getR() / maximum, getG() / maximum, getB() / maximum));
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
