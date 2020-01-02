#include "Filesystem.hpp"

#include <fstream>
#include <sstream>

namespace WayoutPlayer {
std::string readFile(const std::string &path) {
  std::ostringstream buffer;
  std::ifstream input(path);
  buffer << input.rdbuf();
  return buffer.str();
}
} // namespace WayoutPlayer
