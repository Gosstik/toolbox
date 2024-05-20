#include <type_traits>
#include <vector>
#include <array>
#include <memory>

#include <gtest/gtest.h>

#include <toolbox/std/concepts.hpp>

namespace tb = toolbox;

TEST(concepts, complete) {
  struct incomplete_struct;

  static_assert(tb::complete<int>);
  static_assert(tb::complete<std::true_type>);

  static_assert(!tb::complete<void>);
  static_assert(!tb::complete<incomplete_struct>);
}

TEST(concepts, detected) {
  static_assert(tb::detected<std::is_void, void>);
  static_assert(tb::detected<std::is_same, int, float>);

  static_assert(!tb::detected<std::is_same, int>);
  static_assert(!tb::detected<std::is_same, int, float, bool>);
}

TEST(concepts, default_erasable) {
  static_assert(tb::default_erasable<int>);

  static_assert(!tb::default_erasable<int&>);
}

TEST(concepts, allocator_erasable) {
  using vector = std::vector<int, std::allocator<int>>;
  static_assert(tb::allocator_erasable<vector,
                                       vector::allocator_type,
                                       vector::value_type>);
}

TEST(concepts, allocator_aware) {
  static_assert(tb::allocator_aware<std::vector<int>>);

  static_assert(!tb::allocator_aware<std::array<int, 1>>);
}

#include <string>

TEST(concepts, erasable) {
  static_assert(tb::erasable<std::vector<int>, int>);
  static_assert(tb::erasable<std::array<int, 1>, int>);
  static_assert(tb::erasable<std::string, char>);

  static_assert(!tb::erasable<std::vector<int>, float>);
}

TEST(concepts, container) {
  static_assert(tb::container<std::vector<int>>);
  static_assert(tb::container<std::array<int, 1>>);
  static_assert(tb::container<std::string>);

  static_assert(!tb::container<int>);
}
