#pragma once

#include <concepts>

namespace toolbox {

template <typename T>
concept complete = requires(T obj) {
  sizeof(obj);
};

////////////////////////////////////////////////////////////////////////////////
//        detected
////////////////////////////////////////////////////////////////////////////////

template <template <typename...> class Template, typename... Types>
concept detected = requires() {
  typename Template<Types...>;
};

////////////////////////////////////////////////////////////////////////////////
//        default_erasable
////////////////////////////////////////////////////////////////////////////////

template <typename Value>
concept default_erasable = requires(Value* p) {
  std::destroy_at(p);
};

////////////////////////////////////////////////////////////////////////////////
//        allocator_erasable
////////////////////////////////////////////////////////////////////////////////

template <typename Container, typename Alloc, typename Value>
concept allocator_erasable = requires(Alloc a, Value* p) {
  requires std::same_as<
      typename Container::allocator_type,
      typename std::allocator_traits<Alloc>::template rebind_alloc<Value>
  >;
  std::allocator_traits<Alloc>::destroy(a, p);
};

////////////////////////////////////////////////////////////////////////////////
//        allocator_aware
////////////////////////////////////////////////////////////////////////////////

template <typename Container>
concept allocator_aware = requires (Container c) {
  { c.get_allocator() } -> std::same_as<typename Container::allocator_type>;
};

////////////////////////////////////////////////////////////////////////////////
//        erasable
////////////////////////////////////////////////////////////////////////////////

template <typename Container, typename Value>
concept erasable =
    (allocator_aware<Container>
     && allocator_erasable<Container,
                           typename Container::allocator_type, Value>)
    || (!allocator_aware<Container> && default_erasable<Value>);

////////////////////////////////////////////////////////////////////////////////
//        container
////////////////////////////////////////////////////////////////////////////////

template <typename C>
concept container = requires(C c, const C cc) {
  requires std::regular<C>;
  requires std::swappable<C>;
  requires erasable<typename C::value_type, C>;

  requires std::same_as<typename C::reference, typename C::value_type &>;
  requires std::same_as<typename C::const_reference,
                        const typename C::value_type &>;

  requires std::forward_iterator<typename C::iterator>;
  requires std::forward_iterator<typename C::const_iterator>;
  requires std::convertible_to<typename C::iterator,
                               typename C::const_iterator>;

  requires std::signed_integral<typename C::difference_type>;
  requires std::unsigned_integral<typename C::size_type>;
  requires std::numeric_limits<typename C::difference_type>::max() <=
           std::numeric_limits<typename C::size_type>::max();

  requires std::same_as<
      typename C::difference_type,
      typename std::iterator_traits<typename C::iterator>::difference_type
  >;
  requires std::same_as<
      typename C::difference_type,
      typename std::iterator_traits<typename C::const_iterator>::difference_type
  >;

  { c.begin() } -> std::same_as<typename C::iterator>;
  { c.end() } -> std::same_as<typename C::iterator>;
  { cc.begin() } -> std::same_as<typename C::const_iterator>;
  { cc.end() } -> std::same_as<typename C::const_iterator>;
  { c.cbegin() } -> std::same_as<typename C::const_iterator>;
  { c.cend() } -> std::same_as<typename C::const_iterator>;
  { c.size() } -> std::same_as<typename C::size_type>;
  { c.max_size() } -> std::same_as<typename C::size_type>;
  { c.empty() } -> std::convertible_to<bool>;
};

} // namespace toolbox
