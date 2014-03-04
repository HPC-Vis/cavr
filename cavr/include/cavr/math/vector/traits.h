#pragma once
#include <type_traits>

namespace cavr {

namespace math {

namespace vector {

/**
  Contains the dimensionality of a single type. Specializations should
  be of the form "template<typename T> struct base_dims<T, Specialized>"
  in order to strip qualifiers from the Specialized class.
*/
template<typename T, typename = typename std::decay<T>::type>
struct base_dims {
  static const int value = 1;
};

/**
  Contains the dimensionality of a variadic list of types.
  g++ workaround.
*/
template<typename... T>
struct dims {
};

/**
  Contains the dimensionality of a variadic list of types.
*/
template<typename T, typename... U>
struct dims<T, U...> {
  static const int value = base_dims<T>::value + dims<U...>::value;
};

/**
  Contains the dimensionality of a variadic list of types.
  Zero argument expansion.
*/
template<>
struct dims<> {
  static const int value = 0;
};

/**
  True if the first value is found in the rest of the variadic list.
  g++ workaround.
*/
template<int... I>
struct is_in {
};

/**
  True if the first value is found in the rest of the variadic list.
*/
template<int N, int M, int... I>
struct is_in<N, M, I...> {
  static const bool value = (N == M) || is_in<N, I...>::value;
};

/**
  True if the first value is found in the rest of the variadic list.
  Single value expansion.
*/
template<int N>
struct is_in<N> {
  static const bool value = false;
};

/**
  True if the values in I... are all unique.
  g++ workaround.
*/
template<int... I>
struct is_unique {
};

/**
  True if the values in I... are all unique.
*/
template<int N, int... I>
struct is_unique<N, I...> {
  static const bool value = !is_in<N, I...>::value && is_unique<I...>::value;
};

/**
  True if the values in I... are all unique.
  Zero argument expansion.
*/
template<>
struct is_unique<> {
  static const bool value = true;
};

} // namespace vector

} // namespace math

} // namespace cavr
