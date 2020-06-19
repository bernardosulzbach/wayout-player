#pragma once

#include <string>

namespace WayoutPlayer {
enum class ImageComponentType { Top, RaisedSide, LoweredSide, Arrow, Disk };

std::string imageComponentTypeToString(ImageComponentType imageComponentType);
} // namespace WayoutPlayer