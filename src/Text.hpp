#pragma once

#include <locale>
#include <sstream>

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
