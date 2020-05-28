#include "Text.hpp"

namespace WayoutPlayer {
std::string toPluralizedString(U64 count, const std::string &singular) {
  if (count == 1) {
    return std::to_string(count) + " " + singular;
  }
  return std::to_string(count) + " " + singular + "s";
}
} // namespace WayoutPlayer