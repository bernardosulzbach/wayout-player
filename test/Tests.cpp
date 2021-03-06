#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"
#include "../src/Hashing.hpp"
#include "../src/Solver.hpp"
#include "../src/TileType.hpp"

using namespace WayoutPlayer;

BOOST_AUTO_TEST_CASE(hashingTheEmptyStringTest) {
  BOOST_CHECK(0xcf83e1357eefb8bdUL == hashString(""));
}

BOOST_AUTO_TEST_CASE(hashingDigitsTest) {
  BOOST_CHECK(0xbb96c2fc40d2d546UL == hashString("0123456789"));
}

BOOST_AUTO_TEST_CASE(tileTypeConversionsToCharacters) {
  for (const auto tileType : TileTypes) {
    BOOST_CHECK(tileType == tileTypeFromCharacter(tileTypeToCharacter(tileType)));
  }
}

BOOST_AUTO_TEST_CASE(tileTypeConversionsToIntegers) {
  for (const auto tileType : TileTypes) {
    BOOST_CHECK(tileType == tileTypeFromInteger(tileTypeToInteger(tileType)));
  }
}

BOOST_AUTO_TEST_CASE(boardIsSolvedWorksOnSolvedBoards) {
  BOOST_CHECK(Board::fromString("D0").isSolved());
}

BOOST_AUTO_TEST_CASE(boardIsSolvedWorksOnUnsolvedBoards) {
  BOOST_CHECK(!Board::fromString("D1").isSolved());
  BOOST_CHECK(!Board::fromString("B0").isSolved());
  BOOST_CHECK(!Board::fromString("B1").isSolved());
}

BOOST_AUTO_TEST_CASE(boardSplittingShouldWorkWithDefaultTiles) {
  const auto connectedBoardString = "D0 D0\n"
                                    "   D0";
  BOOST_CHECK(Board::fromString(connectedBoardString).splitComponents().size() == 1);
  const auto disconnectedBoardString = "D0   \n"
                                       "   D0";
  BOOST_CHECK(Board::fromString(disconnectedBoardString).splitComponents().size() == 2);
}

BOOST_AUTO_TEST_CASE(boardSplittingShouldWorkWithTwins) {
  const auto boardString = "P0   \n"
                           "   P0";
  BOOST_CHECK(Board::fromString(boardString).splitComponents().size() == 1);
}

BOOST_AUTO_TEST_CASE(boardConversionsTest) {
  const auto boardString = "D0 V1 D0\n"
                           "D1 D1 D1\n"
                           "D0 D1 D0\n"
                           "T1 H1 T1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(boardConversionsTestWithGaps) {
  const auto boardString = "D0    D0\n"
                           "D1 D1 D1\n"
                           "   D1   \n"
                           "T1 H1 T1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(activatingAChainTest) {
  const auto boardString = "C1 D0";
  auto board = Board::fromString(boardString);
  board.activate(0, 0);
  BOOST_CHECK(board.toString() == "C0 D1");
}

BOOST_AUTO_TEST_CASE(boardSolutionTest) {
  const auto boardString = "D0 D1 D0\n"
                           "D1 D1 D1\n"
                           "D0 D1 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

BOOST_AUTO_TEST_CASE(boardSolutionTestWithTaps) {
  const auto boardString = "D0 D1 D0\n"
                           "D1 T1 D1\n"
                           "D0 D1 D0";
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
  const auto boardString = "D1 D0 D1 D0 D1\n"
                           "D0 D1 D1 D0 D0\n"
                           "D1 D0 D1 D1 D1\n"
                           "D1 D0 D0 D0 D1\n"
                           "D0 D1 D0 D0 D0";
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

BOOST_AUTO_TEST_CASE(positionsShouldBecomeSensibleStrings) {
  BOOST_CHECK(Position(-1, -1).toString() == "(-1, -1)");
  BOOST_CHECK(Position(0, 0).toString() == "(0, 0)");
  BOOST_CHECK(Position(127, 127).toString() == "(127, 127)");
}

BOOST_AUTO_TEST_CASE(splittingComponentsShouldJudgeTheNeedForMultipleClicks) {
  const auto boardString = "D0    B1 D0\n"
                           "D1    D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.splitComponents().size() == 2);
  auto requiringMultipleClicks = 0;
  auto notRequiringMultipleClicks = 0;
  for (const auto &component : board.splitComponents()) {
    if (component.mayNeedMultipleClicks()) {
      requiringMultipleClicks++;
    } else {
      notRequiringMultipleClicks++;
    }
  }
  BOOST_CHECK(requiringMultipleClicks == 1);
  BOOST_CHECK(notRequiringMultipleClicks == 1);
}

BOOST_AUTO_TEST_CASE(shouldUnderstandThatABlockedTilesMayNeedMultipleClicks) {
  const auto boardString = "B1 D0\n"
                           "D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.mayNeedMultipleClicks());
}

BOOST_AUTO_TEST_CASE(boardWithBlockedTilesShouldBehaveAsInTheGame) {
  const auto boardString = "B1 D0\n"
                           "D0 B1";
  auto board = Board::fromString(boardString);
  board.activate(0, 1);
  BOOST_CHECK(board.toString() == "D1 D1\nD0 D1");
  board.activate(0, 1);
  BOOST_CHECK(board.toString() == "D0 D0\nD0 D0");
}

BOOST_AUTO_TEST_CASE(shouldSolveSmallBoardWithBlockedTiles) {
  const auto boardString = "B1 D0\n"
                           "D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(Solver().findSolution(board).getClicks().size() == 2);
}

BOOST_AUTO_TEST_CASE(shouldFindComponentsInASmallBoardWithBlockedTiles) {
  const auto boardString = "B1 D0\n"
                           "D0 B1";
  const auto board = Board::fromString(boardString);
  const auto components = board.splitComponents();
  BOOST_CHECK(components.size() == 1);
}

BOOST_AUTO_TEST_CASE(splittingComponentsShouldWork) {
  const auto boardString = "D0   \n"
                           "   D1";
  const auto board = Board::fromString(boardString);
  const auto components = board.splitComponents();
  BOOST_CHECK(components.size() == 2);
  BOOST_CHECK(components[0] != board);
  BOOST_CHECK(components[1] != board);
  BOOST_CHECK(Board::mergeComponents(components) == board);
}

BOOST_AUTO_TEST_CASE(splittingComponentsShouldRespectTwins) {
  const auto boardString = "D0 D0         \n"
                           "P1    D0 D1 D0\n"
                           "D0    P1 P0 P1\n"
                           "P1    D0 D1 D0\n"
                           "D1 D0         ";
  const auto board = Board::fromString(boardString);
  const auto components = board.splitComponents();
  BOOST_CHECK(components.size() == 1);
  BOOST_CHECK(components.front() == board);
  BOOST_CHECK(Board::mergeComponents(components) == board);
}
