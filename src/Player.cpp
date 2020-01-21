#include "ArgumentParser.hpp"
#include "Board.hpp"
#include "Filesystem.hpp"
#include "Solver.hpp"
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
    auto solver = Solver();
    solver.getSolverConfiguration().setVerbose(true);
    const auto solution = solver.findSolution(board);
    std::cout << solution.toString() << '\n';
    std::cout << solution.getStatisticsString() << '\n';
  } catch (const std::exception &exception) {
    informAboutException(exception);
  }
  try {
    SystemInformation systemInformation;
    std::cout << "Used " << systemInformation.getMaximumResidentSetSizeAsHumanReadableString() << " of memory." << '\n';
  } catch (const std::exception &exception) {
    informAboutException(exception);
  }
  return 0;
}
