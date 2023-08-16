#include <cstddef>
#include <type_traits>

#include <gtest/gtest.h>

#include <toolbox/std/type_traits.hpp>

TEST(IntegralConstant, Usings) {
  using int_struct_1 = tb::integral_constant<int, 1>;
  using int_struct_2 = tb::integral_constant<int, 2>;

  // type
  using int_type_1 = typename int_struct_1::type;

  // value_type
  using int_value_t = typename int_struct_1::value_type;
  using int_type_1_v2 [[maybe_unused]] = tb::integral_constant<int, int_value_t(1)>;

  // value
  using int_type_1_v3 [[maybe_unused]] = tb::integral_constant<int,
                                              decltype(int_struct_1::value)(1)>;
  static_assert(int_struct_1::value == int_type_1::value);
  static_assert(int_struct_1::value != int_struct_2::value);
}

////////////////////////////////////////////////////////////////////////////////

TEST(IntegralConstant, Operators) {
  tb::integral_constant<int, 1> int_struct;
  constexpr int int_val_conversion = int_struct;
  constexpr int int_val_call = int_struct();
  static_assert(int_val_conversion == int_val_call);
}

////////////////////////////////////////////////////////////////////////////////

TEST(BoolConstant, JustWorks) {
  static_assert(tb::bool_constant<true>{} == tb::true_type{});
  static_assert(tb::bool_constant<false>{} == tb::false_type{});
}

////////////////////////////////////////////////////////////////////////////////

TEST(TypeIdentity, JustWorks) {
  static_assert(int(1) == typename tb::type_identity<int>::type(1));
  static_assert(int64_t(1) != tb::type_identity_t<int64_t>(2));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Conditional, JustWorks) {
  using true_cond = tb::conditional_t<true, int, tb::nonesuch>;
  using false_cond = tb::conditional_t<false, tb::nonesuch, int>;
  static_assert(true_cond(1) == false_cond(1));
}

////////////////////////////////////////////////////////////////////////////////

template <bool kCond>
struct not_compiled_false : public tb::bool_constant<kCond> {
  static_assert(kCond);
};

template <bool kCond>
struct not_compiled_true : public tb::bool_constant<kCond> {
  static_assert(!kCond);
};

TEST(LogicalOperators, JustWorks) {
  using tt = tb::true_type;
  using ft = tb::false_type;
  struct tts : public tt {
    const int test_val = 1;
  };

  struct fts : public ft {
    const int test_val = 1;
  };

  // or_v
  static_assert(tb::or_v<tt>);
  static_assert(!tb::or_v<ft>);
  static_assert(tb::or_v<ft, tt>);
  static_assert(tb::or_v<ft, ft, tt, ft>);
  static_assert(!tb::or_v<ft, ft, ft, ft, ft>);
  // or_s
  static_assert(tb::or_s<ft, ft, tts, ft>{}.test_val == 1);
  // or short-circuiting
  static_assert(tb::or_v<ft, ft, tt, not_compiled_true<true>>);

  // and_v
  static_assert(tb::and_v<tt>);
  static_assert(!tb::and_v<ft>);
  static_assert(tb::and_v<tt, tt>);
  static_assert(tb::and_v<tt, tt, tt, tt>);
  static_assert(!tb::and_v<tt, ft, tt, tt, tt>);
  // and_s
  static_assert(tb::and_s<tt, tt, tt, fts>{}.test_val == 1);
  // and short-circuiting
  static_assert(!tb::and_v<tt, tt, tt, ft, not_compiled_false<false>>);

  // not_v
  static_assert(tb::not_v<tb::false_type>);
  static_assert(!tb::not_v<tb::true_type>);
  // not_s
  static_assert(tb::not_s<tb::false_type>::value);
  static_assert(!tb::not_s<tb::true_type>::value);
}

////////////////////////////////////////////////////////////////////////////////

TEST(IsSame, JustWorks) {
  // is_same
  static_assert(tb::is_same<int, int>::value);
  static_assert(!tb::is_same<int, char>::value);

  // is_same_v
  static_assert(tb::is_same_v<const char*, const char*>);
  static_assert(!tb::is_same_v<int, volatile int>);
  static_assert(tb::is_same_v<volatile int&, volatile int&>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemoveConst, JustWorks) {
  // remove_const
  static_assert(tb::is_same_v<typename tb::remove_const<int>::type, int>);
  static_assert(tb::is_same_v<typename tb::remove_const<const int>::type, int>);

  // remove_const_t
  static_assert(tb::is_same_v<tb::remove_const_t<const char* const>, const char*>);
  static_assert(tb::is_same_v<tb::remove_const_t<const volatile int>, volatile int>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemoveVolatile, JustWorks) {
  // remove_volatile
  static_assert(tb::is_same_v<typename tb::remove_volatile<int>::type, int>);
  static_assert(tb::is_same_v<typename tb::remove_volatile<volatile int>::type, int>);
  // remove_volatile_t
  static_assert(tb::is_same_v<tb::remove_volatile_t<const volatile char>, const char>);
  static_assert(tb::is_same_v<tb::remove_volatile_t<const char* volatile>, const char*>);
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

TEST(AddLvalueReference, JustWorks) {
  static_assert(tb::is_same_v<typename tb::add_lvalue_reference<int>::type, int&>);
  static_assert(tb::is_same_v<tb::add_lvalue_reference_t<const char&&>, const char&>);
  // SFINAE
  static_assert(tb::is_same_v<tb::add_lvalue_reference_t<void>, void>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(AddRvalueReference, JustWorks) {
  static_assert(tb::is_same_v<typename tb::add_rvalue_reference<int>::type, int&&>);
  static_assert(tb::is_same_v<typename tb::add_rvalue_reference<int&>::type, int&>);
  static_assert(tb::is_same_v<tb::add_rvalue_reference_t<const char&&>, const char&&>);
  // SFINAE
  static_assert(tb::is_same_v<tb::add_rvalue_reference_t<void>, void>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemovePointer, JustWorks) {
  static_assert(tb::is_same_v<int, tb::remove_pointer_t<int>>);
  static_assert(tb::is_same_v<int, tb::remove_pointer_t<int*>>);
  static_assert(tb::is_same_v<int*, tb::remove_pointer_t<int**>>);
  static_assert(tb::is_same_v<int, tb::remove_pointer_t<int* const>>);
  static_assert(tb::is_same_v<int, tb::remove_pointer_t<int* volatile>>);
  static_assert(tb::is_same_v<int, tb::remove_pointer_t<int* const volatile>>);
  static_assert(tb::is_same_v<const int, tb::remove_pointer_t<const int*>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(AddPointer, JustWorks) {
  static_assert(tb::is_same_v<int*, tb::add_pointer_t<int>>);
  static_assert(tb::is_same_v<int**, tb::add_pointer_t<int*>>);
  static_assert(tb::is_same_v<int*, tb::add_pointer_t<int&>>);
  // SFINAE
  static_assert(tb::is_same_v<void() const, tb::add_pointer_t<void() const>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemoveExtent, JustWorks) {
  static_assert(tb::is_same_v<int, tb::remove_extent_t<int>>);
  static_assert(tb::is_same_v<int, tb::remove_extent_t<int[]>>);
  static_assert(tb::is_same_v<int, tb::remove_extent_t<int[1]>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemoveAllExtents, JustWorks) {
  static_assert(tb::is_same_v<int, tb::remove_all_extents_t<int>>);
  static_assert(tb::is_same_v<int, tb::remove_all_extents_t<int[]>>);
  static_assert(tb::is_same_v<int, tb::remove_all_extents_t<int[1]>>);
  static_assert(tb::is_same_v<int, tb::remove_all_extents_t<int[1][2][3]>>);
  static_assert(tb::is_same_v<int, tb::remove_all_extents_t<int[][2][3]>>);
}
