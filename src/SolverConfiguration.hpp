#pragma once

#include <string>

namespace WayoutPlayer {
class SolverConfiguration {
  std::size_t maximumBoardHashTableSize = 1u << 30u;
  std::size_t maximumStateQueueSize = 1u << 30u;

  bool flipOnlyUp = false;
  bool verbose = false;

public:
  [[nodiscard]] std::size_t getMaximumBoardHashTableSize() const;
  void setMaximumBoardHashTableSize(size_t newMaximumBoardHashTableSize);

  [[nodiscard]] std::size_t getMaximumStateQueueSize() const;
  void setMaximumStateQueueSize(size_t newMaximumStateQueueSize);

  [[nodiscard]] bool isFlippingOnlyUp() const;
  void setFlipOnlyUp(bool newFlipOnlyUp);

  [[nodiscard]] bool isVerbose() const;
  void setVerbose(bool newVerbose);
};
} // namespace WayoutPlayer
