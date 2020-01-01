#include "Board.hpp"

#include <boost/container_hash/hash.hpp>

#include <iostream>
#include <queue>
#include <unordered_set>

namespace WayoutPlayer {
S32 Board::getRowCount() const {
  return static_cast<S32>(matrix.size());
}

S32 Board::getColumnCount() const {
  return static_cast<S32>(matrix.front().size());
}

void Board::safeInvert(S32 i, S32 j, bool clicked) {
  if (!hasTile(i, j)) {
    return;
  }
  if (matrix[i][j]->type == TileType::Tap) {
    if (clicked) {
      matrix[i][j]->up = !matrix[i][j]->up;
    }
  } else {
    matrix[i][j]->up = !matrix[i][j]->up;
  }
}

Board::Board(std::vector<std::vector<std::optional<Tile>>> tileMatrix) : matrix(std::move(tileMatrix)) {
  const auto firstRowSize = matrix.front().size();
  for (const auto &row : matrix) {
    if (row.size() != firstRowSize) {
      throw std::invalid_argument("Matrix is not rectangular.");
    }
  }
}

[[nodiscard]] bool Board::hasTile(S32 i, S32 j) const {
  return i >= 0 && i < getRowCount() && j >= 0 && j < getColumnCount() && matrix[i][j].has_value();
}

[[nodiscard]] Tile Board::getTile(S32 i, S32 j) const {
  return matrix[i][j].value();
}

bool Board::isSolved() const {
  for (S32 i = 0; i < getRowCount(); i++) {
    for (S32 j = 0; j < getColumnCount(); j++) {
      if (hasTile(i, j)) {
        if (matrix[i][j]->up) {
          return false;
        }
      }
    }
  }
  return true;
}

void Board::activate(S32 i, S32 j) {
  if (hasTile(i, j)) {
    const auto tile = getTile(i, j);
    safeInvert(i, j, true);
    if (tile.type == TileType::Normal || tile.type == TileType::Tap) {
      safeInvert(i - 1, j);
      safeInvert(i, j - 1);
      safeInvert(i, j + 1);
      safeInvert(i + 1, j);
    } else if (tile.type == TileType::Horizontal) {
      safeInvert(i, j - 1);
      safeInvert(i, j + 1);
    } else if (tile.type == TileType::Vertical) {
      safeInvert(i - 1, j);
      safeInvert(i + 1, j);
    }
  }
}

Solution Board::findSolution(bool flipOnlyUp) const {
  if (isSolved()) {
    return Solution({}, true);
  }
  struct State {
    Board board;
    std::vector<std::vector<bool>> clicked;

    [[nodiscard]] std::vector<Position> getClickPositionVector() const {
      std::vector<Position> positions;
      for (S32 i = 0; i < board.getRowCount(); i++) {
        for (S32 j = 0; j < board.getColumnCount(); j++) {
          if (clicked[i][j]) {
            positions.push_back({i, j});
          }
        }
      }
      return positions;
    }
  };
  const auto n = getRowCount();
  const auto m = getColumnCount();
  struct Hash {
    std::size_t operator()(const Board &board) const {
      return board.hash();
    }
  };
  std::queue<State> stateQueue;
  stateQueue.push({*this, std::vector<std::vector<bool>>(n, std::vector<bool>(m))});
  std::unordered_set<Board, Hash> seenBoards;
  seenBoards.insert(*this);
  U64 exploredNodes = 0;
  while (!stateQueue.empty()) {
    if (stateQueue.size() > MaximumStateQueueSize) {
      const auto limitString = std::to_string(MaximumStateQueueSize);
      throw std::runtime_error("State queue size exceeded the limit of " + limitString + ".");
    }
    if (seenBoards.size() > MaximumBoardHashTableSize) {
      const auto limitString = std::to_string(MaximumBoardHashTableSize);
      throw std::runtime_error("Board hash table size exceeded the limit of " + limitString + ".");
    }
    const auto state = stateQueue.front();
    stateQueue.pop();
    exploredNodes++;
    for (S32 i = 0; i < n; i++) {
      for (S32 j = 0; j < m; j++) {
        if (state.clicked[i][j]) {
          continue;
        }
        if (flipOnlyUp) {
          if (!hasTile(i, j) || !state.board.matrix[i][j]->up) {
            continue;
          }
        }
        auto derivedState = state;
        derivedState.board.activate(i, j);
        derivedState.clicked[i][j] = true;
        if (derivedState.board.isSolved()) {
          auto solution = Solution(derivedState.getClickPositionVector(), !flipOnlyUp);
          solution.setExploredNodes(exploredNodes);
          solution.setDistinctNodes(seenBoards.size());
          return solution;
        }
        if (seenBoards.count(derivedState.board) == 0) {
          stateQueue.push(derivedState);
          seenBoards.insert(derivedState.board);
        }
      }
    }
  }
  throw std::runtime_error("Could not find a solution, which should be impossible.");
}

std::size_t Board::hash() const {
  std::size_t seed = 0;
  for (S32 i = 0; i < getRowCount(); i++) {
    for (S32 j = 0; j < getColumnCount(); j++) {
      if (hasTile(i, j)) {
        boost::hash_combine(seed, matrix[i][j]->up);
      }
    }
  }
  return seed;
}

bool Board::operator==(const Board &rhs) const {
  return matrix == rhs.matrix;
}

bool Board::operator!=(const Board &rhs) const {
  return !(rhs == *this);
}

std::string Board::toString() const {
  const auto rowCount = getRowCount();
  const auto columnCount = getColumnCount();
  std::string board;
  for (S32 i = 0; i < rowCount; i++) {
    for (S32 j = 0; j < columnCount; j++) {
      if (hasTile(i, j)) {
        board += matrix[i][j]->toString();
      } else {
        board += "  ";
      }
      if (j + 1 < columnCount) {
        board += ' ';
      }
    }
    if (i + 1 < rowCount) {
      board += '\n';
    }
  }
  return board;
}

Board Board::fromString(const std::string &string) {
  std::vector<std::vector<std::optional<Tile>>> matrix(1);
  auto atNextTile = true;
  for (std::size_t i = 0; i < string.size(); i++) {
    const auto character = string[i];
    if (atNextTile) {
      if (isalpha(character)) {
        matrix.back().emplace_back(Tile(false, tileTypeFromCharacter(character)));
        atNextTile = false;
      } else if (character == ' ') {
        matrix.back().push_back(std::nullopt);
        atNextTile = false;
        i++;
      } else {
        throw std::invalid_argument("Found an unexpected start of tile: " + std::string(1, character) + ".");
      }
    } else {
      if (isdigit(character)) {
        matrix.back().back()->up = character == '1';
      } else if (character == '\n') {
        matrix.emplace_back();
        atNextTile = true;
      } else if (character == ' ') {
        atNextTile = true;
      } else {
        const auto message = "String has an invalid character: '" + std::string(1, character) + "'.";
        throw std::invalid_argument(message);
      }
    }
  }
  while (matrix.back().empty()) {
    matrix.pop_back();
  }
  return Board(matrix);
}
} // namespace WayoutPlayer