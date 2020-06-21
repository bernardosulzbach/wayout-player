#include "HashingTests.hpp"

#include <boost/test/unit_test.hpp>

#include "../src/Hashing.hpp"

namespace WayoutPlayer {
namespace Tests {
void hashingTheEmptyStringProducesTheExpectedResult() {
  BOOST_CHECK(0xcf83e1357eefb8bdUL == hashString(""));
}

void hashingTenDigitsProducesTheExpectedResult() {
  BOOST_CHECK(0xbb96c2fc40d2d546UL == hashString("0123456789"));
}
} // namespace Tests
} // namespace WayoutPlayer