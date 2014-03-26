#pragma once
#include <cavr/input/input.h>
#include <cavr/math/matrix.h>
#include <cavr/math/vector.h>

namespace cavr {

namespace input {

class SixDOF : public Input {
public:
  SixDOF();
  math::vec3d getPosition() const;
  math::vec3d getForward() const;
  math::vec3d getBackward() const;
  math::vec3d getLeft() const;
  math::vec3d getRight() const;
  math::vec3d getUp() const;
  math::vec3d getDown() const;
  const math::mat4d& getMatrix() const;
  void setState(const math::mat4d& m);
private:
  math::mat4d state_;
};

} // namespace input

} // namespace cavr
