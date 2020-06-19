#include "ImageComponentType.hpp"

#include <stdexcept>
#include <string>

namespace WayoutPlayer {
std::string imageComponentTypeToString(const ImageComponentType imageComponentType) {
  switch (imageComponentType) {
  case ImageComponentType::Top:
    return "Top";
  case ImageComponentType::RaisedSide:
    return "Raised side";
  case ImageComponentType::LoweredSide:
    return "Lowered side";
  case ImageComponentType::Arrow:
    return "Arrow";
  case ImageComponentType::Disk:
    return "Disk";
  }
  throw std::runtime_error("Cannot get string for component type.");
}
} // namespace WayoutPlayer
