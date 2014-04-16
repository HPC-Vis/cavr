#include <cavr/config/config.h>
#include <cavr/input/input_manager.h>
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

sixdof_marker transform::operator*(const sixdof_marker& s) const {
  return sixdof_marker(*this * s.pretransform(), s.name(), s.posttransform());
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

sixdof_marker::sixdof_marker(const std::string& name)
  : pre_(identity()),
    post_(identity()),
    name_(name) {
}

sixdof_marker::sixdof_marker(const transform& pre,
                       const std::string& name,
                       const transform& post) 
  : pre_(pre),
    post_(post),
    name_(name) {
}

const std::string& sixdof_marker::name() const {
  return name_;
}

const transform& sixdof_marker::pretransform() const {
  return pre_;
}

const transform& sixdof_marker::posttransform() const {
  return post_;
}

sixdof_marker sixdof_marker::operator*(const transform& t) const {
  return sixdof_marker(pre_, name_, post_ * t);
}

sixdof_marker sixdof(const std::string& name) {
  return sixdof_marker(name);
}

vec::vec(const math::vector::vec<double, 3>& v)
  : v_(new math::vector::vec<double, 3>(v)) {
}

vec::vec() 
  : v_(new math::vector::vec<double, 3>(0)) {
}

vec::vec(double x) 
  : v_(new math::vector::vec<double, 3>(x)) {
}

vec::vec(double x, double y, double z) 
  : v_(new math::vector::vec<double, 3>(x, y, z)) {
}

vec vec::operator+(const vec& v) const {
  return vec(*vector() + *v.vector());
}

vec vec::operator-(const vec& v) const {
  return vec(*vector() - *v.vector());
}

vec vec::operator*(double v) const {
  return vec(*vector() * v);
}

vec vec::operator/(double v) const {
  return vec(*vector() / v);
}

vec& vec::operator+=(const vec& v) {
  *v_ += *v.vector();
}

vec& vec::operator-=(const vec& v) {
  *v_ -= *v.vector();
}

vec& vec::operator*=(double v) {
  *v_ *= v;
}

vec& vec::operator/=(double v) {
  *v_ /= v;
}

double& vec::operator[](int i) {
  return (*v_)[i];
}

const double& vec::operator[](int i) const {
  return (*v_)[i];
}

struct math::vector::vec<double, 3>* vec::vector() {
  return v_;
}

const struct math::vector::vec<double, 3>* vec::vector() const {
  return v_;
}

vec::~vec() {
  delete v_;
}

} // namespace config

} // namespace cavr
