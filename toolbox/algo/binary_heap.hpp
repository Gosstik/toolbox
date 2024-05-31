#pragma once

#include <functional>
#include <vector>

/**
 * Content:
 * - BinaryHeap
 * - BinaryHeapErase
 * - MinMaxBinaryHeap
 */

namespace tb {

template <typename T, typename LessComp = std::less<T>>
class BinaryHeap {
 public:
  using Value = T;
  using Comp = LessComp;
  using Ind = size_t;

  BinaryHeap(LessComp cmp = std::less<T>{}) : cmp_(cmp), buf_(0) {

  }

  void Push(T val) {
    Ind ind = buf_.size();
    buf_.push_back(std::move(val));
    SiftUp(ind);
  }

  void Pop() {
    buf_[0] = std::move(buf_.back());
    buf_.pop_back();
    SiftDown(0);
  }

  T& Top() {
    return buf_[0];
  }

  Ind Size() const {
    return buf_.size();
  }

  bool Empty() const {
    return buf_.empty();
  }

 private:
  static Ind Parent(Ind ind) {
    return ind == 0 ? 0 : (ind - 1) / 2;
  }

  static Ind Left(Ind ind) {
    return 2 * ind + 1;
  }

  static Ind Right(Ind ind) {
    return 2 * ind + 2;
  }

  void SiftUp(Ind ind) {
    while (ind != 0) {
      Ind par_ind = Parent(ind);
      if (cmp_(buf_[ind], buf_[par_ind])) {
        std::swap(buf_[ind], buf_[par_ind]);
        ind = par_ind;
      } else {
        break;
      }
    }
  }

  void SiftDown(Ind ind) {
    size_t buf_size = buf_.size();
    while (Right(ind) < buf_size) {
      Ind l_ind = Left(ind);
      Ind r_ind = Right(ind);
      Ind cmp_ind = cmp_(buf_[l_ind], buf_[r_ind]) ? l_ind : r_ind;
      if (cmp_(buf_[cmp_ind], buf_[ind])) {
        std::swap(buf_[ind], buf_[cmp_ind]);
        ind = cmp_ind;
      } else {
        return;
      }
    }
    size_t l_ind = Left(ind);
    if (l_ind < buf_size && cmp_(buf_[l_ind], buf_[ind])) {
      std::swap(buf_[ind], buf_[l_ind]);
    }
  }

  LessComp cmp_;
  std::vector<T> buf_;
};

//------------------------------------------------------------------------------

// Impl with two BinaryHeap.
template <
    typename T,
    typename LessComp = std::less<T>,
    typename Eq = std::equal_to<T>
>
class BinaryHeapErase {
 public:
  using Value = T;
  using Comp = typename BinaryHeap<T, LessComp>::Comp;
  using Equal = Eq;
  using Ind = typename BinaryHeap<T, LessComp>::Ind;

  BinaryHeapErase(
      LessComp less_cmp = std::less<T>{},
      Eq equal_cmp = std::equal_to<T>{}
  )
      : is_equal_(equal_cmp),
        inserted_(less_cmp),
        deleted_(less_cmp) {
  }

  void Push(T val) {
    inserted_.Push(std::move(val));
  }

  void Pop() {
    ProcessDeleted();
    inserted_.Pop();
  }

  T& Top() {
    ProcessDeleted();
    return inserted_.Top();
  }

  void Erase(T val) {
    deleted_.Push(std::move(val));
  }

  Ind Size() const {
    return inserted_.size();
  }

  bool Empty() const {
    return inserted_.Empty();
  }

 private:
  void ProcessDeleted() {
    while (true) {
      T& ins = inserted_.Top();
      T& del = deleted_.Top();
      if (is_equal_(ins, del)) {
        inserted_.Pop();
        deleted_.Pop();
      } else {
        break;
      }
    }
  }

  Eq is_equal_;
  BinaryHeap<T, LessComp> inserted_;
  BinaryHeap<T, LessComp> deleted_;
};

//------------------------------------------------------------------------------

template <
    typename K,
    typename T,
    typename LessComp = std::less<T>,
    typename Equal = std::equal_to<K>
>
class BinaryHeapEraseByKey {
 // TODO
};

//------------------------------------------------------------------------------

template <typename T, typename LessComp = std::less<T>>
class MinMaxBinaryHeap {
 public:
  using Value = T;
  using Comp = LessComp;
  using Ind = size_t;

  MinMaxBinaryHeap(LessComp cmp = std::less<T>{}) : cmp_(cmp) {}

  ~MinMaxBinaryHeap() = default;

  void Push(T val) {
    size_t new_i = Size();
    Node new_node = {val, new_i};
    min_buf_.push_back(new_node);
    max_buf_.push_back(new_node);
    SiftUpMin(new_i);
    SiftUpMax(new_i);
  }

  void ExtractMin() {
    size_t del_i = Size() - 1;
    size_t max_i = min_buf_[0].other_i;
    SwapMax(max_i, del_i);
    SwapMin(0, del_i);
    min_buf_.pop_back();
    max_buf_.pop_back();

    SiftUpMax(max_i);
    SiftDownMax(max_i);
    SiftDownMin(0);
  }

  T& GetMin() {
    return min_buf_[0].val;
  }

  void ExtractMax() {
    size_t del_i = Size() - 1;
    size_t min_i = max_buf_[0].other_i;
    SwapMin(min_i, del_i);
    SwapMax(0, del_i);
    min_buf_.pop_back();
    max_buf_.pop_back();

    SiftUpMin(min_i);
    SiftDownMin(min_i);
    SiftDownMax(0);
  }

  T& GetMax() {
    return max_buf_[0].val;
  }

  size_t Size() const { return min_buf_.size(); }

  void Clear() {
    min_buf_.clear();
    max_buf_.clear();
  }

  bool Empty() { return Size() == 0; }

 private:
  struct Node {
    T val;
    size_t other_i;  // index in another array
  };

  void SwapMin(size_t ind1, size_t ind2) {
    std::swap(max_buf_[min_buf_[ind1].other_i].other_i,
              max_buf_[min_buf_[ind2].other_i].other_i);
    std::swap(min_buf_[ind1], min_buf_[ind2]);
  }

  void SwapMax(size_t ind1, size_t ind2) {
    std::swap(min_buf_[max_buf_[ind1].other_i].other_i,
              min_buf_[max_buf_[ind2].other_i].other_i);
    std::swap(max_buf_[ind1], max_buf_[ind2]);
  }

  static size_t Left(size_t ind) {
    return 2 * ind + 1;
  }

  static size_t Right(size_t ind) {
    return 2 * ind + 2;
  }

  static size_t Parent(size_t ind) {
    return ind == 0 ? 0 : (ind - 1) / 2;
  }

  void SiftUpMin(size_t ind) {
    size_t par_ind = Parent(ind);
    while (ind != 0 && cmp_(min_buf_[ind].val, min_buf_[par_ind].val)) {
      SwapMin(ind, par_ind);
      ind = par_ind;
      par_ind = Parent(ind);
    }
  }

  void SiftUpMax(size_t ind) {
    size_t par_ind = Parent(ind);
    while (ind != 0 && cmp_(max_buf_[par_ind].val, max_buf_[ind].val)) {
      SwapMax(ind, par_ind);
      ind = par_ind;
      par_ind = Parent(ind);
    }
  }

  void SiftDownMin(size_t ind) {
    size_t size = Size();
    while (Right(ind) < size) {
      size_t r_ind = Right(ind);
      size_t l_ind = Left(ind);

      size_t cmp_ind = 0;
      if (cmp_(min_buf_[l_ind].val, min_buf_[r_ind].val)) {
        cmp_ind = l_ind;
      } else {
        cmp_ind = r_ind;
      }

      if (cmp_(min_buf_[cmp_ind].val, min_buf_[ind].val)) {
        SwapMin(ind, cmp_ind);
        ind = cmp_ind;
      } else {
        return;
      }
    }

    size_t l_ind = Left(ind);
    if (l_ind < size && cmp_(min_buf_[l_ind].val, min_buf_[ind].val)) {
      SwapMin(ind, l_ind);
    }
  }

  void SiftDownMax(size_t ind) {
    size_t size = Size();
    while (Right(ind) < size) {
      size_t r_ind = Right(ind);
      size_t l_ind = Left(ind);

      size_t cmp_ind = 0;
      if (cmp_(max_buf_[l_ind].val, max_buf_[r_ind].val)) {
        cmp_ind = r_ind;
      } else {
        cmp_ind = l_ind;
      }

      if (cmp_(max_buf_[ind].val, max_buf_[cmp_ind].val)) {
        SwapMax(ind, cmp_ind);
        ind = cmp_ind;
      } else {
        return;
      }
    }

    size_t l_ind = Left(ind);
    if (l_ind < size && cmp_(max_buf_[ind].val, max_buf_[l_ind].val)) {
      SwapMax(ind, l_ind);
    }
  }

  LessComp cmp_;
  std::vector<Node> min_buf_;
  std::vector<Node> max_buf_;
};

} // namespace tb
