#include "BoardScanner.hpp"

namespace WayoutPlayer {
BoardScanner::BoardScanner() = default;

Board BoardScanner::scan(const Image &image) {
  // Find the black edges.
  const auto mask = image.findPixels([](const Pixel pixel) { return pixel.getLightness() <= 30.0; });
  auto imageCopy = image;
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      if (mask.getValue(i, j)) {
        imageCopy.setPixel(i, j, Pixel(255, 255, 255));
      } else {
        imageCopy.setPixel(i, j, Pixel(0, 0, 0));
      }
    }
  }
  return Board(std::vector<std::vector<std::optional<Tile>>>{});
}
} // namespace WayoutPlayer