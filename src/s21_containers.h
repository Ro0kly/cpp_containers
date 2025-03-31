#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <type_traits>
template <typename T> class Vector {

private:
  size_t size_;
  size_t capacity_;
  T *data_ = nullptr;

public:
  Vector() = default;
  Vector(size_t size) : size_(size), capacity_(size), data_(new T[size]) {
    // for (size_t i = 0; i < size; ++i) {
    //   new (&data_[i]) T();
    // }
  }

  const size_t size() const { return size_; }
  const size_t capacity() const { return capacity_; }
  const T *const data() const { return data_; }
  T *accessed_data() const { return data_; }
};
