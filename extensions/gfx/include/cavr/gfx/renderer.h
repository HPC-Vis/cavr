#pragma once
#include <cavr/cavr.h>

namespace cavr {

namespace gfx {

class Renderer {
public:
  virtual void setNear(double d) = 0;
  virtual void setFar(double d) = 0;
  virtual double getNear() const = 0;
  virtual double getFar() const = 0;
  virtual math::mat4f getProjection() const = 0;
  virtual math::mat4f getView() const = 0;
  virtual math::vec3f getEyePosition() const = 0;
#ifdef __GNUC__
  static __thread Renderer* renderer;
#else
  static thread_local Renderer* renderer;
#endif
};

void setNear(double d);
void setFar(double d);
double getNear();
double getFar();
math::mat4f getProjection();
math::mat4f getView();
math::vec3f getEyePosition();

} // namespace gfx

} // namespace cavr
