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
} // namespace WayoutPlayer