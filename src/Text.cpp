#include "Text.hpp"

std::string toPluralizedString(U64 count, const std::string &singular) {
  if (count == 1) {
    return std::to_string(count) + " " + singular;
  }
  return std::to_string(count) + " " + singular + "s";
}
