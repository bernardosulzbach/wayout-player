#include "Mask.hpp"

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

void Mask::setValue(const U32 i, const U32 j, const bool value) {
  data[i][j] = value;
}
}; // namespace WayoutPlayer