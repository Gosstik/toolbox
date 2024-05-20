#include <cassert>
#include <iostream>
#include <vector>

int main() {
  std::cout << "Start play vector.\n";
  std::vector<int> vec;
  assert(vec.size() == 0);
  vec.push_back(1);
  assert(vec.size() == 1);
  std::cout << "Finish play vector.\n";
}
