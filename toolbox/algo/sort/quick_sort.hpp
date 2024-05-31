#pragma once

#include <functional>
#include <iterator>
#include <random>

#include <toolbox/algo/sort/partition.hpp>

namespace tb {

namespace internal {

template <typename RandomIt, typename LessComp, typename RandomGen>
void QuickSortRecursive(
    RandomIt first,
    RandomIt last,
    LessComp cmp,
    RandomGen& gen) {
  using ValueT = typename std::iterator_traits<RandomIt>::value_type;

  if (last - first < 2) {
    return;
  }

  size_t dif = last - first;
  size_t pivot_ind = gen() % dif;
  ValueT pivot = *(first + pivot_ind);
  PartitionBounds<RandomIt> bounds = Partition(first, last, pivot, cmp);
  QuickSortRecursive(first, bounds.equal, cmp, gen);
  QuickSortRecursive(bounds.greater, last, cmp, gen);
}

} // namespace internal


template <
    typename RandomIt,
    typename LessComp = std::less<
        typename std::iterator_traits<RandomIt>::value_type>
>
void QuickSort(
    RandomIt first,
    RandomIt last,
    LessComp cmp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >{}) {
  if (last - first < 2) {
    return;
  }

  std::mt19937_64 gen(std::random_device{}());
  internal::QuickSortRecursive(first, last, cmp, gen);
}

} // namespace tb
