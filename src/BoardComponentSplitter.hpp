#pragma once

#include <vector>

#include "Board.hpp"

namespace WayoutPlayer {
/**
 * Splits a board into its connected components.
 *
 * Supports all tile types.
 *
 * The components are returned in no particular order.
 */
[[nodiscard]] std::vector<Board> splitComponents(const Board &board);

Board mergeComponents(const std::vector<Board> &components);
} // namespace WayoutPlayer