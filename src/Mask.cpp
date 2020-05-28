#include "Mask.hpp"

#include <queue>
#include <vector>

namespace WayoutPlayer {
Mask::Mask(U32 maskHeight, U32 maskWidth) : data(maskHeight, std::vector<bool>(maskWidth)) {
}

U32 Mask::getHeight() const {
  return data.size();
}

U32 Mask::getWidth() const {
  if (data.empty()) {
    return 0;
  }
  return data.front().size();
}

bool Mask::getValue(const U32 i, const U32 j) const {
  return data[i][j];
}

void Mask::open(const std::function<bool(U32, U32)> &predicate) {
  const auto getNeighbors = [this](U32 i, U32 j) {
    std::vector<std::pair<U32, U32>> neighbors;
    if (i > 0) {
      neighbors.emplace_back(i - 1, j);
    }
    if (j > 0) {
      neighbors.emplace_back(i, j - 1);
    }
    if (i + 1 < getHeight()) {
      neighbors.emplace_back(i + 1, j);
    }
    if (j + 1 < getWidth()) {
      neighbors.emplace_back(i, j + 1);
    }
    return neighbors;
  };
  std::queue<std::pair<U32, U32>> next;
  const auto testAndAddNeighbors = [this, &predicate, &getNeighbors, &next](U32 i, U32 j) {
    for (const auto &neighbor : getNeighbors(i, j)) {
      U32 ni;
      U32 nj;
      std::tie(ni, nj) = neighbor;
      if (!getValue(ni, nj) && predicate(ni, nj)) {
        setValue(ni, nj, true);
        next.push({ni, nj});
      }
    }
  };
  for (U32 i = 0; i < getHeight(); i++) {
    for (U32 j = 0; j < getWidth(); j++) {
      if (getValue(i, j)) {
        testAndAddNeighbors(i, j);
      }
    }
  }
  while (!next.empty()) {
    U32 i;
    U32 j;
    std::tie(i, j) = next.front();
    next.pop();
    testAndAddNeighbors(i, j);
  }
}

void Mask::setValue(const U32 i, const U32 j, const bool value) {
  data[i][j] = value;
}
}; // namespace WayoutPlayer