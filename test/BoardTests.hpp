#pragma once

namespace WayoutPlayer::Tests {
void boardIsSolvedWorksOnSolvedBoards();

void boardIsSolvedWorksOnUnsolvedBoards();

void boardStringConversionsWork();

void activatingAChainShouldWork();

void boardsShouldBeDifferentIfTheirTilesAreNotBlocked();

void activatingLoweredNeighborTwinsShouldBehaveAsInTheGame();

void activatingRaisedNeighborTwinsShouldBehaveAsInTheGame();

void solvingABoardWithNeighboringTwinsShouldBehaveAsInTheGame();

void shouldUnderstandThatBlockedTilesMayNeedMultipleClicks();

void boardWithBlockedTilesShouldBehaveAsInTheGame();
} // namespace WayoutPlayer::Tests