#pragma once
#include <utility>

#include <cavr/math/vector/traits.h>

namespace cavr {

namespace math {

namespace vector {

template<typename T, int N>
struct vec;

template<typename T, int N, int... I>
struct swizzle;

/**
  Assigns an array with a variadic list of values.
  Zero value expansion.
*/
template<typename D>
void assign(D* d) {
}

/**
  Assigns an array with a variadic list of values.
  Single generic value assignment.
*/
template<typename D, typename T>
void assign(D* d, const T& v) {
  *d = v;
}

/**
  Assigns an array with a variadic list of values.
  Variadic expansion.
*/
template<typename D, typename T, typename... U>
void assign(D* d, const T& v, U&&... u) {
  assign(d, v);
  assign(d + dims<T>::value, std::forward<U>(u)...);
}

/**
  Assigns an array with a variadic list of values.
  vec<T,N> specialization.
*/
template<typename D, typename T, int N>
void assign(D* d, const vec<T, N>& v) {
  for (int i = 0; i < N; ++i) {
    d[i] = v.v[i];
  }
}

/**
  Assigns an array with a variadic list of values.
  swizzle<T, N, I...> specialization.
*/
template<typename D, typename T, int N, int... I>
void assign(D* d, const swizzle<T, N, I...>& v) {
  for (int i = 0; i < sizeof...(I); ++i) {
    d[i] = v[i];
  }
}

} // namespace vector

} // namespace math

} // namespace cavr
