#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"

using namespace WayOutPlayer;

BOOST_AUTO_TEST_CASE(boardConversionsTest) {
  const auto boardString = "..###\n.....\n#....\n####.\n.#...";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(boardSolutionTest) {
  const auto boardString = ".....\n..#..\n.###.\n..#..\n.....";
  const auto boardSolution = Board::fromString(boardString).findOptimalSolution();
  const auto center = Position{2, 2};
  BOOST_CHECK(boardSolution == std::vector<Position>{center});
}
