#include "ImageTests.hpp"

#include <boost/test/unit_test.hpp>

#include <filesystem>

#include "../src/Image.hpp"

namespace WayoutPlayer::Tests {
void readingAndWritingImagesToFilesShouldWork() {
  auto temporaryDirectoryPath = std::filesystem::temp_directory_path();
  Image image(128, 128);
  Color<U8> pixel;
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      pixel.getR() = i;
      pixel.getG() = j;
      pixel.getB() = i + j;
      image.setPixel(IntegralScreenCoordinates(i, j), pixel);
    }
  }
  const auto sampleImagePath = temporaryDirectoryPath / "sample-image.png";
  image.writeImageToFile(sampleImagePath);
  const auto readImage = readImageFromFile(sampleImagePath);
  BOOST_CHECK(image == readImage);
}

void shouldCorrectlyFindPixelValuesInImages() {
  Image image(4, 4);
  const auto diagonalPixel = Color<U8>(0, 1, 2);
  const auto nonDiagonalPixel = Color<U8>(2, 1, 0);
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      if (i == j) {
        image.setPixel(IntegralScreenCoordinates(i, j), diagonalPixel);
      } else {
        image.setPixel(IntegralScreenCoordinates(i, j), nonDiagonalPixel);
      }
    }
  }
  const auto mask = image.findPixels([diagonalPixel](const Color<U8> pixel) { return pixel == diagonalPixel; });
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      BOOST_CHECK((i == j) == mask.getValue(IntegralScreenCoordinates(i, j)));
    }
  }
}
} // namespace WayoutPlayer::Tests
