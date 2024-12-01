#include <iostream>
#include <vector>

namespace tb::util::print {

template <typename T>
void PrintVec(
    const std::vector<T>& vec,
    bool linefeed = true,
    std::ostream& out = std::cout,
    const char* delim = ", ",
    const char* start = "[",
    const char* end = "]"
) {
  out << start;
  for (size_t i = 0; i + 1 < vec.size(); ++i) {
    out << vec[i] << delim;
  }
  if (!vec.empty()) {
    out << vec.back();
  }
  out << end;
  if (linefeed) {
    out << '\n';
  }
}

template <typename T>
void PrintVec2(
    const std::vector<std::vector<T>>& vec2,
    bool linefeed = true,
    std::ostream& out = std::cout,
    const char* delim = ", ",
    const char* start = "[",
    const char* end = "]"
) {
  out << start;
  for (size_t i = 0; i + 1 < vec2.size(); ++i) {
    PrintVec(vec2[i], false, out);
    out << delim;
  }
  if (!vec2.empty()) {
    PrintVec(vec2.back(), false, out);
  }
  out << end;
  if (linefeed) {
    out << '\n';
  }
}

}  // namespace tb::util::print
