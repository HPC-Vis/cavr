#pragma once
#include <cavr/gfx/ray.h>

namespace cavr {

namespace gfx {

class Sphere {
public:
  Sphere(const math::vec3f& center, float radius);
  const math::vec3f& center() const;
  float radius() const;
  bool intersect(const Ray& ray, float& t);
private:
  math::vec3f center_;
  float radius_;
};

} // namespace gfx

} // namespace cavr
