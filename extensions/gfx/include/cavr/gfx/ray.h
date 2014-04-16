#pragma once
#include <cavr/math/vector.h>

namespace cavr {

namespace gfx {

class Ray {
public:
  Ray(const math::vec3f& origin, const math::vec3f& direction);
  const math::vec3f& origin() const;
  const math::vec3f& direction() const;
private:
  math::vec3f origin_;
  math::vec3f direction_;
};

} // namespace gfx

} // namespace cavr
