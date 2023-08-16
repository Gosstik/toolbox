#pragma once

#include <memory>

template <typename T>
class AllocatorTraits {
  using U = std::allocator_traits<T>;
};
