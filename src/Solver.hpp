#pragma once

#include "Board.hpp"
#include "SolverConfiguration.hpp"

namespace WayoutPlayer {
class Solver {
  SolverConfiguration solverConfiguration;

public:
  SolverConfiguration &getSolverConfiguration();

  [[nodiscard]] Solution findSolution(const Board &initialState, bool flipOnlyUp = false) const;
};
} // namespace WayoutPlayer