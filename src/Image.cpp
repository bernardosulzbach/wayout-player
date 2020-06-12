#include "Image.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace WayoutPlayer {
Image::Image(U32 imageHeight, U32 imageWidth) : data(imageHeight, std::vector<Color<U8>>(imageWidth)) {
}

U32 Image::getHeight() const {
  return data.size();
}

U32 Image::getWidth() const {
  if (data.empty()) {
    return 0;
  }
  return data.front().size();
}

Color<U8> Image::getPixel(const U32 i, const U32 j) const {
  return data[i][j];
}

void Image::setPixel(const U32 i, const U32 j, const Color<U8> color) {
  data[i][j] = color;
}

void Image::setCross(const U32 centerI, const U32 centerJ, const U32 diameter, const Color<U8> color) {
  if (diameter == 0) {
    throw std::invalid_argument("Diameter must be nonzero.");
  }
  if (diameter % 2 == 0) {
    throw std::invalid_argument("Diameter must be odd.");
  }
  const auto setPixelIfInImage = [this, color](const S64 i, const S64 j) {
    if (i < 0 || i >= getHeight() || j < 0 || j >= getWidth()) {
      return;
    }
    setPixel(i, j, color);
  };
  setPixelIfInImage(centerI, centerJ);
  for (S64 distance = 1; distance <= (diameter - 1) / 2; distance++) {
    setPixelIfInImage(centerI - distance, centerJ);
    setPixelIfInImage(centerI, centerJ - distance);
    setPixelIfInImage(centerI, centerJ + distance);
    setPixelIfInImage(centerI + distance, centerJ);
  }
}

Mask Image::findPixels(const std::function<bool(Color<U8>)> &predicate) const {
  Mask mask(getHeight(), getWidth());
  for (U32 i = 0; i < getHeight(); i++) {
    for (U32 j = 0; j < getWidth(); j++) {
      if (predicate(getPixel(i, j))) {
        mask.setValue(i, j, true);
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
      cvImage.at<cv::Vec3b>(i, j)[0] = getPixel(i, j).getB();
      cvImage.at<cv::Vec3b>(i, j)[1] = getPixel(i, j).getG();
      cvImage.at<cv::Vec3b>(i, j)[2] = getPixel(i, j).getR();
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
      image.setPixel(i, j, Color(r, g, b));
    }
  }
  return image;
}
} // namespace WayoutPlayer