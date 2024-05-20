#include <type_traits>
#include <concepts>

#include <toolbox/intrusive/list.hpp>
#include <list>

namespace tbi = toolbox::intrusive;

template <std::input_iterator Iter>
void foo() {

}

template <typename T>
void deleted() = delete;

int main() {
  return 0;
}
