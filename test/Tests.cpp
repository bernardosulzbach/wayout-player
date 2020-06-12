#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK

#include <filesystem>

#include <boost/test/unit_test.hpp>

#include "../src/Board.hpp"
#include "../src/BoardScanner.hpp"
#include "../src/Hashing.hpp"
#include "../src/Image.hpp"
#include "../src/Mask.hpp"
#include "../src/MaskComponentFinder.hpp"
#include "../src/Range.hpp"
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
  const auto *const connectedBoardString = "D0 D0\n"
                                           "   D0";
  BOOST_CHECK(Board::fromString(connectedBoardString).splitComponents().size() == 1);
  const auto *const disconnectedBoardString = "D0   \n"
                                              "   D0";
  BOOST_CHECK(Board::fromString(disconnectedBoardString).splitComponents().size() == 2);
}

BOOST_AUTO_TEST_CASE(boardSplittingShouldWorkWithTwins) {
  const auto *const boardString = "P0   \n"
                                  "   P0";
  BOOST_CHECK(Board::fromString(boardString).splitComponents().size() == 1);
}

BOOST_AUTO_TEST_CASE(boardConversionsTest) {
  const auto *const boardString = "D0 V1 D0\n"
                                  "D1 D1 D1\n"
                                  "D0 D1 D0\n"
                                  "T1 H1 T1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(boardConversionsTestWithGaps) {
  const auto *const boardString = "D0    D0\n"
                                  "D1 D1 D1\n"
                                  "   D1   \n"
                                  "T1 H1 T1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.toString() == boardString);
}

BOOST_AUTO_TEST_CASE(activatingAChainTest) {
  const auto *const boardString = "C1 D0";
  auto board = Board::fromString(boardString);
  board.activate(0, 0);
  BOOST_CHECK(board.toString() == "C0 D1");
}

BOOST_AUTO_TEST_CASE(boardSolutionTest) {
  const auto *const boardString = "D0 D1 D0\n"
                                  "D1 D1 D1\n"
                                  "D0 D1 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

BOOST_AUTO_TEST_CASE(boardSolutionTestWithTaps) {
  const auto *const boardString = "D0 D1 D0\n"
                                  "D1 T1 D1\n"
                                  "D0 D1 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  const auto expectedSolution = Solution({Position{1, 1}}, true);
  BOOST_CHECK(boardSolution == expectedSolution);
}

BOOST_AUTO_TEST_CASE(boardsShouldBeDifferentIfTheirTilesAreNotBlocked) {
  const auto *const boardAString = "B1";
  const auto *const boardBString = "D1";
  const auto boardA = Board::fromString(boardAString);
  const auto boardB = Board::fromString(boardBString);
  BOOST_CHECK(boardA != boardB);
}

BOOST_AUTO_TEST_CASE(largeDefaultBoardShouldBeSolved) {
  const auto *const boardString = "D1 D0 D1 D0 D1\n"
                                  "D0 D1 D1 D0 D0\n"
                                  "D1 D0 D1 D1 D1\n"
                                  "D1 D0 D0 D0 D1\n"
                                  "D0 D1 D0 D0 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  BOOST_CHECK(boardSolution.getClicks().size() == 8);
}

BOOST_AUTO_TEST_CASE(boardWithTwinsShouldBeSolved) {
  const auto *const boardString = "D0 D1 D0 D0 D0\n"
                                  "D0 P1 D1 D0 D0\n"
                                  "D0 D0 D0 D1 D0\n"
                                  "D1 D1 D0 D1 P0\n"
                                  "P0 D0 D1 D0 D0";
  const auto solver = Solver();
  const auto boardSolution = solver.findSolution(Board::fromString(boardString));
  BOOST_CHECK(boardSolution.getClicks().size() == 6);
}

BOOST_AUTO_TEST_CASE(activatingDownNeighborTwinsShouldBehaveAsInTheGame) {
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

BOOST_AUTO_TEST_CASE(activatingUpNeighborTwinsShouldBehaveAsInTheGame) {
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

BOOST_AUTO_TEST_CASE(solvingABoardWithNeighboringTwinsShouldBehaveAsInTheGame) {
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

BOOST_AUTO_TEST_CASE(positionsShouldBecomeSensibleStrings) {
  BOOST_CHECK(Position(-1, -1).toString() == "(-1, -1)");
  BOOST_CHECK(Position(0, 0).toString() == "(0, 0)");
  BOOST_CHECK(Position(127, 127).toString() == "(127, 127)");
}

BOOST_AUTO_TEST_CASE(splittingComponentsShouldJudgeTheNeedForMultipleClicks) {
  const auto *const boardString = "D0    B1 D0\n"
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
  const auto *const boardString = "B1 D0\n"
                                  "D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(board.mayNeedMultipleClicks());
}

BOOST_AUTO_TEST_CASE(boardWithBlockedTilesShouldBehaveAsInTheGame) {
  const auto *const boardString = "B1 D0\n"
                                  "D0 B1";
  auto board = Board::fromString(boardString);
  board.activate(0, 1);
  BOOST_CHECK(board.toString() == "D1 D1\nD0 D1");
  board.activate(0, 1);
  BOOST_CHECK(board.toString() == "D0 D0\nD0 D0");
}

BOOST_AUTO_TEST_CASE(shouldSolveSmallBoardWithBlockedTiles) {
  const auto *const boardString = "B1 D0\n"
                                  "D0 B1";
  const auto board = Board::fromString(boardString);
  BOOST_CHECK(Solver().findSolution(board).getClicks().size() == 2);
}

BOOST_AUTO_TEST_CASE(shouldFindComponentsInASmallBoardWithBlockedTiles) {
  const auto *const boardString = "B1 D0\n"
                                  "D0 B1";
  const auto board = Board::fromString(boardString);
  const auto components = board.splitComponents();
  BOOST_CHECK(components.size() == 1);
}

BOOST_AUTO_TEST_CASE(splittingComponentsShouldWork) {
  const auto *const boardString = "D0   \n"
                                  "   D1";
  const auto board = Board::fromString(boardString);
  const auto components = board.splitComponents();
  BOOST_CHECK(components.size() == 2);
  BOOST_CHECK(components[0] != board);
  BOOST_CHECK(components[1] != board);
  BOOST_CHECK(Board::mergeComponents(components) == board);
}

BOOST_AUTO_TEST_CASE(splittingComponentsShouldRespectTwins) {
  const auto *const boardString = "D0 D0         \n"
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

BOOST_AUTO_TEST_CASE(readingAndWritingImageToFiles) {
  auto temporaryDirectoryPath = std::filesystem::temp_directory_path();
  Image image(128, 128);
  Color<U8> pixel;
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      pixel.getR() = i;
      pixel.getG() = j;
      pixel.getB() = i + j;
      image.setPixel(i, j, pixel);
    }
  }
  const auto sampleImagePath = temporaryDirectoryPath / "sample-image.png";
  image.writeImageToFile(sampleImagePath);
  const auto readImage = readImageFromFile(sampleImagePath);
  BOOST_CHECK(image == readImage);
}

BOOST_AUTO_TEST_CASE(shouldCorrectlyFindPixelValuesInImages) {
  Image image(4, 4);
  const auto diagonalPixel = Color<U8>(0, 1, 2);
  const auto nonDiagonalPixel = Color<U8>(2, 1, 0);
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      if (i == j) {
        image.setPixel(i, j, diagonalPixel);
      } else {
        image.setPixel(i, j, nonDiagonalPixel);
      }
    }
  }
  const auto mask = image.findPixels([diagonalPixel](const Color<U8> pixel) { return pixel == diagonalPixel; });
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      BOOST_CHECK((i == j) == mask.getValue(i, j));
    }
  }
}

BOOST_AUTO_TEST_CASE(shouldCorrectlyEvaluateLightness) {
  const auto threshold = 1.0;
  BOOST_CHECK_CLOSE(Color<U8>(0, 0, 0).getLightness(), 0.0, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(31, 63, 127).getLightness(), 27.79, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(63, 63, 63).getLightness(), 26.65, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(63, 127, 191).getLightness(), 51.83, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(127, 127, 127).getLightness(), 53.19, threshold);
  BOOST_CHECK_CLOSE(Color<U8>(255, 255, 255).getLightness(), 100.0, threshold);
}

BOOST_AUTO_TEST_CASE(shouldCorrectlyUpdateRanges) {
  auto range = Range<S32>{};
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 0);
  range = range.include(1);
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 1);
  range = range.include(-1);
  BOOST_CHECK_EQUAL(range.getLow(), -1);
  BOOST_CHECK_EQUAL(range.getHigh(), 1);
}

BOOST_AUTO_TEST_CASE(shouldNotMutateRanges) {
  auto range = Range<S32>{};
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 0);
  static_cast<void>(range.include(1));
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 0);
  static_cast<void>(range.include(-1));
  BOOST_CHECK_EQUAL(range.getLow(), 0);
  BOOST_CHECK_EQUAL(range.getHigh(), 0);
}

BOOST_AUTO_TEST_CASE(shouldProperlyDissolveComponents) {
  constexpr U32 Side = 7;
  static_assert(Side % 2 == 1);
  Mask mask(Side, Side);
  for (U32 i = 0; i < Side; i++) {
    for (U32 j = 0; j < Side; j++) {
      const auto di = absoluteDifference(i, Side / 2U);
      const auto dj = absoluteDifference(j, Side / 2U);
      const auto squareSide = std::max(di, dj) + 1U;
      mask.setValue(i, j, squareSide % 2 == 0);
    }
  }
  MaskComponentFinder maskComponentFinder(mask);
  const auto oldBorderId = maskComponentFinder.getComponentId(1U, 1U);
  const auto oldCenterId = maskComponentFinder.getComponentId(Side / 2U, Side / 2U);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(oldBorderId), 5 * 5 - 3 * 3);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(oldCenterId), 1);
  BOOST_CHECK_NO_THROW(maskComponentFinder.dissolveComponent(oldCenterId));
  unsigned int newCenterId = maskComponentFinder.getComponentId(Side / 2U, Side / 2U);
  BOOST_CHECK_NE(newCenterId, oldCenterId);
  BOOST_CHECK_EQUAL(newCenterId, oldBorderId);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(oldBorderId), 5 * 5);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(oldCenterId), 0);
  BOOST_CHECK_EQUAL(maskComponentFinder.getComponentSize(newCenterId), 5 * 5);
}
