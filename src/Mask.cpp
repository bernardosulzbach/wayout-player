#include "Mask.hpp"

#include <queue>
#include <vector>

#include "GetNeighbors.hpp"

namespace WayoutPlayer {
Mask::Mask(const U32 maskHeight, const U32 maskWidth) : data(maskHeight, std::vector<bool>(maskWidth)) {
}

U32 Mask::getHeight() const noexcept {
  return data.size();
}

U32 Mask::getWidth() const noexcept {
  if (data.empty()) {
    return 0;
  }
  return data.front().size();
}

bool Mask::getValue(const IntegralScreenCoordinates coordinates) const {
  return data[coordinates.getI()][coordinates.getJ()];
}

void Mask::setValue(const IntegralScreenCoordinates coordinates, const bool value) {
  data[coordinates.getI()][coordinates.getJ()] = value;
}

void Mask::open(const std::function<bool(IntegralScreenCoordinates)> &predicate) {
  std::queue<IntegralScreenCoordinates> next;
  const auto testAndAddNeighbors = [this, &predicate, &next](const IntegralScreenCoordinates coordinates) {
    for (const auto &neighbor : getNeighbors(*this, coordinates)) {
      if (!getValue(neighbor) && predicate(neighbor)) {
        setValue(neighbor, true);
        next.push(neighbor);
      }
    }
  };
  for (U32 i = 0; i < getHeight(); i++) {
    for (U32 j = 0; j < getWidth(); j++) {
      const auto coordinates = IntegralScreenCoordinates(i, j);
      if (getValue(coordinates)) {
        testAndAddNeighbors(coordinates);
      }
    }
  }
  while (!next.empty()) {
    const auto coordinates = next.front();
    next.pop();
    testAndAddNeighbors(coordinates);
  }
}
}; // namespace WayoutPlayer