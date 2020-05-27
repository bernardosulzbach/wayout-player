#include "Pixel.hpp"

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

bool Pixel::operator==(const Pixel &rhs) const {
  return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool Pixel::operator!=(const Pixel &rhs) const {
  return !(rhs == *this);
}
} // namespace WayoutPlayer