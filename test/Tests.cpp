#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"

BOOST_AUTO_TEST_CASE(boardConversionsTest) {
  const auto boardString = "..###\n.....\n#....\n####.\n.#...";
  const auto board = WayoutPlayer::Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}
