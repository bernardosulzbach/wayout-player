#include "BoardScanner.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>

#include "Average.hpp"
#include "Color.hpp"
#include "MaskComponentFinder.hpp"
#include "Numeric.hpp"

namespace WayoutPlayer {
static constexpr std::array<U32, 4> ObservedLoweredSideSizes = {873, 872, 870, 867};
static constexpr std::array<U32, 9> ObservedTopSizes = {4709, 4711, 4722, 4720, 4722, 4723, 4842, 4723, 4577};

static constexpr auto RelativeTolerance = 0.05f;
static_assert(RelativeTolerance > 0.0f);
static_assert(RelativeTolerance < 1.0f);

static constexpr std::array<Color, 4> LoweredColors = {{{95, 95, 94}, {94, 97, 93}, {97, 99, 96}, {96, 97, 96}}};

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

static F32 getScaledMinimumExpectedLoweredSaturation() {
  const auto begin = std::begin(LoweredColors);
  const auto end = std::end(LoweredColors);
  const auto compareBySaturation = [](const Color &a, const Color &b) { return a.getSaturation() < b.getSaturation(); };
  const auto maximumObservedSaturation = std::min_element(begin, end, compareBySaturation)->getSaturation();
  return maximumObservedSaturation * (1.0f - RelativeTolerance);
}

static F32 getScaledMaximumExpectedLoweredSaturation() {
  const auto begin = std::begin(LoweredColors);
  const auto end = std::end(LoweredColors);
  const auto compareBySaturation = [](const Color &a, const Color &b) { return a.getSaturation() < b.getSaturation(); };
  const auto maximumObservedSaturation = std::max_element(begin, end, compareBySaturation)->getSaturation();
  return std::min(1.0f, maximumObservedSaturation * (1.0f + RelativeTolerance));
}

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
    // Cannot be made constants as getSaturation() is not a constant-expression.
    const auto MinimumExpectedLoweredSaturation = getScaledMinimumExpectedLoweredSaturation();
    const auto MaximumExpectedLoweredSaturation = getScaledMaximumExpectedLoweredSaturation();
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
    std::unordered_map<U32, Average<Color>> componentOfInterestAverageColor;
    for (U32 i = 0; i < componentFinder.getHeight(); i++) {
      for (U32 j = 0; j < componentFinder.getWidth(); j++) {
        const auto componentId = componentFinder.getComponentId(i, j);
        if (!componentsOfInterest.contains(componentId)) {
          continue;
        }
        imageCopy.setPixel(i, j, componentOfInterestColor[componentId]);
        componentOfInterestAverageColor[componentId].add(image.getPixel(i, j));
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
        textFile << "Found a component centered at (" << i << ", " << j << ")" << ' ';
        const auto size = componentFinder.getComponentSize(componentId);
        textFile << "with size " << size << ' ';
        const auto averageColor = componentOfInterestAverageColor[componentId].getAverage();
        textFile << "and average color " << averageColor.toString() << "." << '\n';
        textFile << "Based on this, it is considered to be a ";
        const auto saturation = averageColor.getSaturation();
        const auto isUp = !inRange(MinimumExpectedLoweredSaturation, saturation, MaximumExpectedLoweredSaturation);
        const auto tile = Tile(isUp, TileType::Default);
        textFile << tile.toString() << "." << '\n';
      }
    }
  }
  return Board(std::vector<std::vector<std::optional<Tile>>>{});
}

void BoardScanner::setDebuggingPath(std::optional<std::filesystem::path> newDebuggingPath) {
  debuggingPath = std::move(newDebuggingPath);
}
} // namespace WayoutPlayer