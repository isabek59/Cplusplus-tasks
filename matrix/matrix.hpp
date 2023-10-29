#pragma once
#include <algorithm>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  Matrix() { matr_.assign(N, std::vector<T>(M)); }

  Matrix(const std::vector<std::vector<T>>& other) { matr_ = other; }

  Matrix(const T& elem) {
    matr_.assign(N, std::vector<T>(M));
    for (size_t i = 0; i < matr_.size(); ++i) {
      for (size_t j = 0; j < matr_[0].size(); ++j) {
        matr_[i][j] = elem;
      }
    }
  }

  Matrix(const Matrix<N, M, T>& other) {
    matr_.assign(N, std::vector<T>(M));
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matr_[i][j] = other(i, j);
      }
    }
  }

  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& other) {
    for (size_t i = 0; i < matr_.size(); ++i) {
      for (size_t j = 0; j < matr_[0].size(); ++j) {
        matr_[i][j] += other(i, j);
      }
    }
    return *this;
  }

  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& other) {
    for (size_t i = 0; i < matr_.size(); ++i) {
      for (size_t j = 0; j < matr_[0].size(); ++j) {
        matr_[i][j] -= other(i, j);
      }
    }
    return *this;
  }

  Matrix<N, M, T> operator*(const T& elem) {
    std::vector<std::vector<T>> result(N, std::vector<T>(M));
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        result[i][j] = matr_[i][j] * elem;
      }
    }
    Matrix<N, M, T> res(result);
    return res;
  }

  friend Matrix<N, M, T> operator+(const Matrix<N, M, T>& first,
                                   const Matrix<N, M, T>& second) {
    Matrix<N, M, T> temp = first;
    temp += second;
    return temp;
  }

  friend Matrix<N, M, T> operator-(const Matrix<N, M, T>& first,
                                   const Matrix<N, M, T>& second) {
    Matrix<N, M, T> temp = first;
    temp -= second;
    return temp;
  }

  Matrix<M, N, T> Transposed() {
    std::vector<std::vector<T>> result(M, std::vector<T>(N));
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        result[i][j] = matr_[j][i];
      }
    }
    Matrix<M, N, T> res(result);
    return res;
  }

  T& operator()(size_t i, size_t j) { return matr_[i][j]; }

  const T& operator()(size_t i, size_t j) const { return matr_[i][j]; }

  T Trace() {
    T res = 0;
    for (size_t i = 0; i < N; ++i) {
      res += matr_[i][i];
    }
    return res;
  }

  ~Matrix() = default;

 private:
  std::vector<std::vector<T>> matr_;
};

template <size_t N, size_t K, size_t M, typename T>
Matrix<N, M, T> operator*(const Matrix<N, K, T>& first,
                          const Matrix<K, M, T>& second) {
  std::vector<std::vector<T>> result(N, std::vector<T>(M));
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      for (size_t k = 0; k < K; ++k) {
        result[i][j] += first(i, k) * second(k, j);
      }
    }
  }
  Matrix<N, M, T> res(result);
  return res;
}