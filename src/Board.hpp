#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Position.hpp"
#include "Solution.hpp"
#include "Tile.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
class InversionHistory {
public:
  std::vector<Position> inversions;
  std::optional<bool> twinFinalState;
};

class Board {
  std::vector<std::vector<std::optional<Tile>>> matrix;
  bool startedWithBlockedTiles = false;

public:
  [[nodiscard]] S32 getRowCount() const;

  [[nodiscard]] S32 getColumnCount() const;

  [[nodiscard]] bool mayNeedMultipleClicks() const;

  /**
   * Returns whether or not this board can be solved optimally by clearing it in any direction.
   *
   * A board that can be solved optimally in any direction has optimal solutions that involve lowering all raised tiles
   * in a file before clicking any tiles which are two or more files after this file.
   *
   * This function may return false in cases where a board could be solved optimally by clearing it in any direction.
   */
  [[nodiscard]] bool canBeSolvedOptimallyDirectionally() const;

  /**
   * Returns whether clicking at (i, j) may lower or unblock at least one tile.
   *
   * This function may return true even if clicking (i, j) does not actually lower or unblocks any tiles.
   */
  [[nodiscard]] bool mayLowerOrUnblocksTilesIfClicked(S32 i, S32 j) const;

  void safeInvert(IndexType i, IndexType j, bool clicked, InversionHistory &history);

  explicit Board(std::vector<std::vector<std::optional<Tile>>> tileMatrix);

  [[nodiscard]] bool hasUnsolvedTilesAtRow(IndexType i) const;

  [[nodiscard]] bool hasTile(IndexType i, IndexType j) const;

  [[nodiscard]] Tile getTile(IndexType i, IndexType j) const;

  [[nodiscard]] bool isSolved() const;

  void activate(IndexType i, IndexType j);

  [[nodiscard]] std::size_t hash() const;

  bool operator==(const Board &rhs) const;

  bool operator!=(const Board &rhs) const;

  /**
   * Splits a board into its connected components.
   *
   * Supports all tile types.
   *
   * The components are returned in no particular order.
   */
  [[nodiscard]] std::vector<Board> splitComponents() const;

  static Board mergeComponents(const std::vector<Board> &components);

  [[nodiscard]] std::string toString() const;

  static Board fromString(const std::string &string);
};
} // namespace WayoutPlayer
