#pragma once

#include <iostream>

// Add "#define NDEBUG" for Release version

////////////////////////////////////////////////////////////////////////////////

// Unreachable by control flow
#define TOOLBOX_UNREACHABLE() std::abort()

////////////////////////////////////////////////////////////////////////////////

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
  (void)nullptr

#define TOOLBOX_VERIFY(cond, error) \
  (void)nullptr

#endif
