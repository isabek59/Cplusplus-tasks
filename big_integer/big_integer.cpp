#include "big_integer.hpp"

BigInt::BigInt() = default;

BigInt::BigInt(int64_t num) : BigInt(std::to_string(num)) {}

BigInt::BigInt(const std::string& str) {
  if (str[0] == '-' && str[1] == '0') {
    number_.push_back(str[1] - '0');
    return;
  }
  if (str[0] == '-') {
    this->sign_ = -1;
    for (int i = str.size() - 1; i >= 1; --i) {
      number_.push_back(str[i] - '0');
    }
  } else {
    for (int i = str.size() - 1; i >= 0; --i) {
      number_.push_back(str[i] - '0');
    }
  }
}

BigInt::BigInt(const std::vector<int>& arr, int sn) : number_(arr), sign_(sn) {}

BigInt::BigInt(const BigInt& other) {
  number_ = other.number_;
  sign_ = other.sign_;
}

BigInt& BigInt::operator=(const BigInt&) = default;

BigInt::~BigInt() = default;

BigInt BigInt::Add(const BigInt& first, const BigInt& second, bool is_neg) {
  std::vector<int> result;
  int carry = 0;
  int min_of_two = std::min(first.number_.size(), second.number_.size());
  int max_of_two = std::max(first.number_.size(), second.number_.size());
  for (int i = 0; i < min_of_two; ++i) {
    int cur_sum = carry + first.number_[i] + second.number_[i];
    result.push_back(cur_sum % kBase);
    carry = cur_sum / kBase;
  }
  for (int i = min_of_two; i < max_of_two; ++i) {
    if (first.number_.size() > second.number_.size()) {
      int cur_sum = carry + first.number_[i];
      result.push_back(cur_sum % kBase);
      carry = cur_sum / kBase;
    } else {
      int cur_sum = carry + second.number_[i];
      result.push_back(cur_sum % kBase);
      carry = cur_sum / kBase;
    }
  }
  if (carry != 0) {
    result.push_back(carry);
  }
  BigInt sum_of_bigint(result);
  if (is_neg) {
    sum_of_bigint.sign_ = -1;
  }
  return sum_of_bigint;
}

BigInt operator+(const BigInt& a, const BigInt& b) {
  BigInt res;
  if (a.sign_ == 1 && b.sign_ == 1) {
    return res.Add(a, b, false);
  }
  if (a.sign_ == -1 && b.sign_ == 1) {
    if (IsMore(a, b)) {
      return BigInt::Substract(a, b, true);
    }
    return BigInt::Substract(b, a, false);
  }
  if (a.sign_ == 1 && b.sign_ == -1) {
    if (IsLess(a, b)) {
      return BigInt::Substract(b, a, true);
    }
    return BigInt::Substract(a, b, false);
  }
  if (a.sign_ == -1 && b.sign_ == -1) {
    return res.Add(a, b, true);
  }
  return res;
}

BigInt& BigInt::operator+=(const BigInt& other) {
  *this = *this + other;
  return *this;
}

BigInt BigInt::Substract(const BigInt& first, const BigInt& second,
                         bool is_neg) {
  std::vector<int> result;
  int carry = 0;
  for (size_t i = 0; i < second.number_.size(); ++i) {
    int cur_diff = first.number_[i] - second.number_[i] - carry;
    if (cur_diff < 0) {
      cur_diff += kBase;
      carry = 1;
    } else {
      carry = 0;
    }
    result.push_back(cur_diff);
  }
  for (size_t i = second.number_.size(); i < first.number_.size(); ++i) {
    if (first.number_[i] - carry < 0) {
      result.push_back(first.number_[i] + kBase - carry);
      carry = 1;
    } else {
      result.push_back(first.number_[i] - carry);
      carry = 0;
    }
  }
  while (result.size() > 1 && result.back() == 0) {
    result.pop_back();
  }
  BigInt diff_of_bigint(result);
  if (is_neg) {
    diff_of_bigint.sign_ = -1;
  }
  return diff_of_bigint;
}

BigInt BigInt::operator-(const BigInt& a) const {
  BigInt res;
  if (sign_ == 1 && a.sign_ == 1) {
    if (*this < a) {
      return res.Substract(a, *this, true);
    }
    return res.Substract(*this, a, false);
  }
  if (sign_ == -1 && a.sign_ == 1) {
    return res.Add(*this, a, true);
  }
  if (sign_ == 1 && a.sign_ == -1) {
    return res.Add(*this, a, false);
  }
  if (sign_ == -1 && a.sign_ == -1) {
    if (IsMore(*this, a)) {
      return res.Substract(*this, a, true);
    }
    return res.Substract(a, *this, false);
  }
  return res;
}

BigInt& BigInt::operator-=(const BigInt& other) {
  *this = *this - other;
  return *this;
}

BigInt BigInt::Mult(const BigInt& first, const BigInt& second) {
  std::vector<int> res(first.number_.size() * second.number_.size() + 1, 0);
  for (size_t i = 0; i < first.number_.size(); ++i) {
    int carry = 0;
    for (size_t j = 0; j < second.number_.size(); ++j) {
      int cur = res[i + j] + first.number_[i] * second.number_[j] + carry;
      carry = cur / kBase;
      res[i + j] = cur % kBase;
    }
    int cur = second.number_.size();
    while (carry != 0) {
      res[i + cur] = carry % kBase;
      ++cur;
      carry /= kBase;
    }
  }
  while (res.size() > 1 && res.back() == 0) {
    res.pop_back();
  }
  BigInt mult_of_bigint(res);
  if ((first.number_[0] == 0 && first.number_.size() == 1) ||
      (first.number_[0] == 0 && first.number_.size() == 1)) {
    return mult_of_bigint;
  }
  if ((first.sign_ == -1 && second.sign_ == 1) ||
      (first.sign_ == 1 && second.sign_ == -1)) {
    mult_of_bigint.sign_ = -1;
  }
  return mult_of_bigint;
}

BigInt operator*(const BigInt& a, const BigInt& b) {
  BigInt c;
  return c.Mult(a, b);
}

BigInt BigInt::Divide(const BigInt& first, const BigInt& second) {
  int min_of_two = std::min(first.number_.size(), second.number_.size());
  int max_of_two = std::max(first.number_.size(), second.number_.size());
  std::vector<int> res(max_of_two - min_of_two + 1, 0);
  BigInt div_of_bigint(res);
  for (int i = res.size() - 1; i >= 0; --i) {
    while (IsLess(second * div_of_bigint, first)) {
      div_of_bigint.number_[i]++;
    }
    div_of_bigint.number_[i]--;
  }
  while (div_of_bigint.number_.size() > 1 &&
         div_of_bigint.number_.back() == 0) {
    div_of_bigint.number_.pop_back();
  }
  if (div_of_bigint.number_[0] == 0 && div_of_bigint.number_.size() == 1) {
    return div_of_bigint;
  }
  if ((first.sign_ == -1 && second.sign_ == 1) ||
      (first.sign_ == 1 && second.sign_ == -1)) {
    div_of_bigint.sign_ = -1;
  }
  return div_of_bigint;
}

BigInt BigInt::operator/(const BigInt& b) const {
  BigInt res;
  return res.Divide(*this, b);
}

BigInt BigInt::Mod(const BigInt& first, const BigInt& second) {
  BigInt div = (first / second);
  BigInt mod = first - div * second;
  if (first.sign_ == 1 && second.sign_ == 1) {
    mod.sign_ = 1;
  }
  return mod;
}

BigInt BigInt::operator%(const BigInt& a) const {
  BigInt res;
  return res.Mod(*this, a);
}

bool BigInt::operator==(const BigInt& other) const {
  bool f = true;
  if (this->sign_ != other.sign_) {
    f = false;
  }
  if (this->number_ != other.number_) {
    f = false;
  }
  return f;
}

bool BigInt::operator!=(const BigInt& other) const {
  bool f = false;
  if (this->sign_ != other.sign_) {
    f = true;
  }
  if (this->number_ != other.number_) {
    f = true;
  }
  return f;
}

BigInt& BigInt::operator++() {
  BigInt temp("1");
  *this += temp;
  return *this;
}

BigInt BigInt::operator++(int) {
  BigInt temp(*this);
  ++(*this);
  return temp;
}

BigInt& BigInt::operator--() {
  BigInt temp("1");
  *this -= temp;
  return *this;
}

BigInt BigInt::operator--(int) {
  BigInt temp(*this);
  --(*this);
  return temp;
}

BigInt BigInt::operator-() const {
  if (number_.size() == 1 && number_[0] == 0) {
    return BigInt(number_);
  }
  return BigInt(number_, -1);
}

bool IsLess(const BigInt& first, const BigInt& second) {
  if (first.number_.size() < second.number_.size()) {
    return true;
  }
  if (first.number_.size() > second.number_.size()) {
    return false;
  }
  for (int i = first.number_.size() - 1; i >= 0; --i) {
    if (first.number_[i] > second.number_[i]) {
      return false;
    }
    if (first.number_[i] < second.number_[i]) {
      return true;
    }
  }
  return true;
}

bool IsMore(const BigInt& first, const BigInt& second) {
  if (first.number_.size() < second.number_.size()) {
    return false;
  }
  if (first.number_.size() > second.number_.size()) {
    return true;
  }
  for (int i = first.number_.size() - 1; i >= 0; --i) {
    if (first.number_[i] > second.number_[i]) {
      return true;
    }
    if (first.number_[i] < second.number_[i]) {
      return false;
    }
  }
  return false;
}

bool BigInt::operator<(const BigInt& other) const {
  if (sign_ == -1 && other.sign_ == 1) {
    return true;
  }
  if (sign_ == 1 && other.sign_ == -1) {
    return false;
  }
  if (number_.size() < other.number_.size()) {
    return sign_ == 1 && other.sign_ == 1;
  }
  if (number_.size() > other.number_.size()) {
    return !(sign_ == 1 && other.sign_ == 1);
  }
  for (int i = number_.size() - 1; i >= 0; --i) {
    if (number_[i] > other.number_[i]) {
      return !(sign_ == 1);
    }
    if (number_[i] < other.number_[i]) {
      return sign_ == 1;
    }
  }
  return false;
}

bool BigInt::operator>(const BigInt& other) const {
  if (sign_ == -1 && other.sign_ == 1) {
    return false;
  }
  if (sign_ == 1 && other.sign_ == -1) {
    return true;
  }
  if (number_.size() < other.number_.size()) {
    return !(sign_ == 1 && other.sign_ == 1);
  }
  if (number_.size() > other.number_.size()) {
    return sign_ == 1 && other.sign_ == 1;
  }
  for (int i = number_.size() - 1; i >= 0; --i) {
    if (number_[i] > other.number_[i]) {
      return sign_ == 1;
    }
    if (number_[i] < other.number_[i]) {
      return !(sign_ == 1);
    }
  }
  return false;
}

bool BigInt::operator<=(const BigInt& other) const {
  if (sign_ == -1 && other.sign_ == 1) {
    return true;
  }
  if (sign_ == 1 && other.sign_ == -1) {
    return false;
  }
  if (number_.size() < other.number_.size()) {
    return sign_ == 1 && other.sign_ == 1;
  }
  if (number_.size() > other.number_.size()) {
    return !(sign_ == 1 && other.sign_ == 1);
  }
  for (int i = number_.size() - 1; i >= 0; --i) {
    if (number_[i] > other.number_[i]) {
      return !(sign_ == 1);
    }
    if (number_[i] < other.number_[i]) {
      return sign_ == 1;
    }
  }
  return true;
}

bool BigInt::operator>=(const BigInt& other) const {
  if (sign_ == -1 && other.sign_ == 1) {
    return true;
  }
  if (sign_ == 1 && other.sign_ == -1) {
    return false;
  }
  if (number_.size() < other.number_.size()) {
    return sign_ == 1 && other.sign_ == 1;
  }
  if (number_.size() > other.number_.size()) {
    return !(sign_ == 1 && other.sign_ == 1);
  }
  for (int i = number_.size() - 1; i >= 0; --i) {
    if (number_[i] > other.number_[i]) {
      return !(sign_ == 1);
    }
    if (number_[i] < other.number_[i]) {
      return sign_ == 1;
    }
  }
  return true;
}

std::istream& operator>>(std::istream& is, BigInt& other) {
  std::string tmp;
  is >> tmp;
  other = tmp;
  return is;
}

std::ostream& operator<<(std::ostream& os, const BigInt& other) {
  if (other.sign_ == -1) {
    os << '-';
  }
  for (int i = other.number_.size() - 1; i >= 0; --i) {
    os << other.number_[i];
  }
  return os;
}