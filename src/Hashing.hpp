#pragma once

#include "Types.hpp"
#include <string>

namespace WayoutPlayer {

/**
 * Returns the first 64 bits of the SHA-512 hash of the string.
 */
U64 hashString(const std::string &string);
} // namespace WayoutPlayer
