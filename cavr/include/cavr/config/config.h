#pragma once
#ifndef SWIG
#include <cavr/math/matrix.h>
#include <cavr/math/vector.h>
#endif // SWIG

namespace cavr {

namespace config {

class transform;
class sixdof_marker;

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
  sixdof_marker operator*(const sixdof_marker& s) const;
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

class sixdof_marker {
public:
  sixdof_marker(const std::string& name);
  sixdof_marker(const transform& pre,
             const std::string& name,
             const transform& post);
  const std::string& name() const;
  const transform& pretransform() const;
  const transform& posttransform() const;
  sixdof_marker operator*(const transform& t) const;
private:
  std::string name_;
  transform pre_;
  transform post_;
};

sixdof_marker sixdof(const std::string& name);

class vec {
public:
#ifndef SWIG
  vec(const math::vector::vec<double, 3>& v);
#endif // SWIG
  vec();
  vec(double x);
  vec(double x, double y, double z);
  vec operator+(const vec& v) const;
  vec operator-(const vec& v) const;
  vec operator*(double v) const;
  vec operator/(double v) const;
  vec& operator+=(const vec& v);
  vec& operator-=(const vec& v);
  vec& operator*=(double v);
  vec& operator/=(double v);
  double& operator[](int i);
  const double& operator[](int i) const;
#ifndef SWIG
  struct math::vector::vec<double, 3>* vector();
  const struct math::vector::vec<double, 3>* vector() const;
#endif // SWIG
  ~vec();
private:
  struct math::vector::vec<double, 3>* v_;
};

} // namespace config

} // namespace cavr
