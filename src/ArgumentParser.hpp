#pragma once

#include <optional>
#include <string>
#include <vector>

class ArgumentParser {
  std::vector<std::string> arguments;

public:
  void parseArguments(int argc, char **argv);

  std::string getArgument(std::size_t position) const;
};