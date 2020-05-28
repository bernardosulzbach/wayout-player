#pragma once

#include "Types.hpp"

#include <string>
#include <vector>

namespace WayoutPlayer {
class Argument {
  std::string name;
  std::string description;
  U32 arity;

public:
  Argument(std::string argumentName, std::string argumentDescription, U32 argumentArity);

  [[nodiscard]] const std::string &getName() const;

  [[nodiscard]] const std::string &getDescription() const;

  [[nodiscard]] std::string toHelpString() const;

  [[nodiscard]] U32 getArity() const;

  bool operator==(const Argument &rhs) const;
  bool operator!=(const Argument &rhs) const;
};
} // namespace WayoutPlayer

template <> struct std::hash<WayoutPlayer::Argument> {
  std::size_t operator()(const WayoutPlayer::Argument &argument) const {
    return std::hash<std::string>()(argument.getName());
  }
};