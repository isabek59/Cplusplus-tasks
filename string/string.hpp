#pragma once
#include <cstring>
#include <iostream>
#include <vector>

const int kMax = 2000;
const size_t kDefaultSize = 19;

class String {
 public:
  String();

  explicit String(size_t, char);

  String(const char*);

  String(size_t);

  String(const String&);

  String& operator=(const String& s);

  void PushBack(const char&);

  void PopBack();

  void Resize(size_t);

  void Resize(size_t, char);

  void Reserve(size_t);

  void ShrinkToFit();

  String& Swap(String&);

  char& operator[](size_t);

  const char& operator[](size_t i) const;

  char& Front();

  const char& Front() const;

  char& Back();

  const char& Back() const;

  bool Empty() const;

  size_t Size() const;

  size_t Capacity() const;

  char* Data();

  const char* Data() const;

  friend bool operator<(const String&, const String&);

  friend bool operator>=(const String&, const String&);

  friend bool operator>(const String&, const String&);

  friend bool operator<=(const String&, const String&);

  friend bool operator==(const String&, const String&);

  friend bool operator!=(const String&, const String&);

  String& operator+=(const String&);

  friend String operator+(const String&, const String&);

  String& operator*=(size_t);

  friend String operator*(const String&, size_t);

  friend std::istream& operator>>(std::istream&, String&);

  friend std::ostream& operator<<(std::ostream&, const String&);

  std::vector<String> Split(const String& delim = " ") const;

  String Join(const std::vector<String>&) const;

  ~String();

  void Clear();

 private:
  char* str_;
  size_t size_;
  size_t capacity_;
};
