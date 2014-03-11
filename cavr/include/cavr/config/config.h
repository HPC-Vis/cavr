#pragma once
#ifndef SWIG
#include <cavr/math/matrix.h>
#endif // SWIG

namespace cavr {

namespace config {

class transform {
public:
  transform();
  transform(const struct math::matrix::mat<double, 4, 4>* matrix);
private:
  struct math::matrix::mat<double, 4, 4>* matrix_;
};

transform identity();
transform translate();
transform rotate();
transform scale();

} // namespace config

} // namespace cavr
