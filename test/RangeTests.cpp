#include <boost/test/unit_test.hpp>

#include "../src/Range.hpp"

using namespace WayoutPlayer;
namespace WayoutPlayer {
namespace Tests {
void correctlyUpdatesRanges() {
  auto range = Range<S32>{};
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 0);
  range = range.include(1);
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 1);
  range = range.include(-1);
  BOOST_CHECK_EQUAL(range.getLow(), -1);
  BOOST_CHECK_EQUAL(range.getHigh(), 1);
}

void doesNotMutateRanges() {
  auto range = Range<S32>{};
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 0);
  static_cast<void>(range.include(1));
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 0);
  static_cast<void>(range.include(-1));
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 0);
}
} // namespace Tests
} // namespace WayoutPlayer
