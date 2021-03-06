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

public:
  Solution(std::vector<Position> clickVector, bool isOptimal);

  [[nodiscard]] const std::vector<Position> &getClicks() const;

  [[nodiscard]] bool isOptimal() const;

  bool operator==(const Solution &rhs) const;

  bool operator!=(const Solution &rhs) const;

  [[nodiscard]] std::optional<U64> getExploredNodes() const;
  void setExploredNodes(U64 newExploredNodes);

  [[nodiscard]] std::optional<U64> getDistinctNodes() const;
  void setDistinctNodes(U64 newDistinctNodes);

  [[nodiscard]] std::optional<F64> getMeanBranchingFactor() const;

  [[nodiscard]] std::string toString() const;

  [[nodiscard]] std::string getStatisticsString() const;

  void add(const Solution &other);
};
} // namespace WayoutPlayer