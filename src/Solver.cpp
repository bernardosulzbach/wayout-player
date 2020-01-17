#include "Solver.hpp"

#include <algorithm>
#include <queue>
#include <unordered_set>

namespace WayoutPlayer {
SolverConfiguration &Solver::getSolverConfiguration() {
  return solverConfiguration;
}

Solution Solver::findSolution(const Board &initialBoard, bool flipOnlyUp) const {
  if (initialBoard.isSolved()) {
    return Solution({}, true);
  }
  class State {
    std::vector<Position> clicked;

  public:
    void click(IndexType i, IndexType j) {
      clicked.emplace_back(i, j);
    }

    void popClick() {
      if (clicked.empty()) {
        throw std::runtime_error("Cannot pop click because there are no clicks left.");
      }
      clicked.pop_back();
    }

    [[nodiscard]] bool hasClicked(IndexType i, IndexType j) const {
      return std::find(std::begin(clicked), std::end(clicked), Position{i, j}) != std::end(clicked);
    }

    [[nodiscard]] std::vector<Position> getClickPositionVector() const {
      return clicked;
    }

    [[nodiscard]] Board getBoard(const Board &initialBoard) const {
      auto board = initialBoard;
      for (const auto &click : clicked) {
        board.activate(click.i, click.j);
      }
      return board;
    }
  };
  const auto n = initialBoard.getRowCount();
  const auto m = initialBoard.getColumnCount();
  struct Hash {
    std::size_t operator()(const Board &board) const {
      return board.hash();
    }
  };
  U64 exploredNodes = 0;
  U64 discoveredNodes = 0;
  // This might be get quite large in memory, but using a sequence of clicks will not detect redundancies.
  // Because in some boards order does not matter, using a  sequence of clicks in those cases could be done.
  std::unordered_set<Board, Hash> seenBoards;
  seenBoards.insert(initialBoard);
  auto mayNeedMultipleClicks = false;
  std::queue<State> stateQueue;
  // Refactor this into a function, maybe. I don't want initialState defined below to avoid clashes.
  {
    State initialState;
    for (S32 i = 0; i < n; i++) {
      for (S32 j = 0; j < m; j++) {
        if (initialBoard.hasTile(i, j)) {
          const auto &board = initialState.getBoard(initialBoard);
          if (board.getTile(i, j).type == TileType::Tap && board.getTile(i, j).up) {
            initialState.click(i, j);
            exploredNodes++;
            seenBoards.insert(initialState.getBoard(initialBoard));
            discoveredNodes++;
          }
        }
      }
    }
    if (initialState.getBoard(initialBoard).isSolved()) {
      return Solution(initialState.getClickPositionVector(), true);
    }
    mayNeedMultipleClicks = initialState.getBoard(initialBoard).mayNeedMultipleClicks();
    stateQueue.push(initialState);
  }
  std::optional<Solution> solution;
  while (!stateQueue.empty()) {
    if (stateQueue.size() > solverConfiguration.getMaximumStateQueueSize()) {
      const auto limitString = std::to_string(solverConfiguration.getMaximumStateQueueSize());
      throw std::runtime_error("State queue size exceeded the limit of " + limitString + ".");
    }
    if (seenBoards.size() > solverConfiguration.getMaximumBoardHashTableSize()) {
      const auto limitString = std::to_string(solverConfiguration.getMaximumBoardHashTableSize());
      throw std::runtime_error("Board hash table size exceeded the limit of " + limitString + ".");
    }
    const auto state = stateQueue.front();
    stateQueue.pop();
    State derivedState;
    for (S32 i = 0; i < n; i++) {
      for (S32 j = 0; j < m; j++) {
        if (!initialBoard.hasTile(i, j)) {
          continue;
        }
        if (!mayNeedMultipleClicks && state.hasClicked(i, j)) {
          continue;
        }
        const auto stateBoard = state.getBoard(initialBoard);
        if (stateBoard.getTile(i, j).type == TileType::Tap) {
          if (stateBoard.getTile(i, j).up) {
            throw std::runtime_error("Should not have up taps during search.");
          }
          continue;
        }
        if (stateBoard.getTile(i, j).type == TileType::Blocked) {
          continue;
        }
        if (flipOnlyUp) {
          if (stateBoard.getTile(i, j).up) {
            continue;
          }
        }
        derivedState = state;
        derivedState.click(i, j);
        const auto derivedStateBoard = derivedState.getBoard(initialBoard);
        if (!solution && derivedStateBoard.isSolved()) {
          solution = Solution(derivedState.getClickPositionVector(), !flipOnlyUp);
        }
        if (seenBoards.count(derivedStateBoard) == 0) {
          stateQueue.push(derivedState);
          seenBoards.insert(derivedStateBoard);
          discoveredNodes++;
        }
        derivedState.popClick();
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
} // namespace WayoutPlayer
