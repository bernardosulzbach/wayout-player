#include <boost/test/unit_test.hpp>

#include "../src/TileType.hpp"

namespace WayoutPlayer::Tests {
void tileTypeConversionsToCharacters() {
  for (const auto tileType : TileTypes) {
    BOOST_CHECK(tileType == tileTypeFromCharacter(tileTypeToCharacter(tileType)));
  }
}

void tileTypeConversionsToIntegers() {
  for (const auto tileType : TileTypes) {
    BOOST_CHECK(tileType == tileTypeFromInteger(tileTypeToInteger(tileType)));
  }
}
}; // namespace WayoutPlayer::Tests