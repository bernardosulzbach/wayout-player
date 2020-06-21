#pragma once

#include "Board.hpp"
#include "Image.hpp"
#include "MaskComponentFinder.hpp"

namespace WayoutPlayer {
class BoardScanner {
  std::optional<std::filesystem::path> debuggingPath;

  [[nodiscard]] bool isDebugging() const noexcept;

  void writeSizeRanges() const;

  void writeColoredByComponent(const MaskComponentFinder &componentFinder,
                               const std::vector<MaskComponentFinder::ComponentId> &componentIds,
                               const std::string &filename) const;

public:
  Board scan(const Image &image);

  void setDebuggingPath(std::optional<std::filesystem::path> newDebuggingPath);
};
} // namespace WayoutPlayer