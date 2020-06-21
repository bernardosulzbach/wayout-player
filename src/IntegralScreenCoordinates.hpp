#pragma once

#include "Types.hpp"

#include <boost/functional/hash.hpp>

namespace WayoutPlayer {
class IntegralScreenCoordinates {
  U32 i{};
  U32 j{};

public:
  IntegralScreenCoordinates(U32 screenI, U32 screenJ);

  [[nodiscard]] U32 getI() const;

  [[nodiscard]] U32 getJ() const;

  bool operator==(const IntegralScreenCoordinates &rhs) const;

  bool operator!=(const IntegralScreenCoordinates &rhs) const;
};

} // namespace WayoutPlayer

namespace std {
template <> struct hash<WayoutPlayer::IntegralScreenCoordinates> {
  std::size_t operator()(const WayoutPlayer::IntegralScreenCoordinates coordinates) const {
    using boost::hash_combine;
    using boost::hash_value;
    std::size_t seed = 0;
    hash_combine(seed, hash_value(coordinates.getI()));
    hash_combine(seed, hash_value(coordinates.getJ()));
    return seed;
  }
};
}; // namespace std
