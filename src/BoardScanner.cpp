#include "BoardScanner.hpp"

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>

#include "MaskComponentFinder.hpp"
#include "Numeric.hpp"

namespace WayoutPlayer {
static constexpr std::array<U32, 4> ObservedLoweredSideSizes = {873, 872, 870, 867};
static constexpr std::array<U32, 9> ObservedTopSizes = {4709, 4711, 4722, 4720, 4722, 4723, 4842, 4723, 4577};

static constexpr auto RelativeTolerance = 0.05f;

template <std::size_t N> static constexpr U32 getScaledMinimumObservedSizes(const std::array<U32, N> &observedSizes) {
  return *std::min_element(std::begin(observedSizes), std::end(observedSizes)) * (1.0f - RelativeTolerance);
}

template <std::size_t N> static constexpr U32 getScaledMaximumObservedSizes(const std::array<U32, N> &observedSizes) {
  return *std::max_element(std::begin(observedSizes), std::end(observedSizes)) * (1.0f + RelativeTolerance);
}

static constexpr auto MinimumExpectedLoweredSideSize = getScaledMinimumObservedSizes(ObservedLoweredSideSizes);
static constexpr auto MaximumExpectedLoweredSideSize = getScaledMaximumObservedSizes(ObservedLoweredSideSizes);

static constexpr auto MinimumExpectedTopSize = getScaledMinimumObservedSizes(ObservedTopSizes);
static constexpr auto MaximumExpectedTopSize = getScaledMaximumObservedSizes(ObservedTopSizes);

static constexpr auto EdgesImageFilename = "edges.png";

static constexpr auto ComponentsTextFilename = "components.txt";
static constexpr auto ComponentsImageFilename = "components.png";

BoardScanner::BoardScanner() = default;

Board BoardScanner::scan(const Image &image) {
  // Find the darkest edges.
  auto mask = image.findPixels([](const Color color) { return color.getLightness() <= 25.0f; });
  // Open them by including their lighter neighbors.
  mask.open([&image](const U32 i, const U32 j) { return image.getPixel(i, j).getLightness() <= 32.5f; });
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
    imageCopy.writeImageToFile(debuggingPath.value() / EdgesImageFilename);
  }
  MaskComponentFinder componentFinder(mask);
  const auto componentCount = componentFinder.getComponentCount();
  if (componentCount > 0) {
    std::unordered_set<U32> loweredSideComponents;
    std::unordered_set<U32> componentsOfInterest;
    if (debuggingPath) {
      std::ofstream textFile(debuggingPath.value() / ComponentsTextFilename);
      const auto writeExpectedRange = [&textFile](const std::string &type, const U32 low, const U32 high) {
        textFile << type << " are expected to range from " << low << " to " << high << "." << '\n';
      };
      writeExpectedRange("Lowered sides", MinimumExpectedLoweredSideSize, MaximumExpectedLoweredSideSize);
      writeExpectedRange("Tops", MinimumExpectedTopSize, MaximumExpectedTopSize);
      textFile << std::string(80, '-') << '\n';
    }
    for (U32 component = 0; component < componentCount; component++) {
      const auto componentSize = componentFinder.getComponentSize(component);
      if (inRange(MinimumExpectedLoweredSideSize, componentSize, MaximumExpectedLoweredSideSize)) {
        loweredSideComponents.insert(component);
      } else if (inRange(MinimumExpectedTopSize, componentSize, MaximumExpectedTopSize)) {
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
        const auto componentId = componentFinder.getComponentId(i, j);
        if (!componentsOfInterest.contains(componentId)) {
          continue;
        }
        imageCopy.setPixel(i, j, componentOfInterestColor[componentId]);
      }
    }
    for (const auto componentId : componentsOfInterest) {
      const auto coordinates = componentFinder.getComponentCentroid(componentId).roundToIntegralScreenCoordinates();
      const auto highContrastGrey = componentOfInterestColor[componentId].getHighContrastGrey();
      imageCopy.setCross(coordinates.getI(), coordinates.getJ(), 5, highContrastGrey);
    }
    if (debuggingPath) {
      imageCopy.writeImageToFile(debuggingPath.value() / ComponentsImageFilename);
      std::ofstream textFile(debuggingPath.value() / ComponentsTextFilename, std::ios::app);
      for (const auto componentId : componentsOfInterest) {
        const auto coordinates = componentFinder.getComponentCentroid(componentId).roundToIntegralScreenCoordinates();
        const auto i = coordinates.getI();
        const auto j = coordinates.getJ();
        const auto size = componentFinder.getComponentSize(componentId);
        textFile << "Found a component centered at (" << i << ", " << j << ") with size " << size << "." << '\n';
      }
    }
  }
  return Board(std::vector<std::vector<std::optional<Tile>>>{});
}

void BoardScanner::setDebuggingPath(std::optional<std::filesystem::path> newDebuggingPath) {
  debuggingPath = std::move(newDebuggingPath);
}
} // namespace WayoutPlayer