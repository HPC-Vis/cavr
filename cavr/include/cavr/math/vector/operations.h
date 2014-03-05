#pragma once
#include <type_traits>

#include <cavr/math/vector/traits.h>

namespace cavr {

namespace math {

template<typename T, int N>
struct vec;

namespace vector {

template<typename T, int N>
struct operations {
  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == N>::type>
  inline T& operator=(const U& rhs) {
    T& self = *static_cast<T*>(this);
    // check for self assigment through potential swizzling
    if (reinterpret_cast<const T*>(&rhs) == this) {
      vec<typename T::type, N> temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] = temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] = rhs[i];
      }
    }
    return *static_cast<T*>(this);
  }

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == N>::type>
  struct vector_op {
    typedef typename U::type U_type;
    typedef typename T::type T_type;
    typedef vec<typename std::common_type<U_type, T_type>::type, N> return_type;
  };

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == 1>::type>
  struct vector_scalar_op {
    typedef typename T::type T_type;
    typedef vec<typename std::common_type<U, T_type>::type, N> return_type;
  };

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == N>::type>
  struct vector_dot_op {
    typedef typename U::type U_type;
    typedef typename T::type T_type;
    typedef typename std::common_type<U_type, T_type>::type return_type;
  };

  template<typename U>
  inline typename vector_op<U>::return_type operator+(const U& rhs) const {
    typename vector_op<U>::return_type result(*static_cast<const T*>(this));
    result += rhs;
    return result;
  }

  template<typename U>
  inline T& operator+=(const U& rhs) {
    T& self = *static_cast<T*>(this);
    // check for self assignment through potential swizzling
    if (reinterpret_cast<const T*>(&rhs) == this) {
      vec<typename T::type, N> temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] += temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] += rhs[i];
      }
    }
    return *static_cast<T*>(this);
  }

  template<typename U>
  inline typename vector_op<U>::return_type operator-(const U& rhs) const {
    typename vector_op<U>::return_type result(*static_cast<const T*>(this));
    result -= rhs;
    return result;
  }

  template<typename U>
  inline T& operator-=(const U& rhs) {
    T& self = *static_cast<T*>(this);
    // check for self assignment through potential swizzling
    if (reinterpret_cast<const T*>(&rhs) == this) {
      vec<typename T::type, N> temp(rhs);
      for (int i = 0; i < N; ++i) {
        self[i] -= temp[i];
      }
    } else {
      for (int i = 0; i < N; ++i) {
        self[i] -= rhs[i];
      }
    }
    return *static_cast<T*>(this);
  }

  inline T operator-() const {
    T result(*static_cast<const T*>(this));
    for (int i = 0; i < N; ++i) {
      result[i] = -result[i];
    }
    return result;
  }

  template<typename U>
  inline typename vector_scalar_op<U>::return_type operator*(const U& u) const {
    typename vector_scalar_op<U>::return_type
      result(*static_cast<const T*>(this));
    result *= u;
    return result;
  }

  template<typename U>
  T& operator*=(const U& u) {
    T& self = *static_cast<T*>(this);
    for (int i = 0; i < N; ++i) {
      self[i] *= u;
    }
    return *static_cast<T*>(this);
  }

  template<typename U>
  friend inline typename vector_scalar_op<U>::return_type
  operator*(const U& u, const T& r) {
    return r * u;
  }

  template<typename U>
  inline typename vector_scalar_op<U>::return_type operator/(const U& u) const {
    typename vector_scalar_op<U>::return_type 
      result(*static_cast<const T*>(this));
    result /= u;
    return result;
  }

  template<typename U>
  inline T& operator/=(const U& u) {
    T& self = *static_cast<T*>(this);
    for (int i = 0; i < N; ++i) {
      self[i] /= u;
    }
    return *static_cast<T*>(this);
  }

  template<typename U>
  inline typename vector_dot_op<U>::return_type dot(const U& u) const {
    typename vector_dot_op<U>::return_type result = 0;
    const T& self = *static_cast<const T*>(this);
    for (int i = 0; i < N; ++i) {
      result += self[i] * u[i];
    }
    return result;
  }

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == 3>::type>
  inline typename vector_op<U>::return_type cross(const U& u) const {
    const T& s = *static_cast<const T*>(this);
    return typename vector_op<U>::return_type(s[1] * u[2] - u[1] * s[2],
                                              s[2] * u[0] - u[2] * s[0],
                                              s[0] * u[1] - u[0] * s[1]);
  }
};

} // namespace vector

} // namespace math

} // namespace cavr
