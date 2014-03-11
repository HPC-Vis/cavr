#include <cavr/config/config.h>
#include <cavr/math/matrix.h>

namespace cavr {

namespace config {

transform::transform()
  : matrix_(new math::mat4d()) {
}

transform::transform(const math::mat4d* matrix)
  : matrix_(new math::mat4d(*matrix)) {
}

} // namespace config

} // namespace cavr
