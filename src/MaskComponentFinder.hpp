#pragma once

#include <limits>
#include <queue>
#include <unordered_set>
#include <vector>

#include "Average.hpp"
#include "FloatingPointScreenCoordinates.hpp"
#include "IntegralScreenCoordinates.hpp"
#include "Mask.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
class MaskComponentFinder {
public:
  using ComponentId = U32;
  using IntegralScreenCoordinatesSet = std::unordered_set<IntegralScreenCoordinates>;
  static constexpr auto None = std::numeric_limits<ComponentId>::max();

private:
  std::vector<std::vector<ComponentId>> componentId;
  ComponentId componentCount{};
  std::vector<Average<FloatingPointScreenCoordinates>> componentCentroid;

  void floodFill(IntegralScreenCoordinates start, const std::function<bool(IntegralScreenCoordinates)> &visit) const;

  void findComponents(const Mask &mask);

public:
  explicit MaskComponentFinder(const Mask &mask);

  [[nodiscard]] U32 getHeight() const;

  [[nodiscard]] U32 getWidth() const;

  [[nodiscard]] ComponentId getComponentId(IntegralScreenCoordinates coordinates) const;

  void setComponentId(IntegralScreenCoordinates coordinates, ComponentId newId);

  [[nodiscard]] ComponentId getComponentCount() const;

  [[nodiscard]] ComponentId getComponentSize(ComponentId component) const;

  [[nodiscard]] FloatingPointScreenCoordinates getComponentCentroid(ComponentId component) const;

private:
  [[nodiscard]] IntegralScreenCoordinatesSet getEdgeAroundComponent(ComponentId id) const;

  [[nodiscard]] std::vector<ComponentId> getComponentsTouchingEdge(const IntegralScreenCoordinatesSet &set) const;

public:
  /**
   * Dissolves the component with the specified identifier.
   *
   * This procedure requires the specified component to be perfectly contained within another component.
   *
   * If it touches one other component, this other component will incorporate the edges and area of the component being
   * dissolved. If it touches more than one component, an exception is thrown as it is impossible to determine what
   * component this component is being dissolved into.
   */
  void dissolveComponent(ComponentId id);
};
} // namespace WayoutPlayer
