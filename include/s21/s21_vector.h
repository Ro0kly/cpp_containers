#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <new>
#include <ostream>
#include <stdexcept>
#include <utility>

#ifndef VECTOR_H
#define VECTOR_H

namespace s21 {
template <typename T> class vector {
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
  vector() noexcept = default;
  vector(size_type size) {
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
  ~vector() noexcept {
    clear();
    capacity_ = 0;
    ::operator delete(data_);
  }

  vector(const vector<value_type> &other) {
    if (other.size_ > 0) {
      allocate(other.size_);
      for (size_type i = 0; i < size_; ++i) {
        new (&data_[i]) value_type(other[i]);
      }
    }
  }

  vector(std::initializer_list<value_type> list) : size_(0), capacity_(0) {
    data_ =
        static_cast<pointer>(::operator new(list.size() * sizeof(value_type)));
    for (const auto &item : list) {
      new (&data_[size_++]) value_type(item);
    }
    capacity_ = size_;
  }

  vector(vector &&other) noexcept
      : size_(other.size_), capacity_(other.size_), data_(other.data_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  vector &operator=(const vector &other) {
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

  vector &operator=(vector &&other) {
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
      throw std::out_of_range(
          "Operator []: Invalid index. Index out of range.");
    }
    return data_[index];
  }

  reference at(size_type index) const {
    if (index >= size_) {
      throw std::out_of_range(
          "Operator \"at\": Invalid index. Index out of range.");
    }
    return data_[index];
  }

  iterator insert(iterator pos, const_reference value) {
    size_type new_pos = pos - begin();
    if (new_pos > size_) {
      throw std::out_of_range(
          "Insert. Invalid position: position to insert, out of range.");
    }
    if (size_ == capacity_) {
      size_type new_size = size_ + 1;
      size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
      pointer new_data = static_cast<pointer>(
          ::operator new(new_capacity * sizeof(value_type)));

      for (size_type i = 0; i < new_pos; ++i) {
        new (&new_data[i]) value_type(std::move_if_noexcept(data_[i]));
      }
      new (&new_data[new_pos]) T(std::move_if_noexcept(value));
      for (size_type i = new_pos + 1; i < new_size; ++i) {
        new (&new_data[i]) value_type(std::move_if_noexcept(data_[i - 1]));
      }
      clear();
      ::operator delete(data_);
      data_ = new_data;
      size_ = new_size;
      /*std::cout << "SIZE 1: " << size_ << " VALUE: " << value << std::endl;*/
      capacity_ = new_capacity;
    } else if (size_ < capacity_) {
      new (&data_[size_]) T(std::move_if_noexcept(data_[size_ - 1]));
      for (size_type i = size_ - 1; i > new_pos; --i) {
        data_[i] = data_[i - 1];
      }
      data_[new_pos] = std::move_if_noexcept(value);
      ++size_;
      /*std::cout << "SIZE 2: " << size_ << " VALUE: " << value << std::endl;*/
    }
    return begin() + new_pos;
  }

  void erase(iterator pos) {
    if (size_ == 0) {
      throw std::runtime_error(
          "Erase. The size is zero, you can't remove anything.");
    }
    size_type remove_pos = pos - begin();
    if (remove_pos > size_) {
      throw std::runtime_error(
          "Erase. Invalid position: position to erase, out of bounds.");
    }
    data_[remove_pos].~value_type();
    for (size_type i = remove_pos; i < size_ - 1; ++i) {
      data_[i] = data_[i + 1];
    }
    --size_;
  }

  void push_back(const_reference value) { insert(end(), value); }
  void pop_back() { erase(end()); }
  void swap(vector &other) noexcept {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
  }

  pointer data() const noexcept { return data_; }
  size_type size() const noexcept { return size_; }
  size_type capacity() const noexcept { return capacity_; }
  const_reference front() const {
    if (size_ == 0) {
      throw std::out_of_range(
          "Front. The size is zero, you can't get anything.");
    }
    return data_[0];
  }
  const_reference back() const {
    if (size_ == 0) {
      throw std::out_of_range(
          "Back. The size is zero, you can't get anything.");
    }

    return data_[size_ - 1];
  }
  iterator begin() const { return data_; }
  iterator end() const { return data_ + size_; }
  bool empty() const { return begin() == end(); }
};

} // namespace s21

#endif
