#pragma once
#include <cavr/gl/vbo.h>

namespace cavr {

namespace gl {

class VAO {
public:
  VAO();
  void bind();
  void setAttribute(GLint index,
                    VBO* vbo,
                    unsigned int num_components,
                    GLenum data_type,
                    bool normalized,
                    size_t stride,
                    size_t offset);
  ~VAO();
private:
  GLuint vao_id_;
};

} // namespace gl

} // namespace cavr
