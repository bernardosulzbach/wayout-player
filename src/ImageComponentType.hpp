#pragma once

#include <string>

namespace WayoutPlayer {
enum class ImageComponentType { Top, RaisedSide, LoweredSide, Arrow };

std::string imageComponentTypeToString(ImageComponentType imageComponentType);
} // namespace WayoutPlayer