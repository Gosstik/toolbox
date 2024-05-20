//#pragma once
//
//#include <type_traits>
//
//template <typename T>
//class count_allocator {
//  using value_type = T;
//
//  struct propagate_on_container_copy_assignment : public std::true_type {};
//  struct propagate_on_container_move_assignment : public std::true_type {};
//  struct propagate_on_container_swap : public std::true_type {};
//  struct is_always_equal : public std::false_type {};
//
//};
