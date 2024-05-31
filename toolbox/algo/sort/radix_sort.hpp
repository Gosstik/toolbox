#pragma once

#include <cstdint>
#include <vector>

namespace tb {

template <typename T>
void RadixSort(std::vector<T>& vec) {
  static constexpr uint64_t kBytes = sizeof(T);
  static constexpr uint64_t kBitsInByte = 8;
  static constexpr uint64_t kValsInByte = 256;
  static constexpr uint64_t kLastVal = 255;

  // 255_10 = 11111111_2
  std::vector<T> res = vec;
  for (size_t i = 0; i < kBytes; ++i) {
    std::vector<size_t> count(kValsInByte, 0);
    std::vector<size_t> pref(kValsInByte, 0); // indexes
    for (auto& value : res) {
      uint64_t ind = (value >> (i * kBitsInByte)) & kLastVal;
      ++count[ind];
    }
    for (size_t j = 0; j < kLastVal; ++j) {
      pref[j + 1] = pref[j] + count[j];
    }
    for (const auto& value : vec) {
      uint64_t ind = (value >> (i * kBitsInByte)) & kLastVal;
      res[pref[ind]++] = value;
    }
    vec = res;
  }
}

} // namespace tb
