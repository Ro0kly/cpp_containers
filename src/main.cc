#include "s21/s21_containers.h"
#include <iostream>
int main() {
  s21::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  std::cout << v.size();
  return 0;
}
