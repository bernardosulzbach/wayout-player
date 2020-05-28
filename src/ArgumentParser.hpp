#pragma once

#include "Argument.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace WayoutPlayer {
class ArgumentParser {
  std::unordered_map<Argument, std::vector<std::string>> arguments;

public:
  ArgumentParser(const std::vector<std::string> &commandLineArguments);

  [[nodiscard]] bool hasArgument(const Argument &argument) const;

  [[nodiscard]] std::vector<std::string> getArgument(const Argument &argument) const;
};
} // namespace WayoutPlayer
