#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <new>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>
template <typename T> class Vector {
public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = pointer;

private:
  size_type size_ = 0;
  size_type capacity_ = 0;
  pointer data_ = nullptr;

public:
  Vector() = default;
  Vector(size_type size) {
    if (size > 0) {
      allocate(size);
      for (size_type i = 0; i < size_; ++i) {
        new (&data_[i]) value_type();
      }
    } else if (size < 0) {
      throw std::runtime_error(
          "Constructor: Invalid size. Size can't be less than zero.");
    }
  }
  ~Vector() {
    clear();
    capacity_ = 0;
    ::operator delete(data_);
  }

  Vector(const Vector<value_type> &other) {
    if (other.size_ > 0) {
      allocate(other.size_);
      for (size_type i = 0; i < size_; ++i) {
        new (&data_[i]) value_type(other[i]);
      }
    }
  }

  Vector(std::initializer_list<value_type> list) : size_(0), capacity_(0) {
    data_ =
        static_cast<pointer>(::operator new(list.size() * sizeof(value_type)));
    for (const auto &item : list) {
      new (&data_[size_++]) value_type(item);
    }
    capacity_ = size_;
  }

  Vector(Vector &&other)
      : size_(other.size_), capacity_(other.size_), data_(other.data_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  reference operator=(const Vector &other) {
    if (this == &other) {
      return *this;
    }
    clear();
    capacity_ = 0;
    ::operator delete(data_);
    allocate(other.size_);
    for (size_type i = 0; i < size_; ++i) {
      new (&data_[i]) value_type(other[i]);
    }
    return *this;
  }

  Vector &operator=(Vector &&other) {
    if (this == &other) {
      return *this;
    }
    clear();
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.size_;
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
    return *this;
  }

  void allocate(int count) {
    size_ = count;
    capacity_ = count;
    data_ =
        static_cast<pointer>(::operator new(capacity_ * sizeof(value_type)));
  }
  void clear() {
    for (size_type i = 0; i < size_; ++i) {
      data_[i].~value_type();
    }
    size_ = 0;
  }

  void reserve(size_type new_capacity) {
    if (new_capacity <= capacity_) {
      return;
    }
    pointer new_data =
        static_cast<pointer>(::operator new(new_capacity * sizeof(value_type)));
    for (size_type i = 0; i < size_; ++i) {
      new (&new_data[i]) value_type(std::move_if_noexcept(data_[i]));
      data_[i].~value_type();
    }
    ::operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
  }

  void shrink_to_fit() {
    if (size_ == capacity_) {
      return;
    }
    pointer new_data =
        static_cast<pointer>(::operator new(size_ * sizeof(value_type)));
    for (size_type i = 0; i < size_; ++i) {
      new (&new_data[i]) value_type(data_[i]);
      data_[i].~value_type();
    }
    ::operator delete(data_);
    data_ = new_data;
    capacity_ = size_;
  }

  reference operator[](size_type index) const {
    if (index >= size_) {
      throw std::runtime_error(
          "Operator []: Invalid index. Index out of range.");
    }
    return data_[index];
  }

  reference at(size_type index) const {
    if (index >= size_) {
      throw std::runtime_error(
          "Operator \"at\": Invalid index. Index out of range.");
    }
    return data_[index];
  }

  pointer data() const { return data_; }
  size_type size() const { return size_; }
  size_type capacity() const { return capacity_; }
  const_reference front() const { return data_[0]; }
  const_reference back() const { return data_[size_ - 1]; }
  iterator begin() const { return data_; }
  iterator end() const { return data_ + size_; }
  bool empty() const { return begin() == end(); }

  iterator insert(iterator pos, const_reference value) {
    size_type new_pos = pos - begin();
    if (new_pos > size_) {
      throw std::runtime_error("Insert. Invalid position: position to insert "
                               "is bigger than container size.");
    }
    if (size_ == capacity_) {
      size_type new_size = size_ + 1;
      size_type new_capacity = capacity_ * 2;
      pointer new_data = static_cast<pointer>(
          ::operator new(new_capacity * sizeof(value_type)));

      for (size_type i = 0; i < new_pos; ++i) {
        new (&new_data[i]) value_type(std::move_if_noexcept(data_[i]));
      }
      new (&new_data[new_pos]) T(value);
      for (size_type i = new_pos + 1; i < new_size; ++i) {
        new (&new_data[i]) value_type(std::move_if_noexcept(data_[i - 1]));
      }
      clear();
      ::operator delete(data_);
      data_ = new_data;
      size_ = new_size;
      capacity_ = new_capacity;
    } else if (size_ < capacity_) {
      new (&data_[size_]) T(std::move_if_noexcept(data_[size_ - 1]));
      for (size_type i = size_ - 1; i > new_pos; --i) {
        data_[i] = data_[i - 1];
      }
      data_[new_pos] = value;
      ++size_;
    }

    return pos;
  }
};
