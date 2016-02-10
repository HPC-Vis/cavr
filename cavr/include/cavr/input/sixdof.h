#pragma once
#include <cavr/lock.h>
#include <cavr/input/input.h>
#include <cavr/math/matrix.h>
#include <cavr/math/vector.h>

namespace cavr {

namespace input {

class SixDOF : public Input {
public:
  static const std::string type_name; 
  SixDOF();
  math::vec3d getPosition() const;
  math::vec3d getForward() const;
  math::vec3d getBackward() const;
  math::vec3d getLeft() const;
  math::vec3d getRight() const;
  math::vec3d getUp() const;
  math::vec3d getDown() const;
  math::mat4d getMatrix() const;
  void setState(const math::mat4d& m);
  void syncState(const math::mat4d& m);
  void sync();
  void setReverse(bool rev);
private:
  mutable cavr::Lock sync_lock_;
  mutable cavr::Lock live_lock_;
  math::mat4d state_;
  math::mat4d live_state_;
  bool reverse;
};

} // namespace input

} // namespace cavr
