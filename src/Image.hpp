#pragma once

#include <filesystem>
#include <string>
#include <vector>

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

  [[nodiscard]] U32 getWidth() const;

  [[nodiscard]] U32 getHeight() const;

  [[nodiscard]] Pixel getPixel(U32 i, U32 j) const;

  void setPixel(U32 i, U32 j, Pixel pixel);

  void writeImageToFile(const std::filesystem::path &path) const;

  bool operator==(const Image &rhs) const;
  bool operator!=(const Image &rhs) const;
};

Image readImageFromFile(const std::filesystem::path &path);
} // namespace WayoutPlayer
