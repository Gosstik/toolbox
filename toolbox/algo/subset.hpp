#include <iostream>
#include <vector>

// TODO: overload for iterators (to use with std::set and std::map)
// TODO: for subsets with iterators
// TODO: template args

namespace tb::algo::subsets {

template <typename Callback>  // TODO: requires
class ApplyToCnkIndexSubsets {
 public:
  ApplyToCnkIndexSubsets(size_t n, size_t k, Callback cb)
      : n_(n),
        k_(k),
        cb_(cb),
        cur_indexes_() {
    cur_indexes_.reserve(k);
  }

  void operator()() {
    RecursiveImpl();
  }

 private:
  void RecursiveImpl() {
    if (cur_indexes_.size() == k_) {
      cb_(cur_indexes_);
      return;
    }

    size_t cur_i = (cur_indexes_.empty() ? 0 : cur_indexes_.back() + 1);

    for (; cur_i < n_; ++cur_i) {
      cur_indexes_.push_back(cur_i);
      RecursiveImpl();
      cur_indexes_.pop_back();
    }
  }

  size_t n_;
  size_t k_;
  Callback cb_;
  std::vector<size_t> cur_indexes_;  // TODO: replace vec to more simple array
};

////////////////////////////////////////////////////////////////////////////////

// TODO: get all subsets

}
