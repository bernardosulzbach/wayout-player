#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "Mask.hpp"
#include "Pixel.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
/**
 * An Image object represents a in-memory 8-bit per channel RGB image.
 */
class Image {
  std::vector<std::vector<Pixel>> data;

public:
  Image(U32 imageHeight, U32 imageWidth);

  [[nodiscard]] U32 getHeight() const;

  [[nodiscard]] U32 getWidth() const;

  [[nodiscard]] Pixel getPixel(U32 i, U32 j) const;

  void setPixel(U32 i, U32 j, Pixel pixel);

  [[nodiscard]] Mask findPixels(const std::function<bool(Pixel)> &predicate) const;

  bool operator==(const Image &rhs) const;
  bool operator!=(const Image &rhs) const;

  void writeImageToFile(const std::filesystem::path &path) const;
};

Image readImageFromFile(const std::filesystem::path &path);
} // namespace WayoutPlayer
