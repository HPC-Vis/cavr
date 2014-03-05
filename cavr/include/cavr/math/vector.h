#pragma once
#include <cavr/math/vector/data.h>
#include <cavr/math/vector/utils.h>

namespace cavr {

namespace math {

template<typename T, int N>
struct vec
  : public vector::data<T, N> {
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

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == N>::type>
  inline vec& operator=(const U& rhs) {
    vec& self = *this;
    // check for self assigment through potential swizzling
    if (reinterpret_cast<const vec*>(&rhs) == this) {
      vec temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] = temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] = rhs[i];
      }
    }
    return *this;
  }

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == N>::type>
  struct vector_op {
    typedef typename U::type U_type;
    typedef vec<typename std::common_type<U_type, T>::type, N> return_type;
  };

  template<typename U>
  inline typename vector_op<U>::return_type operator+(const U& rhs) {
    typename vector_op<U>::return_type result(*this);
    result += rhs;
    return result;
  }

  template<typename U>
  inline vec& operator+=(const U& rhs) {
    vec& self = *this;
    // check for self assignment through potential swizzling
    if (reinterpret_cast<const vec*>(&rhs) == this) {
      vec temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] += temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] += rhs[i];
      }
    }
    return *this;
  }

  template<typename U>
  inline typename vector_op<U>::return_type operator-(const U& rhs) {
    typename vector_op<U>::return_type result(*this);
    result -= rhs;
    return result;
  }

  template<typename U>
  inline vec& operator-=(const U& rhs) {
    vec& self = *this;
    // check for self assignment through potential swizzling
    if (reinterpret_cast<const vec*>(&rhs) == this) {
      vec temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] -= temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] -= rhs[i];
      }
    }
    return *this;
  }

  inline vec operator-() {
    vec result(*this);
    for (int i = 0; i < N; ++i) {
      result[i] = -result[i];
    }
    return result;
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
