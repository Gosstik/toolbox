#pragma once

#include <functional>

namespace tb {

template <typename RandomIt>
struct PartitionBounds {
  RandomIt equal;    // iter with elems: pivot <= val
  RandomIt greater;  // iter with elems: pivot < val
};

template <
    typename RandomIt,
    typename LessComp =
        std::less<typename std::iterator_traits<RandomIt>::value_type> >
PartitionBounds<RandomIt> Partition(
    RandomIt first,
    RandomIt last,
    typename std::iterator_traits<RandomIt>::value_type pivot,
    LessComp cmp =
        std::less<typename std::iterator_traits<RandomIt>::value_type>{}
) {
  // One or zero elements.
  if (last - first < 2) {
    return {first, last};
  }

  // General case.
  PartitionBounds<RandomIt> res{first, last};
  RandomIt cur = first;
  while (cur < res.greater) {  // [<pivot, =pivot, >pivot]
    // elements with index in [first, res.equal) are smaller to pivot.
    // elements with index in [res.equal, cur) are equal to pivot.
    // elements with index in [cur, res.greater] are not examined yet.
    // elements with index in (res.greater, last) are greater than pivot.

    if (cmp(*cur, pivot)) {
      // pivot > *cur
      std::swap(*res.equal, *cur);
      ++res.equal;
      ++cur;
    } else if (cmp(pivot, *cur)) {
      // pivot < *cur
      --res.greater;
      std::swap(*res.greater, *cur);
    } else {
      // pivot == *cur.
      ++cur;
    }
  }
  return res;
}

}  // namespace tb
