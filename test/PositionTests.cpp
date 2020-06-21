#include "PositionTests.hpp"

#include <boost/test/unit_test.hpp>

#include "../src/Position.hpp"

namespace WayoutPlayer::Tests {
void positionsShouldBecomeSensibleStrings() {
  BOOST_CHECK(Position(-1, -1).toString() == "(-1, -1)");
  BOOST_CHECK(Position(0, 0).toString() == "(0, 0)");
  BOOST_CHECK(Position(127, 127).toString() == "(127, 127)");
}
} // namespace WayoutPlayer::Tests