#include "MaskComponentFinder.hpp"

#include <iostream>

#include "GetNeighbors.hpp"
#include "Text.hpp"

namespace WayoutPlayer {
using ComponentId = MaskComponentFinder::ComponentId;
using IntegralScreenCoordinatesVector = std::vector<IntegralScreenCoordinates>;

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
      for (const auto &neighborPosition : getNeighbors(*this, currentPosition)) {
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
      const auto coordinates = IntegralScreenCoordinates(i, j);
      if (mask.getValue(coordinates) || getComponentId(coordinates) != None) {
        continue;
      }
      componentCentroid.emplace_back();
      std::queue<IntegralScreenCoordinates> nextPixels;
      componentId[i][j] = componentCount;
      componentCentroid[componentCount].add(FloatingPointScreenCoordinates(i, j));
      nextPixels.push({i, j});
      while (!nextPixels.empty()) {
        const auto pixelCoordinates = nextPixels.front();
        nextPixels.pop();
        if (getComponentId(pixelCoordinates) != componentCount) {
          throw std::logic_error("Should not have found another component.");
        }
        for (const auto &neighbor : getNeighbors(*this, pixelCoordinates)) {
          if (!mask.getValue(neighbor) && getComponentId(neighbor) == None) {
            setComponentId(neighbor, componentCount);
            const auto ni = neighbor.getI();
            const auto nj = neighbor.getJ();
            componentCentroid[componentCount].add(FloatingPointScreenCoordinates(ni, nj));
            nextPixels.push(neighbor);
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

MaskComponentFinder::ComponentId
MaskComponentFinder::getComponentId(const IntegralScreenCoordinates coordinates) const {
  return componentId[coordinates.getI()][coordinates.getJ()];
}

void MaskComponentFinder::setComponentId(const IntegralScreenCoordinates coordinates, const ComponentId newId) {
  componentId[coordinates.getI()][coordinates.getJ()] = newId;
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
      if (getComponentId(IntegralScreenCoordinates(i, j)) == id) {
        for (const auto neighbor : getNeighbors(*this, IntegralScreenCoordinates(i, j))) {
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
    for (const auto neighbor : getNeighbors(*this, coordinates)) {
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
    const auto numberString = std::to_string(touchingEdge.size());
    std::string message = "Expected exactly 2 components to be touching the edge, but found " + numberString;
    std::sort(std::begin(touchingEdge), std::end(touchingEdge));
    const auto componentList = joinIntoString(touchingEdge, ", ");
    message += " (" + joinIntoString(touchingEdge, ", ") + ").";
    throw std::runtime_error(message);
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