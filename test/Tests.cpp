#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"

using namespace WayoutPlayer;

BOOST_AUTO_TEST_CASE(boardConversionsTest) {
  const auto boardString = "D0 V1 D0\nD1 D1 D1\nD0 D1 D0\nT1 H1 T1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(boardConversionsTestWithGaps) {
  const auto boardString = "D0    D0\nD1 D1 D1\n   D1   \nT1 H1 T1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(boardSolutionTest) {
  const auto boardString = "D0 D1 D0\nD1 D1 D1\nD0 D1 D0";
  const auto boardSolution = Board::fromString(boardString).findSolution();
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

BOOST_AUTO_TEST_CASE(boardSolutionTestWithTaps) {
  const auto boardString = "D0 D1 D0\nD1 T1 D1\nD0 D1 D0";
  const auto boardSolution = Board::fromString(boardString).findSolution();
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

BOOST_AUTO_TEST_CASE(boardsShouldBeDifferentIfTheirTilesAreNotBlocked) {
  const auto boardAString = "B1";
  const auto boardBString = "D1";
  const auto boardA = Board::fromString(boardAString);
  const auto boardB = Board::fromString(boardBString);
  BOOST_CHECK(boardA != boardB);
}

BOOST_AUTO_TEST_CASE(largeDefaultBoardShouldBeSolved) {
  const auto boardString = "D1 D0 D1 D0 D1\nD0 D1 D1 D0 D0\nD1 D0 D1 D1 D1\nD1 D0 D0 D0 D1\nD0 D1 D0 D0 D0";
  const auto boardSolution = Board::fromString(boardString).findSolution();
  BOOST_CHECK(boardSolution.getClicks().size() == 8);
}

BOOST_AUTO_TEST_CASE(boardWithTwinsShouldBeSolved) {
  const auto boardString = "D0 D1 D0 D0 D0\nD0 P1 D1 D0 D0\nD0 D0 D0 D1 D0\nD1 D1 D0 D1 P0\nP0 D0 D1 D0 D0";
  const auto boardSolution = Board::fromString(boardString).findSolution();
  BOOST_CHECK(boardSolution.getClicks().size() == 6);
}
