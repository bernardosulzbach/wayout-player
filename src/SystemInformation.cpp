#include "SystemInformation.hpp"

#include "Text.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace WayoutPlayer {
SystemInformation::SystemInformation() {
#ifdef __linux__
  rusage resourceUsage{};
  const auto result = getrusage(RUSAGE_SELF, &resourceUsage);
  if (result != 0) {
    std::cerr << "Failed to get resource usage from POSIX.\n";
    return;
  }
  S64 readMaximumResidentSetSize = resourceUsage.ru_maxrss;
  if (readMaximumResidentSetSize < 0) {
    std::cerr << "POSIX maximum RSS is negative.\n";
    return;
  }
  // This is expressed in KB.
  //
  //   ru_maxrss (since Linux 2.6.32)
  //   This is the maximum resident set size used (in kilobytes).
  //
  // Bernardo Sulzbach: they actually meant kibibytes.
  maximumResidentSetSize = 1024 * readMaximumResidentSetSize;
#endif
}

U64 SystemInformation::getMaximumResidentSetSizeInBytes() const {
  return maximumResidentSetSize;
}

std::string SystemInformation::getMaximumResidentSetSizeAsHumanReadableString() const {
  F64 value = maximumResidentSetSize;
  std::vector<std::string> units = {"B", "KiB", "MiB", "GiB"};
  U32 multiple = 0;
  while (multiple + 1 < units.size() && value > 1024.0) {
    value /= 1024.0;
    multiple++;
  }
  std::stringstream stream;
  stream << integerToStringWithThousandSeparators(maximumResidentSetSize) << " B";
  stream << " ";
  stream << "(";
  stream << static_cast<U64>(std::ceil(value)) << " " << units[multiple];
  stream << ")";
  return stream.str();
}
} // namespace WayoutPlayer
