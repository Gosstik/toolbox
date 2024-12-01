#include <gtest/gtest.h>

#include <vector>

#include <toolbox/algo/subset.hpp>

static constexpr size_t n = 5;

TEST(ApplyToCnkIndexSubsets, k0) {
  std::vector<std::vector<size_t>> indexes;
  tb::algo::subsets::
      ApplyToCnkIndexSubsets(n, 0, [&indexes](const std::vector<size_t>& cur_indexes) mutable {
        indexes.push_back(cur_indexes);
      })();

  ASSERT_EQ(indexes.size(), 1);
  ASSERT_EQ(indexes.front(), std::vector<size_t>{});
}

TEST(ApplyToCnkIndexSubsets, k1) {
  std::vector<std::vector<size_t>> indexes;
  tb::algo::subsets::
      ApplyToCnkIndexSubsets(n, 1, [&indexes](const std::vector<size_t>& cur_indexes) mutable {
        indexes.push_back(cur_indexes);
      })();

  auto expected = std::vector<std::vector<size_t>>{
      {0},
      {1},
      {2},
      {3},
      {4},
  };

  ASSERT_EQ(indexes.size(), expected.size());
  ASSERT_EQ(indexes, expected);
}

TEST(ApplyToCnkIndexSubsets, k3) {
  std::vector<std::vector<size_t>> indexes;
  tb::algo::subsets::
      ApplyToCnkIndexSubsets(n, 3, [&indexes](const std::vector<size_t>& cur_indexes) mutable {
        indexes.push_back(cur_indexes);
      })();

  auto expected = std::vector<std::vector<size_t>>{
      {0, 1, 2},
      {0, 1, 3},
      {0, 1, 4},
      {0, 2, 3},
      {0, 2, 4},
      {0, 3, 4},
      {1, 2, 3},
      {1, 2, 4},
      {1, 3, 4},
      {2, 3, 4}
  };

  ASSERT_EQ(indexes.size(), expected.size());
  ASSERT_EQ(indexes, expected);
}
