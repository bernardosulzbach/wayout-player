#include "SystemInformation.hpp"

#include "Text.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

namespace WayoutPlayer {
static constexpr auto BytesInKiB = 1024;

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
  maximumResidentSetSize = BytesInKiB * readMaximumResidentSetSize;
#endif
}

U64 SystemInformation::getMaximumResidentSetSizeInBytes() const {
  return maximumResidentSetSize;
}

std::string SystemInformation::getMaximumResidentSetSizeAsHumanReadableString() const {
  auto value = getMaximumResidentSetSizeInBytes();
  std::vector<std::string> units = {"B", "KiB", "MiB", "GiB"};
  U32 multiple = 0;
  while (multiple + 1 < units.size() && value > BytesInKiB) {
    value /= BytesInKiB;
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
