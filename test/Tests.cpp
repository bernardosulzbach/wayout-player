#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "BoardComponentSplitterTests.hpp"
#include "BoardTests.hpp"
#include "ColorTests.hpp"
#include "HashingTests.hpp"
#include "ImageTests.hpp"
#include "MaskComponentFinderTests.hpp"
#include "PositionTests.hpp"
#include "RangeTests.hpp"
#include "SolverTests.hpp"
#include "TextTests.hpp"

using namespace WayoutPlayer::Tests;

bool init_unit_test() {
  auto &suite = boost::unit_test::framework::master_test_suite();
  // Board
  suite.add(BOOST_TEST_CASE_NAME(&isSolvedReturnsTrueOnSolvedBoards, "isSolved() returns true on solved boards"));
  suite.add(BOOST_TEST_CASE_NAME(&isSolvedReturnsFalseOnUnsolvedBoards, "isSolved() returns false on unsolved boards"));
  suite.add(BOOST_TEST_CASE_NAME(&boardStringConversionsWork, "Board string conversions work"));
  suite.add(BOOST_TEST_CASE_NAME(&activatingAChainShouldWork, "Activating a chain should work"));
  suite.add(BOOST_TEST_CASE_NAME(&boardsShouldBeDifferentIfTheirTilesAreNotBlocked,
                                 "Boards should be different if their tiles are not blocked"));
  suite.add(BOOST_TEST_CASE_NAME(&activatingLoweredNeighborTwinsShouldBehaveAsInTheGame,
                                 "Activating lowered neighbor twins should behave as in the game"));
  suite.add(BOOST_TEST_CASE_NAME(&activatingRaisedNeighborTwinsShouldBehaveAsInTheGame,
                                 "Activating raised neighbor twins should behave as in the game"));
  suite.add(BOOST_TEST_CASE_NAME(&solvingABoardWithNeighboringTwinsShouldBehaveAsInTheGame,
                                 "Solving a board with neighboring twins should behave as in the game"));
  suite.add(BOOST_TEST_CASE_NAME(&shouldUnderstandThatBlockedTilesMayNeedMultipleClicks,
                                 "Should understand that blocked tiles may need multiple clicks"));
  suite.add(BOOST_TEST_CASE_NAME(&boardWithBlockedTilesShouldBehaveAsInTheGame,
                                 "Board with blocked tiles should behave as in the game"));
  // Solver
  suite.add(BOOST_TEST_CASE_NAME(&solverSolvesSmallDefaultBoards, "Solver solves small default boards"));
  suite.add(BOOST_TEST_CASE_NAME(&solverSolvesLargeDefaultBoards, "Solver solves large default boards"));
  suite.add(BOOST_TEST_CASE_NAME(&solverSolvesBoardsWithBlockedTiles, "Solver solves boards with blocked tiles"));
  suite.add(BOOST_TEST_CASE_NAME(&solverSolvesBoardsWithTaps, "Solver solves boards with traps"));
  suite.add(BOOST_TEST_CASE_NAME(&solverSolvesBoardsWithTwins, "Boards with twins should be solved"));
  // Board splitter
  suite.add(BOOST_TEST_CASE_NAME(&boardSplittingWorksWithDefaultTiles, "Board splitting works with default tiles"));
  suite.add(BOOST_TEST_CASE_NAME(&boardSplittingWorksWithBlockedTiles, "Board splitting works with blocked tiles"));
  suite.add(BOOST_TEST_CASE_NAME(&boardSplittingWorksWithTwinTiles, "Board splitting works with twin tiles"));
  suite.add(BOOST_TEST_CASE_NAME(&boardSplittingShouldJudgeTheNeedForMultipleClicks,
                                 "Board splitting should judge the need for multiple clicks"));
  // Mask component finder
  suite.add(BOOST_TEST_CASE_NAME(&shouldProperlyDissolveComponents, "Properly dissolves components"));
  // Ranges
  suite.add(BOOST_TEST_CASE_NAME(&correctlyUpdatesRanges, "Correctly updates ranges"));
  suite.add(BOOST_TEST_CASE_NAME(&doesNotMutateRanges, "Does not mutate ranges"));
  // Text
  suite.add(BOOST_TEST_CASE_NAME(&joinIntoStringWorks, "Join into string works"));
  suite.add(BOOST_TEST_CASE_NAME(&integerToStringWithThousandSeparatorsWorks,
                                 "Integer to string with thousand separators works"));
  // Hashing
  suite.add(BOOST_TEST_CASE_NAME(&hashingTheEmptyStringProducesTheExpectedResult,
                                 "Hashing the empty string produces the expected result"));
  suite.add(BOOST_TEST_CASE_NAME(&hashingTenDigitsProducesTheExpectedResult,
                                 "Hashing ten digits produces the expected result"));
  // Position
  suite.add(BOOST_TEST_CASE_NAME(&positionsShouldBecomeSensibleStrings, "Positions should become sensible strings"));
  // Color
  suite.add(BOOST_TEST_CASE_NAME(&shouldBeAbleToDeriveColorsFromHsv, "Should be able to derive colors from HSV"));
  suite.add(BOOST_TEST_CASE_NAME(&shouldCorrectlyEvaluateLightness, "Should correctly evaluate lightness"));
  // Images
  suite.add(BOOST_TEST_CASE_NAME(&readingAndWritingImagesToFilesShouldWork,
                                 "Reading and writing images to files should work"));
  suite.add(
      BOOST_TEST_CASE_NAME(&shouldCorrectlyFindPixelValuesInImages, "Should correctly find pixel values in images"));
  return true;
}

int main(int argc, char *argv[]) {
  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
