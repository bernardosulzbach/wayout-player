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
class Board {
  std::vector<std::vector<std::optional<Tile>>> matrix;
  bool startedWithBlockedTiles = false;

public:
  [[nodiscard]] S32 getRowCount() const;

  [[nodiscard]] S32 getColumnCount() const;

  [[nodiscard]] bool mayNeedMultipleClicks() const;

  void safeInvert(IndexType i, IndexType j, std::vector<Position> &inversions, bool clicked,
                  std::optional<bool> &twinFinalState);

  explicit Board(std::vector<std::vector<std::optional<Tile>>> tileMatrix);

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
