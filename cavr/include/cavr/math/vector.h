#pragma once
#include <cavr/math/vector/vector.h>

namespace cavr {

namespace math {

extern template struct vector::vec<int, 2>;
extern template struct vector::vec<int, 3>;
extern template struct vector::vec<int, 4>;
extern template struct vector::vec<float, 2>;
extern template struct vector::vec<float, 3>;
extern template struct vector::vec<float, 4>;
extern template struct vector::vec<double, 2>;
extern template struct vector::vec<double, 3>;
extern template struct vector::vec<double, 4>;

typedef vector::vec<int, 2> vec2i;
typedef vector::vec<int, 3> vec3i;
typedef vector::vec<int, 4> vec4i;
typedef vector::vec<float, 2> vec2f;
typedef vector::vec<float, 3> vec3f;
typedef vector::vec<float, 4> vec4f;
typedef vector::vec<double, 2> vec2d;
typedef vector::vec<double, 3> vec3d;
typedef vector::vec<double, 4> vec4d;

} // namespace math

} // namespace cavr
