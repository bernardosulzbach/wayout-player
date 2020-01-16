#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"
#include "../src/Hashing.hpp"
#include "../src/Solver.hpp"

using namespace WayoutPlayer;

BOOST_AUTO_TEST_CASE(hashingTheEmptyStringTest) {
  BOOST_CHECK(0xcf83e1357eefb8bdUL == hashString(""));
}

BOOST_AUTO_TEST_CASE(hashingDigitsTest) {
  BOOST_CHECK(0xbb96c2fc40d2d546UL == hashString("0123456789"));
}

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
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

BOOST_AUTO_TEST_CASE(boardSolutionTestWithTaps) {
  const auto boardString = "D0 D1 D0\nD1 T1 D1\nD0 D1 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
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
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  BOOST_CHECK(boardSolution.getClicks().size() == 8);
}

BOOST_AUTO_TEST_CASE(boardWithTwinsShouldBeSolved) {
  const auto boardString = "D0 D1 D0 D0 D0\n"
                           "D0 P1 D1 D0 D0\n"
                           "D0 D0 D0 D1 D0\n"
                           "D1 D1 D0 D1 P0\n"
                           "P0 D0 D1 D0 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  BOOST_CHECK(boardSolution.getClicks().size() == 6);
}

BOOST_AUTO_TEST_CASE(activatingDownNeighborTwinsShouldBehaveAsInTheGame) {
  const auto boardString = "D0 D0         \n"
                           "P1    D0 D1 D0\n"
                           "D0    P1 P0 P1\n"
                           "P1    D0 D1 D0\n"
                           "D1 D0         ";
  auto board = Board::fromString(boardString);
  board.activate(2, 3);
  const auto expectedFinalBoardString = "D0 D0         \n"
                                        "P1    D0 D0 D0\n"
                                        "D0    P1 P1 P1\n"
                                        "P1    D0 D0 D0\n"
                                        "D1 D0         ";
  BOOST_CHECK(board == Board::fromString(expectedFinalBoardString));
}

BOOST_AUTO_TEST_CASE(activatingUpNeighborTwinsShouldBehaveAsInTheGame) {
  const auto boardString = "D0 D0         \n"
                           "P1    D0 D1 D0\n"
                           "D0    P1 P0 P1\n"
                           "P1    D0 D1 D0\n"
                           "D1 D0         ";
  auto board = Board::fromString(boardString);
  board.activate(3, 0);
  const auto expectedFinalBoardString = "D0 D0         \n"
                                        "P0    D0 D1 D0\n"
                                        "D1    P0 P0 P0\n"
                                        "P0    D0 D1 D0\n"
                                        "D0 D0         ";
  BOOST_CHECK(board == Board::fromString(expectedFinalBoardString));
}

BOOST_AUTO_TEST_CASE(solvingABoardWithNeighboringTwinsShouldBehaveAsInTheGame) {
  const auto boardString = "D0 D0         \n"
                           "P1    D0 D1 D0\n"
                           "D0    P1 P0 P1\n"
                           "P1    D0 D1 D0\n"
                           "D1 D0         ";
  auto board = Board::fromString(boardString);
  board.activate(3, 0);
  board.activate(2, 3);
  board.activate(2, 0);
  const auto expectedFinalBoardString = "D0 D0         \n"
                                        "P0    D0 D0 D0\n"
                                        "D0    P0 P0 P0\n"
                                        "P0    D0 D0 D0\n"
                                        "D0 D0         ";
  BOOST_CHECK(board == Board::fromString(expectedFinalBoardString));
}
