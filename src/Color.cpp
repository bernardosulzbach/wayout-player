#include "Color.hpp"

#include <cmath>

#include <opencv2/imgproc.hpp>

namespace WayoutPlayer {
Color::Color(U8 r, U8 g, U8 b) : r(r), g(g), b(b) {
}

Color Color::fromHSV(F32 h, F32 s, F32 v) {
  const auto source = cv::Mat3f(cv::Vec3f(h, s, v));
  auto destination = cv::Mat3f(1, 1);
  cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_HSV2RGB);
  const auto result = destination.at<cv::Vec3f>(0, 0) * 255.0f;
  return Color(result[0], result[1], result[2]);
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