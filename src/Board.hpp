#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Position.hpp"
#include "Tile.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
class Board {
  constexpr static std::size_t MaximumBoardHashTableSize = 1u << 22u;
  constexpr static std::size_t MaximumStateQueueSize = 1u << 22u;

  std::vector<std::vector<std::optional<Tile>>> matrix;

  [[nodiscard]] S32 getRowCount() const;

  [[nodiscard]] S32 getColumnCount() const;

  void safeInvert(S32 i, S32 j, bool clicked = false);

public:
  explicit Board(std::vector<std::vector<std::optional<Tile>>> tileMatrix);

  [[nodiscard]] bool hasTile(S32 i, S32 j) const;

  [[nodiscard]] Tile getTile(S32 i, S32 j) const;

  [[nodiscard]] bool isSolved() const;

  void activate(S32 i, S32 j);

  [[nodiscard]] std::vector<Position> findOptimalSolution(bool flipOnlyUp = false) const;

  [[nodiscard]] std::size_t hash() const;

  bool operator==(const Board &rhs) const;

  bool operator!=(const Board &rhs) const;

  [[nodiscard]] std::string toString() const;

  static Board fromString(const std::string &string);
};
} // namespace WayoutPlayer
