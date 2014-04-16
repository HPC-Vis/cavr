#include <cavr/gfx/renderer.h>

namespace cavr {

namespace gfx {

#ifdef __GNUC__
__thread Renderer* Renderer::renderer = nullptr;
#else
thread_local Renderer* Renderer::renderer = nullptr;
#endif

void setNear(double d) {
  Renderer::renderer->setNear(d);
}

void setFar(double d) {
  Renderer::renderer->setFar(d);
}

double getNear() {
  return Renderer::renderer->getNear();
}

double getFar() {
  return Renderer::renderer->getFar();
}

math::mat4f getProjection() {
  return Renderer::renderer->getProjection();
}

math::mat4f getView() {
  return Renderer::renderer->getView();
}

math::vec3f getEyePosition() {
  return Renderer::renderer->getEyePosition();
}

} // namespace gfx

} // namespace cavr
