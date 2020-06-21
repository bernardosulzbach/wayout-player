#include "Argument.hpp"

#include <utility>

namespace WayoutPlayer {
Argument::Argument(std::string argumentName, std::string argumentDescription, U32 argumentArity)
    : name(std::move(argumentName)), description(std::move(argumentDescription)), arity(argumentArity) {
}

const std::string &Argument::getName() const {
  return name;
}

const std::string &Argument::getDescription() const {
  return description;
}

U32 Argument::getArity() const {
  return arity;
}

std::string Argument::toHelpString() const {
  const auto *const indentationString = "  ";
  std::string helpString;
  helpString += indentationString;
  helpString += getName();
  for (U32 i = 1; i <= getArity(); i++) {

    helpString += " [argument " + std::to_string(i) + "]";
  }
  helpString += "\n";
  helpString += indentationString;
  helpString += indentationString;
  helpString += getDescription();
  return helpString;
}

bool Argument::operator==(const Argument &rhs) const {
  return name == rhs.name;
}

bool Argument::operator!=(const Argument &rhs) const {
  return !(rhs == *this);
}
} // namespace WayoutPlayer
