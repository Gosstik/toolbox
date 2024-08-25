#pragma once

namespace tb {

template <typename T>
T Min3(T f, T s, T t) {
  if (f < s) {
    return (f < t) ? f : t;
  }
  return (s < t) ? s : t;
}

} // namespace tb
