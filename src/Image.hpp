#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Mask.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
/**
 * An Image object represents a in-memory 8-bit per channel RGB image.
 */
class Image {
  std::vector<std::vector<Color<U8>>> data;

public:
  Image(U32 imageHeight, U32 imageWidth);

  [[nodiscard]] U32 getHeight() const;

  [[nodiscard]] U32 getWidth() const;

  [[nodiscard]] Color<U8> getPixel(U32 i, U32 j) const;

  void setPixel(U32 i, U32 j, Color<U8> color);

  void setCross(U32 centerI, U32 centerJ, U32 diameter, Color<U8> color);

  [[nodiscard]] Mask findPixels(const std::function<bool(Color<U8>)> &predicate) const;

  bool operator==(const Image &rhs) const;
  bool operator!=(const Image &rhs) const;

  void writeImageToFile(const std::filesystem::path &path) const;
};

Image readImageFromFile(const std::filesystem::path &path);
} // namespace WayoutPlayer
