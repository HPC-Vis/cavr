#include <cavr/gfx/ray.h>

namespace cavr {

namespace gfx {

Ray::Ray(const math::vec3f& origin, const math::vec3f& direction) 
  : origin_(origin),
    direction_(direction) {
}

const math::vec3f& Ray::origin() const {
  return origin_;
}

const math::vec3f& Ray::direction() const {
  return direction_;
}

} // namespace gfx

} // namespace cavr
