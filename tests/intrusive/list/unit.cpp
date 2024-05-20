#include <iterator>
#include <concepts>
#include <type_traits>
#include <list>
#include <string>
#include <algorithm>
#include <numeric>
#include <random>

#include <gtest/gtest.h>

#include <toolbox/intrusive/list.hpp>
#include <toolbox/debug.hpp>

namespace tb = toolbox;
namespace tbi = toolbox::intrusive;

////////////////////////////////////////////////////////////////////////////////
//           Utils
////////////////////////////////////////////////////////////////////////////////

struct node : public tbi::list_base_hook {
  explicit node (size_t val) : val(val) {}

  node(const node& /*other*/) = default;

  node& operator=(const node& /*other*/) = default;

  node(node&& other) noexcept: val(other.val) {
    other.val = 0;
  }

  node& operator=(node&& other) noexcept{
    val = other.val;
    other.val = 0;
    return *this;
  }

  node& operator+=(const node& other) {
    val += other.val;
    return *this;
  }

  node& operator++() {
    ++val;
    return *this;
  }

  inline friend bool operator<(const node& lhs, const node& rhs) {
    return lhs.val < rhs.val;
  }

  size_t val = 0;
};

template <typename T>
std::list<T> fill_src_list(size_t count) {
  std::list<T> res;
  for (size_t i = 0; i < count; ++i) {
    res.push_back(node{i});
  }
  return res;
}

template <typename T>
std::list<T> fill_trg_list(tbi::list<T>& trg, size_t size) {
  std::list<T> src = fill_src_list<T>(size);
  for (auto& elem: src) {
    trg.push_back(elem);
  }
  return src;
}

template <typename T>
void check_lists_equal(std::list<T>& src,
                       tbi::list<T>& trg,
                       const char* file,
                       size_t line) {
  ASSERT_EQ(src.size(), trg.size()) << file << ':' << line;
  ASSERT_EQ(src.empty(), trg.empty()) << file << ':' << line;;

  // element comparison
  auto trg_it = trg.begin();
  for (auto src_it = src.begin(); src_it != src.end(); ++src_it, ++trg_it) {
    ASSERT_TRUE(trg_it != trg.end()) << file << ':' << line;;
    EXPECT_EQ(src_it->val, trg_it->val) << file << ':' << line;;
  }
  EXPECT_TRUE(trg_it == trg.end()) << file << ':' << line;;
}

template <typename T>
void check_list_empty(tbi::list<T>& lst, const char* file, size_t line) {
  EXPECT_TRUE(lst.size() == 0) << file << ':' << line;
  EXPECT_TRUE(lst.empty()) << file << ':' << line;;
  EXPECT_TRUE(lst.begin() == lst.end()) << file << ':' << line;
}

template <typename T>
void push_back(std::list<T>& src, tbi::list<T>& trg, T val) {
  src.push_back(val);
  trg.push_back(src.back());
}

template <typename T>
void push_front(std::list<T>& src, tbi::list<T>& trg, T val) {
  src.push_front(val);
  trg.push_front(src.front());
}

template <typename T>
void pop_back(std::list<T>& src, tbi::list<T>& trg) {
  trg.pop_back();
  src.pop_back();
}

template <typename T>
void pop_front(std::list<T>& src, tbi::list<T>& trg) {
  trg.pop_front();
  src.pop_front();
}

void modifiers_simple_splice(size_t size1,
                             size_t size2,
                             const char* file,
                             size_t line) {
  // splice `size2` to `size1`.
  // prepare
  tbi::list<node> trg1;
  std::list<node> src1 = fill_trg_list(trg1, size1);

  tbi::list<node> trg2;
  std::list<node> src2 = fill_trg_list(trg2, size2);

  // do
  trg1.splice(trg1.end(), std::move(trg2));
  src1.splice(src1.end(), std::move(src2));

  // check
  check_list_empty(trg2, file, line);
  check_lists_equal(src1, trg1, file, line);
}


template <typename C>
concept list_concept = requires(C c, const C cc) {
  requires std::default_initializable<C>;
  requires !std::copyable<C>;
  requires std::movable<C>;
  requires std::is_nothrow_move_constructible_v<C>;
  requires std::is_nothrow_move_assignable_v<C>;

  requires std::same_as<typename C::reference, typename C::value_type &>;
  requires std::same_as<typename C::const_reference,
                        const typename C::value_type &>;

  requires std::forward_iterator<typename C::iterator>;
  requires std::forward_iterator<typename C::const_iterator>;
  requires std::convertible_to<typename C::iterator,
                               typename C::const_iterator>;

  requires std::signed_integral<typename C::difference_type>;
  requires std::unsigned_integral<typename C::size_type>;
  requires std::numeric_limits<typename C::difference_type>::max() <=
      std::numeric_limits<typename C::size_type>::max();

  requires std::same_as<
      typename C::difference_type,
      typename std::iterator_traits<typename C::iterator>::difference_type
  >;
  requires std::same_as<
      typename C::difference_type,
      typename std::iterator_traits<typename C::const_iterator>::difference_type
  >;

  { c.begin() } -> std::same_as<typename C::iterator>;
  { c.end() } -> std::same_as<typename C::iterator>;
  { cc.begin() } -> std::same_as<typename C::const_iterator>;
  { cc.end() } -> std::same_as<typename C::const_iterator>;
  { c.cbegin() } -> std::same_as<typename C::const_iterator>;
  { c.cend() } -> std::same_as<typename C::const_iterator>;
  { c.size() } -> std::same_as<typename C::size_type>;
  { c.empty() } -> std::convertible_to<bool>;
};

////////////////////////////////////////////////////////////////////////////////
//        Tests
////////////////////////////////////////////////////////////////////////////////

TEST(static_asserts, iterator) {
  using list_type = tbi::list<node>;

  static_assert(std::bidirectional_iterator<list_type::iterator>);
  static_assert(std::bidirectional_iterator<list_type::const_iterator>);
  static_assert(std::bidirectional_iterator<list_type::reverse_iterator>);
  static_assert(std::bidirectional_iterator<list_type::const_reverse_iterator>);

  // implicit conversion: iterator -> const_iterator
  static_assert(std::is_convertible_v<list_type::iterator,
                                      list_type::const_iterator>);

  // no conversion: const_iterator -> iterator
  static_assert(!std::convertible_to<list_type::const_iterator,
                                     list_type::iterator>);
}

TEST(static_asserts, list) {
  static_assert(list_concept<tbi::list<node>>);
}

TEST(construction, default_construct) {
  tbi::list<node> lst;
  check_list_empty(lst, __FILE__, __LINE__);
}

TEST(construction, pair_iterator) {
  std::vector<size_t> sizes({0, 1, 3});
  for (size_t size: sizes) {
    // prepare
    std::list<node> src = fill_src_list<node>(size);

    // do
    tbi::list<node> trg(src.begin(), src.end());

    // check
    check_lists_equal(src, trg, __FILE__, __LINE__);
  }
}

TEST(construction, move) {
  std::vector<size_t> sizes({0, 1, 3});
  for (size_t size: sizes) {
    // prepare
    tbi::list<node> trg_before;
    std::list<node> src = fill_trg_list(trg_before, size);

    // do
    tbi::list<node> trg_after(std::move(trg_before));

    // check
    check_list_empty(trg_before, __FILE__, __LINE__);
    check_lists_equal(src, trg_after, __FILE__, __LINE__);
  }
}

TEST(assignment, move) {
  std::vector<size_t> sizes({0, 1, 3});
  for (size_t size: sizes) {
    // prepare
    tbi::list<node> trg_before;
    std::list<node> src = fill_trg_list(trg_before, size);

    // do
    tbi::list<node> trg_after;
    trg_after = std::move(trg_before);

    // check
    check_list_empty(trg_before, __FILE__, __LINE__);
    check_lists_equal(src, trg_after, __FILE__, __LINE__);
  }
}

TEST(modifiers, seperate_front_back) {
  {
    // push_back
    std::vector<size_t> sizes({1, 3});
    for (size_t size: sizes) {
      // prepare
      tbi::list<node> trg;
      std::list<node> src;

      // do
      for (size_t i = 0; i < size; ++i) {
        push_back(src, trg, node(i));
      }

      // check
      check_lists_equal(src, trg, __FILE__, __LINE__);
    }
  }

  {
    // push_front
    std::vector<size_t> sizes({1, 3});
    for (size_t size: sizes) {
      // prepare
      tbi::list<node> trg;
      std::list<node> src;

      // do
      for (size_t i = 0; i < size; ++i) {
        push_front(src, trg, node(i));
      }

      // check
      check_lists_equal(src, trg, __FILE__, __LINE__);
    }
  }
}

TEST(modifiers, mixed_front_back) {
  std::list<node> src;
  tbi::list<node> trg;

  push_back(src, trg, node(3));
  push_back(src, trg, node(4));
  push_front(src, trg, node(2));
  push_back(src, trg, node(5));
  push_front(src, trg, node(1));

  std::string res;
  for (auto it = trg.begin(); it != trg.end(); ++it) {
    res += std::to_string(it->val);
  }
  EXPECT_EQ(res, std::string("12345"));
}

TEST(modifiers, pop) {
  size_t size = 10;
  tbi::list<node> trg;
  std::list<node> src = fill_trg_list(trg, size);

  for (size_t i = 0; i < size; ++i) {
    if (i % 2 == 0) {
      pop_back(src, trg);
    } else {
      pop_front(src, trg);
    }
    if (src.empty()) {
      break;
    }
    EXPECT_EQ(src.front().val, trg.front().val);
    EXPECT_EQ(src.back().val, trg.back().val);
  }
  EXPECT_TRUE(trg.empty());
}

TEST(modifiers, splice) {
  modifiers_simple_splice(0, 0, __FILE__, __LINE__);
  modifiers_simple_splice(0, 1, __FILE__, __LINE__);
  modifiers_simple_splice(1, 0, __FILE__, __LINE__);
  modifiers_simple_splice(1, 1, __FILE__, __LINE__);
  modifiers_simple_splice(4, 3, __FILE__, __LINE__);
}

TEST(modifiers, insert) {
  {
    // insert element
    tbi::list<node> trg;
    std::list<node> src;

    // res: 0
    src.insert(src.begin(), node(0));
    trg.insert(trg.begin(), src.front());
    EXPECT_EQ(src.front().val, trg.front().val);

    // res: 0 1
    src.insert(src.end(), node(1));
    trg.insert(trg.end(), src.back());
    EXPECT_EQ(src.back().val, trg.back().val);

    // res: 0 1 2
    src.insert(src.end(), node(2));
    trg.insert(trg.end(), src.back());
    EXPECT_EQ(src.back().val, trg.back().val);

    // res: 0 1 1 2
    auto src_mid_it = src.begin();
    std::advance(src_mid_it, 1);
    src_mid_it = src.insert(src_mid_it, node(1));

    auto trg_mid_it = trg.begin();
    std::advance(trg_mid_it, 1);
    trg.insert(trg_mid_it, *src_mid_it);

    check_lists_equal(src, trg, __FILE__, __LINE__);
  }

  {
    // insert range
    // res: 0 1 2 3 4 0 1 2 3 4
    size_t size = 5;
    tbi::list<node> trg;
    std::list<node> src = fill_trg_list(trg, size);

    std::list<node> tmp_src = fill_src_list<node>(size);
    trg.insert(trg.begin(), tmp_src.begin(), tmp_src.end());

    src.splice(src.begin(), std::move(tmp_src));

    check_lists_equal(src, trg, __FILE__, __LINE__);
  }
}

TEST(modifiers, erase) {
  {
    // erase element
    size_t size = 5;
    tbi::list<node> trg;
    std::list<node> src = fill_trg_list(trg, size);

    // 1 2 3 4 5 -> 1 2 3 4
    trg.erase(trg.begin()); // by iterator
    src.erase(src.begin());
    EXPECT_EQ(src.front().val, trg.front().val);

    // 1 2 3 4 -> 1 2 3
    trg.erase(*--trg.end()); // by value
    src.erase(--src.end());
    EXPECT_EQ(src.back().val, trg.back().val);

    // 1 2 3 -> 1 3
    auto src_mid_it = src.begin();
    std::advance(src_mid_it, 1);
    src_mid_it = src.erase(src_mid_it);

    auto trg_mid_it = trg.begin();
    std::advance(trg_mid_it, 2);
    trg.erase(trg_mid_it);

    // 1 3 -> empty
    trg.erase(trg.begin());
    trg.erase(*trg.begin());
    check_list_empty(trg, __FILE__, __LINE__);
  }

  {
    // erase range
    // 0 1 2 3 4 -> 0 4
    size_t size = 5;
    tbi::list<node> trg;
    std::list<node> src = fill_trg_list(trg, size);

    trg.erase(++trg.begin(), --trg.end());
    src.erase(++src.begin(), --src.end());
    check_lists_equal(src, trg, __FILE__, __LINE__);

    // 0 4 -> empty
    trg.erase(trg.begin(), trg.end());
    check_list_empty(trg, __FILE__, __LINE__);

    // empty -> empty
    trg.erase(trg.begin(), trg.end());
    check_list_empty(trg, __FILE__, __LINE__);
  }
}

TEST(hooks, coping) {
  // NOTE: only values must be copied, not pointers.
  // copy from trg1 to trg2
  // prepare
  size_t src_size = 6;
  std::list<node> src = fill_src_list<node>(src_size);
  auto src_it = src.begin();

  tbi::list<node> trg1;
  for (size_t i = 0; i < src_size / 2; ++i) {
    trg1.push_back(*src_it);
    ++src_it;
  }

  tbi::list<node> trg2;
  for (size_t i = src_size / 2; i < src_size; ++i) {
    trg2.push_back(*src_it);
    ++src_it;
  }

  // do
  auto it1 = trg1.begin();
  auto it2 = trg2.begin();
  for (; it1 != trg1.end(); ++it1, ++it2) {
    *it2 = *it1;
  }

  // check
  std::string res1;
  for (auto it = trg1.begin(); it != trg1.end(); ++it) {
    res1 += std::to_string(it->val);
  }
  EXPECT_EQ(res1, std::string("012"));

  std::string res2;
  for (auto it = trg2.begin(); it != trg2.end(); ++it) {
    res2 += std::to_string(it->val);
  }
  EXPECT_EQ(res2, std::string("012"));
}

TEST(hooks, moving) {
  // prepare
  size_t src_size = 6;
  tbi::list<node> trg;
  std::list<node> tmp_src;
  for (size_t i = 0; i < src_size; ++i) {
    tmp_src.push_back(node{i});
    trg.push_back(tmp_src.back());
  }

  // do
  std::list<node> src;
  for (auto& elem: tmp_src) {
    src.emplace_back(std::move(elem));
  }

  // check
  for (auto& elem: trg) {
    EXPECT_EQ(elem.val, 0);
  }
}

TEST(iterator, algo) {
  size_t size = 1000;
  size_t half = size / 2;
  size_t quarter = half / 2;
  tbi::list<node> trg;
  std::list<node> src = fill_trg_list(trg, size);

  {
    // 0 1 ... 998 9999 -> 1 2 ... 999 1000
    std::iota(trg.begin(), trg.end(), node(1));
    size_t expected_val = 1;
    for (auto it = trg.cbegin(); it != trg.cend(); ++it) {
      EXPECT_EQ(it->val, expected_val);
      ++expected_val;
    }
  }

  {
    // 1 2 ... 999 1000 -> 500 499 ... 2 1 1000 999 ... 502 501
    auto mid_it = trg.begin();
    std::advance(mid_it, half);
    std::reverse(mid_it, trg.end());
    std::reverse(trg.begin(), mid_it);
    size_t expected_val = 501;
    for (auto it = trg.crbegin(); it != trg.crend(); ++it) {
      EXPECT_EQ(expected_val, it->val);
      if (expected_val == 1000) {
        expected_val = 1;
      } else {
        ++expected_val;
      }
    }
  }

  {
    // 500 499 ... 2 1 1000 999 ... 502 501 -> 1 2 ... 999 1000
    auto mid_rit = trg.rbegin();
    std::advance(mid_rit, half);
    std::reverse(mid_rit, trg.rend());
    std::reverse(trg.rbegin(), mid_rit);
    size_t cur_val = 1;
    const tbi::list<node> const_trg(std::move(trg));
    for (auto it = const_trg.begin(); it != const_trg.end(); ++it) {
      EXPECT_EQ(cur_val, it->val);
      ++cur_val;
    }
  }

  {
    // 1 2 ... 999 1000 -> 1 2 ... 749 750 1000 999 ... 751
    auto three_quarters_it = trg.begin();
    std::advance(three_quarters_it, half + quarter);
    std::reverse(three_quarters_it, trg.end());
    auto it = std::is_sorted_until(trg.cbegin(), trg.cend());
    std::advance(it, quarter);
    EXPECT_EQ(it, trg.end());
    auto rit = std::is_sorted_until(trg.begin(), trg.end());
    std::advance(rit, half + quarter);
    EXPECT_EQ(it, trg.end());
  }
}
