#include <cstddef>
#include <type_traits>

#include <gtest/gtest.h>

#include <toolbox/std/type_traits.hpp>

TEST(IntegralConstant, Usings) {
  using int_struct_1 = toolbox::integral_constant<int, 1>;
  using int_struct_2 = toolbox::integral_constant<int, 2>;

  // type
  using int_type_1 = typename int_struct_1::type;

  // value_type
  using int_value_t = typename int_struct_1::value_type;
  using int_type_1_v2 [[maybe_unused]] =
      toolbox::integral_constant<int, int_value_t(1)>;

  // value
  using value_type = decltype(int_struct_1::value);
  EXPECT_EQ(typeid(value_type), typeid(const int));

  static_assert(int_struct_1::value == int_type_1::value);
  static_assert(int_struct_1::value != int_struct_2::value);
}

////////////////////////////////////////////////////////////////////////////////

TEST(IntegralConstant, Operators) {
  toolbox::integral_constant<int, 1> int_struct;
  constexpr int int_val_conversion = int_struct;
  constexpr int int_val_call = int_struct();
  static_assert(int_val_conversion == int_val_call);
}

////////////////////////////////////////////////////////////////////////////////

TEST(BoolConstant, JustWorks) {
  static_assert(toolbox::bool_constant<true>{} == toolbox::true_type{});
  static_assert(toolbox::bool_constant<false>{} == toolbox::false_type{});
}

////////////////////////////////////////////////////////////////////////////////

TEST(TypeIdentity, JustWorks) {
  static_assert(int(1) == typename toolbox::type_identity<int>::type(1));
  static_assert(int64_t(1) != toolbox::type_identity_t<int64_t>(2));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Conditional, JustWorks) {
  using true_cond = toolbox::conditional_t<true, int, toolbox::nonesuch>;
  using false_cond = toolbox::conditional_t<false, toolbox::nonesuch, int>;
  static_assert(true_cond(1) == false_cond(1));
}

////////////////////////////////////////////////////////////////////////////////

template <bool kCond>
struct not_compiled_false : public toolbox::bool_constant<kCond> {
  static_assert(kCond);
};

template <bool kCond>
struct not_compiled_true : public toolbox::bool_constant<kCond> {
  static_assert(!kCond);
};

TEST(LogicalOperators, JustWorks) {
  using tt = toolbox::true_type;
  using ft = toolbox::false_type;
  struct tts : public tt {
    const int test_val = 1;
  };

  struct fts : public ft {
    const int test_val = 1;
  };

  // or_v
  static_assert(toolbox::or_v<tt>);
  static_assert(!toolbox::or_v<ft>);
  static_assert(toolbox::or_v<ft, tt>);
  static_assert(toolbox::or_v<ft, ft, tt, ft>);
  static_assert(!toolbox::or_v<ft, ft, ft, ft, ft>);
  // or_s
  static_assert(toolbox::or_s<ft, ft, tts, ft>{}.test_val == 1);
  // or short-circuiting
  static_assert(toolbox::or_v<ft, ft, tt, not_compiled_true<true>>);

  // and_v
  static_assert(toolbox::and_v<tt>);
  static_assert(!toolbox::and_v<ft>);
  static_assert(toolbox::and_v<tt, tt>);
  static_assert(toolbox::and_v<tt, tt, tt, tt>);
  static_assert(!toolbox::and_v<tt, ft, tt, tt, tt>);
  // and_s
  static_assert(toolbox::and_s<tt, tt, tt, fts>{}.test_val == 1);
  // and short-circuiting
  static_assert(!toolbox::and_v<tt, tt, tt, ft, not_compiled_false<false>>);

  // not_v
  static_assert(toolbox::not_v<toolbox::false_type>);
  static_assert(!toolbox::not_v<toolbox::true_type>);
  // not_s
  static_assert(toolbox::not_s<toolbox::false_type>::value);
  static_assert(!toolbox::not_s<toolbox::true_type>::value);
}

////////////////////////////////////////////////////////////////////////////////

TEST(IsSame, JustWorks) {
  // is_same
  static_assert(toolbox::is_same<int, int>::value);
  static_assert(!toolbox::is_same<int, char>::value);

  // is_same_v
  static_assert(toolbox::is_same_v<const char*, const char*>);
  static_assert(!toolbox::is_same_v<int, volatile int>);
  static_assert(toolbox::is_same_v<volatile int&, volatile int&>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemoveConst, JustWorks) {
  // remove_const
  static_assert(toolbox::is_same_v<typename toolbox::remove_const<int>::type, int>);
  static_assert(toolbox::is_same_v<typename toolbox::remove_const<const int>::type, int>);

  // remove_const_t
  static_assert(toolbox::is_same_v<toolbox::remove_const_t<const char* const>, const char*>);
  static_assert(toolbox::is_same_v<toolbox::remove_const_t<const volatile int>, volatile int>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemoveVolatile, JustWorks) {
  // remove_volatile
  static_assert(toolbox::is_same_v<typename toolbox::remove_volatile<int>::type, int>);
  static_assert(toolbox::is_same_v<typename toolbox::remove_volatile<volatile int>::type, int>);
  // remove_volatile_t
  static_assert(toolbox::is_same_v<toolbox::remove_volatile_t<const volatile char>, const char>);
  static_assert(toolbox::is_same_v<toolbox::remove_volatile_t<const char* volatile>, const char*>);
}

////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////

TEST(AddLvalueReference, JustWorks) {
  static_assert(toolbox::is_same_v<typename toolbox::add_lvalue_reference<int>::type, int&>);
  static_assert(toolbox::is_same_v<toolbox::add_lvalue_reference_t<const char&&>, const char&>);
  // SFINAE
  static_assert(toolbox::is_same_v<toolbox::add_lvalue_reference_t<void>, void>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(AddRvalueReference, JustWorks) {
  static_assert(toolbox::is_same_v<typename toolbox::add_rvalue_reference<int>::type, int&&>);
  static_assert(toolbox::is_same_v<typename toolbox::add_rvalue_reference<int&>::type, int&>);
  static_assert(toolbox::is_same_v<toolbox::add_rvalue_reference_t<const char&&>, const char&&>);
  // SFINAE
  static_assert(toolbox::is_same_v<toolbox::add_rvalue_reference_t<void>, void>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemovePointer, JustWorks) {
  static_assert(toolbox::is_same_v<int, toolbox::remove_pointer_t<int>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_pointer_t<int*>>);
  static_assert(toolbox::is_same_v<int*, toolbox::remove_pointer_t<int**>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_pointer_t<int* const>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_pointer_t<int* volatile>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_pointer_t<int* const volatile>>);
  static_assert(toolbox::is_same_v<const int, toolbox::remove_pointer_t<const int*>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(AddPointer, JustWorks) {
  static_assert(toolbox::is_same_v<int*, toolbox::add_pointer_t<int>>);
  static_assert(toolbox::is_same_v<int**, toolbox::add_pointer_t<int*>>);
  static_assert(toolbox::is_same_v<int*, toolbox::add_pointer_t<int&>>);
  // SFINAE
  static_assert(toolbox::is_same_v<void() const, toolbox::add_pointer_t<void() const>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemoveExtent, JustWorks) {
  static_assert(toolbox::is_same_v<int, toolbox::remove_extent_t<int>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_extent_t<int[]>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_extent_t<int[1]>>);
}

////////////////////////////////////////////////////////////////////////////////

TEST(RemoveAllExtents, JustWorks) {
  static_assert(toolbox::is_same_v<int, toolbox::remove_all_extents_t<int>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_all_extents_t<int[]>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_all_extents_t<int[1]>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_all_extents_t<int[1][2][3]>>);
  static_assert(toolbox::is_same_v<int, toolbox::remove_all_extents_t<int[][2][3]>>);
}
