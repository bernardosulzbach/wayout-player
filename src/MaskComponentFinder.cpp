#include "MaskComponentFinder.hpp"

#include <iostream>

namespace WayoutPlayer {
using ComponentId = MaskComponentFinder::ComponentId;
using IntegralScreenCoordinatesVector = std::vector<IntegralScreenCoordinates>;
IntegralScreenCoordinatesVector MaskComponentFinder::getNeighbors(const IntegralScreenCoordinates coordinates) const {
  std::vector<IntegralScreenCoordinates> neighbors;
  const auto i = coordinates.getI();
  const auto j = coordinates.getJ();
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
}

using FloodFillVisitor = std::function<bool(IntegralScreenCoordinates)>;
void MaskComponentFinder::floodFill(const IntegralScreenCoordinates start, const FloodFillVisitor &visit) const {
  std::queue<IntegralScreenCoordinates> next;
  next.push(start);
  std::unordered_set<IntegralScreenCoordinates> enqueued;
  enqueued.insert(start);
  while (!next.empty()) {
    const auto currentPosition = next.front();
    next.pop();
    if (visit(currentPosition)) {
      for (const auto &neighborPosition : getNeighbors(currentPosition)) {
        if (!enqueued.contains(neighborPosition)) {
          next.push(neighborPosition);
          enqueued.insert(neighborPosition);
        }
      }
    }
  }
}

void MaskComponentFinder::findComponents(const Mask &mask) {
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
        U32 pi = 0;
        U32 pj = 0;
        std::tie(pi, pj) = nextPixels.front();
        nextPixels.pop();
        if (componentId[pi][pj] != componentCount) {
          throw std::logic_error("Should not have found another component.");
        }
        for (const auto &neighbor : getNeighbors({pi, pj})) {
          const auto ni = neighbor.getI();
          const auto nj = neighbor.getJ();
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
    : componentId(mask.getHeight(), std::vector<ComponentId>(mask.getWidth(), None)) {
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

MaskComponentFinder::ComponentId MaskComponentFinder::getComponentId(U32 i, U32 j) const {
  return componentId[i][j];
}

MaskComponentFinder::ComponentId MaskComponentFinder::getComponentId(IntegralScreenCoordinates coordinates) const {
  return getComponentId(coordinates.getI(), coordinates.getJ());
}

MaskComponentFinder::ComponentId MaskComponentFinder::getComponentCount() const {
  return componentCount;
}

MaskComponentFinder::ComponentId MaskComponentFinder::getComponentSize(ComponentId component) const {
  return componentCentroid[component].getSampleCount();
}

FloatingPointScreenCoordinates MaskComponentFinder::getComponentCentroid(ComponentId component) const {
  return componentCentroid[component].getAverage();
}

using IntegralScreenCoordinatesSet = MaskComponentFinder::IntegralScreenCoordinatesSet;
IntegralScreenCoordinatesSet MaskComponentFinder::getEdgeAroundComponent(const ComponentId id) const {
  IntegralScreenCoordinatesSet set;
  for (U32 i = 0; i < getHeight(); i++) {
    for (U32 j = 0; j < getWidth(); j++) {
      if (componentId[i][j] == id) {
        for (const auto neighbor : getNeighbors(IntegralScreenCoordinates(i, j))) {
          if (getComponentId(neighbor) == None && !set.contains(neighbor)) {
            floodFill(neighbor, [this, &set](const IntegralScreenCoordinates coordinates) {
              if (set.contains(coordinates)) {
                return false;
              }
              if (getComponentId(coordinates) != None) {
                return false;
              }
              set.insert(coordinates);
              return true;
            });
          }
        }
      }
    }
  }
  return set;
}

std::vector<ComponentId> MaskComponentFinder::getComponentsTouchingEdge(const IntegralScreenCoordinatesSet &set) const {
  std::unordered_set<ComponentId> components;
  for (const auto coordinates : set) {
    for (const auto neighbor : getNeighbors(coordinates)) {
      const auto neighborId = getComponentId(neighbor);
      if (neighborId != None) {
        components.insert(neighborId);
      }
    }
  }
  return std::vector<ComponentId>(std::begin(components), std::end(components));
}

void MaskComponentFinder::dissolveComponent(const ComponentId id) {
  const auto edge = getEdgeAroundComponent(id);
  auto touchingEdge = getComponentsTouchingEdge(edge);
  if (touchingEdge.size() != 2) {
    throw std::runtime_error("Expected exactly two components to be touching the edge.");
  }
  touchingEdge.erase(std::remove(std::begin(touchingEdge), std::end(touchingEdge), id), std::end(touchingEdge));
  if (touchingEdge.size() != 1) {
    throw std::runtime_error("Expected one component from the touching set to be the dissolved component.");
  }
  const auto acquiringComponent = touchingEdge[0];
  for (U32 i = 0; i < getHeight(); i++) {
    for (U32 j = 0; j < getWidth(); j++) {
      if (componentId[i][j] == id) {
        componentId[i][j] = acquiringComponent;
        componentCentroid[acquiringComponent].add(FloatingPointScreenCoordinates(i, j));
      }
    }
  }
  for (const auto coordinates : edge) {
    componentId[coordinates.getI()][coordinates.getJ()] = acquiringComponent;
    componentCentroid[acquiringComponent].add(FloatingPointScreenCoordinates(coordinates.getI(), coordinates.getJ()));
  }
  componentCentroid[id] = Average<FloatingPointScreenCoordinates>();
}
}; // namespace WayoutPlayer