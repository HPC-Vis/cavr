#pragma once
#include <algorithm>

#include <cavr/math/vector/traits.h>
#include <cavr/math/vector/vector.h>

namespace cavr {

namespace math {

namespace matrix {

template<typename T, int C, int R>
struct mat {
  union {
    T v[R* C];
    struct {
      vector::vec<T, R> columns[C];
    };
  };

  const vector::vec<T, R>& operator[](int i) const {
    return columns[i];
  }

  vector::vec<T, R>& operator[](int i) {
    return columns[i];
  }

  inline mat() {
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  inline mat(const U& u) {
    zero_();
    mat& self = *this;
    for (int i = 0; i < std::min(C, R); ++i) {
      self[i][i] = u;
    }
  }

  template<typename... U,
           typename =
             typename std::enable_if<vector::dims<U...>::value == R * C>::type>
  inline mat(U&&... u) {
    vector::assign(v, std::forward<U>(u)...);
  }

  template<typename U, int CU, int RU>
  inline mat(const mat<U, CU, RU>& m) {
    zero_();
    mat& self = *this;
    for (int i = 0; i < std::min(C, R); ++i) {
      self[i][i] = 1;
    }
    for (int c = 0; c < std::min(C, CU); ++c) {
      for (int r = 0; r < std::min(R, RU); ++r) {
        self[c][r] = m[c][r];
      }
    }
  }

  vector::vec<T, C> row(int i) const {
    vector::vec<T, C> result;
    for (int c = 0; c < C; ++c) {
      result[c] = columns[c][i];
    }
    return result;
  }

  mat<T, R, C> transpose() const {
    mat<T, R, C> result;
    for (int r = 0; r < R; ++r) {
      result[r] = row(r);
    }
    return result;
  }

  template<typename U>
  inline mat& operator=(const mat<U, C, R>& rhs) {
    for (int i = 0; i < C * R; ++i) {
      v[i] = rhs.v[i];
    }
  }

  template<typename U, int CU>
  inline mat<typename std::common_type<T, U>::type, CU, R>
  operator*(const mat<U, CU, C>& u) const {
    mat<typename std::common_type<T, U>::type, CU, R> result;
    for (int r = 0; r < R; ++r) {
      const auto row_vector = row(r);
      for (int c = 0; c < CU; ++c) {
        const auto& column_vector = u[c];
        result[c][r] = row_vector.dot(column_vector);
      }
    }
    return result;
  }

  template<typename U>
  inline mat<typename std::common_type<T, U>::type, C, R>
  operator*(const U& u) const {
    mat<typename std::common_type<T, U>::type, C, R> result(*this);
    for (int i = 0; i < C * R; ++i) {
      result.v[i] *= u;
    }
    return result;
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  inline mat& operator*=(const U& u) {
    for (int i = 0; i < C * R; ++i) {
      v[i] *= u;
    }
    return *this;
  }

  template<typename U>
  struct is_square {
    static const bool value = (C == R);
  };

  template<typename U,
           typename =
             typename std::enable_if<is_square<U>::value>::type>
  inline mat& operator*=(const mat<U, C, R>& m) {
    *this = *this * m;
    return *this;
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  friend inline mat<typename std::common_type<T, U>::type, C, R>
  operator*(const U& u, const mat& r) {
    return r * u;
  }

  template<typename U>
  inline mat<typename std::common_type<T, U>::type, C, R>
  operator/(const U& u) const {
    mat<typename std::common_type<T, U>::type, C, R> result(*this);
    for (int i = 0; i < C * R; ++i) {
      result.v[i] /= u;
    }
    return result;
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  inline mat& operator/=(const U& u) {
    for (int i = 0; i < C * R; ++i) {
      v[i] /= u;
    }
    return *this;
  }

private:
  void zero_() {
    for (int i = 0; i < R * C; ++i) {
      v[i] = 0;
    }
  }
};

} // namespace matrix

} // namespace math

} // namespace cavr
