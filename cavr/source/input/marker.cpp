#include <cavr/input/marker.h>

namespace cavr {

namespace input {

StaticMarker::StaticMarker(const math::vec3d& position)
  : position_(position) {
}

math::vec3d StaticMarker::getPosition() const {
  return position_;
}

SixDOFMarker::SixDOFMarker(const math::mat4d& pretransform,
                           const math::mat4d& posttransform,
                           const SixDOF* sixdof)
  : sixdof_(sixdof),
    pretransform_(pretransform),
    posttransform_(posttransform) {
}

math::vec3d SixDOFMarker::getPosition() const {
  math::mat4d m = pretransform_ * sixdof_->getMatrix() * posttransform_;
  return m[3].xyz;
}

} // namespace input

} // namespace cavr
