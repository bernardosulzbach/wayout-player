#include "BoardTests.hpp"

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"

namespace WayoutPlayer::Tests {
void isSolvedReturnsTrueOnSolvedBoards() {
  BOOST_CHECK(Board::fromString("D0").isSolved());
}

void isSolvedReturnsFalseOnUnsolvedBoards() {
  BOOST_CHECK(!Board::fromString("D1").isSolved());
  BOOST_CHECK(!Board::fromString("B0").isSolved());
  BOOST_CHECK(!Board::fromString("B1").isSolved());
}

void boardStringConversionsWork() {
  const auto *const boardStringA = "D0 V1 D0\n"
                                   "D1 D1 D1\n"
                                   "D0 D1 D0\n"
                                   "T1 H1 T1";
  BOOST_CHECK(Board::fromString(boardStringA).toString() == boardStringA);
  const auto *const boardStringB = "D0    D0\n"
                                   "D1 D1 D1\n"
                                   "   D1   \n"
                                   "T1 H1 T1";
  BOOST_CHECK(Board::fromString(boardStringB).toString() == boardStringB);
}

void activatingAChainShouldWork() {
  const auto *const boardString = "C1 D0";
  auto board = Board::fromString(boardString);
  board.activate(0, 0);
  BOOST_CHECK(board.toString() == "C0 D1");
}

void boardsShouldBeDifferentIfTheirTilesAreNotBlocked() {
  const auto *const boardAString = "B1";
  const auto *const boardBString = "D1";
  const auto boardA = Board::fromString(boardAString);
  const auto boardB = Board::fromString(boardBString);
  BOOST_CHECK(boardA != boardB);
}

void activatingLoweredNeighborTwinsShouldBehaveAsInTheGame() {
  const auto *const boardString = "D0 D0         \n"
                                  "P1    D0 D1 D0\n"
                                  "D0    P1 P0 P1\n"
                                  "P1    D0 D1 D0\n"
                                  "D1 D0         ";
  auto board = Board::fromString(boardString);
  board.activate(2, 3);
  const auto *const expectedFinalBoardString = "D0 D0         \n"
                                               "P1    D0 D0 D0\n"
                                               "D0    P1 P1 P1\n"
                                               "P1    D0 D0 D0\n"
                                               "D1 D0         ";
  BOOST_CHECK(board == Board::fromString(expectedFinalBoardString));
}

void activatingRaisedNeighborTwinsShouldBehaveAsInTheGame() {
  const auto *const boardString = "D0 D0         \n"
                                  "P1    D0 D1 D0\n"
                                  "D0    P1 P0 P1\n"
                                  "P1    D0 D1 D0\n"
                                  "D1 D0         ";
  auto board = Board::fromString(boardString);
  board.activate(3, 0);
  const auto *const expectedFinalBoardString = "D0 D0         \n"
                                               "P0    D0 D1 D0\n"
                                               "D1    P0 P0 P0\n"
                                               "P0    D0 D1 D0\n"
                                               "D0 D0         ";
  BOOST_CHECK(board == Board::fromString(expectedFinalBoardString));
}

void solvingABoardWithNeighboringTwinsShouldBehaveAsInTheGame() {
  const auto *const boardString = "D0 D0         \n"
                                  "P1    D0 D1 D0\n"
                                  "D0    P1 P0 P1\n"
                                  "P1    D0 D1 D0\n"
                                  "D1 D0         ";
  auto board = Board::fromString(boardString);
  board.activate(3, 0);
  board.activate(2, 3);
  board.activate(2, 0);
  const auto *const expectedFinalBoardString = "D0 D0         \n"
                                               "P0    D0 D0 D0\n"
                                               "D0    P0 P0 P0\n"
                                               "P0    D0 D0 D0\n"
                                               "D0 D0         ";
  BOOST_CHECK(board == Board::fromString(expectedFinalBoardString));
}

void shouldUnderstandThatBlockedTilesMayNeedMultipleClicks() {
  const auto *const boardString = "B1 D0\n"
                                  "D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.mayNeedMultipleClicks());
}

void boardWithBlockedTilesShouldBehaveAsInTheGame() {
  const auto *const boardString = "B1 D0\n"
                                  "D0 B1";
  auto board = Board::fromString(boardString);
  board.activate(0, 1);
  BOOST_CHECK(board.toString() == "D1 D1\nD0 D1");
  board.activate(0, 1);
  BOOST_CHECK(board.toString() == "D0 D0\nD0 D0");
}
} // namespace WayoutPlayer::Tests