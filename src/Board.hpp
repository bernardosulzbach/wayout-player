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

  void safeInvert(S32 i, S32 j, std::vector<Position> &inversions, bool clicked, std::optional<bool> &twinFinalState);

  explicit Board(std::vector<std::vector<std::optional<Tile>>> tileMatrix);

  [[nodiscard]] bool hasTile(S32 i, S32 j) const;

  [[nodiscard]] Tile getTile(S32 i, S32 j) const;

  [[nodiscard]] bool isSolved() const;

  void activate(S32 i, S32 j);

  [[nodiscard]] std::size_t hash() const;

  bool operator==(const Board &rhs) const;

  bool operator!=(const Board &rhs) const;

  [[nodiscard]] std::string toString() const;

  static Board fromString(const std::string &string);
};
} // namespace WayoutPlayer
