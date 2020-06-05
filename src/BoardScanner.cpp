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
#include "Range.hpp"

namespace WayoutPlayer {
static constexpr std::array<U32, 4> ObservedLoweredSideSizes = {873, 872, 870, 867};
static constexpr std::array<U32, 9> ObservedTopSizes = {4709, 4711, 4722, 4720, 4722, 4723, 4842, 4723, 4577};

static constexpr auto RelativeTolerance = 0.05f;
static_assert(RelativeTolerance > 0.0f);
static_assert(RelativeTolerance < 1.0f);

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

static constexpr auto HeightChange = 17;

static F32 getScaledMinimumExpectedLoweredSaturation() {
  return 0.0f;
}

static F32 getScaledMaximumExpectedLoweredSaturation() {
  return 0.01f;
}

static constexpr auto EdgesImageFilename = "edges.png";

static constexpr auto ComponentsTextFilename = "components.txt";
static constexpr auto ComponentsImageFilename = "components.png";

BoardScanner::BoardScanner() = default;

Board BoardScanner::scan(const Image &image) {
  // For now, this only supports the game in 1080p.
  if (image.getWidth() != 1920 || image.getHeight() != 1080) {
    throw std::invalid_argument("Image is not 1920x1080. Scaling issues prevent other sizes from being used.");
  }
  // Find the darkest edges.
  auto mask = image.findPixels([](const Color<U8> color) { return color.getLightness() <= 25.0f; });
  // Open them by including their lighter neighbors.
  mask.open([&image](const U32 i, const U32 j) { return image.getPixel(i, j).getLightness() <= 32.5f; });
  auto imageCopy = image;
  for (U32 i = 0; i < image.getHeight(); i++) {
    for (U32 j = 0; j < image.getWidth(); j++) {
      if (mask.getValue(i, j)) {
        imageCopy.setPixel(i, j, Color<U8>(255, 255, 255));
      } else {
        imageCopy.setPixel(i, j, Color<U8>(0, 0, 0));
      }
    }
  }
  if (debuggingPath) {
    imageCopy.writeImageToFile(debuggingPath.value() / EdgesImageFilename);
  }
  MaskComponentFinder componentFinder(mask);
  const auto componentCount = componentFinder.getComponentCount();
  if (componentCount == 0) {
    return Board({});
  }
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
  std::unordered_map<U32, Color<U8>> componentOfInterestColor;
  U32 componentOfInterestIndex = 0;
  for (const auto component : componentsOfInterest) {
    const auto hue = 360.0f * componentOfInterestIndex / componentsOfInterest.size();
    componentOfInterestColor[component] = Color<U8>::fromHSV(hue, 0.8f, 0.8f);
    componentOfInterestIndex++;
  }
  std::unordered_map<U32, Average<Color<F32>>> componentOfInterestAverageColor;
  for (U32 i = 0; i < componentFinder.getHeight(); i++) {
    for (U32 j = 0; j < componentFinder.getWidth(); j++) {
      const auto componentId = componentFinder.getComponentId(i, j);
      if (!componentsOfInterest.contains(componentId)) {
        continue;
      }
      imageCopy.setPixel(i, j, componentOfInterestColor[componentId]);
      const auto color = image.getPixel(i, j);
      componentOfInterestAverageColor[componentId].add(Color<F32>(color.getR(), color.getG(), color.getB()));
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
      textFile << "and average color " << averageColor.toString() << ' ';
      textFile << "(saturation = " << std::to_string(averageColor.getSaturation()) << ")." << '\n';
      textFile << "Based on this, it is considered to be a ";
      const auto saturation = averageColor.getSaturation();
      const auto isUp = !inRange(MinimumExpectedLoweredSaturation, saturation, MaximumExpectedLoweredSaturation);
      const auto tile = Tile(isUp, TileType::Default);
      textFile << tile.toString() << "." << '\n';
    }
  }
  std::unordered_map<U32, std::pair<IndexType, IndexType>> componentPositions;
  std::optional<IntegralScreenCoordinates> originComponentCoordinates;
  for (const auto componentId : componentsOfInterest) {
    const auto coordinates = componentFinder.getComponentCentroid(componentId).roundToIntegralScreenCoordinates();
    const auto averageColor = componentOfInterestAverageColor[componentId].getAverage();
    const auto saturation = averageColor.getSaturation();
    const auto isUp = !inRange(MinimumExpectedLoweredSaturation, saturation, MaximumExpectedLoweredSaturation);
    // Normalize the i coordinate if the tile is lowered.
    const auto i = coordinates.getI() - (isUp ? 0 : HeightChange);
    const auto j = coordinates.getJ();
    if (!originComponentCoordinates) {
      // For any tile, add it as being at (0, 0).
      originComponentCoordinates = IntegralScreenCoordinates(i, j);
      componentPositions[componentId] = {0, 0};
    } else {
      // For all other tiles, compute how long from the origin it is using the vectors u = (78, 48) and v = (48, 78).
      const auto di = static_cast<S32>(i) - static_cast<S32>(originComponentCoordinates->getI());
      const auto dj = static_cast<S32>(j) - static_cast<S32>(originComponentCoordinates->getJ());
      /*
       * Find a and b such that au + bv == <di, dj>.
       *
       * |48 -48| |a| = |di|
       * |78  78| |b| = |dj|
       *
       * On Maxima, algsys([48 * a - 48 * b = di, 78 * a + 78 * b = dj], [a, b]) gives
       */
      const auto a = static_cast<IndexType>(roundWithin((8 * dj + 13 * di) / 1248.0, 0.01));
      const auto b = static_cast<IndexType>(roundWithin((8 * dj - 13 * di) / 1248.0, 0.01));
      // std::cout << "Found component at " << static_cast<S32>(a) << ", " << static_cast<S32>(b) << "." << '\n';
      componentPositions[componentId] = {a, b};
    }
  }
  auto iRange = Range<IndexType>{};
  auto jRange = Range<IndexType>{};
  for (const auto &componentPosition : componentPositions) {
    iRange = iRange.include(componentPosition.second.first);
    jRange = jRange.include(componentPosition.second.second);
  }
  const auto height = iRange.getHigh() - iRange.getLow() + 1;
  const auto width = jRange.getHigh() - jRange.getLow() + 1;
  std::vector<std::vector<std::optional<Tile>>> matrix(height, std::vector<std::optional<Tile>>(width));
  for (const auto componentId : componentsOfInterest) {
    const auto averageColor = componentOfInterestAverageColor[componentId].getAverage();
    const auto saturation = averageColor.getSaturation();
    const auto isUp = !inRange(MinimumExpectedLoweredSaturation, saturation, MaximumExpectedLoweredSaturation);
    const auto i = componentPositions[componentId].first;
    const auto j = componentPositions[componentId].second;
    matrix[i - iRange.getLow()][j - jRange.getLow()] = Tile(isUp, TileType::Default);
  }
  return Board(matrix);
}

void BoardScanner::setDebuggingPath(std::optional<std::filesystem::path> newDebuggingPath) {
  debuggingPath = std::move(newDebuggingPath);
}
} // namespace WayoutPlayer