#include <boost/test/unit_test.hpp>

#include "../src/MaskComponentFinder.hpp"

namespace WayoutPlayer::Tests {
void shouldProperlyDissolveComponents() {
  constexpr U32 Side = 7;
  static_assert(Side % 2 == 1);
  Mask mask(Side, Side);
  for (U32 i = 0; i < Side; i++) {
    for (U32 j = 0; j < Side; j++) {
      const auto di = absoluteDifference(i, Side / 2U);
      const auto dj = absoluteDifference(j, Side / 2U);
      const auto squareSide = std::max(di, dj) + 1U;
      mask.setValue(IntegralScreenCoordinates(i, j), squareSide % 2 == 0);
    }
  }
  MaskComponentFinder maskComponentFinder(mask);
  const auto oldBorderId = maskComponentFinder.getComponentId(IntegralScreenCoordinates(1U, 1U));
  const auto oldCenterId = maskComponentFinder.getComponentId(IntegralScreenCoordinates(Side / 2U, Side / 2U));
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(oldBorderId), 5 * 5 - 3 * 3);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(oldCenterId), 1);
  BOOST_CHECK_NO_THROW(maskComponentFinder.dissolveComponent(oldCenterId));
  unsigned int newCenterId = maskComponentFinder.getComponentId(IntegralScreenCoordinates(Side / 2U, Side / 2U));
  BOOST_CHECK_NE(newCenterId, oldCenterId);
  BOOST_CHECK_EQUAL(newCenterId, oldBorderId);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(oldBorderId), 5 * 5);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(oldCenterId), 0);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(newCenterId), 5 * 5);
}
} // namespace WayoutPlayer::Tests