#pragma once
#include <cavr/math/vector/operations.h>
#include <cavr/math/vector/traits.h>

namespace cavr {

namespace math {

namespace vector {

// Forward declaration
template<typename T, int N>
struct vec;

template<typename T, int N, int... I>
struct swizzle 
  : public operations<swizzle<T, N, I...>, sizeof...(I)> {
  typedef T type;
  T v[N];
  static inline int get_index(int i) {
    constexpr int indices[] = { I... };
    return indices[i];
  }

  inline T& operator[](int i) {
    return v[get_index(i)];
  }

  inline const T& operator[](int i) const {
    return v[get_index(i)];
  }

  static constexpr void assert_assignable() {
    static_assert(vector::is_unique<I...>::value,
                  "swizzle is not assignable");
  }

  template<typename U>
  static constexpr void assert_dimensionality() {
    static_assert(vector::dims<U>::value == sizeof...(I),
                  "dimensionalities do not match");
  }

  swizzle() = default;

  template<typename U>
  struct is_assignable {
    static const bool value = is_unique<I...>::value;
  };

  template<typename U,
           typename =
             typename std::enable_if<dims<U>::value == sizeof...(I)>::type,
           typename =
             typename std::enable_if<is_assignable<U>::value>::type>
  inline swizzle& operator=(const U& rhs) {
    swizzle& self = *this;
    // check for self assigment through potential swizzling
    if (reinterpret_cast<const swizzle*>(&rhs) == this) {
      vec<T, sizeof...(I)> temp(rhs);
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
};

} // namespace vector

} // namespace math

} // namespace cavr
