#include "Position.hpp"

#include <iomanip>
#include <sstream>

namespace WayoutPlayer {
Position::Position(IndexType positionI, IndexType positionJ) : i(positionI), j(positionJ) {
}

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
  stream << "(";
  if (width > 0) {
    stream << std::setw(width);
  }
  stream << static_cast<S64>(i);
  stream << ", ";
  if (width > 0) {
    stream << std::setw(width);
  }
  stream << static_cast<S64>(j);
  stream << ")";
  return stream.str();
}
} // namespace WayoutPlayer