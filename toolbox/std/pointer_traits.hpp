#pragma once

#include <memory>

namespace toolbox {

namespace detail {

/// ReplaceFirstArg
// Get Template<T, Types...> and U.
// Return Template<U, Types...>.
template <typename Template, typename U>
struct ReplaceFirstArg {};

template <template <typename, typename...> class Template, typename U, typename T, typename... Types>
struct ReplaceFirstArg<Template<T, Types...>, U> {
  using Type = Template<U, Types...>;
};

template <typename Template, typename U>
using ReplaceFirstArgT = typename ReplaceFirstArg<Template,U>::Type;

// -----------------------------------------------------------------------------

/// PtrTraitsElem


} // namespace detail

// -----------------------------------------------------------------------------

/// PointerTraits
// For custom type
template <typename Ptr>
struct PointerTraits {

};


// For raw pointer
template <typename Ptr>
struct PointerTraits<Ptr*> {
  using ElementType = Ptr;
  using Pointer = Ptr*;
  using DifferenceType = ptrdiff_t;

  template <typename U>
  using Rebind = U*;
};


} // namespace tb