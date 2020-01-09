#include "Board.hpp"

#include <boost/functional/hash.hpp>

#include <queue>
#include <unordered_set>

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

void Board::safeInvert(S32 i, S32 j, std::vector<Position> &inversions, bool clicked) {
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
    const auto propagate = [this, &inversions](S32 ni, S32 nj) {
      if (std::find(std::begin(inversions), std::end(inversions), Position{ni, nj}) == std::end(inversions)) {
        safeInvert(ni, nj, inversions);
      }
    };
    propagate(i - 1, j);
    propagate(i, j - 1);
    propagate(i, j + 1);
    propagate(i + 1, j);
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
  if (hasTile(i, j)) {
    const auto tile = getTile(i, j);
    safeInvert(i, j, inversions, true);
    if (tile.type == TileType::Default || tile.type == TileType::Tap || tile.type == TileType::Chain) {
      safeInvert(i - 1, j, inversions);
      safeInvert(i, j - 1, inversions);
      safeInvert(i, j + 1, inversions);
      safeInvert(i + 1, j, inversions);
    } else if (tile.type == TileType::Horizontal) {
      safeInvert(i, j - 1, inversions);
      safeInvert(i, j + 1, inversions);
    } else if (tile.type == TileType::Vertical) {
      safeInvert(i - 1, j, inversions);
      safeInvert(i + 1, j, inversions);
    } else if (tile.type == TileType::Blocked) {
      throw std::runtime_error("Cannot activate a blocked tile.");
    }
  }
}

Solution Board::findSolution(bool flipOnlyUp) const {
  if (isSolved()) {
    return Solution({}, true);
  }
  struct State {
    Board board;
    std::vector<Position> clicked;

    void click(S32 i, S32 j) {
      return clicked.push_back({i, j});
    }

    [[nodiscard]] bool hasClicked(S32 i, S32 j) const {
      return std::find(std::begin(clicked), std::end(clicked), Position{i, j}) != std::end(clicked);
    }

    [[nodiscard]] std::vector<Position> getClickPositionVector() const {
      return clicked;
    }
  };
  const auto n = getRowCount();
  const auto m = getColumnCount();
  struct Hash {
    std::size_t operator()(const Board &board) const {
      return board.hash();
    }
  };
  U64 exploredNodes = 0;
  U64 discoveredNodes = 0;
  State initialState{*this, {}};
  std::unordered_set<Board, Hash> seenBoards;
  seenBoards.insert(initialState.board);
  for (S32 i = 0; i < n; i++) {
    for (S32 j = 0; j < m; j++) {
      if (hasTile(i, j) && getTile(i, j).type == TileType::Tap && getTile(i, j).up) {
        initialState.board.activate(i, j);
        initialState.click(i, j);
        exploredNodes++;
        seenBoards.insert(initialState.board);
        discoveredNodes++;
      }
    }
  }
  if (initialState.board.isSolved()) {
    return Solution(initialState.getClickPositionVector(), true);
  }
  std::queue<State> stateQueue;
  stateQueue.push(initialState);
  std::optional<Solution> solution;
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
    for (S32 i = 0; i < n; i++) {
      for (S32 j = 0; j < m; j++) {
        if (!state.board.hasTile(i, j)) {
          continue;
        }
        if (!mayNeedMultipleClicks() && state.hasClicked(i, j)) {
          continue;
        }
        if (state.board.getTile(i, j).type == TileType::Tap) {
          if (state.board.getTile(i, j).up) {
            throw std::runtime_error("Should not have up taps during search.");
          }
          continue;
        }
        if (state.board.getTile(i, j).type == TileType::Blocked) {
          continue;
        }
        if (flipOnlyUp) {
          if (state.board.getTile(i, j).up) {
            continue;
          }
        }
        auto derivedState = state;
        derivedState.board.activate(i, j);
        derivedState.click(i, j);
        if (!solution && derivedState.board.isSolved()) {
          solution = Solution(derivedState.getClickPositionVector(), !flipOnlyUp);
        }
        if (seenBoards.count(derivedState.board) == 0) {
          stateQueue.push(derivedState);
          seenBoards.insert(derivedState.board);
          discoveredNodes++;
        }
      }
    }
    exploredNodes++;
    if (solution) {
      solution->setExploredNodes(exploredNodes);
      solution->setDistinctNodes(seenBoards.size());
      solution->setMeanBranchingFactor(static_cast<F64>(discoveredNodes) / exploredNodes);
      return solution.value();
    }
  }
  throw std::runtime_error("Could not find a solution.");
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
