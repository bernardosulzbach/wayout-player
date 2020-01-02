#include "ArgumentParser.hpp"
#include "Board.hpp"
#include "Filesystem.hpp"
#include "SystemInformation.hpp"

#include <iostream>

using namespace WayoutPlayer;

void informAboutException(const std::exception &exception) {
  std::cout << "Threw an exception." << '\n';
  std::cout << "  " << exception.what() << '\n';
}

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    const auto boardString = readFile(argumentParser.getArgument(1));
    const auto board = Board::fromString(boardString);
    std::cout << board.toString() << '\n';
    const auto solution = board.findSolution();
    std::cout << solution.toString() << '\n';
    std::cout << solution.getStatisticsString() << '\n';
  } catch (const std::exception &exception) {
    informAboutException(exception);
  }
  try {
    SystemInformation systemInformation;
    std::cout << "Used up to " << systemInformation.getMaximumResidentSetSizeAsHumanReadableString() << "." << '\n';
  } catch (const std::exception &exception) {
    informAboutException(exception);
  }
  return 0;
}
