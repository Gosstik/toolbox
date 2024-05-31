#pragma once

#include <toolbox/std/concepts.hpp>

namespace toolbox {

///**************************************************************************///
///                            integral_constant                             ///
///**************************************************************************///

template <typename T, T kVal>
struct integral_constant {
 public:
  using type = integral_constant<T, kVal>;
  using value_type = T;

  static constexpr T value = kVal;

  // NOLINTNEXTLINE (explicit conversion is required by standard)
  constexpr operator value_type() const noexcept {
    return value;
  }

  constexpr value_type operator()() const noexcept {
    return value;
  }
};

template <typename T, T kVal>
constexpr T integral_constant<T, kVal>::value;

///**************************************************************************///
///                              bool_constant                               ///
///**************************************************************************///

template <bool kVal>
using bool_constant = integral_constant<bool, kVal>;

using true_type = bool_constant<true>;

using false_type = bool_constant<false>;

///**************************************************************************///
///                              type_identity                               ///
///**************************************************************************///

template <typename T>
struct type_identity {
  using type = T;
};

template <typename T>
using type_identity_t = typename type_identity<T>::type;

///**************************************************************************///
///                               conditional                                ///
///**************************************************************************///

template <bool kCond, typename T, typename U>
struct conditional {
  using type = U;
};

template <typename T, typename U>
struct conditional<true, T, U> {
  using type = T;
};

template <bool kCond, typename T, typename U>
using conditional_t = typename conditional<kCond, T, U>::type;

///**************************************************************************///
///                                   or_s                                   ///
///**************************************************************************///

template <typename...>
struct or_s;

template <>
struct or_s<> : public false_type {};

template <typename C1>
struct or_s<C1> : public C1 {};

template <typename C1, typename... Cn>
struct or_s<C1, Cn...> : public conditional_t<C1::value, C1, or_s<Cn...>> {};

template <typename... Cn>
inline constexpr bool or_v = or_s<Cn...>::value;

///**************************************************************************///
///                                  and_s                                   ///
///**************************************************************************///

template <typename...>
struct and_s;

template <>
struct and_s<> : public true_type {};

template <typename C1>
struct and_s<C1> : public C1 {};

template <typename C1, typename... Cn>
struct and_s<C1, Cn...> : public conditional_t<C1::value, and_s<Cn...>, C1> {};

template <typename... Cn>
inline constexpr bool and_v = and_s<Cn...>::value;

///**************************************************************************///
///                                  not_s                                   ///
///**************************************************************************///

template <typename C>
struct not_s : public bool_constant<!bool(C::value)> {};

template <typename C>
inline constexpr bool not_v = not_s<C>::value;

///**************************************************************************///
///                                 is_same                                  ///
///**************************************************************************///

template <typename T, typename U>
struct is_same : public false_type {};

template <typename T>
struct is_same<T, T> : public true_type {};

template <typename T, typename U>
static constexpr bool is_same_v = is_same<T, U>::value;

///**************************************************************************///
///                               remove_const                               ///
///**************************************************************************///

template <typename T>
struct remove_const : public type_identity<T> {};

template <typename T>
struct remove_const<const T> : public type_identity<T> {};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

///**************************************************************************///
///                             remove_volatile                              ///
///**************************************************************************///

template <typename T>
struct remove_volatile : public type_identity<T> {};

template <typename T>
struct remove_volatile<volatile T> : public type_identity<T> {};

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

///**************************************************************************///
///                                remove_cv                                 ///
///**************************************************************************///

template <typename T>
using remove_cv = remove_volatile<remove_const_t<T>>;

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

///**************************************************************************///
///                     add_const, add_volatile, add_cv                      ///
///**************************************************************************///

template <typename T>
struct add_const : public type_identity<const T> {};

template <typename T>
using add_const_t = typename add_const<T>::type;

template <typename T>
struct add_volatile : public type_identity<volatile T> {};

template <typename T>
using add_volatile_t = typename add_volatile<T>::type;

template <typename T>
struct add_cv : public add_const<add_volatile_t<T>> {};

template <typename T>
using add_cv_t = typename add_cv<T>::type;

///**************************************************************************///
///                             remove_reference                             ///
///**************************************************************************///

template <typename T>
struct remove_reference : public type_identity<T> {};

template <typename T>
struct remove_reference<T&> : public type_identity<T> {};

template <typename T>
struct remove_reference<T&&> : public type_identity<T> {};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

///**************************************************************************///
///                           add_lvalue_reference                           ///
///**************************************************************************///

namespace detail {

// Use helper because void& doesn't exist.
template <typename T>
concept lvalue_ref_addable = requires(T& val) {
  val;
};

template <typename T, bool kConcept = lvalue_ref_addable<T>>
struct add_lvref_impl : public type_identity<T> {};

template <typename T>
struct add_lvref_impl<T, true> : public type_identity<T&> {};

} // namespace detail

template <typename T>
struct add_lvalue_reference
    : public detail::add_lvref_impl<T> {
};

template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

///**************************************************************************///
///                           add_rvalue_reference                           ///
///**************************************************************************///

namespace detail {

// Use helper because void&& doesn't exist.
template <typename T>
concept rvalue_ref_addable = requires(T&& val) {
  val;
};

template <typename T, bool kConcept = rvalue_ref_addable<T>>
struct add_rvref_impl : public type_identity<T> {};

template <typename T>
struct add_rvref_impl<T, true> : public type_identity<T&&> {};

} // namespace detail

template <typename T>
struct add_rvalue_reference
    : public detail::add_rvref_impl<T> {
};

template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

///**************************************************************************///
///                              remove_pointer                              ///
///**************************************************************************///

template <typename T>
struct remove_pointer : public type_identity<T> {};

template <typename T>
struct remove_pointer<T*> : public type_identity<T> {};

template <typename T>
struct remove_pointer<T* const> : public type_identity<T> {};

template <typename T>
struct remove_pointer<T* volatile> : public type_identity<T> {};

template <typename T>
struct remove_pointer<T* const volatile> : public type_identity<T> {};

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

///**************************************************************************///
///                               add_pointer                                ///
///**************************************************************************///

namespace detail {

// Use concept because of 'abominable function types'
// (function type followed by a cv-ref qualifier)
template <typename T>
concept ptr_addable = requires(remove_reference_t<T>* val) {
  val;
};

template <typename T, bool kConcept = ptr_addable<T>>
struct add_ptr_impl : public type_identity<T> {};

template <typename T>
struct add_ptr_impl<T, true> : public type_identity<remove_reference_t<T>*> {};

} // namespace detail

template <typename T>
struct add_pointer : public detail::add_ptr_impl<T> {};

template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

///**************************************************************************///
///                              remove_extent                               ///
///**************************************************************************///

template <typename T>
struct remove_extent : public type_identity<T> {};

template <typename T>
struct remove_extent<T[]> : public type_identity<T> {};

template <typename T, size_t kSize>
struct remove_extent<T[kSize]> : public type_identity<T> {};

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

///**************************************************************************///
///                          always_false, declval                           ///
///**************************************************************************///

template <typename T>
struct always_false : public true_type {};

template <typename T>
static constexpr bool always_false_v = always_false<T>::value;

template <typename T>
add_rvalue_reference_t<T>&& declval() noexcept {
  static_assert(always_false_v<T>,
                "declval must not be used in evaluated context");
}

///**************************************************************************///
///                            remove_all_extents                            ///
///**************************************************************************///

template <typename T>
struct remove_all_extents : public type_identity<T> {};

template <typename T>
struct remove_all_extents<T[]> : public remove_all_extents<T> {};

template <typename T, size_t kSize>
struct remove_all_extents<T[kSize]> : public remove_all_extents<T> {};

template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

///**************************************************************************///
///                               remove_cvref                               ///
///**************************************************************************///

template <typename T>
struct remove_cvref : public type_identity<remove_cv_t<remove_reference_t<T>>> {};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::value;

///**************************************************************************///
///                                enable_if                                 ///
///**************************************************************************///

template <bool kCond, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> : public type_identity<T> {};











///**************************************************************************///
///                                  decay                                   ///
///**************************************************************************///

namespace detail {

template <typename T, bool kIsArray, bool kIsFunction>
struct decay_helper {};

} // namespace detail

template <typename T>
struct decay;








///**************************************************************************///
///                               common_type                                ///
///**************************************************************************///

// primary declaration (zero types)
template <typename...>
struct common_type {};

// one type
template <typename T>
struct common_type<T> : public common_type<T, T> {};

// two types
namespace detail {

//template <typename T1, typename T2, typename D1 = >
//struct common_type_2_impl {
//  /
//};

} // namespace detail

///**************************************************************************///
///                                 is_void                                  ///
///**************************************************************************///

//template <typename IntT>
//struct is_void



////////////////////////////////////////////////////////////////////////////////
/**
 * Addition to std.
 */

////////////////////////////////////////////////////////////////////////////////
////////                           detect                               ////////
////////////////////////////////////////////////////////////////////////////////

// NOTE: use concepts instead of detection idiom

struct nonesuch {
  nonesuch() = delete;
  nonesuch(const nonesuch& other) = delete;
  nonesuch& operator=(const nonesuch& other) = delete;
  ~nonesuch() = delete;
};

template <template <typename...> class Template, typename... Types>
struct is_detected : public bool_constant<detected<Template, Types...>> {};

template <template <typename...> class Template, typename... Types>
static constexpr bool is_detected_v = is_detected<Template, Types...>::value;

namespace detail {

template <bool kIsDetected,
    typename Default,
    template <typename...> class Template,
    typename... Types>
struct detector_impl {
  using value_t = false_type;
  using type = Default;
};

template <typename Default,
    template <typename...> class Template,
    typename... Types>
struct detector_impl<true, Default, Template, Types...> {
  using value_t = true_type;
  using type = Default;
};

template <typename Default,
    template <typename...> class Template,
    typename... Types>
struct detector
    : public detector_impl<is_detected_v<Template, Types...>,
                           Default,
                           Template,
                           Types...> {
};

} // namespace detail

template <template <typename...> class Template, typename... Types>
using detected_t = detail::detector<nonesuch, Template, Types...>;

template <typename Default,
    template <typename...> class Template,
    typename... Types>
using detected_or = detail::detector<Default, Template, Types...>;

template <typename Default,
    template <typename...> class Template,
    typename... Types>
using detected_or_t = detected_or<Default, Template, Types...>::type;

//template <typename Expected,
//    template <typename...> class Template,
//    typename... Types>
//using is_detected_exact =




} // namespace tb
