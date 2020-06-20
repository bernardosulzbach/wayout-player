#include "ColorTests.hpp"

#include <boost/test/unit_test.hpp>

#include "../src/Color.hpp"

namespace WayoutPlayer::Tests {
void shouldBeAbleToDeriveColorsFromHsv() {
  const auto u8Color = Color<U8>::fromHSV(195.0F, 0.9F, 0.4F);
  BOOST_CHECK_EQUAL(u8Color.getR(), 10U);
  BOOST_CHECK_EQUAL(u8Color.getG(), 79U);
  BOOST_CHECK_EQUAL(u8Color.getB(), 102U);
  const auto threshold = 0.01;
  const auto f32Color = Color<F32>::fromHSV(195.0F, 0.9F, 0.4F);
  BOOST_CHECK(std::fabs(f32Color.getR() - 10.0F / 255.0F) < threshold);
  BOOST_CHECK(std::fabs(f32Color.getG() - 79.0F / 255.0F) < threshold);
  BOOST_CHECK(std::fabs(f32Color.getB() - 102.0F / 255.0F) < threshold);
}

void shouldCorrectlyEvaluateLightness() {
  const auto threshold = 1.0;
  BOOST_CHECK_CLOSE(Color<U8>(0, 0, 0).getLightness(), 0.0, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(31, 63, 127).getLightness(), 27.79, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(63, 63, 63).getLightness(), 26.65, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(63, 127, 191).getLightness(), 51.83, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(127, 127, 127).getLightness(), 53.19, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(255, 255, 255).getLightness(), 100.0, threshold);
}
} // namespace WayoutPlayer::Tests
