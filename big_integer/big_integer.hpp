#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

const int kBase = 10;

class BigInt {
 public:
  BigInt();

  BigInt(int64_t);

  BigInt(const std::string&);

  BigInt(const std::vector<int>&, int sn = 1);

  BigInt(const BigInt&);

  BigInt& operator=(const BigInt&);

  BigInt& operator+=(const BigInt&);

  friend BigInt operator+(const BigInt&, const BigInt&);

  BigInt& operator-=(const BigInt&);

  BigInt operator-(const BigInt&) const;

  BigInt& operator*=(const BigInt&);

  friend BigInt operator*(const BigInt&, const BigInt&);

  BigInt& operator/=(const BigInt&);

  BigInt operator/(const BigInt&) const;

  BigInt& operator%=(const BigInt&);

  BigInt operator%(const BigInt&) const;

  static BigInt Add(const BigInt&, const BigInt&, bool);

  static BigInt Substract(const BigInt&, const BigInt&, bool);

  static BigInt Mult(const BigInt&, const BigInt&);

  static BigInt Divide(const BigInt&, const BigInt&);

  static BigInt Mod(const BigInt&, const BigInt&);

  BigInt& operator++();

  BigInt operator++(int);

  BigInt& operator--();

  BigInt operator--(int);

  BigInt operator-() const;

  friend bool IsLess(const BigInt&, const BigInt&);

  friend bool IsMore(const BigInt&, const BigInt&);

  bool operator<(const BigInt&) const;

  bool operator>=(const BigInt&) const;

  bool operator>(const BigInt&) const;

  bool operator<=(const BigInt&) const;

  bool operator==(const BigInt&) const;

  bool operator!=(const BigInt&) const;

  ~BigInt();

  friend std::istream& operator>>(std::istream&, BigInt&);

  friend std::ostream& operator<<(std::ostream&, const BigInt&);

 private:
  std::vector<int> number_;
  int sign_ = 1;
};
