#include <cavr/input/sixdof.h>
#include <iostream>

namespace cavr {

namespace input {

const std::string SixDOF::type_name("SixDOF");

SixDOF::SixDOF() 
  : state_(1.0),
    live_state_(1.0),
    reverse(false) {
}

math::vec3d SixDOF::getPosition() const {
  sync_lock_.readLock();
  math::vec3d result = state_[3].xyz;
  sync_lock_.readUnlock();
  return result;
}

math::vec3d SixDOF::getForward() const {
  sync_lock_.readLock();
  math::vec3d result = -state_[2].xyz;
  if(reverse) {
    result = -result;
  }
  sync_lock_.readUnlock();
  return result;
}

math::vec3d SixDOF::getBackward() const {
  sync_lock_.readLock();
  math::vec3d result = state_[2].xyz;
  if(reverse) {
    result = -result;
  }
  sync_lock_.readUnlock();
  return result;
}

math::vec3d SixDOF::getLeft() const {
  sync_lock_.readLock();
  math::vec3d result = -state_[0].xyz;
  if(reverse) {
    result = -result;
  }
  sync_lock_.readUnlock();
  return result;
}

math::vec3d SixDOF::getRight() const {
  sync_lock_.readLock();
  math::vec3d result = state_[0].xyz;
  if(reverse) {
    result = -result;
  }
  sync_lock_.readUnlock();
  return result;
}

math::vec3d SixDOF::getUp() const {
  sync_lock_.readLock();
  math::vec3d result = state_[1].xyz;
  if(reverse) {
    result = -result;
  }
  sync_lock_.readUnlock();
  return result;
}

math::vec3d SixDOF::getDown() const {
  sync_lock_.readLock();
  math::vec3d result = -state_[1].xyz;
  if(reverse) {
    result = -result;
  }
  sync_lock_.readUnlock();
  return result;
}

math::mat4d SixDOF::getMatrix() const {
  sync_lock_.readLock();
  math::mat4d m = state_;
  if(reverse) {
    m[0][0] *= -1;
    m[0][1] *= -1;
    m[0][2] *= -1;
    m[1][0] *= -1;
    m[1][1] *= -1;
    m[1][2] *= -1;
    m[2][0] *= -1;
    m[2][1] *= -1;
    m[2][2] *= -1;
  }
  sync_lock_.readUnlock();
  return m;
}

void SixDOF::setState(const math::mat4d& m) {
  live_lock_.writeLock();
  live_state_ = m;
  live_lock_.writeUnlock();
}

void SixDOF::syncState(const math::mat4d& m) {
  sync_lock_.writeLock();
  state_ = m;
  sync_lock_.writeUnlock();
}

void SixDOF::sync() {
  live_lock_.readLock();
  math::mat4d m = live_state_;
  live_lock_.readUnlock();
  syncState(m);
}

void SixDOF::setReverse(bool rev) {
  reverse = rev;
}

} // namespace input

} // namespace cavr
