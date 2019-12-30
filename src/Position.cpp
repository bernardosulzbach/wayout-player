#include "Position.hpp"

#include <iomanip>
#include <sstream>

namespace WayOutPlayer {
bool Position::operator==(const Position &rhs) const {
  return i == rhs.i && j == rhs.j;
}

bool Position::operator!=(const Position &rhs) const {
  return !(rhs == *this);
}

std::string Position::toString(std::size_t width) const {
  std::stringstream stream;
  if (width > 0) {
    stream << std::setw(width);
  }
  stream << "(" << i << ", " << j << ")";
  return stream.str();
}
} // namespace WayOutPlayer