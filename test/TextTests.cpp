#include <boost/test/unit_test.hpp>

#include "../src/Text.hpp"

#include <vector>

namespace WayoutPlayer {
namespace Tests {
void joinIntoStringWorks() {
  BOOST_CHECK_EQUAL(joinIntoString(std::vector<int>{}, ", "), "");
  BOOST_CHECK_EQUAL(joinIntoString(std::vector<int>{1}, ", "), "1");
  BOOST_CHECK_EQUAL(joinIntoString(std::vector<int>{1, 2}, ", "), "1, 2");
  BOOST_CHECK_EQUAL(joinIntoString(std::vector<int>{1, 2, 3}, ", "), "1, 2, 3");
}

void integerToStringWithThousandSeparatorsWorks() {
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(1), "1");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(-1), "-1");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(10), "10");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(-10), "-10");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(100), "100");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(-100), "-100");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(1000), "1,000");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(-1000), "-1,000");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(10000), "10,000");
  BOOST_CHECK_EQUAL(integerToStringWithThousandSeparators(-10000), "-10,000");
}
} // namespace Tests
} // namespace WayoutPlayer
