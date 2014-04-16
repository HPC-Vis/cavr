#include <GL/glew.h>
#include <cavr/gl/vbo.h>

namespace cavr {

namespace gl {

VBO::VBO() {
  glGenBuffers(1, &vbo_id_);
}

void VBO::bufferData(const char* data, int size, GLenum usage) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
  glBufferData(GL_ARRAY_BUFFER, size, data, usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint VBO::id() const {
  return vbo_id_;
}

VBO::~VBO() {
  glDeleteBuffers(1, &vbo_id_);
}

} // namespace gl

} // namespace cavr
