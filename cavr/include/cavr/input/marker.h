#pragma once
#include <cavr/input/sixdof.h>
#include <cavr/math/vector.h>
#include <cavr/math/matrix.h>

namespace cavr {

namespace input {

class Marker {
public:
  virtual math::vec3d getPosition() const = 0;
private:
};

class StaticMarker : public Marker {
public:
  StaticMarker(const math::vec3d& position);
  virtual math::vec3d getPosition() const;
private:
  math::vec3d position_;
};

class SixDOFMarker : public Marker {
public:
  SixDOFMarker(const math::mat4d& pretransform,
               const math::mat4d& posttransform,
               const SixDOF* sixdof);
  virtual math::vec3d getPosition() const;
private:
  const SixDOF* sixdof_;
  math::mat4d pretransform_;
  math::mat4d posttransform_;
};

} // namespace input

} // namespace cavr
