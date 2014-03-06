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
