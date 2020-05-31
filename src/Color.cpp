#include "Color.hpp"

#include <opencv2/imgproc.hpp>

namespace WayoutPlayer {
Color::Color(U8 rChannel, U8 gChannel, U8 bChannel) : r(rChannel), g(gChannel), b(bChannel) {
}

static void assertArgumentInRange(const F32 c, const F32 a, const F32 b) {
  if (c < a || c > b) {
    const auto rangeString = "[" + std::to_string(a) + ", " + std::to_string(b) + "]";
    throw std::runtime_error("Value " + std::to_string(c) + " is outside of expected range " + rangeString + ".");
  }
}

Color Color::fromHSV(const F32 h, const F32 s, const F32 v) {
  assertArgumentInRange(h, 0.0f, 360.0f);
  assertArgumentInRange(s, 0.0f, 1.0f);
  assertArgumentInRange(v, 0.0f, 1.0f);
  const auto source = cv::Mat3f(cv::Vec3f(h, s, v));
  auto destination = cv::Mat3f(1, 1);
  cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_HSV2RGB);
  const auto result = destination.at<cv::Vec3f>(0, 0) * 255.0f;
  return Color(result[0], result[1], result[2]);
}

Color Color::getHighContrastGrey() const {
  if (getLightness() >= 50.0) {
    return Color(31, 31, 31);
  } else {
    return Color(224, 224, 224);
  }
}

U8 Color::getR() const {
  return r;
}

U8 &Color::getR() {
  return r;
}

U8 Color::getG() const {
  return g;
}

U8 &Color::getG() {
  return g;
}

U8 Color::getB() const {
  return b;
}

U8 &Color::getB() {
  return b;
}

F32 Color::getLightness() const {
  const auto source = cv::Mat3f(cv::Vec3f(getR(), getG(), getB()) / 255.0f);
  auto destination = cv::Mat3f(1, 1);
  cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_RGB2Lab);
  return destination.at<cv::Vec3f>(0, 0)[0];
}

bool Color::operator==(const Color &rhs) const {
  return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool Color::operator!=(const Color &rhs) const {
  return !(rhs == *this);
}
} // namespace WayoutPlayer