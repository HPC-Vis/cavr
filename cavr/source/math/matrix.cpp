#include <cavr/math/matrix/matrix.h>

namespace cavr {

namespace math {

namespace matrix {

template struct matrix::mat<int, 2, 2>;
template struct matrix::mat<int, 2, 3>;
template struct matrix::mat<int, 2, 4>;
template struct matrix::mat<int, 3, 2>;
template struct matrix::mat<int, 3, 3>;
template struct matrix::mat<int, 3, 4>;
template struct matrix::mat<int, 4, 2>;
template struct matrix::mat<int, 4, 3>;
template struct matrix::mat<int, 4, 4>;

template struct matrix::mat<float, 2, 2>;
template struct matrix::mat<float, 2, 3>;
template struct matrix::mat<float, 2, 4>;
template struct matrix::mat<float, 3, 2>;
template struct matrix::mat<float, 3, 3>;
template struct matrix::mat<float, 3, 4>;
template struct matrix::mat<float, 4, 2>;
template struct matrix::mat<float, 4, 3>;
template struct matrix::mat<float, 4, 4>;

template struct matrix::mat<double, 2, 2>;
template struct matrix::mat<double, 2, 3>;
template struct matrix::mat<double, 2, 4>;
template struct matrix::mat<double, 3, 2>;
template struct matrix::mat<double, 3, 3>;
template struct matrix::mat<double, 3, 4>;
template struct matrix::mat<double, 4, 2>;
template struct matrix::mat<double, 4, 3>;
template struct matrix::mat<double, 4, 4>;

} // namespace matrix

} // namespace math

} // namespace cavr

