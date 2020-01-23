#include "Solver.hpp"

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_set>

#include "Text.hpp"

namespace WayoutPlayer {
const SolverConfiguration &Solver::getSolverConfiguration() const {
  return solverConfiguration;
}

SolverConfiguration &Solver::getSolverConfiguration() {
  return solverConfiguration;
}

Solution Solver::findSolutionWithoutSplitting(const Board &initialBoard) const {
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
        }
      }
    }
  }
  if (initialState.board.isSolved()) {
    return Solution(initialState.getClickPositionVector(), true);
  }
  const auto mayNeedMultipleClicks = initialState.board.mayNeedMultipleClicks();
  const auto canBeSolvedOptimallyDirectionally = initialState.board.canBeSolvedOptimallyDirectionally();
  std::queue<State> stateQueue;
  stateQueue.push(initialState);
  std::optional<Solution> solution;
  const auto configuration = getSolverConfiguration();
  const auto maximumStateQueueSize = configuration.getMaximumStateQueueSize();
  const auto maximumBoardHashTableSize = configuration.getMaximumBoardHashTableSize();
  const auto flippingOnlyUp = configuration.isFlippingOnlyUp();
  if (configuration.isVerbose()) {
    if (canBeSolvedOptimallyDirectionally) {
      std::cout << "Can be solved from any direction." << '\n';
    }
  }
  while (!stateQueue.empty()) {
    if (stateQueue.size() > maximumStateQueueSize) {
      const auto limitString = std::to_string(maximumStateQueueSize);
      throw std::runtime_error("State queue size exceeded the limit of " + limitString + ".");
    }
    if (seenBoards.size() > maximumBoardHashTableSize) {
      const auto limitString = std::to_string(maximumBoardHashTableSize);
      throw std::runtime_error("Board hash table size exceeded the limit of " + limitString + ".");
    }
    const auto state = stateQueue.front();
    stateQueue.pop();
    auto derivedState = state;
    auto exploring = true;
    for (S32 i = 0; i < n && exploring; i++) {
      for (S32 j = 0; j < m && exploring; j++) {
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
        if (flippingOnlyUp) {
          if (!state.board.getTile(i, j).up) {
            continue;
          }
        }
        const auto click = [&solution, &derivedState, &state, flippingOnlyUp, &stateQueue, &seenBoards](S32 i, S32 j) {
          derivedState.board = state.board;
          derivedState.board.activate(i, j);
          derivedState.click(i, j);
          if (!solution && derivedState.board.isSolved()) {
            solution = Solution(derivedState.getClickPositionVector(), !flippingOnlyUp);
          }
          if (seenBoards.count(derivedState.board) == 0) {
            stateQueue.push(derivedState);
            seenBoards.insert(derivedState.board);
          }
          derivedState.clicked.pop_back();
        };
        const auto clickTileIfExists = [&state, &click](S32 i, S32 j) {
          if (state.board.hasTile(i, j)) {
            click(i, j);
          }
        };
        const auto considerClickingTileAndNeighbors = [&clickTileIfExists](S32 centerI, S32 centerJ) {
          clickTileIfExists(centerI, centerJ);
          clickTileIfExists(centerI - 1, centerJ);
          clickTileIfExists(centerI, centerJ - 1);
          clickTileIfExists(centerI, centerJ + 1);
          clickTileIfExists(centerI + 1, centerJ);
        };
        if (canBeSolvedOptimallyDirectionally) {
          // Temporary: if we can solve this directionally, don't try all possible clicks for a board.
          if (state.board.getTile(i, j).up || state.board.getTile(i, j).type == TileType::Blocked) {
            considerClickingTileAndNeighbors(i, j);
            exploring = false;
          }
        } else {
          clickTileIfExists(i, j);
        }
      }
    }
    exploredNodes++;
    if (solution) {
      solution->setExploredNodes(exploredNodes);
      solution->setDistinctNodes(seenBoards.size());
      return solution.value();
    }
  }
  const auto exploredNodeCount = toPluralizedString(exploredNodes, "node");
  throw std::runtime_error("Could not find a solution after exploring " + exploredNodeCount + ".");
}

Solution Solver::findSolution(const Board &initialBoard) const {
  const auto components = initialBoard.splitComponents();
  if (getSolverConfiguration().isVerbose()) {
    std::cout << "Found " << toPluralizedString(components.size(), "component") << "." << '\n';
  }
  std::optional<Solution> solution;
  for (const auto &component : components) {
    const auto componentSolution = findSolutionWithoutSplitting(component);
    if (solution) {
      solution->add(componentSolution);
    } else {
      solution = componentSolution;
    }
  }
  return *solution;
}
} // namespace WayoutPlayer
