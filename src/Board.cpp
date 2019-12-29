#include "Board.hpp"

S32 WayoutPlayer::Board::getRowCount() const {
  return static_cast<S32>(matrix.size());
}

S32 WayoutPlayer::Board::getColumnCount() const {
  return static_cast<S32>(matrix.front().size());
}

void WayoutPlayer::Board::safeInvert(S32 i, S32 j) {
  if (i >= 0 && i < getRowCount() && j >= 0 && j < getColumnCount()) {
    matrix[i][j] = !matrix[i][j];
  }
}

WayoutPlayer::Board::Board(std::vector<std::vector<bool>> booleanMatrix) : matrix(std::move(booleanMatrix)) {
  const auto firstRowSize = matrix.front().size();
  for (const auto &row : matrix) {
    if (row.size() != firstRowSize) {
      throw std::invalid_argument("Matrix is not rectangular.");
    }
  }
}

void WayoutPlayer::Board::activate(S32 i, S32 j) {
  matrix[i][j] = !matrix[i][j];
}

std::string WayoutPlayer::Board::toString() const {
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

WayoutPlayer::Board WayoutPlayer::Board::fromString(const std::string &string) {
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
