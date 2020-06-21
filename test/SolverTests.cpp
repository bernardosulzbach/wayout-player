#include "SolverTests.hpp"

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"
#include "../src/Solver.hpp"

namespace WayoutPlayer::Tests {
void solverSolvesSmallDefaultBoards() {
  const auto *const boardString = "D0 D1 D0\n"
                                  "D1 D1 D1\n"
                                  "D0 D1 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

void solverSolvesLargeDefaultBoards() {
  const auto *const boardString = "D1 D0 D1 D0 D1\n"
                                  "D0 D1 D1 D0 D0\n"
                                  "D1 D0 D1 D1 D1\n"
                                  "D1 D0 D0 D0 D1\n"
                                  "D0 D1 D0 D0 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  BOOST_CHECK(boardSolution.getClicks().size() == 8);
}

void solverSolvesBoardsWithBlockedTiles() {
  const auto *const boardString = "B1 D0\n"
                                  "D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(Solver().findSolution(board).getClicks().size() == 2);
}

void solverSolvesBoardsWithTaps() {
  const auto *const boardString = "D0 D1 D0\n"
                                  "D1 T1 D1\n"
                                  "D0 D1 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

void solverSolvesBoardsWithTwins() {
  const auto *const boardString = "D0 D1 D0 D0 D0\n"
                                  "D0 P1 D1 D0 D0\n"
                                  "D0 D0 D0 D1 D0\n"
                                  "D1 D1 D0 D1 P0\n"
                                  "P0 D0 D1 D0 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  BOOST_CHECK(boardSolution.getClicks().size() == 6);
}
} // namespace WayoutPlayer::Tests