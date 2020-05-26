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
  Image(U32 imageWidth, U32 imageHeight);

  [[nodiscard]] U32 getWidth() const {
    if (data.empty()) {
      return 0;
    }
    return data.front().size();
  }

  [[nodiscard]] U32 getHeight() const {
    return data.size();
  }

  [[nodiscard]] Pixel getPixel(U32 i, U32 j) const {
    return data.at(i).at(j);
  }

  void saveImageToFile(const std::filesystem::path &path);
};

Image loadImageFromFile(const std::filesystem::path &path);
} // namespace WayoutPlayer
