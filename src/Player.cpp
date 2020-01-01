#include "Board.hpp"
#include "Filesystem.hpp"

#include <iostream>

using namespace WayoutPlayer;

int main() {
  try {
    const auto boardString = readFile("../input/M.txt");
    const auto board = Board::fromString(boardString);
    std::cout << board.toString() << '\n';
    for (const auto position : board.findOptimalSolution(true)) {
      std::cout << position.toString(2) << '\n';
    }
  } catch (std::exception &exception) {
    std::cout << "Threw an exception." << '\n';
    std::cout << "  " << exception.what() << '\n';
  }
  return 0;
}
