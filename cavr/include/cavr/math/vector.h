#pragma once
#include <cavr/math/vector/data.h>
#include <cavr/math/vector/operations.h>
#include <cavr/math/vector/utils.h>

namespace cavr {

namespace math {

template<typename T, int N>
struct vec
  : public vector::data<T, N>,
    public vector::operations<vec<T, N>, N> {
  typedef T type;

  inline vec() {
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  inline vec(const U& u) {
    for (int i = 0; i < N; ++i) {
      this->v[i] = u;
    }
  }

  template<typename... U,
           typename =
             typename std::enable_if<vector::dims<U...>::value == N>::type>
  inline vec(U&&... u) {
    vector::assign(this->v, std::forward<U>(u)...);
  }

  inline const T& operator[](int i) const {
    return this->v[i];
  }

  inline T& operator[](int i) {
    return this->v[i];
  }
};

} // namespace math

} // namespace cavr
