#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"
#include "../src/BoardComponentSplitter.hpp"

namespace WayoutPlayer::Tests {
void boardSplittingWorksWithDefaultTiles() {
  const auto *const connectedBoardString = "D0 D0\n"
                                           "   D0";
  BOOST_CHECK(splitComponents(Board::fromString(connectedBoardString)).size() == 1);
  const auto *const disconnectedBoardString = "D0   \n"
                                              "   D0";
  const auto disconnectedBoard = Board::fromString(disconnectedBoardString);
  BOOST_CHECK(splitComponents(disconnectedBoard).size() == 2);
  BOOST_CHECK(mergeComponents(splitComponents(disconnectedBoard)) == disconnectedBoard);
}

void boardSplittingWorksWithBlockedTiles() {
  const auto *const boardString = "B1 D0\n"
                                  "D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(splitComponents(board).size() == 1);
}

void boardSplittingWorksWithTwinTiles() {
  const auto *const boardStringA = "P0   \n"
                                   "   P0";
  BOOST_CHECK(splitComponents(Board::fromString(boardStringA)).size() == 1);
  const auto *const boardStringB = "D0 D0         \n"
                                   "P1    D0 D1 D0\n"
                                   "D0    P1 P0 P1\n"
                                   "P1    D0 D1 D0\n"
                                   "D1 D0         ";
  const auto board = Board::fromString(boardStringB);
  const auto components = splitComponents(board);
  BOOST_CHECK(components.size() == 1);
  BOOST_CHECK(components.front() == board);
  BOOST_CHECK(mergeComponents(components) == board);
}

void boardSplittingShouldJudgeTheNeedForMultipleClicks() {
  const auto *const boardString = "D0    B1 D0\n"
                                  "D1    D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(splitComponents(board).size() == 2);
  auto requiringMultipleClicks = 0;
  auto notRequiringMultipleClicks = 0;
  for (const auto &component : splitComponents(board)) {
    if (component.mayNeedMultipleClicks()) {
      requiringMultipleClicks++;
    } else {
      notRequiringMultipleClicks++;
    }
  }
  BOOST_CHECK(requiringMultipleClicks == 1);
  BOOST_CHECK(notRequiringMultipleClicks == 1);
}
} // namespace WayoutPlayer::Tests