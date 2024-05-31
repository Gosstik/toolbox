#pragma once

#include <functional>

namespace std {
template <typename U, typename T>
struct hash<std::pair<U, T>> {
  static constexpr size_t kNum = 0x9e3779b9;
  static constexpr size_t kOff1 = 6;
  static constexpr size_t kOff2 = 2;

  size_t operator()(const std::pair<U, T>& elem) const {
    size_t seed = hash<U>()(elem.first);
    size_t value = hash<T>()(elem.second);
    seed ^= value + kNum + (seed >> kOff1) + (seed >> kOff2);
    return seed;
  }

};

}  // namespace std
