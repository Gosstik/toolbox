//#pragma once
//
//#include <memory>
//
//#include "toolbox/std/concepts.hpp"
//#include <limits>
//#include <cstddef>
//
//namespace tb {
//
//template <IsComplete T>
//class NewAllocator {
// public:
//  using ValueType = T;
//
//  constexpr NewAllocator() {}
//
//  constexpr NewAllocator(const NewAllocator& /*other*/) noexcept {}
//
//  constexpr NewAllocator& operator=(const NewAllocator& /*other*/) noexcept {}
//
//  constexpr NewAllocator(NewAllocator&& /*other*/) noexcept {}
//
//  constexpr NewAllocator& operator=(NewAllocator&& /*other*/) noexcept {}
//
//  T* Allocate() {
//    constexpr if() {
//
//    }
//  }
//
//  void Deallocate() {
//
//  }
//
//  // To satisfy Allocator named requirements
//  // NOLINTBEGIN
//  using value_type = T;
//
//  T* allocate() {
//    return Allocate();
//  }
//  // NOLINTEND
//
//};
//
//} // namespace tb
//
//
//void Foo() {
//  std::allocator<int> alloc;
//}
