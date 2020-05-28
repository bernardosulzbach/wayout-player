#pragma once

#include "Argument.hpp"

#include <string>
#include <vector>

namespace WayoutPlayer {
class ArgumentFactory {
public:
  ArgumentFactory() = delete;

  [[nodiscard]] static Argument makeInputArgument();
  [[nodiscard]] static Argument makeDebuggingPathArgument();
  [[nodiscard]] static Argument makeHelpArgument();

  [[nodiscard]] static std::vector<Argument> makeAllArguments();
};
} // namespace WayoutPlayer
