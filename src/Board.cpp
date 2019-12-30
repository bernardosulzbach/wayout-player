#include "Board.hpp"

#include <boost/container_hash/hash.hpp>

#include <iostream>
#include <queue>
#include <unordered_set>

namespace WayOutPlayer {
S32 Board::getRowCount() const {
  return static_cast<S32>(matrix.size());
}

S32 Board::getColumnCount() const {
  return static_cast<S32>(matrix.front().size());
}

void Board::safeInvert(S32 i, S32 j) {
  if (i >= 0 && i < getRowCount() && j >= 0 && j < getColumnCount()) {
    matrix[i][j] = !matrix[i][j];
  }
}

Board::Board(std::vector<std::vector<bool>> booleanMatrix) : matrix(std::move(booleanMatrix)) {
  const auto firstRowSize = matrix.front().size();
  for (const auto &row : matrix) {
    if (row.size() != firstRowSize) {
      throw std::invalid_argument("Matrix is not rectangular.");
    }
  }
}

bool Board::isSolved() const {
  for (S32 i = 0; i < getRowCount(); i++) {
    for (S32 j = 0; j < getColumnCount(); j++) {
      if (matrix[i][j]) {
        return false;
      }
    }
  }
  return true;
}

void Board::activate(S32 i, S32 j) {
  safeInvert(i - 1, j);
  safeInvert(i, j - 1);
  safeInvert(i, j);
  safeInvert(i, j + 1);
  safeInvert(i + 1, j);
}

std::vector<Position> Board::findOptimalSolution(bool flipOnlyTrue) const {
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
    if (state.board.isSolved()) {
      return state.getClickPositionVector();
    }
    for (S32 i = 0; i < n; i++) {
      for (S32 j = 0; j < m; j++) {
        if (state.clicked[i][j]) {
          continue;
        }
        if (flipOnlyTrue) {
          if (!state.board.matrix[i][j]) {
            continue;
          }
        }
        auto derivedState = state;
        derivedState.board.activate(i, j);
        derivedState.clicked[i][j] = true;
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
      boost::hash_combine(seed, matrix[i][j]);
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
  std::string board;
  for (S32 i = 0; i < getRowCount(); i++) {
    for (S32 j = 0; j < getColumnCount(); j++) {
      if (matrix[i][j]) {
        board += '#';
      } else {
        board += '.';
      }
    }
    if (i + 1 < getRowCount()) {
      board += '\n';
    }
  }
  return board;
}

Board Board::fromString(const std::string &string) {
  std::vector<std::vector<bool>> matrix(1);
  for (const auto c : string) {
    if (c == '.') {
      matrix.back().push_back(false);
    } else if (c == '#') {
      matrix.back().push_back(true);
    } else if (c == '\n') {
      matrix.emplace_back();
    } else {
      const auto message = "String has an invalid character: '" + std::string(1, c) + "'.";
      throw std::invalid_argument(message);
    }
  }
  return Board(matrix);
}
} // namespace WayOutPlayer