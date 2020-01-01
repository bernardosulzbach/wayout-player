#include "ArgumentParser.hpp"

#include <stdexcept>

void ArgumentParser::parseArguments(int argc, char **argv) {
  for (int i = 0; i < argc; i++) {
    arguments.emplace_back(argv[i]);
  }
}

std::string ArgumentParser::getArgument(std::size_t position) const {
  if (position >= arguments.size()) {
    throw std::invalid_argument("Not enough arguments.");
  }
  return arguments[position];
}
