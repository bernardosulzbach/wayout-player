#pragma once

#include "Board.hpp"
#include "SolverConfiguration.hpp"

namespace WayoutPlayer {
class Solver {
  SolverConfiguration solverConfiguration;

  [[nodiscard]] Solution findSolutionWithoutSplitting(const Board &initialState, bool flipOnlyUp = false) const;

public:
  SolverConfiguration &getSolverConfiguration();

  // TODO: Move flipOnlyUp into the solver configuration object.
  [[nodiscard]] Solution findSolution(const Board &initialState, bool flipOnlyUp = false) const;
};
} // namespace WayoutPlayer