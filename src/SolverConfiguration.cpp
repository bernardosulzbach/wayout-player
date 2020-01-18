#include "SolverConfiguration.hpp"

namespace WayoutPlayer {
std::size_t SolverConfiguration::getMaximumBoardHashTableSize() const {
  return maximumBoardHashTableSize;
}

void SolverConfiguration::setMaximumBoardHashTableSize(std::size_t newMaximumBoardHashTableSize) {
  maximumBoardHashTableSize = newMaximumBoardHashTableSize;
}

std::size_t SolverConfiguration::getMaximumStateQueueSize() const {
  return maximumStateQueueSize;
}

void SolverConfiguration::setMaximumStateQueueSize(std::size_t newMaximumStateQueueSize) {
  maximumStateQueueSize = newMaximumStateQueueSize;
}

bool SolverConfiguration::isFlippingOnlyUp() const {
  return flipOnlyUp;
}

void SolverConfiguration::setFlipOnlyUp(bool newFlipOnlyUp) {
  flipOnlyUp = newFlipOnlyUp;
}

bool SolverConfiguration::isVerbose() const {
  return verbose;
}

void SolverConfiguration::setVerbose(bool newVerbose) {
  verbose = newVerbose;
}
} // namespace WayoutPlayer
