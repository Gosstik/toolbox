#pragma once

template <typename T>
concept complete = requires(T obj) {
  sizeof(obj);
};

template <template <typename...> class Template, typename... Types>
concept detected = requires() {
  typename Template<Types...>;
};
