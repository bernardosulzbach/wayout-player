#include "ArgumentFactory.hpp"
#include "ArgumentParser.hpp"
#include "Board.hpp"
#include "BoardScanner.hpp"
#include "Filesystem.hpp"
#include "Solver.hpp"
#include "SystemInformation.hpp"

#include <iostream>
#include <span>

using namespace WayoutPlayer;

void informAboutException(const std::exception &exception) {
  std::cout << "Threw an exception." << '\n';
  std::cout << "  " << exception.what() << '\n';
}

Board boardFromInputPath(const ArgumentParser &argumentParser) {
  const auto inputPath = argumentParser.getArgument(ArgumentFactory::makeInputArgument()).at(0);
  if (inputPath.ends_with(".txt")) {
    const auto boardString = readFile(inputPath);
    return Board::fromString(boardString);
  }
  if (inputPath.ends_with(".png")) {
    const auto image = readImageFromFile(inputPath);
    BoardScanner boardScanner;
    if (argumentParser.hasArgument(ArgumentFactory::makeDebuggingPathArgument())) {
      const auto debuggingPath = argumentParser.getArgument(ArgumentFactory::makeDebuggingPathArgument()).at(0);
      boardScanner.setDebuggingPath(debuggingPath);
    }
    return boardScanner.scan(image);
  }
  throw std::runtime_error("Could not figure out the file type from the extension.");
}

int main(int argc, char **argv) {
  try {
    std::vector<std::string> commandLineArguments(argc - 1);
    std::span<char *, std::dynamic_extent> argumentSpan(argv, argc);
    for (int i = 1; i < argc; i++) {
      commandLineArguments[i - 1] = argumentSpan[i];
    }
    ArgumentParser argumentParser(commandLineArguments);
    if (argumentParser.hasArgument(ArgumentFactory::makeHelpArgument())) {
      std::cout << "Wayout Player Options" << '\n';
      for (const auto &argument : ArgumentFactory::makeAllArguments()) {
        std::cout << '\n' << argument.toHelpString() << '\n';
      }
      return 0;
    }
    const auto board = boardFromInputPath(argumentParser);
    std::cout << "Solving the following board." << '\n';
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
