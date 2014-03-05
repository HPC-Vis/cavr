#pragma once
#include <cavr/math/vector/operations.h>
#include <cavr/math/vector/traits.h>

namespace cavr {

namespace math {

// Forward declaration
template<typename T, int N>
struct vec;

namespace vector {

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
  inline swizzle(const U& u) {
    assert_assignable();
    assert_dimensionality<U>();
    set_to_vector_(u);
  }

  template<typename U>
  inline swizzle& operator=(const U& u) {
    assert_assignable();
    assert_dimensionality<U>();
    set_to_vector_(u);
    return *this;
  }

private:
  inline void set_to_vector_(const vec<T, sizeof...(I)>& t) {
    swizzle& self = *this;
    for (int i = 0; i < sizeof...(I); ++i) {
      self[i] = t[i];
    }
  }
};

} // namespace vector

} // namespace math

} // namespace cavr
