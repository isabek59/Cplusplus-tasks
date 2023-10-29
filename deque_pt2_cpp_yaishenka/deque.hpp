/**
 * @file deque.hpp
 * @author yaishenka
 * @date 05.01.2023
 */
#pragma once
#include <exception>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

template <typename T, typename Alloc = std::allocator<T>>
class Deque {
 public:
  template <bool IsConst>
  class Iterator;

  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() { return iterator(&arr_[first_block_], first_element_); }

  iterator end() {
    iterator res(&arr_[last_block_], last_element_);
    return res;
  }

  const_iterator cbegin() {
    return const_iterator(&arr_[first_block_], first_element_);
  }

  const_iterator cend() {
    return const_iterator(&arr_[last_block_], last_element_);
  }

  reverse_iterator rbegin() {
    auto iter = this->end();
    return reverse_iterator(iter);
  }

  reverse_iterator rend() {
    auto iter = this->begin();
    return reverse_iterator(iter);
  }

  const_reverse_iterator crbegin() { return const_reverse_iterator(end()); }

  const_reverse_iterator crend() { return const_reverse_iterator(begin()); }

  Deque();

  Deque(const Alloc& alloc);

  Deque(const Deque& other);

  Deque(size_t count, const Alloc& alloc = Alloc());

  Deque(size_t count, const T& value, const Alloc& alloc = Alloc());

  Deque(Deque&& other);

  Deque(std::initializer_list<T> init, const Alloc& alloc = Alloc());

  Deque& operator=(const Deque& other);

  Deque& operator=(Deque&& other);

  ~Deque() {
    if (!arr_.empty()) {
      if (arr_[first_block_] == nullptr || arr_[last_block_] == nullptr) {
        return;
      }
      while (first_block_ < last_block_) {
        while (first_element_ < kDefaultBlockSize) {
          alloc_traits::destroy(alloc_, arr_[first_block_] + first_element_);
          ++first_element_;
        }
        alloc_traits::deallocate(alloc_, arr_[first_block_], kDefaultBlockSize);
        first_element_ = 0;
        ++first_block_;
      }

      while (first_element_ < last_element_) {
        alloc_traits::destroy(alloc_, arr_[last_block_] + first_element_);
        ++first_element_;
      }
      alloc_traits::deallocate(alloc_, arr_[last_block_], kDefaultBlockSize);
    }
  }

  T& operator[](size_t num);
  const T& operator[](size_t num) const;
  T& at(size_t num);
  const T& at(size_t num) const;

  void push_back(const T& value);
  void push_back(T&& value);
  void push_front(const T& value);
  void push_front(T&& value);
  void pop_back();
  void pop_front();

  template <typename... Args>
  void emplace_back(Args&&... args);

  template <typename... Args>
  void emplace_front(Args&&... args);

  size_t size() const;
  bool empty();

  Alloc get_allocator() { return alloc_; }

  void insert(const iterator& iter, const T& elem) {
    if (iter == end()) {
      push_back(elem);
      return;
    }

    T temp = *(end() - 1);
    auto iter2 = end() - 1;
    while (iter < iter2) {
      *iter2 = *(iter2 - 1);
      --iter2;
    }
    *iter2 = T(elem);
    push_back(temp);
  }

  void erase(iterator iter) {
    auto it_end = end();

    while (iter <= it_end) {
      *iter = *(iter + 1);
      ++iter;
    }

    --arr_size_;
    if (last_element_ == 0) {
      last_element_ = kDefaultBlockSize - 1;
      --last_block_;
    } else {
      --last_element_;
    }
  }

 private:
  std::vector<T*> arr_;
  size_t arr_size_;
  size_t map_size_;

  size_t first_element_;
  size_t first_block_;

  size_t last_element_;
  size_t last_block_;
  Alloc alloc_;
  using alloc_traits = std::allocator_traits<Alloc>;

  static const size_t kDefaultArrSize = 8;
  static const size_t kDefaultBlockSize = 1000;

  void expand_arr();
  void expand_arr1();
};

template <typename T, typename Alloc>
template <bool IsConst>
class Deque<T, Alloc>::Iterator {
 public:
  using is_const = std::conditional_t<IsConst, const T, T>;
  using value_type = T;
  using pointer = is_const*;
  using reference = is_const&;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = int;

  Iterator(T** ptr, const size_t& index) : ptr_(ptr), num_(index) {}

  reference operator*() const { return *(*ptr_ + num_); }

  pointer operator->() { return *ptr_ + num_; }

  value_type operator->() const { return *(*ptr_ + num_); }

  Iterator& operator++() {
    if (num_ < kDefaultBlockSize - 1) {
      ++num_;
    } else {
      ++ptr_;
      num_ = 0;
    }
    return *this;
  }

  Iterator operator++(int) {
    Iterator copy(*this);
    ++*this;
    return copy;
  }

  Iterator& operator--() {
    if (num_ > 0) {
      --num_;
    } else {
      --ptr_;
      num_ = kDefaultBlockSize - 1;
    }
    return *this;
  }

  Iterator operator--(int) {
    Iterator copy(*this);
    --*this;
    return copy;
  }

  Iterator& operator+=(int value) {
    num_ += value;
    ptr_ += num_ / kDefaultBlockSize;
    num_ = num_ % kDefaultBlockSize;
    return *this;
  }

  Iterator& operator-=(int value) {
    if (static_cast<size_t>(value) >= num_ + 1) {
      value -= num_ + 1;
      ptr_ -= value / kDefaultBlockSize + 1;
      num_ = kDefaultBlockSize - 1 - (value % kDefaultBlockSize);

      return *this;
    }
    num_ -= value;
    return *this;
  }

  friend Iterator operator+(int value, const Iterator& iter) {
    Iterator copy(iter);
    copy += value;
    return copy;
  }

  friend Iterator operator+(const Iterator& iter, int value) {
    return value + iter;
  }

  friend Iterator operator-(const Iterator& iter, int value) {
    Iterator copy(iter);
    copy -= value;
    return copy;
  }

  friend bool operator==(const Iterator& first, const Iterator& second) {
    return (first.ptr_ == second.ptr_) && (first.num_ == second.num_);
  }
  friend bool operator!=(const Iterator& first, const Iterator& second) {
    return !(first == second);
  }

  friend bool operator<(const Iterator& first, const Iterator& second) {
    return (first.ptr_ < second.ptr_) ||
           (first.ptr_ == second.ptr_ && first.num_ < second.num_);
  }

  friend bool operator>(const Iterator& first, const Iterator& second) {
    return !(first < second || first == second);
  }

  friend bool operator<=(const Iterator& first, const Iterator& second) {
    return !(first > second);
  }

  friend bool operator>=(const Iterator& first, const Iterator& second) {
    return !(first < second);
  }

  difference_type operator-(const Iterator& other) {
    return (ptr_ - other.ptr_) * kDefaultBlockSize + (num_ - other.num_);
  }

 private:
  T** ptr_;
  size_t num_;
};

template <typename T, typename Alloc>
Deque<T, Alloc>::Deque() {
  arr_.resize(kDefaultArrSize);
  map_size_ = kDefaultArrSize;

  first_block_ = kDefaultArrSize / 2;
  first_element_ = 0;

  last_block_ = first_block_;
  last_element_ = first_element_;

  arr_size_ = 0;

  arr_[first_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
}

template <typename T, typename Alloc>
Deque<T, Alloc>::Deque(const Alloc& alloc) {
  arr_.resize(kDefaultArrSize);
  map_size_ = kDefaultArrSize;

  first_block_ = kDefaultArrSize / 2;
  first_element_ = 0;

  last_block_ = first_block_;
  last_element_ = first_element_;

  arr_size_ = 0;

  arr_[first_block_] = alloc_traits::allocate(alloc, kDefaultBlockSize);
}

template <typename T, typename Alloc>
Deque<T, Alloc>& Deque<T, Alloc>::operator=(const Deque& other) {
  if (alloc_traits::propagate_on_container_copy_assignment::value &&
      alloc_ != other.alloc_) {
    alloc_ = other.alloc_;
  }
  Deque<T, Alloc> tmp(other);
  std::swap(arr_, tmp.arr_);
  std::swap(arr_size_, tmp.arr_size_);
  std::swap(map_size_, tmp.map_size_);

  std::swap(first_element_, tmp.first_element_);
  std::swap(first_block_, tmp.first_block_);

  std::swap(last_element_, tmp.last_element_);
  std::swap(last_block_, tmp.last_block_);
  return *this;
}

template <typename T, typename Alloc>
Deque<T, Alloc>& Deque<T, Alloc>::operator=(Deque&& other) {
  if (*this != other) {
    alloc_ = std::move(other.alloc_);
    arr_size_ = other.arr_size_;
    map_size_ = other.map_size_;

    first_element_ = other.first_element_;
    first_block_ = other.first_block_;

    last_element_ = other.last_element_;
    last_block_ = other.last_block_;
    arr_.resize(arr_size_);

    for (size_t i = first_block_; i <= last_block_; ++i) {
      arr_[i] = other.arr_[i];
      other.arr_[i] = nullptr;
    }
    other.arr_.clear();
    other.arr_size_ = 0;
  }
  return *this;
}

template <typename T, typename Alloc>
Deque<T, Alloc>::Deque(const Deque& other) {
  arr_size_ = other.arr_size_;
  map_size_ = other.map_size_;

  first_element_ = 0;
  first_block_ = other.first_block_;

  last_element_ = other.last_element_ - other.first_element_;
  last_block_ = other.last_block_;
  alloc_ = alloc_traits::select_on_container_copy_construction(other.alloc_);

  arr_.resize(map_size_);
  for (size_t i = first_block_; i < last_block_; ++i) {
    arr_[i] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    size_t arrays_to_del = 0;
    try {
      for (; arrays_to_del < kDefaultBlockSize; arrays_to_del++) {
        alloc_traits::construct(alloc_, arr_[i] + arrays_to_del,
                                other.arr_[i][arrays_to_del]);
      }
    } catch (...) {
      for (size_t j = 0; j < arrays_to_del; ++j) {
        alloc_traits::destroy(alloc_, arr_[i] + j);
      }
      alloc_traits::deallocate(alloc_, arr_[i], kDefaultBlockSize);
      throw;
    }
  }
  arr_[last_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
  size_t arrays_to_del = 0;
  try {
    for (; arrays_to_del < last_element_; ++arrays_to_del) {
      alloc_traits::construct(alloc_, arr_[last_block_] + arrays_to_del,
                              other.arr_[last_block_][arrays_to_del]);
    }
  } catch (...) {
    for (size_t j = 0; j < arrays_to_del; ++j) {
      alloc_traits::destroy(alloc_, arr_[last_block_] + j);
    }
    alloc_traits::deallocate(alloc_, arr_[last_block_], kDefaultBlockSize);
    throw;
  }
}

template <typename T, typename Alloc>
Deque<T, Alloc>::Deque(size_t count, const Alloc& alloc)
    : arr_size_(count), alloc_(alloc) {
  size_t amount_of_arr;
  size_t rest_blocks = count % kDefaultBlockSize;
  amount_of_arr = count / kDefaultBlockSize + 1;

  arr_.resize(amount_of_arr * 4);
  map_size_ = amount_of_arr * 4;

  first_block_ = map_size_ / 2;
  first_element_ = 0;

  last_block_ = first_block_ + amount_of_arr - 1;

  for (size_t i = first_block_; i < last_block_; ++i) {
    arr_[i] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    size_t arrays_to_del = 0;
    try {
      for (; arrays_to_del < kDefaultBlockSize; ++arrays_to_del) {
        alloc_traits::construct(alloc_, arr_[i] + arrays_to_del);
      }
    } catch (...) {
      for (size_t j = 0; j < arrays_to_del; ++j) {
        alloc_traits::destroy(alloc_, arr_[arrays_to_del] + j);
      }
      alloc_traits::deallocate(alloc_, arr_[i], kDefaultBlockSize);
      throw;
    }
  }
  arr_[last_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
  size_t arrays_to_del = 0;
  try {
    for (; arrays_to_del < rest_blocks; ++arrays_to_del) {
      alloc_traits::construct(alloc_, arr_[last_block_] + arrays_to_del);
    }
  } catch (...) {
    for (size_t j = 0; j < arrays_to_del; ++j) {
      alloc_traits::destroy(alloc_, arr_[last_block_] + j);
    }
    alloc_traits::deallocate(alloc_, arr_[last_block_], kDefaultBlockSize);
    throw;
  }

  last_element_ = rest_blocks;
}

template <typename T, typename Alloc>
Deque<T, Alloc>::Deque(Deque&& other) {
  arr_size_ = other.arr_size_;
  map_size_ = other.map_size_;

  first_element_ = other.first_element_;
  first_block_ = other.first_block_;

  last_element_ = other.last_element_;
  last_block_ = other.last_block_;
  alloc_ = std::move(other.alloc_);
  arr_ = std::move(other.arr_);
  other.arr_.clear();
  other.arr_size_ = 0;
  other.first_block_ = kDefaultArrSize / 2;
  other.last_block_ = other.first_block_;
  other.first_element_ = 0;
  other.last_element_ = 0;
}

template <typename T, typename Alloc>
Deque<T, Alloc>::Deque(std::initializer_list<T> init, const Alloc& alloc)
    : alloc_(alloc) {
  arr_.resize(kDefaultArrSize);
  map_size_ = kDefaultArrSize;

  first_block_ = kDefaultArrSize / 2;
  first_element_ = 0;

  last_block_ = first_block_;
  last_element_ = first_element_;

  arr_size_ = 0;
  arr_[first_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
  for (const auto& elem : init) {
    push_back(elem);
  }
}

template <typename T, typename Alloc>
Deque<T, Alloc>::Deque(size_t count, const T& value, const Alloc& alloc)
    : arr_size_(count), alloc_(alloc) {
  size_t amount_of_arr;
  size_t rest_blocks = count % kDefaultBlockSize;
  amount_of_arr = count / kDefaultBlockSize + 1;

  arr_.resize(amount_of_arr * 4);
  map_size_ = amount_of_arr * 4;

  first_block_ = map_size_ / 2;
  first_element_ = 0;

  last_block_ = first_block_ + amount_of_arr - 1;

  for (size_t i = first_block_; i < last_block_; ++i) {
    arr_[i] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    size_t arrays_to_del = 0;
    try {
      for (; arrays_to_del < kDefaultBlockSize; ++arrays_to_del) {
        alloc_traits::construct(alloc_, arr_[i] + arrays_to_del, value);
      }
    } catch (...) {
      for (size_t j = 0; j < arrays_to_del; ++j) {
        alloc_traits::destroy(alloc_, arr_[arrays_to_del] + j);
      }
      alloc_traits::deallocate(alloc_, arr_[i], kDefaultBlockSize);
      throw;
    }
  }

  arr_[last_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
  size_t arrays_to_del = 0;
  try {
    for (; arrays_to_del < rest_blocks; ++arrays_to_del) {
      alloc_traits::construct(alloc_, arr_[last_block_] + arrays_to_del, value);
    }
  } catch (...) {
    for (size_t j = 0; j < arrays_to_del; ++j) {
      alloc_traits::destroy(alloc_, arr_[last_block_] + j);
    }
    alloc_traits::deallocate(alloc_, arr_[last_block_], kDefaultBlockSize);
    throw;
  }

  last_element_ = rest_blocks;
}

template <typename T, typename Alloc>
T& Deque<T, Alloc>::operator[](size_t num) {
  size_t block_num = first_block_;
  size_t element_num;
  if (num < kDefaultBlockSize - first_element_) {
    element_num = num + first_element_;
  } else {
    num -= kDefaultBlockSize - first_element_;
    ++block_num;
    size_t rest = num / kDefaultBlockSize;
    block_num += rest;
    element_num = num - rest * kDefaultBlockSize;
  }
  return arr_[block_num][element_num];
}

template <typename T, typename Alloc>
const T& Deque<T, Alloc>::operator[](size_t num) const {
  size_t block_num = first_block_;
  size_t element_num;
  if (num < kDefaultBlockSize - first_element_) {
    element_num = num + first_element_;
  } else {
    num -= kDefaultBlockSize - first_element_;
    ++block_num;
    size_t rest = num / kDefaultBlockSize;
    block_num += rest;
    element_num = num - rest * kDefaultBlockSize;
  }
  return arr_[block_num][element_num];
}

template <typename T, typename Alloc>
T& Deque<T, Alloc>::at(size_t num) {
  if (num >= arr_size_) {
    throw std::out_of_range("out of deque");
  }
  size_t block_num = first_block_;
  size_t element_num;
  if (num < kDefaultBlockSize - first_element_) {
    element_num = num + first_element_;
  } else {
    num -= kDefaultBlockSize - first_element_;
    ++block_num;
    size_t rest = num / kDefaultBlockSize;
    block_num += rest;
    element_num = num - rest * kDefaultBlockSize;
  }
  return arr_[block_num][element_num];
}

template <typename T, typename Alloc>
const T& Deque<T, Alloc>::at(size_t num) const {
  if (num >= arr_size_) {
    throw std::out_of_range("out of deque");
  }
  size_t block_num = first_block_;
  size_t element_num;
  if (num < kDefaultBlockSize - first_element_) {
    element_num = num + first_element_;
  } else {
    num -= kDefaultBlockSize - first_element_;
    ++block_num;
    size_t rest = num / kDefaultBlockSize;
    block_num += rest;
    element_num = num - rest * kDefaultBlockSize;
  }
  return arr_[block_num][element_num];
}

template <typename T, typename Alloc>
void Deque<T, Alloc>::expand_arr() {
  std::vector<T*> new_arr(3 * map_size_);
  std::copy(arr_.begin(), arr_.end(), new_arr.begin() + map_size_ / 2);
  first_block_ += map_size_ / 2;
  last_block_ += map_size_ / 2 + 1;
  map_size_ *= 3;
  arr_.clear();
  arr_ = new_arr;
  arr_[last_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
}

template <typename T, typename Alloc>
void Deque<T, Alloc>::expand_arr1() {
  std::vector<T*> new_arr(2 * map_size_);
  std::copy(arr_.begin(), arr_.end(), new_arr.begin() + map_size_ / 2);
  first_block_ += map_size_ / 2 - 1;
  last_block_ += map_size_ / 2;
  map_size_ *= 2;

  arr_.clear();
  arr_ = new_arr;
  arr_[first_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
}

template <typename T, typename Alloc>
void Deque<T, Alloc>::push_back(const T& value) {
  try {
    alloc_traits::construct(alloc_, arr_[last_block_] + last_element_, value);
  } catch (...) {
    alloc_traits::destroy(alloc_, arr_[last_block_] + last_element_);
    throw;
  }
  ++last_element_;
  ++arr_size_;
  if (last_element_ == kDefaultBlockSize) {
    if (last_block_ != map_size_ - 1) {
      last_element_ = 0;
      ++last_block_;
      arr_[last_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    } else {
      expand_arr();
      last_element_ = 0;
    }
  }
}

template <typename T, typename Alloc>
void Deque<T, Alloc>::push_back(T&& value) {
  try {
    alloc_traits::construct(alloc_, arr_[last_block_] + last_element_,
                            std::move(value));
  } catch (...) {
    alloc_traits::destroy(alloc_, arr_[last_block_] + last_element_);
    throw;
  }
  ++last_element_;
  ++arr_size_;
  if (last_element_ == kDefaultBlockSize) {
    if (last_block_ != map_size_ - 1) {
      last_element_ = 0;
      ++last_block_;
      arr_[last_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    } else {
      expand_arr();
      last_element_ = 0;
    }
  }
}

template <typename T, typename Alloc>
void Deque<T, Alloc>::pop_back() {
  --arr_size_;
  if (last_element_ == 0) {
    alloc_traits::destroy(alloc_,
                          arr_[last_block_ - 1] + kDefaultBlockSize - 1);
    last_element_ = kDefaultBlockSize - 1;
    alloc_traits::deallocate(alloc_, arr_[last_block_], kDefaultBlockSize);
    --last_block_;
  } else {
    alloc_traits::destroy(alloc_, arr_[last_block_] + last_element_ - 1);
    --last_element_;
  }
}

template <typename T, typename Alloc>
void Deque<T, Alloc>::push_front(const T& value) {
  size_t copy_first_block = first_block_;
  size_t copy_first_elem = first_element_;

  if (first_element_ == 0) {
    if (first_block_ == 0) {
      expand_arr1();
      first_element_ = kDefaultBlockSize;
    } else {
      first_element_ = kDefaultBlockSize;
      --first_block_;
      arr_[first_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    }
  }
  try {
    alloc_traits::construct(alloc_, arr_[first_block_] + first_element_ - 1,
                            value);
  } catch (...) {
    alloc_traits::destroy(alloc_, arr_[first_block_] + first_element_ - 1);
    first_block_ = copy_first_block;
    first_element_ = copy_first_elem;
    throw;
  }
  ++arr_size_;
  --first_element_;
}

template <typename T, typename Alloc>
void Deque<T, Alloc>::push_front(T&& value) {
  size_t copy_first_block = first_block_;
  size_t copy_first_elem = first_element_;

  if (first_element_ == 0) {
    if (first_block_ == 0) {
      expand_arr1();
      first_element_ = kDefaultBlockSize;
    } else {
      first_element_ = kDefaultBlockSize;
      --first_block_;
      arr_[first_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    }
  }
  try {
    alloc_traits::construct(alloc_, arr_[first_block_] + first_element_ - 1,
                            std::move(value));
  } catch (...) {
    alloc_traits::destroy(alloc_, arr_[first_block_] + first_element_ - 1);
    first_block_ = copy_first_block;
    first_element_ = copy_first_elem;
    throw;
  }
  ++arr_size_;
  --first_element_;
}

template <typename T, typename Alloc>
void Deque<T, Alloc>::pop_front() {
  --arr_size_;
  ++first_element_;

  if (first_element_ == kDefaultBlockSize) {
    alloc_traits::destroy(alloc_, arr_[first_block_] + kDefaultBlockSize - 1);
    alloc_traits::deallocate(alloc_, arr_[first_block_], kDefaultBlockSize);
    ++first_block_;
    first_element_ = 0;
  } else {
    alloc_traits::destroy(alloc_, arr_[first_block_] + first_element_ - 1);
  }
}

template <typename T, typename Alloc>
template <typename... Args>
void Deque<T, Alloc>::emplace_back(Args&&... args) {
  try {
    alloc_traits::construct(alloc_, arr_[last_block_] + last_element_,
                            std::forward<Args>(args)...);
  } catch (...) {
    alloc_traits::destroy(alloc_, arr_[last_block_] + last_element_);
    throw;
  }
  ++last_element_;
  ++arr_size_;
  if (last_element_ == kDefaultBlockSize) {
    if (last_block_ != map_size_ - 1) {
      last_element_ = 0;
      ++last_block_;
      arr_[last_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    } else {
      expand_arr();
      last_element_ = 0;
    }
  }
}

template <typename T, typename Alloc>
template <typename... Args>
void Deque<T, Alloc>::emplace_front(Args&&... args) {
  size_t copy_first_block = first_block_;
  size_t copy_first_elem = first_element_;

  if (first_element_ == 0) {
    if (first_block_ == 0) {
      expand_arr1();
      first_element_ = kDefaultBlockSize;
    } else {
      first_element_ = kDefaultBlockSize;
      --first_block_;
      arr_[first_block_] = alloc_traits::allocate(alloc_, kDefaultBlockSize);
    }
  }
  try {
    alloc_traits::construct(alloc_, arr_[first_block_] + first_element_ - 1,
                            std::forward<Args>(args)...);
  } catch (...) {
    alloc_traits::destroy(alloc_, arr_[first_block_] + first_element_ - 1);
    first_block_ = copy_first_block;
    first_element_ = copy_first_elem;
    throw;
  }
  ++arr_size_;
  --first_element_;
}

template <typename T, typename Alloc>
size_t Deque<T, Alloc>::size() const {
  return arr_size_;
}

template <typename T, typename Alloc>
bool Deque<T, Alloc>::empty() {
  return arr_size_ == 0;
}