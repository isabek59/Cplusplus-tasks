#include "string.hpp"

String::String() {
  size_ = 0;
  capacity_ = 0;
  str_ = nullptr;
}

String::String(size_t size) : size_(size), capacity_(size) {
  str_ = new char[size + 1];
  str_[size] = '\0';
}

String::String(size_t size, char character) : size_(size), capacity_(size_) {
  str_ = new char[size + 1];
  for (size_t i = 0; i < size; ++i) {
    str_[i] = character;
  }
  str_[size] = '\0';
}

String::String(const char* s) {
  size_t lenght = strlen(s);
  str_ = new char[lenght + 1];
  size_ = lenght;
  capacity_ = lenght;
  for (size_t i = 0; i < lenght; ++i) {
    str_[i] = s[i];
  }
  str_[size_] = '\0';
}

String::String(const String& other) {
  size_ = other.Size();
  capacity_ = size_;
  str_ = new char[capacity_ + 1];

  for (size_t i = 0; i < size_; i++) {
    str_[i] = other.str_[i];
  }
  str_[size_] = '\0';
}

String& String::operator=(const String& s) {
  String copy(s);
  Swap(copy);
  return *this;
}

void String::PushBack(const char& character) {
  if (size_ == capacity_) {
    Reserve(2 * capacity_);
  }
  str_[size_] = character;
  ++size_;
}

void String::PopBack() {
  if (size_ > 0) {
    Resize(size_ - 1);
  }
}

void String::Resize(size_t new_size) {
  if (new_size > capacity_) {
    char* temp_str = new char[new_size];
    capacity_ = new_size;
    for (size_t i = 0; i < size_; ++i) {
      temp_str[i] = str_[i];
    }
    delete[] str_;
    size_ = new_size;
    str_ = temp_str;
  } else {
    size_ = new_size;
  }
}

void String::Resize(size_t new_size, char character) {
  if (new_size < size_ || (new_size > size_ && new_size < capacity_)) {
    size_ = new_size;
  }
  if (new_size > capacity_ && str_ != nullptr) {
    Reserve(new_size);
    for (size_t i = size_; i < new_size; ++i) {
      str_[i] = character;
    }
    size_ = new_size;
  }
}

void String::Reserve(size_t new_cap) {
  if (str_ == nullptr || size_ == 0) {
    new_cap = kDefaultSize;
  }
  if (new_cap > capacity_) {
    char* new_str = new char[new_cap];
    for (size_t i = 0; i < size_; ++i) {
      new_str[i] = str_[i];
    }
    delete[] str_;
    str_ = new_str;
    capacity_ = new_cap;
  }
}

void String::ShrinkToFit() {
  if (capacity_ > size_) {
    capacity_ = size_;
  }
}

String& String::Swap(String& other) {
  char* temp = str_;
  this->str_ = other.str_;
  other.str_ = temp;
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  return *this;
}

char& String::operator[](size_t i) { return str_[i]; }

const char& String::operator[](size_t i) const { return str_[i]; }

char& String::Front() { return str_[0]; }

const char& String::Front() const { return str_[0]; }

char& String::Back() { return str_[size_ - 1]; }

const char& String::Back() const { return str_[size_ - 1]; }

bool String::Empty() const { return size_ == 0; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_; }

char* String::Data() { return str_; }

const char* String::Data() const { return str_; }

bool operator<(const String& first, const String& second) {
  bool f = true;
  int res = strcmp(first.str_, second.str_);
  if (res >= 0) {
    f = false;
  }
  return f;
}

bool operator>=(const String& first, const String& second) {
  return !(first < second);
}

bool operator>(const String& first, const String& second) {
  bool f = true;
  int res = strcmp(first.str_, second.str_);
  if (res <= 0) {
    f = false;
  }
  return f;
}

bool operator<=(const String& first, const String& second) {
  return !(first > second);
}

bool operator==(const String& first, const String& second) {
  bool f = true;
  for (size_t i = 0; i < second.Size(); ++i) {
    if (first[i] != second[i]) {
      f = false;
      break;
    }
  }
  return f;
}

bool operator!=(const String& first, const String& second) {
  return first.str_ != second.str_;
}

String& String::operator+=(const String& other) {
  size_t t = Size();
  Resize(size_ + other.size_);
  memcpy(Data() + t, other.Data(), other.Size());
  capacity_ = size_;
  return *this;
}

String operator+(const String& first, const String& second) {
  String temp = first;
  temp += second;
  return temp;
}

String& String::operator*=(size_t n) {
  size_t temp = Size();
  Resize(Size() * n);
  for (size_t i = 1; i < n; ++i) {
    memcpy(Data() + i * temp, Data(), temp);
  }
  return *this;
}

String operator*(const String& first, size_t n) {
  String temp = first;
  temp *= n;
  return temp;
}

std::istream& operator>>(std::istream& is, String& other) {
  char input[kMax];
  is >> input;
  String temp(input);
  other.Swap(temp);
  return is;
}

std::ostream& operator<<(std::ostream& os, const String& other) {
  for (size_t i = 0; i < other.Size(); ++i) {
    os << other[i];
  }
  return os;
}

std::vector<String> String::Split(const String& delim) const {
  std::vector<String> res;
  char* begin = str_;
  char* end = nullptr;
  while (end != str_ + size_) {
    end = std::strstr(begin, delim.str_);
    if (end == NULL) {
      end = str_ + size_;
    }
    String temp(end - begin);
    memcpy(temp.str_, begin, end - begin);
    res.emplace_back(temp);
    begin = end + delim.size_;
  }
  return res;
}

String String::Join(const std::vector<String>& lines) const {
  String res("");
  for (size_t i = 0; i < lines.size(); ++i) {
    res += lines[i];
    if (i < lines.size() - 1) {
      res += *this;
    }
  }
  return res;
}

String::~String() { delete[] str_; }

void String::Clear() { size_ = 0; }
