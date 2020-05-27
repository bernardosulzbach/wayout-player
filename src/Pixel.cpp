#include "Pixel.hpp"

#include <cmath>

#include <opencv2/imgproc.hpp>

namespace WayoutPlayer {
Pixel::Pixel(U8 r, U8 g, U8 b) : r(r), g(g), b(b) {
}

U8 Pixel::getR() const {
  return r;
}

U8 &Pixel::getR() {
  return r;
}

U8 Pixel::getG() const {
  return g;
}

U8 &Pixel::getG() {
  return g;
}

U8 Pixel::getB() const {
  return b;
}

U8 &Pixel::getB() {
  return b;
}

F32 Pixel::getLightness() const {
  const auto normalizedR = static_cast<float>(getR()) / 255.0f;
  const auto normalizedG = static_cast<float>(getG()) / 255.0f;
  const auto normalizedB = static_cast<float>(getB()) / 255.0f;
  const auto source = cv::Mat3f(cv::Vec3f(normalizedR, normalizedG, normalizedB));
  auto destination = cv::Mat3f(1, 1);
  cv::cvtColor(source, destination, cv::ColorConversionCodes::COLOR_RGB2Lab);
  return destination.at<cv::Vec3f>(0, 0)[0];
}

bool Pixel::operator==(const Pixel &rhs) const {
  return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool Pixel::operator!=(const Pixel &rhs) const {
  return !(rhs == *this);
}
} // namespace WayoutPlayer