#pragma once

#include <vector>
#include <string>

#include <toolbox/util/misc.hpp>

/**
 * Content: \n
 *
 * - LevenshteinDistance() \n
 * - LevenshteinDistanceRecursive() \n
 */

namespace tb {

template <typename T>
struct LevenshteinCosts {
  T replace = 1;
  T insert = 1;
  T remove = 1;
};

template <typename T>
T LevenshteinDistance(
    const std::string& lhs,
    const std::string& rhs,
    const LevenshteinCosts<T>& costs = LevenshteinCosts<T>()
) {
  // We try to remove from or insert to lhs.

  // Induction base.
  size_t l_len = lhs.size() + 1;
  size_t r_len = rhs.size() + 1;
  std::vector<std::vector<T>> dp(l_len, std::vector<T>(r_len, T()));
  for (size_t i = 1; i <= lhs.size(); ++i) {
    dp[i][0] = dp[i - 1][0] + costs.remove;
  }
  for (size_t j = 1; j <= rhs.size(); ++j) {
    dp[0][j] = dp[0][j - 1] + costs.insert; // insert cost
  }

  // Induction transition.
  for (size_t i = 1; i <= lhs.size(); ++i) {
    for (size_t j = 1; j <= rhs.size(); ++j) {
      if (lhs[i - 1] == rhs[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = Min3(
            dp[i - 1][j] + costs.insert,
            dp[i][j - 1] + costs.remove,
            dp[i - 1][j - 1] + costs.replace
        );
      }
    }
  }

  return dp.back().back();
}

////////////////////////////////////////////////////////////////////////////////

namespace detail {

template <typename T>
struct LevenshteinDistanceRecursiveImpl {
  LevenshteinDistanceRecursiveImpl(
      const std::string& lhs,
      const std::string& rhs,
      const LevenshteinCosts<T>& costs
  ) : lhs_(lhs),
      rhs_(rhs),
      costs_(costs),
      dp_(lhs.size() + 1, std::vector<T>(rhs.size() + 1, T(-1))) {
  }

  int Solve() {
    return SolveRec(lhs_.size(), rhs_.size());
  }

 private:
  int SolveRec(size_t l_i, size_t r_i) {
    if (l_i == 0) {
      return r_i * costs_.insert;
    }
    if (r_i == 0) {
      return l_i * costs_.remove;
    }
    if (dp_[l_i][r_i] != T(-1)) {
      return dp_[l_i][r_i];
    }

    if (lhs_[l_i - 1] == rhs_[r_i - 1]) {
      dp_[l_i][r_i] = SolveRec(l_i - 1, r_i - 1);
    } else {
      dp_[l_i][r_i] = Min3(
          SolveRec(l_i - 1, r_i) + costs_.insert,
          SolveRec(l_i, r_i - 1) + costs_.remove,
          SolveRec(l_i - 1, r_i - 1) + costs_.replace
      );
    }

    return dp_[l_i][r_i];
  }

  const std::string& lhs_;
  const std::string& rhs_;
  const LevenshteinCosts<T>& costs_;
  std::vector<std::vector<T>> dp_;
};

} // namespace detail

template <typename T>
T LevenshteinDistanceRecursive(
    const std::string& lhs,
    const std::string& rhs,
    const LevenshteinCosts<T>& costs = LevenshteinCosts<T>()
) {
  auto solver = detail::LevenshteinDistanceRecursiveImpl(
      lhs,
      rhs,
      costs
  );

  T res = solver.Solve();
  return res;
}

} // namespace tb
