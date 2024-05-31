#pragma once

#include <functional>

namespace tb {

template <
    typename RandomIt,
    typename LessComp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >
>
void InsertionSort(
    RandomIt first,
    RandomIt last,
    LessComp cmp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >{}
) {
  using DiffT = typename std::iterator_traits<RandomIt>::difference_type;

  DiffT len = last - first;
  if (len < 2) {
    return;
  }

  for (RandomIt i_it = first + 1; i_it != last; ++i_it) {
    RandomIt cur = i_it;
    RandomIt prev = cur - 1;
    while (prev != first && cmp(*cur, *prev)) {
      std::swap(*cur, *prev);
      --cur;
      --prev;
    }
    if (cmp(*cur, *prev)) {
      std::swap(*cur, *prev);
    }
  }
}

} // namespace tb
