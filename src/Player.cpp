#include "ArgumentParser.hpp"
#include "Board.hpp"
#include "Filesystem.hpp"

#include <iostream>

using namespace WayoutPlayer;

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    const auto boardString = readFile(argumentParser.getArgument(1));
    const auto board = Board::fromString(boardString);
    std::cout << board.toString() << '\n';
    std::cout << board.findOptimalSolution().toString() << '\n';
  } catch (std::exception &exception) {
    std::cout << "Threw an exception." << '\n';
    std::cout << "  " << exception.what() << '\n';
  }
  return 0;
}
