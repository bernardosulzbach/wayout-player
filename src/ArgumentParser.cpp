#include "ArgumentParser.hpp"

#include "ArgumentFactory.hpp"

#include <iostream>
#include <stdexcept>

namespace WayoutPlayer {
ArgumentParser::ArgumentParser(const std::vector<std::string> &commandLineArguments) {
  const auto possibleArguments = ArgumentFactory::makeAllArguments();
  for (std::size_t i = 0; i < commandLineArguments.size(); i++) {
    const auto asString = commandLineArguments[i];
    auto matchFound = false;
    for (const auto &possibleArgument : possibleArguments) {
      if (asString.starts_with("--") && asString.substr(2) == possibleArgument.getName()) {
        matchFound = true;
        if (arguments.contains(possibleArgument)) {
          throw std::logic_error("The argument " + asString + " is repeated.");
        }
        const auto stringsLeft = commandLineArguments.size() - 1 - i;
        if (stringsLeft < possibleArgument.getArity()) {
          const auto arityAsString = std::to_string(possibleArgument.getArity());
          throw std::logic_error("Expected at least " + arityAsString + " string(s) to make an argument.");
        }
        std::vector<std::string> values;
        values.reserve(possibleArgument.getArity());
        for (std::size_t j = 1; j <= possibleArgument.getArity(); j++) {
          values.emplace_back(commandLineArguments[i + j]);
        }
        arguments[possibleArgument] = values;
        i += possibleArgument.getArity();
        break;
      }
    }
    if (!matchFound) {
      throw std::logic_error("Could not recognize argument " + asString + ".");
    }
  }
}

bool ArgumentParser::hasArgument(const Argument &argument) const {
  return arguments.contains(argument);
}

std::vector<std::string> ArgumentParser::getArgument(const Argument &argument) const {
  if (!hasArgument(argument)) {
    throw std::runtime_error("Trying to get argument " + argument.getName() + ", but it is not present.");
  }
  return arguments.at(argument);
}
} // namespace WayoutPlayer
