#pragma once
#ifndef SWIG
#include <cavr/math/matrix.h>
#endif // SWIG

namespace cavr {

namespace config {

class transform {
public:
  transform();
#ifndef SWIG
  transform(const math::matrix::mat<double, 4, 4>& matrix);
#endif // SWIG
  transform(const transform& rhs);
  transform& operator=(const transform& t);
  transform operator*(const transform& t) const;
  transform& operator*=(const transform& t);
  transform operator*(double v) const;
  transform& operator*=(double v);
  transform operator/(double v) const;
  transform& operator/=(double v);
#ifndef SWIG
  struct math::matrix::mat<double, 4, 4>* matrix();
  const struct math::matrix::mat<double, 4, 4>* matrix() const;
#endif // SWIG
  ~transform();
private:
  struct math::matrix::mat<double, 4, 4>* matrix_;
};

transform identity();
transform translate(double x, double y, double z);
transform rotate(double radians, double x, double y, double z);
transform scale(double x, double y, double z);
transform operator*(double v, const transform& t);

} // namespace config

} // namespace cavr
