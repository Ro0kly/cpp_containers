#include "s21/s21_containers.h"
#include <iostream>
#include <map>
#include <string>
#include <utility>
int main() {
  map<int, std::string> m = {{0, "dada"}, {1, "gddfdsf"}, {2, "hgfg"}};
  m.insert({3, "a"});
  m.insert({4, "b"});
  m.insert({5, "c"});
  m.insert({6, "d"});
  std::cout << m.size();
  return 0;
}
