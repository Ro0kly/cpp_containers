#include "s21/s21_containers.h"
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>
int main() {
  /*map<int, std::string> m = {{0, "dada"}, {1, "gddfdsf"}, {2, "hgfg"}};*/
  /*m.insert({3, "a"});*/
  /*m.insert(4, "b");*/
  /*m.insert(5, "c");*/
  /*m.insert(6, "d");*/
  /*std::cout << m.size() << std::endl;*/
  /*std::cout << m.begin()->first << std::endl;*/
  /*std::cout << m.begin()->second << std::endl;*/
  /*map<std::string, int> m;*/
  /**/
  /*// 1. Insert new element*/
  /*auto [it1, inserted1] = m.insert_or_assign("apple", 5);*/
  /*// it1 points to {"apple", 5}, inserted1 = true*/
  /**/
  /*// 2. Update existing*/
  /*auto [it2, inserted2] = m.insert_or_assign("apple", 7);*/
  /*// it2 points to {"apple", 7}, inserted2 = false*/
  /**/
  /*// 3. Chain operations*/
  /*m.insert_or_assign("banana", 3).first->second += 2;*/
  /*m.print();*/

  /*map<std::string, int> m1 = {{"a", 1}, {"b", 2}};*/
  /*map<std::string, int> m2 = {{"x", 10}, {"y", 20}};*/
  /**/
  /*// Member swap*/
  /*m1.swap(m2);*/
  /**/
  // Non-member swap
  /*swap(m1, m2);*/
  /*map<std::string, int> m1 = {{"a", 1}, {"b", 2}};*/
  /*map<std::string, int> m2 = {{"b", 20}, {"c", 30}};*/
  /**/
  /*m1.merge(m2);*/
  /*// m1 = {"a":1, "b":2, "c":30}*/
  /*// m2 = {"b":20} (only conflicting key remains)*/
  /*m1.print();*/
  /*m2.print();*/

  /*map<std::string, int> m = {{"apple", 5}, {"banana", 3}};*/
  /**/
  /*if (m.contains("apple")) {*/
  /*  std::cout << "Found apple!\n";*/
  /*}*/
  /**/
  /*if (!m.contains("orange")) {*/
  /*  std::cout << "No oranges\n";*/
  /*}*/

  /*map<std::string, int> scores;*/
  /*scores["Alice"] = 100; // Inserts new*/
  /*scores["Alice"] += 5;  // Updates existing*/
  /**/
  /*// Default-initializes to 0*/
  /*int bob_score = scores["Bob"]; // bob_score = 0*/
  map<std::string, int> m;
  m.at("foo") = 42;      // Non-const version
  int val = m.at("foo"); // Const version
  return 0;
}
