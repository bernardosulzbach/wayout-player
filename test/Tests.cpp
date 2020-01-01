#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"

using namespace WayoutPlayer;

BOOST_AUTO_TEST_CASE(boardConversionsTest) {
  const auto boardString = "N0 V1 N0\nN1 N1 N1\nN0 N1 N0\nT1 H1 T1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(boardConversionsTestWithGaps) {
  const auto boardString = "N0    N0\nN1 N1 N1\n   N1   \nT1 H1 T1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(boardSolutionTest) {
  const auto boardString = "N0 N1 N0\nN1 N1 N1\nN0 N1 N0";
  const auto boardSolution = Board::fromString(boardString).findOptimalSolution();
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

BOOST_AUTO_TEST_CASE(boardSolutionTestWithTaps) {
  const auto boardString = "N0 N1 N0\nN1 T1 N1\nN0 N1 N0";
  const auto boardSolution = Board::fromString(boardString).findOptimalSolution();
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}
