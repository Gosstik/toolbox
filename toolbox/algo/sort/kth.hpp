#pragma once

#include <toolbox/algo/sort/partition.hpp>
#include <toolbox/algo/sort/insertion_sort.hpp>
#include <toolbox/util/assert.hpp>

/**
 * Content:\n
 * - KthMedianMedians
 */

namespace tb {

namespace internal {

template <
    typename RandomIt,
    typename LessComp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >
>
typename std::iterator_traits<RandomIt>::value_type
MedianMedians(
    RandomIt first,
    RandomIt last,
    LessComp cmp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >{}
) {
  using DiffT = typename std::iterator_traits<RandomIt>::difference_type;
  static constexpr DiffT kBufSize = 5;
  static constexpr DiffT kBufSizeMid = kBufSize / 2;
  static constexpr DiffT kBufSizeFill = kBufSize - kBufSizeMid;

  DiffT len = last - first;
  if (len == 1) {
    return *first;
  }

  // Move medians of buf[5] to the beginning of range, creating res.
  RandomIt buf_first = first;
  RandomIt buf_last = first;
  RandomIt res_last = first;
  while (len >= kBufSize) {
    // Updating state.
    len -= kBufSize;
    buf_last += kBufSize;

    // Sort buf[5].
    tb::InsertionSort(buf_first, buf_last, cmp);

    // Move median to res.
    buf_first += kBufSizeMid;
    std::swap(*res_last, *buf_first);

    // Updating state.
    ++res_last;
    buf_first += kBufSizeFill;
  }

  // Get median from the last buf (its size: 0 <= size <= 5).
  if (len == 1) {
    std::swap(*res_last, *buf_first);
    ++res_last;
  } else if (len != 0) {
    buf_last += len;
    tb::InsertionSort(buf_first, buf_last, cmp);

    // Move median to res.
    buf_first += len / 2;
    std::swap(*res_last, *buf_first);

    // Updating state.
    ++res_last;
  }

  // Recursive call.
  return MedianMedians(first, res_last, cmp);
}

template <
    typename RandomIt,
    typename LessComp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >
>
typename std::iterator_traits<RandomIt>::value_type
KthMedianMediansImpl(
    RandomIt first,
    RandomIt last,
    RandomIt kth,
    LessComp cmp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >{}
) {
  using Value = typename std::iterator_traits<RandomIt>::value_type;

  // Recursion base.
  if (last - first == 1) {
    return *first;
  }

  Value pivot = MedianMedians(first, last, cmp);
  PartitionBounds<RandomIt> bounds = Partition(first, last, pivot, cmp);
  if (kth < bounds.equal) {
    return KthMedianMediansImpl(first, bounds.equal, kth, cmp);
  }
  if (kth < bounds.greater) {
    return pivot;
  }
  return KthMedianMediansImpl(bounds.greater, last, kth, cmp);
}

} // namespace internal


// 0-indexing
template <
    typename RandomIt,
    typename LessComp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >
>
typename std::iterator_traits<RandomIt>::value_type
KthMedianMedians(
    RandomIt first,
    RandomIt last,
    RandomIt kth,
    LessComp cmp = std::less<
        typename std::iterator_traits<RandomIt>::value_type
    >{}
) {
  TOOLBOX_VERIFY(
      last - first > 1 && kth - first <= last - first - 1,
      "invalid arguments"
  );

  return internal::KthMedianMediansImpl(first, last, kth, cmp);
}

} // namespace tb
