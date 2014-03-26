#include <cavr/input/sixdof.h>
#include <iostream>

namespace cavr {

namespace input {

SixDOF::SixDOF() 
  : state_(1.0) {
}

math::vec3d SixDOF::getPosition() const {
  return state_[3].xyz;
}

math::vec3d SixDOF::getForward() const {
  return -state_[2].xyz;
}

math::vec3d SixDOF::getBackward() const {
  return state_[2].xyz;
}

math::vec3d SixDOF::getLeft() const {
  return -state_[0].xyz;
}

math::vec3d SixDOF::getRight() const {
  return state_[0].xyz;
}

math::vec3d SixDOF::getUp() const {
  return state_[1].xyz;
}

math::vec3d SixDOF::getDown() const {
  return -state_[1].xyz;
}

const math::mat4d& SixDOF::getMatrix() const {
  return state_;
}

void SixDOF::setState(const math::mat4d& m) {
  state_ = m;
}

} // namespace input

} // namespace cavr
