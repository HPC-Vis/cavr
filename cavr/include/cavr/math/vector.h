#pragma once
#include <cavr/math/vector/data.h>

namespace cavr {

namespace math {

template<typename T, int N>
struct vec
  : public vector::data<T, N> {
  typedef T type;
};

} // namespace math

} // namespace cavr
