#include "Filesystem.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace WayoutPlayer {
std::string readFile(const std::string &path) {
  std::ifstream input(path);
  if (!input) {
    throw std::runtime_error("Failed to open " + path + ".");
  }
  std::ostringstream buffer;
  buffer << input.rdbuf();
  return buffer.str();
}

} // namespace WayoutPlayer
