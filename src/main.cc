#include "s21_containers.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

class ComplexObject {
public:
  ComplexObject() {
    // Выделяем память для внутреннего массива
    data_ = new int[10];
    data_[0] = 10050;
    std::cout << "ComplexObject constructed, memory allocated." << std::endl;
  }

  ~ComplexObject() {
    // Освобождаем память
    delete[] data_;
    std::cout << "ComplexObject destroyed, memory deallocated." << std::endl;
  }

public:
  int *data_;
};

int main() {
  Vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  v.erase(v.begin());
  for (size_t i = 0; i < v.size(); ++i) {
    std::cout << v[i] << " ";
  }
  return 0;
}
