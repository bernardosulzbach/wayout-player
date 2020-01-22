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

bool Board::canBeSolvedOptimallyDirectionally() const {
  for (S32 i = 0; i < getRowCount(); i++) {
    for (S32 j = 0; j < getColumnCount(); j++) {
      if (hasTile(i, j)) {
        const auto type = getTile(i, j).type;
        if (type == TileType::Blocked || type == TileType::Chain || type == TileType::Twin) {
          return false;
        }
      }
    }
  }
  return true;
}

bool Board::mayLowerOrUnblocksTilesIfClicked(S32 i, S32 j) const {
  const auto iString = std::to_string(i);
  const auto jString = std::to_string(j);
  if (!hasTile(i, j)) {
    throw std::invalid_argument("No tile at (" + iString + ", " + jString + ").");
  }
  const auto type = getTile(i, j).type;
  if (getTile(i, j).up) {
    return true;
  }
  const auto isRaisedOrBlockedAndAffected = [this](S32 neighborI, S32 neighborJ) {
    if (!hasTile(neighborI, neighborJ)) {
      return false;
    }
    const auto neighbor = getTile(neighborI, neighborJ);
    if (neighbor.type == TileType::Tap) {
      return false;
    }
    return neighbor.up || neighbor.type == TileType::Blocked;
  };
  if (type == TileType::Default) {
    if (isRaisedOrBlockedAndAffected(i - 1, j)) {
      return true;
    }
    if (isRaisedOrBlockedAndAffected(i, j - 1)) {
      return true;
    }
    if (isRaisedOrBlockedAndAffected(i, j + 1)) {
      return true;
    }
    if (isRaisedOrBlockedAndAffected(i + 1, j)) {
      return true;
    }
  } else if (type == TileType::Horizontal) {
    if (isRaisedOrBlockedAndAffected(i, j - 1)) {
      return true;
    }
    if (isRaisedOrBlockedAndAffected(i, j + 1)) {
      return true;
    }
  } else if (type == TileType::Vertical) {
    if (isRaisedOrBlockedAndAffected(i - 1, j)) {
      return true;
    }
    if (isRaisedOrBlockedAndAffected(i + 1, j)) {
      return true;
    }
  } else if (type == TileType::Tap) {
    throw std::invalid_argument("Should not be considering taps during the solution search.");
  } else if (type == TileType::Blocked) {
    throw std::invalid_argument("Blocked tile at (" + iString + ", " + jString + ") cannot be clicked.");
  } else if (type == TileType::Chain) {
    // TODO: implement a proper check here.
    return true;
  } else if (type == TileType::Twin) {
    // TODO: implement a proper check here.
    return true;
  }
  return false;
}

void Board::safeInvert(IndexType i, IndexType j, bool clicked, InversionHistory &history) {
  if (!hasTile(i, j)) {
    return;
  }
  if (matrix[i][j]->type == TileType::Tap) {
    if (clicked) {
      matrix[i][j]->up = !matrix[i][j]->up;
      history.inversions.emplace_back(i, j);
    }
  } else if (matrix[i][j]->type == TileType::Blocked) {
    if (clicked) {
      throw std::runtime_error("Cannot click on a blocked tile.");
    }
    matrix[i][j]->type = TileType::Default;
  } else if (matrix[i][j]->type == TileType::Chain) {
    matrix[i][j]->up = !matrix[i][j]->up;
    history.inversions.emplace_back(i, j);
    // This will work as a default tile unless it was not clicked.
    if (clicked) {
      return;
    }
    const auto propagate = [this, &history](IndexType ni, IndexType nj) {
      const auto begin = std::begin(history.inversions);
      const auto end = std::end(history.inversions);
      if (std::find(begin, end, Position{ni, nj}) == end) {
        safeInvert(ni, nj, false, history);
      }
    };
    propagate(i - 1, j);
    propagate(i, j - 1);
    propagate(i, j + 1);
    propagate(i + 1, j);
  } else if (matrix[i][j]->type == TileType::Twin) {
    if (!history.twinFinalState) {
      history.twinFinalState = !matrix[i][j]->up;
    }
    matrix[i][j]->up = *history.twinFinalState;
  } else {
    matrix[i][j]->up = !matrix[i][j]->up;
    history.inversions.emplace_back(i, j);
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

bool Board::hasUnsolvedTilesAtRow(IndexType i) const {
  if (i < 0 || i >= getRowCount()) {
    return false;
  }
  for (S32 j = 0; j < getColumnCount(); j++) {
    if (hasTile(i, j)) {
      if (matrix[i][j]->up || matrix[i][j]->type == TileType::Blocked) {
        return true;
      }
    }
  }
  return false;
}

bool Board::hasTile(IndexType i, IndexType j) const {
  return i >= 0 && i < getRowCount() && j >= 0 && j < getColumnCount() && matrix[i][j].has_value();
}

Tile Board::getTile(IndexType i, IndexType j) const {
  return matrix[i][j].value();
}

bool Board::isSolved() const {
  for (S32 i = 0; i < getRowCount(); i++) {
    for (S32 j = 0; j < getColumnCount(); j++) {
      if (hasTile(i, j)) {
        if (matrix[i][j]->up || matrix[i][j]->type == TileType::Blocked) {
          return false;
        }
      }
    }
  }
  return true;
}

void Board::activate(IndexType i, IndexType j) {
  if (!hasTile(i, j)) {
    return;
  }
  const auto tile = getTile(i, j);
  const auto type = tile.type;
  InversionHistory history;
  safeInvert(i, j, true, history);
  if (type == TileType::Default || type == TileType::Tap || type == TileType::Chain || type == TileType::Twin) {
    safeInvert(i - 1, j, false, history);
    safeInvert(i, j - 1, false, history);
    safeInvert(i, j + 1, false, history);
    safeInvert(i + 1, j, false, history);
  } else if (type == TileType::Horizontal) {
    safeInvert(i, j - 1, false, history);
    safeInvert(i, j + 1, false, history);
  } else if (type == TileType::Vertical) {
    safeInvert(i - 1, j, false, history);
    safeInvert(i + 1, j, false, history);
  } else if (type == TileType::Blocked) {
    throw std::runtime_error("Cannot activate a blocked tile.");
  } else {
    throw std::invalid_argument("Did not match the tile type.");
  }
  if (history.twinFinalState) {
    // Set all other twins to this state.
    for (S32 otherI = 0; otherI < getRowCount(); otherI++) {
      for (S32 otherJ = 0; otherJ < getColumnCount(); otherJ++) {
        if (otherI == i && otherJ == j) {
          continue;
        }
        if (hasTile(otherI, otherJ)) {
          if (matrix[otherI][otherJ]->type == TileType::Twin) {
            matrix[otherI][otherJ]->up = *history.twinFinalState;
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

std::vector<Board> Board::splitComponents() const {
  using TagType = U32;
  const auto NoTag = std::numeric_limits<TagType>::max();
  using TaggedTileMatrix = std::vector<std::vector<TagType>>;
  const auto rowCount = getRowCount();
  const auto columnCount = getColumnCount();
  TaggedTileMatrix tagMatrix(rowCount, std::vector<TagType>(columnCount, NoTag));
  TagType currentTag = 0;
  auto foundTwin = false;
  using PositionFunction = std::function<void(S32, S32)>;
  const PositionFunction propagateTag = [this, &tagMatrix, &currentTag, &foundTwin, &propagateTag](S32 i, S32 j) {
    if (!hasTile(i, j)) {
      return;
    }
    if (tagMatrix[i][j] == NoTag) {
      tagMatrix[i][j] = currentTag;
      // Propagate to all other twins.
      const auto rowCount = getRowCount();
      const auto columnCount = getColumnCount();
      if (getTile(i, j).type == TileType::Twin) {
        // This check is important to prevent an infinite recursion.
        if (!foundTwin) {
          foundTwin = true;
          for (S32 oi = 0; oi < rowCount; oi++) {
            for (S32 oj = 0; oj < columnCount; oj++) {
              if (oi == i && oj == j) {
                continue;
              }
              if (hasTile(oi, oj)) {
                if (getTile(oi, oj).type == TileType::Twin) {
                  propagateTag(oi, oj);
                }
              }
            }
          }
        }
      }
      propagateTag(i - 1, j);
      propagateTag(i, j - 1);
      propagateTag(i, j + 1);
      propagateTag(i + 1, j);
    } else if (tagMatrix[i][j] != currentTag) {
      throw std::runtime_error("Should not happen: a tag found an already existing tag.");
    }
  };
  for (S32 i = 0; i < rowCount; i++) {
    for (S32 j = 0; j < columnCount; j++) {
      if (hasTile(i, j)) {
        if (tagMatrix[i][j] == NoTag) {
          propagateTag(i, j);
          currentTag++;
        }
      }
    }
  }
  const auto componentCount = currentTag;
  const auto emptyRow = std::vector<std::optional<Tile>>(columnCount);
  const auto emptyBoard = Board(std::vector<std::vector<std::optional<Tile>>>(rowCount, emptyRow));
  auto components = std::vector<Board>(componentCount, emptyBoard);
  for (S32 i = 0; i < rowCount; i++) {
    for (S32 j = 0; j < columnCount; j++) {
      if (tagMatrix[i][j] != NoTag) {
        components[tagMatrix[i][j]].matrix[i][j] = getTile(i, j);
      }
    }
  }
  return components;
}

Board Board::mergeComponents(const std::vector<Board> &components) {
  if (components.empty()) {
    throw std::invalid_argument("Components should not be empty.");
  }
  const auto rowCount = components.front().getRowCount();
  const auto columnCount = components.front().getColumnCount();
  for (const auto &component : components) {
    if (component.getRowCount() != rowCount) {
      throw std::invalid_argument("Components have different number of rows.");
    } else {
      if (component.getColumnCount() != columnCount) {
        throw std::invalid_argument("Components have different number of columns.");
      }
    }
  }
  Board merge(std::vector<std::vector<std::optional<Tile>>>(rowCount, std::vector<std::optional<Tile>>(columnCount)));
  for (const auto &component : components) {
    for (S32 i = 0; i < rowCount; i++) {
      for (S32 j = 0; j < columnCount; j++) {
        if (component.hasTile(i, j)) {
          if (merge.hasTile(i, j)) {
            const auto position = Position(i, j);
            throw std::invalid_argument("Found two occurrences of tile " + position.toString() + ".");
          } else {
            merge.matrix[i][j] = component.matrix[i][j];
          }
        }
      }
    }
  }
  return merge;
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
