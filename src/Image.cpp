#include "Image.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace WayoutPlayer {
void Image::setPixelIfInImage(const IntegralScreenCoordinates coordinates, const Color<U8> color) {
  const auto i = coordinates.getI();
  const auto j = coordinates.getJ();
  if (i < 0 || i >= getHeight() || j < 0 || j >= getWidth()) {
    return;
  }
  setPixel(coordinates, color);
}

Image::Image(const U32 imageHeight, const U32 imageWidth) : data(imageHeight, std::vector<Color<U8>>(imageWidth)) {
}

U32 Image::getHeight() const noexcept {
  return data.size();
}

U32 Image::getWidth() const noexcept {
  if (data.empty()) {
    return 0;
  }
  return data.front().size();
}

Color<U8> Image::getPixel(const IntegralScreenCoordinates coordinates) const {
  return data[coordinates.getI()][coordinates.getJ()];
}

void Image::setPixel(const IntegralScreenCoordinates coordinates, const Color<U8> color) {
  data[coordinates.getI()][coordinates.getJ()] = color;
}

void Image::setCross(const IntegralScreenCoordinates center, const U32 diameter, const Color<U8> color) {
  if (diameter == 0) {
    throw std::invalid_argument("Diameter must be nonzero.");
  }
  if (diameter % 2 == 0) {
    throw std::invalid_argument("Diameter must be odd.");
  }
  const auto centerI = center.getI();
  const auto centerJ = center.getJ();
  setPixelIfInImage(IntegralScreenCoordinates(centerI, centerJ), color);
  for (S64 distance = 1; distance <= (diameter - 1) / 2; distance++) {
    setPixelIfInImage(IntegralScreenCoordinates(centerI - distance, centerJ), color);
    setPixelIfInImage(IntegralScreenCoordinates(centerI, centerJ - distance), color);
    setPixelIfInImage(IntegralScreenCoordinates(centerI, centerJ + distance), color);
    setPixelIfInImage(IntegralScreenCoordinates(centerI + distance, centerJ), color);
  }
}

Mask Image::findPixels(const std::function<bool(Color<U8>)> &predicate) const {
  Mask mask(getHeight(), getWidth());
  for (U32 i = 0; i < getHeight(); i++) {
    for (U32 j = 0; j < getWidth(); j++) {
      const auto coordinates = IntegralScreenCoordinates(i, j);
      if (predicate(getPixel(coordinates))) {
        mask.setValue(coordinates, true);
      }
    }
  }
  return mask;
}

bool Image::operator==(const Image &rhs) const {
  return data == rhs.data;
}

bool Image::operator!=(const Image &rhs) const {
  return !(rhs == *this);
}

void Image::writeImageToFile(const std::filesystem::path &path) const {
  cv::Mat cvImage(getHeight(), getWidth(), cv::traits::Type<cv::Vec3b>::value);
  for (U32 i = 0; i < getHeight(); i++) {
    for (U32 j = 0; j < getWidth(); j++) {
      const auto coordinates = IntegralScreenCoordinates(i, j);
      cvImage.at<cv::Vec3b>(i, j)[0] = getPixel(coordinates).getB();
      cvImage.at<cv::Vec3b>(i, j)[1] = getPixel(coordinates).getG();
      cvImage.at<cv::Vec3b>(i, j)[2] = getPixel(coordinates).getR();
    }
  }
  cv::imwrite(path, cvImage);
}

Image readImageFromFile(const std::filesystem::path &path) {
  cv::Mat cvImage = cv::imread(path, cv::ImreadModes::IMREAD_COLOR);
  if (cvImage.data == nullptr) {
    throw std::runtime_error("Could not open or find the image " + std::string(path.c_str()) + ".");
  }
  if (cvImage.type() != cv::traits::Type<cv::Vec3b>::value) {
    throw std::runtime_error("Matrix type is not 3-channel 8-bit unsigned.");
  }
  if (cvImage.channels() != 3) {
    throw std::runtime_error("Expected to find 3 channels, found " + std::to_string(cvImage.channels()) + ".");
  }
  Image image(cvImage.size[0], cvImage.size[1]);
  for (int i = 0; i < cvImage.size[0]; i++) {
    for (int j = 0; j < cvImage.size[1]; j++) {
      const auto cvPixel = cvImage.at<cv::Vec3b>(i, j);
      const auto b = cvPixel[0];
      const auto g = cvPixel[1];
      const auto r = cvPixel[2];
      image.setPixel(IntegralScreenCoordinates(i, j), Color(r, g, b));
    }
  }
  return image;
}
} // namespace WayoutPlayer