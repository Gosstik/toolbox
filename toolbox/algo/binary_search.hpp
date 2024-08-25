#pragma once

#include <cstddef>

bool BinarySearch(const int* begin, const int* end, int target) {
  while (true) {
    size_t len = end - begin;
    if (len <= 1) {
      return len > 0 && *begin == target;
    }
    size_t mid_ind = len / 2;
    const int* mid = begin + mid_ind;
    int mid_val = *mid;
    if (mid_val <= target) {
      begin = mid;
    } else if (mid_val > target) {
      end = mid;
    } else if (mid_val == target) {
      return true;
    }
  }
}
