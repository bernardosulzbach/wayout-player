#include "Filesystem.hpp"

#include <fstream>

namespace WayoutPlayer {
std::string readFile(const std::filesystem::path &path) {
  std::ostringstream buffer;
  std::ifstream input(path.native());
  buffer << input.rdbuf();
  return buffer.str();
}
} // namespace WayoutPlayer
