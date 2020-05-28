#include "BoardScanner.hpp"

#include <unordered_set>
#include <utility>

#include "MaskComponentFinder.hpp"

namespace WayoutPlayer {
BoardScanner::BoardScanner() = default;

Board BoardScanner::scan(const Image &image) {
  // Find the black edges.
  const auto mask = image.findPixels([](const Color pixel) { return pixel.getLightness() <= 35.0; });
  auto imageCopy = image;
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      if (mask.getValue(i, j)) {
        imageCopy.setPixel(i, j, Color(255, 255, 255));
      } else {
        imageCopy.setPixel(i, j, Color(0, 0, 0));
      }
    }
  }
  if (debuggingPath) {
    imageCopy.writeImageToFile(debuggingPath.value() / "edges.png");
  }
  MaskComponentFinder componentFinder(mask);
  const auto componentCount = componentFinder.getComponentCount();
  if (componentCount > 0) {
    std::unordered_set<U32> componentsOfInterest;
    for (U32 component = 0; component < componentCount; component++) {
      if (4000 <= componentFinder.getComponentSize(component) && componentFinder.getComponentSize(component) <= 5000) {
        componentsOfInterest.insert(component);
      }
    }
    std::unordered_map<U32, Color> componentOfInterestColor;
    U32 componentOfInterestIndex = 0;
    for (const auto component : componentsOfInterest) {
      const auto hue = 360.0f * componentOfInterestIndex / componentsOfInterest.size();
      componentOfInterestColor[component] = Color::fromHSV(hue, 0.8f, 0.8f);
      componentOfInterestIndex++;
    }
    for (U32 i = 0; i < componentFinder.getHeight(); i++) {
      for (U32 j = 0; j < componentFinder.getWidth(); j++) {
        const auto component = componentFinder.getComponent(i, j);
        if (!componentsOfInterest.contains(component)) {
          continue;
        }
        imageCopy.setPixel(i, j, componentOfInterestColor[component]);
      }
    }
    if (debuggingPath) {
      imageCopy.writeImageToFile(debuggingPath.value() / "components.png");
    }
  }
  return Board(std::vector<std::vector<std::optional<Tile>>>{});
}

void BoardScanner::setDebuggingPath(std::optional<std::filesystem::path> newDebuggingPath) {
  debuggingPath = std::move(newDebuggingPath);
}
} // namespace WayoutPlayer