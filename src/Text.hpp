#pragma once

#include <locale>
#include <sstream>
#include <type_traits>

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
  static_assert(std::is_integral<T>::value);
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
    if constexpr (std::is_same<decltype(element), std::string>::value) {
      result += element;
    } else {
      result += std::to_string(element);
    }
    first = false;
  }
  return result;
}
} // namespace WayoutPlayer