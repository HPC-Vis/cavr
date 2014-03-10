#pragma once
#include <cavr/math/vector/data.h>
#include <cavr/math/vector/operations.h>
#include <cavr/math/vector/utils.h>

namespace cavr {

namespace math {

namespace vector {

template<typename T, int N>
struct vec
  : public vector::data<T, N>,
    public vector::operations<vec<T, N>, T, N> {
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

  friend inline std::ostream& operator<<(std::ostream& os,
                                         const vec& v) {
    os << "vec" << N << "(";
    for (int i = 0; i < N - 1; ++i) {
      os << v[i] << ",";
    }
    os << v[N - 1] << ")";
    return os;
  }
};

} // namespace vector

} // namespace math

} // namespace cavr
