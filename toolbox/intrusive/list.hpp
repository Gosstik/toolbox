#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>

#include <toolbox/debug.hpp>

namespace toolbox {
namespace intrusive {

class list_base_hook {
 public:
  list_base_hook* prev = nullptr;
  list_base_hook* next = nullptr;

  list_base_hook() = default;

  list_base_hook(const list_base_hook& /*other*/) : list_base_hook() {}

  list_base_hook& operator=(const list_base_hook& /*other*/) { return *this; }

  // Hooks current node after `other`.
  void hook(list_base_hook* other) {
    prev = other;
    next = other->next;
    other->next->prev = this;
    other->next = this;
  }

  void unhook() {
    prev->next = next;
    next->prev = prev;
    prev = next = nullptr;
  }

  void make_zero() {
    prev = next = nullptr;
  }

  // Transfer range [first, last) before `this`.
  void transfer(list_base_hook* first, list_base_hook* last) {
    list_base_hook* prev_last = last->prev;

    // save structure of other list.
    first->prev->next = last;
    last->prev = first->prev;

    // transfer
    prev->next = first;
    first->prev = prev;
    prev = prev_last;
    prev_last->next = this;
  }
};

namespace detail {

template <typename T, bool kIsConst>
class list_iterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using pointer = std::conditional_t<kIsConst, const T*, T*>;
  using reference = std::conditional_t<kIsConst, const T&, T&>;
  using difference_type = std::ptrdiff_t;

 private:
  using base_node = list_base_hook;
  using self_node_ptr = std::conditional_t<kIsConst,
                                           const base_node*,
                                           base_node*>;
  using self = list_iterator<T, kIsConst>;
  using iterator = list_iterator<T, false>;
  using const_iterator = list_iterator<T, true>;

 public:
  list_iterator() = default;

  explicit list_iterator(self_node_ptr node) : node(node) {}

  // NOLINTNEXTLINE
  operator const_iterator() { return const_iterator(node); }

  // `const_cast` is required in order not to store additional iterators
  // in the list.
  // NOLINTNEXTLINE
  auto _const_cast() const { return iterator(const_cast<base_node*>(node)); }

  reference operator*() const { return *static_cast<pointer>(node); }

  pointer operator->() const { return static_cast<pointer>(node); }

  self& operator++() {
    node = node->next;
    return *this;
  }

  self operator++(int) {
    self res = *this;
    node = node->next;
    return res;
  }

  self& operator--() {
    node = node->prev;
    return *this;
  }

  self operator--(int) {
    self res = *this;
    node = node->prev;
    return res;
  }

  template <typename U, bool kIsConst1, bool kIsConst2>
  friend bool operator==(const list_iterator<U, kIsConst1>& lhs,
                         const list_iterator<U, kIsConst2>& rhs);

  template <typename U, bool kIsConst1, bool kIsConst2>
  friend bool operator!=(const list_iterator<U, kIsConst1>& lhs,
                         const list_iterator<U, kIsConst2>& rhs);

  self_node_ptr node = nullptr;
};

template <typename T, bool kIsConst1, bool kIsConst2>
bool operator==(const list_iterator<T, kIsConst1>& lhs,
                const list_iterator<T, kIsConst2>& rhs) {
  return lhs.node == rhs.node;
}

template <typename T, bool kIsConst1, bool kIsConst2>
bool operator!=(const list_iterator<T, kIsConst1>& lhs,
                const list_iterator<T, kIsConst2>& rhs) {
  return lhs.node != rhs.node;
}

} // namespace detail

// IntT must be publicly derived from `base_list_node`
template <typename T>
class list {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using iterator = detail::list_iterator<T, false>;
  using const_iterator = detail::list_iterator<T, true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using difference_type = std::iterator_traits<iterator>::difference_type;
  using size_type = size_t;

 private:
  using base_node = list_base_hook;

 public:
  list() { empty_init(); }

  template <std::input_iterator InputIter>
  list(InputIter first, InputIter last) : list() {
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  list(const list& /*other*/) = delete;

  list& operator=(const list& /*list*/) = delete;

  list(list&& other) noexcept
      : size_(other.size_) {
    if (other.size_ == 0) {
      root_.prev = root_.next = &root_;
    } else {
      root_.prev = other.root_.prev;
      root_.next = other.root_.next;
      root_.prev->next = root_.next->prev = &root_;
    }
    other.empty_init();
  }

  list& operator=(list&& other) noexcept {
    clear();
    splice(begin(), std::move(other));
    return *this;
  }

  ~list() = default;

  void push_front(reference val) {
    val.hook(&root_);
    ++size_;
  }

  void pop_front() {
    root_.next->unhook();
    --size_;
  }

  void push_back(reference val) {
    val.hook(root_.prev);
    ++size_;
  }

  void pop_back() {
    root_.prev->unhook();
    --size_;
  }

  // Inserts `val` before `pos`.
  iterator insert(const_iterator pos, reference val) {
    val.hook(pos._const_cast()->prev);
    ++size_;
    return s_iterator_to(val);
  }

  // Inserts range before `pos`.
  template <std::input_iterator InputIter>
  iterator insert(const_iterator pos, InputIter first, InputIter last) {
    list to_insert(first, last);
    if (to_insert.empty()) {
      return pos._const_cast();
    }
    iterator res = to_insert.begin();
    splice(pos, std::move(to_insert));
    return res;
  }

  iterator erase(const_iterator pos) {
    auto res_it = ++pos._const_cast();
    res_it->prev->unhook();
    --size_;
    return res_it;
  }

  iterator erase(reference val) { return erase(s_iterator_to(val)); }

  iterator erase(const_iterator first, const_iterator last) {
    if (first == last) {
      return last._const_cast();
    }

    // update pointers
    iterator f = first._const_cast();
    iterator l = last._const_cast();
    f->prev->next = l.node;
    l->prev = f->prev;

    // update size
    size_ -= zero_range(f, l);

    return l;
  }

  size_t zero_range(iterator first, iterator last) {
    size_t count = 0;
    while (first != last) {
      base_node* cur = first.node;
      ++first;
      cur->make_zero();
      ++count;
    }
    return count;
  }

  void splice(const_iterator pos, list&& lst) {
    if (lst.empty()) {
      return;
    }
    size_ += lst.size_;
    lst.size_ = 0;
    pos._const_cast()->transfer(lst.root_.next, &lst.root_);
  }

  const_iterator begin() const { return const_iterator(root_.next); }

  iterator begin() { return iterator(root_.next); }

  iterator end() { return iterator(&root_); }

  const_iterator end() const { return const_iterator(&root_); }

  const_iterator cbegin() const { return begin(); }

  const_iterator cend() const { return end(); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  const_reverse_iterator crbegin() const { return rbegin(); }

  const_reverse_iterator crend() const { return rend(); }

  void clear() {
    empty_init();
  }

  T& front() noexcept {
    return static_cast<T&>(*root_.next);
  }

  const T& front() const noexcept {
    return static_cast<const T&>(*root_.next);
  }

  T& back() noexcept {
    return static_cast<T&>(*root_.prev);
  }

  const T& back() const noexcept {
    return static_cast<const T&>(*root_.prev);
  }

  size_t size() const noexcept {
    return size_;
  }

  bool empty() const noexcept {
    return root_.prev == &root_;
  }

  static iterator s_iterator_to(reference val) {
    return iterator(&val);
  }

  static const_iterator s_iterator_to(const_reference val) {
    return const_iterator(&val);
  }

 private:
  void empty_init() {
    root_.prev = root_.next = &root_;
    size_ = 0;
  }

  base_node root_;
  size_t size_ = 0;
};

} // namespace intrusive
} // namespace toolbox
