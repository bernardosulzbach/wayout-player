#pragma once

#include "Board.hpp"
#include "Image.hpp"

namespace WayoutPlayer {
class BoardScanner {
  std::optional<std::filesystem::path> debuggingPath;

public:
  BoardScanner();

  Board scan(const Image &image);

  void setDebuggingPath(std::optional<std::filesystem::path> newDebuggingPath);
};
} // namespace WayoutPlayer