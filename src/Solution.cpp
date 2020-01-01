#include "Solution.hpp"

#include <utility>

namespace WayoutPlayer {
Solution::Solution(std::vector<Position> clickVector, bool isOptimal)
    : clicks(std::move(clickVector)), optimal(isOptimal) {
}

const std::vector<Position> &Solution::getClicks() const {
  return clicks;
}

bool Solution::isOptimal() const {
  return optimal;
}

bool Solution::operator==(const Solution &rhs) const {
  return clicks == rhs.clicks && optimal == rhs.optimal;
}

bool Solution::operator!=(const Solution &rhs) const {
  return !(rhs == *this);
}

const std::optional<U64> &Solution::getExploredNodes() const {
  return exploredNodes;
}

void Solution::setExploredNodes(const U64 newExploredNodes) {
  exploredNodes = newExploredNodes;
}

const std::optional<U64> &Solution::getDistinctNodes() const {
  return distinctNodes;
}

void Solution::setDistinctNodes(const U64 newDistinctNodes) {
  distinctNodes = newDistinctNodes;
}

std::string Solution::toString() const {
  std::string string;
  if (isOptimal()) {
    string += "Optimal solution:";
  } else {
    string += "Solution:";
  }
  string += '\n';
  for (const auto click : clicks) {
    string += ' ';
    string += click.toString(2);
    string += '\n';
  }
  string.pop_back();
  return string;
}

std::string Solution::getStatisticsString() const {
  std::string string;
  if (getExploredNodes()) {
    string += "Explored nodes: " + std::to_string(getExploredNodes().value());
  }
  if (getDistinctNodes()) {
    if (!string.empty()) {
      string += '\n';
    }
    string += "Distinct nodes found: " + std::to_string(getDistinctNodes().value());
  }
  return string;
}
} // namespace WayoutPlayer