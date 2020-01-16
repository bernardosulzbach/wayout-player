#include "Board.hpp"

#include <boost/functional/hash.hpp>

namespace WayoutPlayer {
S32 Board::getRowCount() const {
  return static_cast<S32>(matrix.size());
}

S32 Board::getColumnCount() const {
  return static_cast<S32>(matrix.front().size());
}

bool Board::mayNeedMultipleClicks() const {
  return startedWithBlockedTiles;
}

void Board::safeInvert(S32 i, S32 j, std::vector<Position> &inversions, bool clicked,
                       std::optional<bool> &twinFinalState) {
  if (!hasTile(i, j)) {
    return;
  }
  if (matrix[i][j]->type == TileType::Tap) {
    if (clicked) {
      matrix[i][j]->up = !matrix[i][j]->up;
      inversions.push_back({i, j});
    }
  } else if (matrix[i][j]->type == TileType::Blocked) {
    if (clicked) {
      throw std::runtime_error("Cannot click on a blocked tile.");
    }
    matrix[i][j]->type = TileType::Default;
  } else if (matrix[i][j]->type == TileType::Chain) {
    matrix[i][j]->up = !matrix[i][j]->up;
    inversions.push_back({i, j});
    const auto propagate = [this, &inversions, &twinFinalState](S32 ni, S32 nj) {
      if (std::find(std::begin(inversions), std::end(inversions), Position{ni, nj}) == std::end(inversions)) {
        safeInvert(ni, nj, inversions, false, twinFinalState);
      }
    };
    propagate(i - 1, j);
    propagate(i, j - 1);
    propagate(i, j + 1);
    propagate(i + 1, j);
  } else if (matrix[i][j]->type == TileType::Twin) {
    if (!twinFinalState) {
      twinFinalState = !matrix[i][j]->up;
    }
    matrix[i][j]->up = *twinFinalState;
  } else {
    matrix[i][j]->up = !matrix[i][j]->up;
    inversions.push_back({i, j});
  }
}

Board::Board(std::vector<std::vector<std::optional<Tile>>> tileMatrix) : matrix(std::move(tileMatrix)) {
  const auto firstRowSize = matrix.front().size();
  for (const auto &row : matrix) {
    if (row.size() != firstRowSize) {
      throw std::invalid_argument("Matrix is not rectangular.");
    }
    for (const auto tile : row) {
      if (tile && tile->type == TileType::Blocked) {
        startedWithBlockedTiles = true;
      }
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
  std::vector<Position> inversions;
  if (!hasTile(i, j)) {
    return;
  }
  const auto tile = getTile(i, j);
  const auto type = tile.type;
  std::optional<bool> twinFinalState;
  safeInvert(i, j, inversions, true, twinFinalState);
  if (type == TileType::Default || type == TileType::Tap || type == TileType::Chain || type == TileType::Twin) {
    safeInvert(i - 1, j, inversions, false, twinFinalState);
    safeInvert(i, j - 1, inversions, false, twinFinalState);
    safeInvert(i, j + 1, inversions, false, twinFinalState);
    safeInvert(i + 1, j, inversions, false, twinFinalState);
  } else if (type == TileType::Horizontal) {
    safeInvert(i, j - 1, inversions, false, twinFinalState);
    safeInvert(i, j + 1, inversions, false, twinFinalState);
  } else if (type == TileType::Vertical) {
    safeInvert(i - 1, j, inversions, false, twinFinalState);
    safeInvert(i + 1, j, inversions, false, twinFinalState);
  } else if (type == TileType::Blocked) {
    throw std::runtime_error("Cannot activate a blocked tile.");
  } else {
    throw std::invalid_argument("Did not match the tile type.");
  }
  if (twinFinalState) {
    // Set all other twins to this state.
    for (S32 otherI = 0; otherI < getRowCount(); otherI++) {
      for (S32 otherJ = 0; otherJ < getColumnCount(); otherJ++) {
        if (otherI == i && otherJ == j) {
          continue;
        }
        if (hasTile(otherI, otherJ)) {
          if (matrix[otherI][otherJ]->type == TileType::Twin) {
            matrix[otherI][otherJ]->up = *twinFinalState;
          }
        }
      }
    }
  }
}

std::size_t Board::hash() const {
  std::size_t seed = 0;
  for (S32 i = 0; i < getRowCount(); i++) {
    for (S32 j = 0; j < getColumnCount(); j++) {
      if (hasTile(i, j)) {
        boost::hash_combine(seed, tileTypeToInteger(matrix[i][j]->type));
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
  if (string.empty()) {
    throw std::invalid_argument("Cannot create a board from an empty string.");
  }
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
