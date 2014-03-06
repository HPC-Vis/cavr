#pragma once
#include <cavr/math/matrix/matrix.h>

namespace cavr {

namespace math {

extern template struct matrix::mat<int, 2, 2>;
extern template struct matrix::mat<int, 2, 3>;
extern template struct matrix::mat<int, 2, 4>;
extern template struct matrix::mat<int, 3, 2>;
extern template struct matrix::mat<int, 3, 3>;
extern template struct matrix::mat<int, 3, 4>;
extern template struct matrix::mat<int, 4, 2>;
extern template struct matrix::mat<int, 4, 3>;
extern template struct matrix::mat<int, 4, 4>;

extern template struct matrix::mat<float, 2, 2>;
extern template struct matrix::mat<float, 2, 3>;
extern template struct matrix::mat<float, 2, 4>;
extern template struct matrix::mat<float, 3, 2>;
extern template struct matrix::mat<float, 3, 3>;
extern template struct matrix::mat<float, 3, 4>;
extern template struct matrix::mat<float, 4, 2>;
extern template struct matrix::mat<float, 4, 3>;
extern template struct matrix::mat<float, 4, 4>;

extern template struct matrix::mat<double, 2, 2>;
extern template struct matrix::mat<double, 2, 3>;
extern template struct matrix::mat<double, 2, 4>;
extern template struct matrix::mat<double, 3, 2>;
extern template struct matrix::mat<double, 3, 3>;
extern template struct matrix::mat<double, 3, 4>;
extern template struct matrix::mat<double, 4, 2>;
extern template struct matrix::mat<double, 4, 3>;
extern template struct matrix::mat<double, 4, 4>;

typedef matrix::mat<int, 2, 2> mat2x2i;
typedef matrix::mat<int, 2, 3> mat2x3i;
typedef matrix::mat<int, 2, 4> mat2x4i;
typedef matrix::mat<int, 3, 2> mat3x2i;
typedef matrix::mat<int, 3, 3> mat3x3i;
typedef matrix::mat<int, 3, 4> mat3x4i;
typedef matrix::mat<int, 4, 2> mat4x2i;
typedef matrix::mat<int, 4, 3> mat4x3i;
typedef matrix::mat<int, 4, 4> mat4x4i;

typedef matrix::mat<int, 2, 2> mat2i;
typedef matrix::mat<int, 3, 3> mat3i;
typedef matrix::mat<int, 4, 4> mat4i;

typedef matrix::mat<float, 2, 2> mat2x2f;
typedef matrix::mat<float, 2, 3> mat2x3f;
typedef matrix::mat<float, 2, 4> mat2x4f;
typedef matrix::mat<float, 3, 2> mat3x2f;
typedef matrix::mat<float, 3, 3> mat3x3f;
typedef matrix::mat<float, 3, 4> mat3x4f;
typedef matrix::mat<float, 4, 2> mat4x2f;
typedef matrix::mat<float, 4, 3> mat4x3f;
typedef matrix::mat<float, 4, 4> mat4x4f;

typedef matrix::mat<float, 2, 2> mat2f;
typedef matrix::mat<float, 3, 3> mat3f;
typedef matrix::mat<float, 4, 4> mat4f;

typedef matrix::mat<double, 2, 2> mat2x2d;
typedef matrix::mat<double, 2, 3> mat2x3d;
typedef matrix::mat<double, 2, 4> mat2x4d;
typedef matrix::mat<double, 3, 2> mat3x2d;
typedef matrix::mat<double, 3, 3> mat3x3d;
typedef matrix::mat<double, 3, 4> mat3x4d;
typedef matrix::mat<double, 4, 2> mat4x2d;
typedef matrix::mat<double, 4, 3> mat4x3d;
typedef matrix::mat<double, 4, 4> mat4x4d;

typedef matrix::mat<double, 2, 2> mat2d;
typedef matrix::mat<double, 3, 3> mat3d;
typedef matrix::mat<double, 4, 4> mat4d;

} // namespace math

} // namespace cavr
