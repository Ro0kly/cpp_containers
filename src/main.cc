#include "s21_containers.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <utility>

int main() {
  Vector<int> vec(3);
  std::cout << vec.accessed_data()[0];
  return 0;
}
