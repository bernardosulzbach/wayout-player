#pragma once

#include <limits>
#include <queue>
#include <vector>

#include "Mask.hpp"
#include "Types.hpp"

namespace WayoutPlayer {
class MaskComponentFinder {
  using IdType = U32;

  std::vector<std::vector<IdType>> componentId;
  IdType componentCount{};
  std::vector<IdType> componentSize;

  void findComponents(const Mask &mask);

public:
  static constexpr auto None = std::numeric_limits<IdType>::max();

  explicit MaskComponentFinder(const Mask &mask);

  [[nodiscard]] U32 getHeight() const;

  [[nodiscard]] U32 getWidth() const;

  [[nodiscard]] IdType getComponent(U32 i, U32 j) const;

  [[nodiscard]] IdType getComponentCount() const;

  [[nodiscard]] IdType getComponentSize(IdType component) const;
};
} // namespace WayoutPlayer
