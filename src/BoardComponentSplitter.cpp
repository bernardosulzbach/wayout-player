#include "BoardComponentSplitter.hpp"

#include <functional>

namespace WayoutPlayer {
std::vector<Board> splitComponents(const Board &board) {
  using TagType = U32;
  const auto NoTag = std::numeric_limits<TagType>::max();
  using TaggedTileMatrix = std::vector<std::vector<TagType>>;
  const auto rowCount = board.getRowCount();
  const auto columnCount = board.getColumnCount();
  TaggedTileMatrix tagMatrix(rowCount, std::vector<TagType>(columnCount, NoTag));
  TagType currentTag = 0;
  auto foundTwin = false;
  using PositionFunction = std::function<void(S32, S32)>;
  const PositionFunction propagateTag = [board, &tagMatrix, &currentTag, &foundTwin, &propagateTag](S32 i, S32 j) {
    if (!board.hasTile(i, j)) {
      return;
    }
    if (tagMatrix[i][j] == NoTag) {
      tagMatrix[i][j] = currentTag;
      // Propagate to all other twins.
      if (board.getTile(i, j).type == TileType::Twin) {
        // This check is important to prevent an infinite recursion.
        if (!foundTwin) {
          foundTwin = true;
          for (S32 oi = 0; oi < board.getRowCount(); oi++) {
            for (S32 oj = 0; oj < board.getColumnCount(); oj++) {
              if (oi == i && oj == j) {
                continue;
              }
              if (board.hasTile(oi, oj)) {
                if (board.getTile(oi, oj).type == TileType::Twin) {
                  propagateTag(oi, oj);
                }
              }
            }
          }
        }
      }
      propagateTag(i - 1, j);
      propagateTag(i, j - 1);
      propagateTag(i, j + 1);
      propagateTag(i + 1, j);
    } else if (tagMatrix[i][j] != currentTag) {
      throw std::runtime_error("Should not happen: found a tile tagged with another tag while propagating a tag.");
    }
  };
  for (S32 i = 0; i < rowCount; i++) {
    for (S32 j = 0; j < columnCount; j++) {
      if (board.hasTile(i, j)) {
        if (tagMatrix[i][j] == NoTag) {
          propagateTag(i, j);
          currentTag++;
        }
      }
    }
  }
  const auto componentCount = currentTag;
  const auto emptyRow = std::vector<std::optional<Tile>>(columnCount);
  using TileMatrix = std::vector<std::vector<std::optional<Tile>>>;
  const auto emptyTileMatrix = TileMatrix(rowCount, emptyRow);
  auto tileMatrices = std::vector<TileMatrix>(componentCount, emptyTileMatrix);
  for (S32 i = 0; i < rowCount; i++) {
    for (S32 j = 0; j < columnCount; j++) {
      if (tagMatrix[i][j] != NoTag) {
        tileMatrices[tagMatrix[i][j]][i][j] = board.getTile(i, j);
      }
    }
  }
  auto components = std::vector<Board>{};
  for (const auto &tileMatrix : tileMatrices) {
    components.emplace_back(tileMatrix);
  }
  return components;
}

Board mergeComponents(const std::vector<Board> &components) {
  if (components.empty()) {
    throw std::invalid_argument("Components should not be empty.");
  }
  const auto rowCount = components.front().getRowCount();
  const auto columnCount = components.front().getColumnCount();
  for (const auto &component : components) {
    if (component.getRowCount() != rowCount) {
      throw std::invalid_argument("Components have different number of rows.");
    }
    if (component.getColumnCount() != columnCount) {
      throw std::invalid_argument("Components have different number of columns.");
    }
  }
  Board merge(std::vector<std::vector<std::optional<Tile>>>(rowCount, std::vector<std::optional<Tile>>(columnCount)));
  for (const auto &component : components) {
    for (S32 i = 0; i < rowCount; i++) {
      for (S32 j = 0; j < columnCount; j++) {
        if (component.hasTile(i, j)) {
          if (merge.hasTile(i, j)) {
            const auto position = Position(i, j);
            throw std::invalid_argument("Found two occurrences of tile " + position.toString() + ".");
          }
          merge.setTile(i, j, component.getTile(i, j));
        }
      }
    }
  }
  return merge;
}
} // namespace WayoutPlayer