#include "s21_vector.h"
#ifndef QUEUE_H
#define QUEUE_H

namespace s21 {
template <typename T, typename Container = vector<T>> class queue {
private:
  Container container;

public:
  using size_type = Container::size_type;
  using value_type = Container::value_type;
  using reference = Container::reference;
  using const_reference = Container::const_reference;

  queue() {}
  queue(const queue &other) : container(other.container) {}
  queue(queue &&other) : container(std::move(other.container)) {}
  ~queue() {}
  queue(std::initializer_list<value_type> list) : container(list) {}

  queue &operator=(const queue &other) {
    if (this == &other) {
      return *this;
    }
    container = other.container;
    return *this;
  }

  queue &operator=(queue &&other) {
    if (this != &other) {
      container = std::move(other.container);
    }
    return *this;
  }

  void push(const_reference value) { container.push_back(value); }
  void pop() { container.erase(container.begin()); }
  void swap(queue &other) { container.swap(other.container); }

  size_type size() const { return container.size(); }
  bool empty() const { return container.empty(); }
  const_reference front() const { return container.front(); }
  const_reference back() const { return container.back(); }
};

} // namespace s21

#endif
