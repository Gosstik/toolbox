#include <cassert>
#include <iostream>

#include <toolbox/containers/vector.hpp>

int main() {
  std::cout << "Start play vector\n";
  Vector<int> vec;
  assert(vec.size() == 0);
  vec.inc_size();
  assert(vec.size() == 1);
  std::cout << "Finish play vector\n";
}