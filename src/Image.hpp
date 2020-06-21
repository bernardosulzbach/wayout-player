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

  void setPixelIfInImage(IntegralScreenCoordinates coordinates, Color<U8> color);
  ;

public:
  Image(U32 imageHeight, U32 imageWidth);

  [[nodiscard]] U32 getHeight() const noexcept;

  [[nodiscard]] U32 getWidth() const noexcept;

  [[nodiscard]] Color<U8> getPixel(IntegralScreenCoordinates coordinates) const;

  void setPixel(IntegralScreenCoordinates coordinates, Color<U8> color);

  void setCross(IntegralScreenCoordinates center, U32 diameter, Color<U8> color);

  [[nodiscard]] Mask findPixels(const std::function<bool(Color<U8>)> &predicate) const;

  bool operator==(const Image &rhs) const;
  bool operator!=(const Image &rhs) const;

  void writeImageToFile(const std::filesystem::path &path) const;
};

Image readImageFromFile(const std::filesystem::path &path);
} // namespace WayoutPlayer
