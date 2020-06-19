#pragma once

#include <locale>
#include <sstream>

#include "Types.hpp"

namespace WayoutPlayer {
struct ThousandsFacet : public std::numpunct<char> {
  explicit ThousandsFacet(std::size_t refs = 0) : std::numpunct<char>(refs) {
  }
  char do_thousands_sep() const override {
    return ',';
  }
  std::string do_grouping() const override {
    return "\003";
  }
};

template <typename T> std::string integerToStringWithThousandSeparators(T t) {
  std::locale global;
  std::locale withGroupings(global, new ThousandsFacet);
  std::stringstream stream;
  stream.imbue(withGroupings);
  stream << t;
  return stream.str();
}

std::string toPluralizedString(U64 count, const std::string &singular);

template <typename Iterable> std::string joinIntoString(const Iterable &iterable, const std::string &separator) {
  std::string result;
  auto first = true;
  for (const auto &element : iterable) {
    if (!first) {
      result += separator;
    }
    result += std::to_string(element);
    first = false;
  }
  return result;
}
} // namespace WayoutPlayer