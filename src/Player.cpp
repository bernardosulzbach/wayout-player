#include "Board.hpp"

#include <iostream>

using namespace WayOutPlayer;

int main() {
  try {
    const auto boardString = "..###\n.....\n#....\n####.\n.#...";
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
