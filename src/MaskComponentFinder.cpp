#include "MaskComponentFinder.hpp"

#include <iostream>

namespace WayoutPlayer {
void MaskComponentFinder::findComponents(const Mask &mask) {
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
  for (U32 i = 0; i < mask.getHeight(); i++) {
    for (U32 j = 0; j < mask.getWidth(); j++) {
      if (mask.getValue(i, j) || getComponentId(i, j) != None) {
        continue;
      }
      componentCentroid.emplace_back();
      std::queue<std::pair<U32, U32>> nextPixels;
      componentId[i][j] = componentCount;
      componentCentroid[componentCount].add(FloatingPointScreenCoordinates(i, j));
      nextPixels.push({i, j});
      while (!nextPixels.empty()) {
        U32 pi;
        U32 pj;
        std::tie(pi, pj) = nextPixels.front();
        nextPixels.pop();
        if (componentId[pi][pj] != componentCount) {
          throw std::logic_error("Should not have found another component.");
        }
        for (const auto &neighbor : getNeighbors(pi, pj)) {
          U32 ni;
          U32 nj;
          std::tie(ni, nj) = neighbor;
          if (!mask.getValue(ni, nj) && componentId[ni][nj] == None) {
            componentId[ni][nj] = componentCount;
            componentCentroid[componentCount].add(FloatingPointScreenCoordinates(ni, nj));
            nextPixels.push({ni, nj});
          }
        }
      }
      componentCount++;
    }
  }
}

MaskComponentFinder::MaskComponentFinder(const Mask &mask)
    : componentId(mask.getHeight(), std::vector<IdType>(mask.getWidth(), None)) {
  findComponents(mask);
}

U32 MaskComponentFinder::getHeight() const {
  return componentId.size();
}

U32 MaskComponentFinder::getWidth() const {
  if (componentId.empty()) {
    return 0;
  }
  return componentId.front().size();
}

MaskComponentFinder::IdType MaskComponentFinder::getComponentId(U32 i, U32 j) const {
  return componentId[i][j];
}

MaskComponentFinder::IdType MaskComponentFinder::getComponentCount() const {
  return componentCount;
}

MaskComponentFinder::IdType MaskComponentFinder::getComponentSize(IdType component) const {
  return componentCentroid[component].getSampleCount();
}

FloatingPointScreenCoordinates MaskComponentFinder::getComponentCentroid(IdType component) const {
  return componentCentroid[component].getAverage();
}
} // namespace WayoutPlayer