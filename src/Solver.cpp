#include "Solver.hpp"

#include <algorithm>
#include <queue>
#include <unordered_set>

namespace WayoutPlayer {
SolverConfiguration &Solver::getSolverConfiguration() {
  return solverConfiguration;
}

Solution Solver::findSolutionWithoutSplitting(const Board &initialBoard, bool flipOnlyUp) const {
  if (initialBoard.isSolved()) {
    return Solution({}, true);
  }
  struct State {
    Board board;
    std::vector<Position> clicked;

    void click(IndexType i, IndexType j) {
      clicked.emplace_back(i, j);
    }

    [[nodiscard]] bool hasClicked(IndexType i, IndexType j) const {
      return std::find(std::begin(clicked), std::end(clicked), Position{i, j}) != std::end(clicked);
    }

    [[nodiscard]] std::vector<Position> getClickPositionVector() const {
      return clicked;
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
  State initialState{initialBoard, {}};
  std::unordered_set<Board, Hash> seenBoards;
  seenBoards.insert(initialState.board);
  for (S32 i = 0; i < n; i++) {
    for (S32 j = 0; j < m; j++) {
      if (initialState.board.hasTile(i, j)) {
        if (initialState.board.getTile(i, j).type == TileType::Tap && initialState.board.getTile(i, j).up) {
          initialState.board.activate(i, j);
          initialState.click(i, j);
          exploredNodes++;
          seenBoards.insert(initialState.board);
          discoveredNodes++;
        }
      }
    }
  }
  if (initialState.board.isSolved()) {
    return Solution(initialState.getClickPositionVector(), true);
  }
  const auto mayNeedMultipleClicks = initialState.board.mayNeedMultipleClicks();
  std::queue<State> stateQueue;
  stateQueue.push(initialState);
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
    auto derivedState = state;
    for (S32 i = 0; i < n; i++) {
      for (S32 j = 0; j < m; j++) {
        if (!state.board.hasTile(i, j)) {
          continue;
        }
        if (!mayNeedMultipleClicks && state.hasClicked(i, j)) {
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
        derivedState.board = state.board;
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
        derivedState.clicked.pop_back();
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

Solution Solver::findSolution(const Board &initialBoard, bool flipOnlyUp) const {
  const auto components = initialBoard.splitComponents();
  std::optional<Solution> solution;
  for (const auto &component : components) {
    const auto componentSolution = findSolutionWithoutSplitting(component, flipOnlyUp);
    if (solution) {
      solution->add(componentSolution);
    } else {
      solution = componentSolution;
    }
  }
  return *solution;
}
} // namespace WayoutPlayer
