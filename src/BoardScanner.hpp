#pragma once

#include "Board.hpp"
#include "Image.hpp"

namespace WayoutPlayer {
class BoardScanner {
  std::optional<std::filesystem::path> debuggingPath;

  [[nodiscard]] bool isDebugging() const noexcept;

  void writeSizeRanges() const;

public:
  Board scan(const Image &image);

  void setDebuggingPath(std::optional<std::filesystem::path> newDebuggingPath);
};
} // namespace WayoutPlayer