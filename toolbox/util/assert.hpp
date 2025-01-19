#pragma once

#include <iostream>

// Suppress compiler warnings about unused variable/argument
#define TOOLBOX_UNUSED(x) \
  do {                    \
    (void)(x);            \
  } while (false)

// Unreachable by control flow
#define TOOLBOX_UNREACHABLE() std::abort()


#ifndef NDEBUG

#define TOOLBOX_ASSERT(cond, error)                                          \
  do {                                                                       \
    if (!(cond)) {                                                           \
      std::cerr << "Toolbox assertion '"                                     \
      << #cond << "' failed: " << error << '\n';                             \
      TOOLBOX_UNREACHABLE();                                                 \
    }                                                                        \
  } while (false)

#define TOOLBOX_VERIFY(cond, error)                                          \
  do {                                                                       \
    if (!(cond)) {                                                           \
      std::cerr << "Toolbox verify '"                                        \
      << #cond << "' failed: " << error << '\n';                             \
    }                                                                        \
  } while (false)

#else

#define TOOLBOX_ASSERT(cond, error) \
  do {                              \
    if (false) {                    \
      bool to_ignore = cond;        \
      TOOLBOX_UNUSED(to_ignore);    \
    }                               \
  } while (false)

#define TOOLBOX_VERIFY(cond, error)  \
  do {                               \
    if (false) {                     \
      bool to_ignore = cond;         \
      TOOLBOX_UNUSED(to_ignore);     \
    }                                \
  } while (false)

#endif