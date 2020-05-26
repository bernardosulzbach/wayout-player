#include "Image.hpp"

namespace WayoutPlayer {
Image::Image(U32 imageWidth, U32 imageHeight) : data(imageWidth, std::vector<Pixel>(imageHeight)) {
}

void Image::saveImageToFile(const std::filesystem::path &path) {
}

Image loadImageFromFile(const std::string &filename) {
  return Image(0, 0);
}
} // namespace WayoutPlayer