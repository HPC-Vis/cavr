#include <cavr/gfx/sphere.h>

namespace cavr {

namespace gfx {

Sphere::Sphere(const math::vec3f& center, float radius)
  : center_(center),
    radius_(radius) {
}

const math::vec3f& Sphere::center() const {
  return center_;
}

float Sphere::radius() const {
  return radius_;
}

bool Sphere::intersect(const Ray& ray, float& t) {
  math::vec3f o = ray.origin() - center_;
  math::vec3f d = ray.direction();
  float a = d.dot(d);
  float b = 2.0 * d.dot(o);
  float c = o.dot(o) - (radius_ * radius_);
  float discriminant = b * b - 4.0 * a * c;
  if (discriminant < 0.0) {
    return false;
  }
  float q = 0.5 * (-b + (b < 0.0? 1.0 : -1.0) * sqrt(discriminant));
  float t0 = q / a;
  float t1 = c / q;
  if (t0 > t1) {
    std::swap(t0, t1);
  }
  if (t1 < 0) {
    return false;
  }
  if (t0 < 0) {
    t = t1;
  } else {
    t = t0;
  }
  return true;
}


} // namespace gfx

} // namespace cavr
