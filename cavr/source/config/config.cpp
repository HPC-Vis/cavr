#include <cavr/config/config.h>
#include <cavr/math/matrix.h>
using cavr::math::mat4d;

namespace cavr {

namespace config {

transform::transform()
  : matrix_(new mat4d()) {
}

transform::transform(const transform& rhs)
  : matrix_(new mat4d(*(rhs.matrix_))) {
}

transform::transform(const mat4d& matrix)
  : matrix_(new mat4d(matrix)) {
}

transform& transform::operator=(const transform& t) {
  *matrix_ = *(t.matrix_);
  return *this;
}

transform transform::operator*(const transform& t) const {
  mat4d result = *matrix_ * *(t.matrix_);
  return transform(result);
}

transform& transform::operator*=(const transform& t) {
  *matrix_ *= *(t.matrix_);
  return *this;
}

transform transform::operator*(double v) const {
  transform t(*this);
  t *= v;
  return t;
}

transform& transform::operator*=(double v) {
  *matrix_ *= v;
  return *this;
}

transform transform::operator/(double v) const {
  transform t(*this);
  t /= v;
  return t;
}

transform& transform::operator/=(double v) {
  *matrix_ /= v;
  return *this;
}

struct math::matrix::mat<double, 4, 4>* transform::matrix() {
  return matrix_;
}

const struct math::matrix::mat<double, 4, 4>* transform::matrix() const {
  return matrix_;
}

transform::~transform() {
  delete matrix_;
}

transform identity() {
  mat4d m(1.0);
  return transform(m);
}

transform translate(double x, double y, double z) {
  mat4d m = mat4d::translate(x, y, z);
  return transform(m);
}

transform rotate(double radians, double x, double y, double z) {
  mat4d m = mat4d::rotate(radians, x, y, z);
  return transform(m);
}

transform scale(double x, double y, double z) {
  mat4d m(1.0);
  m[0][0] = x;
  m[1][1] = y;
  m[2][2] = z;
  return transform(m);
}

transform operator*(double v, const transform& t) {
  mat4d m = v * *(t.matrix());
  return transform(m);
}

} // namespace config

} // namespace cavr
