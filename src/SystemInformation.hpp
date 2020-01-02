#pragma once

#include <string>

#include "Types.hpp"

#ifdef __linux__
#include <sys/resource.h>
#endif

namespace WayoutPlayer {
class SystemInformation {
  U64 maximumResidentSetSize = 0;

public:
  SystemInformation();

  [[nodiscard]] U64 getMaximumResidentSetSizeInBytes() const;

  [[nodiscard]] std::string getMaximumResidentSetSizeAsHumanReadableString() const;
};
} // namespace WayoutPlayer
