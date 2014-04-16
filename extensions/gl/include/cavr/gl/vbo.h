#pragma once
#include <cavr/math/vector.h>
#include <GL/gl.h>
#include <vector>

namespace cavr {

namespace gl {

class VBO {
public:
  VBO();
  template<typename T, int N>
  VBO(const std::vector<math::vector::vec<T,N>>& v,
      GLenum usage = GL_STATIC_DRAW) {
    glGenBuffers(1, &vbo_id_);
    bufferData((const char*)v.data(),
               sizeof(math::vector::vec<T,N>) * v.size(),
               usage);
  }
  void bufferData(const char* data, int size, GLenum usage);
  GLuint id() const;
  ~VBO();
private:
  GLuint vbo_id_;
};

} // namespace gl

} // namespace cavr
