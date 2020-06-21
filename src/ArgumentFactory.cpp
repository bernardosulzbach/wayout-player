#include "ArgumentFactory.hpp"

namespace WayoutPlayer {
Argument ArgumentFactory::makeInputArgument() {
  return Argument("input", "Sets the path for the input file.", 1);
}

Argument ArgumentFactory::makeDebuggingPathArgument() {
  return Argument("debugging-path", "Sets the path for debugging files.", 1);
}

Argument ArgumentFactory::makeHelpArgument() {
  return Argument("help", "Prints help information.", 0);
}

std::vector<Argument> ArgumentFactory::makeAllArguments() {
  return {makeInputArgument(), makeDebuggingPathArgument(), makeHelpArgument()};
}
} // namespace WayoutPlayer