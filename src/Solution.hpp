#pragma once

#include "Position.hpp"

#include <optional>
#include <vector>

namespace WayoutPlayer {
class Solution {
  std::vector<Position> clicks;
  bool optimal;

  std::optional<U64> exploredNodes;
  std::optional<U64> distinctNodes;
  std::optional<F64> meanBranchingFactor;

public:
  Solution(std::vector<Position> clickVector, bool isOptimal);

  [[nodiscard]] const std::vector<Position> &getClicks() const;

  [[nodiscard]] bool isOptimal() const;

  bool operator==(const Solution &rhs) const;

  bool operator!=(const Solution &rhs) const;

  [[nodiscard]] const std::optional<U64> &getExploredNodes() const;
  void setExploredNodes(U64 newExploredNodes);

  [[nodiscard]] const std::optional<U64> &getDistinctNodes() const;
  void setDistinctNodes(U64 newDistinctNodes);

  [[nodiscard]] const std::optional<F64> &getMeanBranchingFactor() const;
  void setMeanBranchingFactor(F64 newMeanBranchingFactor);

  [[nodiscard]] std::string toString() const;

  [[nodiscard]] std::string getStatisticsString() const;
};
} // namespace WayoutPlayer