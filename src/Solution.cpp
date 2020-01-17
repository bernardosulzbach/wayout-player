#include "Solution.hpp"
#include "Text.hpp"

#include <iomanip>
#include <sstream>
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

std::optional<U64> Solution::getExploredNodes() const {
  return exploredNodes;
}

void Solution::setExploredNodes(const U64 newExploredNodes) {
  exploredNodes = newExploredNodes;
}

std::optional<U64> Solution::getDistinctNodes() const {
  return distinctNodes;
}

void Solution::setDistinctNodes(const U64 newDistinctNodes) {
  distinctNodes = newDistinctNodes;
}

std::optional<F64> Solution::getMeanBranchingFactor() const {
  if (exploredNodes && distinctNodes) {
    return *distinctNodes / static_cast<F64>(*exploredNodes);
  }
  return std::nullopt;
}

std::string Solution::toString() const {
  std::string string;
  if (isOptimal()) {
    string += "Found an optimal solution:";
  } else {
    string += "Found a solution:";
  }
  string += '\n';
  for (const auto click : clicks) {
    string += "  ";
    string += click.toString();
    string += '\n';
  }
  string.pop_back();
  return string;
}

std::string Solution::getStatisticsString() const {
  std::string string;
  if (getExploredNodes()) {
    string += "Explored nodes: " + integerToStringWithThousandSeparators(getExploredNodes().value());
  }
  if (getDistinctNodes()) {
    if (!string.empty()) {
      string += '\n';
    }
    string += "Distinct nodes found: " + integerToStringWithThousandSeparators(getDistinctNodes().value());
  }
  if (getMeanBranchingFactor()) {
    if (!string.empty()) {
      string += '\n';
    }
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << getMeanBranchingFactor().value();
    string += "Mean branching factor: " + stream.str();
  }
  return string;
}

void Solution::add(const Solution &other) {
  clicks.insert(std::end(clicks), std::begin(other.clicks), std::end(other.clicks));
  optimal = optimal && other.isOptimal();

  if (exploredNodes && other.exploredNodes) {
    setExploredNodes(*getExploredNodes() + *other.getExploredNodes());
  } else {
    exploredNodes = std::nullopt;
  }

  if (distinctNodes && other.distinctNodes) {
    setDistinctNodes(*getDistinctNodes() + *other.getDistinctNodes());
  } else {
    distinctNodes = std::nullopt;
  }
}
} // namespace WayoutPlayer