#include <GL/glew.h>
#include <cavr/gl/vao.h>

namespace cavr {

namespace gl {

VAO::VAO() {
  glGenVertexArrays(1, &vao_id_);
}

void VAO::setAttribute(GLint index,
                       VBO* vbo,
                       unsigned int num_components,
                       GLenum data_type,
                       bool normalized,
                       size_t stride,
                       size_t offset) {
  glBindVertexArray(vao_id_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id());
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, 
                        num_components, 
                        data_type, 
                        normalized, 
                        stride, 
                        (const char*)offset);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void VAO::bind() {
  glBindVertexArray(vao_id_);
}

VAO::~VAO() {
  glDeleteVertexArrays(1, &vao_id_);
}

} // namespace gl

} // namespace cavr
