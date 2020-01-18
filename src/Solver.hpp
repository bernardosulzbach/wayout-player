#pragma once

#include "Board.hpp"
#include "SolverConfiguration.hpp"

namespace WayoutPlayer {
class Solver {
  SolverConfiguration solverConfiguration;

  [[nodiscard]] Solution findSolutionWithoutSplitting(const Board &initialState) const;

public:
  [[nodiscard]] const SolverConfiguration &getSolverConfiguration() const;

  [[nodiscard]] SolverConfiguration &getSolverConfiguration();

  [[nodiscard]] Solution findSolution(const Board &initialState) const;
};
} // namespace WayoutPlayer